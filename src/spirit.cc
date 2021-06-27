#include <QRect>
#include <QPixmap>
#include <QPair>
#include <QJsonParseError>
#include <QJsonArray>

#include "spirit.hpp"
#include "helpers_p.hpp"

Spirit::Spirit(const QString &path)
	: QWidget(nullptr, Qt::FramelessWindowHint |
			   Qt::Tool |
			   Qt::WindowStaysOnTopHint |
			   Qt::WindowTransparentForInput |
			   Qt::WindowDoesNotAcceptFocus | 
			   Qt::NoDropShadowWindowHint |
			   Qt::X11BypassWindowManagerHint) {
		
	setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setAttribute(Qt::WA_ShowWithoutActivating, true);
	setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
	setStyleSheet(QString::fromUtf8("background: transparent; border: none;"));

	m_Actions.reset(new QHash<QString, Action*>);

	m_Extractor.reset(new QArchive::MemoryExtractor(/*parent=*/nullptr, /*single threaded=*/false));

	connect(m_Extractor.data(), &QArchive::MemoryExtractor::error,
		this, &Spirit::handleArchiveError);
	connect(m_Extractor.data(), &QArchive::MemoryExtractor::finished,
		this, &Spirit::handleArchiveContents);

	m_Extractor->setArchive(path);
}

Spirit::~Spirit() {
	hide();
	if(!m_Actions->isEmpty()) {
	   for(auto iter = m_Actions->begin(),
		    end = m_Actions->end();
		    iter != end;
		    ++iter) {
	       delete iter.value();
	   }
	   m_Actions->clear();
	}
}

void Spirit::init() {
	getMethod(this, "doInit()")
	.invoke(this, Qt::QueuedConnection);
}

void Spirit::update(QRect geometry) {
	if(m_Spirit.isNull()) {
		return;
	}

	auto point = geometry.topLeft();

	move(point.x() + 100, point.y() - (frameGeometry().height()) + 46);
	if(!isVisible()) {
		show();
	}
}


/// Private Slots.
void Spirit::doInit() {
   if(!m_Spirit.isNull()) {
	   return;
   }

   if(!m_Extractor.isNull()) {
   	m_Extractor->start();
   }
}

void Spirit::handleArchiveError(short code) {
	/// TODO: Handle error.
	emit error(Error::InvalidSpiritFile);
}

void Spirit::handleArchiveContents(QArchive::MemoryExtractorOutput *output) {
	auto files = output->getFiles();
	QHash<QString, int> frame_count;
	QHash<QString, QBuffer*> audio_entry;
	QHash<QString, QBuffer*> audio_buffers;
	QHash<QString, bool> loop_entry;
	QHash<QString, QVector<QPair<int, int>>> frame_ranges;
	QHash<QString, QStringList> frame_files;
	QHash<QString, QBuffer*> frame_buffers;
	QVector<QString> audio_files;

	for(auto iter = files.begin(),
		 end = files.end();
		 iter != end;
		 ++iter) {
	  auto info = (*iter).fileInformation();
	  auto fileName = info.value("FileName").toString();
	  auto fileNameParts = fileName.split("/");


	  if(fileNameParts.size() == 0) { // Highly unlikely event.
		  emit error(Error::InvalidSpiritFile);
		  output->deleteLater();
		  return;
	  }

	  /// If it's compressed by ritual then there should be atmost 
	  /// three parts but we should not assume this.
	  if(fileNameParts.size() == 1) {
		/// It should be meta.json file
		if(fileNameParts.at(0) != "meta.json") {
			emit error(Error::InvalidSpiritFile);
			output->deleteLater();
			return; 
		} else {
			/// Parse the json file.
			QBuffer *buffer = (*iter).buffer();

			if(!buffer->open(QIODevice::ReadOnly)) {
				emit error(Error::InvalidSpiritFile);
				output->deleteLater();
				return;
			}

			QJsonParseError e;
			auto document = QJsonDocument::fromJson(buffer->readAll(), &e);

			if(e.error != QJsonParseError::NoError || !document.isObject()) {
				emit error(Error::InvalidSpiritFile);
				output->deleteLater();
				return;
			}
			buffer->close();

			meta = document.object();
		}
	  } else {
		 if(fileNameParts.at(0) == "actions" && fileNameParts.size() == 3) {
			 if(!frame_count.contains(fileNameParts.at(1))) {
				frame_count.insert(fileNameParts.at(1), 0);
			 }
			 frame_count[fileNameParts.at(1)] += 1;

			 if(!frame_files.contains(fileNameParts.at(1))) {
				 frame_files.insert(fileNameParts.at(1), QStringList());
			 }
			 frame_files[fileNameParts.at(1)] << fileName;
			 frame_buffers.insert(fileName, (*iter).buffer());
		 } else if(fileNameParts.at(0) == "audio" && fileNameParts.size() == 2) {
			 if(!audio_files.contains(fileNameParts.at(1))) {
				 audio_files.append(fileNameParts.at(1));
				 audio_buffers.insert(fileNameParts.at(1), (*iter).buffer());
			 }
		 } else { 
		    emit error(Error::InvalidSpiritFile);
		    output->deleteLater();
		    return;
		 }
	  }
	}

	/// Now check the json file.
	if(!meta.contains("name") ||
	   !meta.contains("version") ||
	   !meta.contains("positions") ||
	   !meta.contains("actions")) {
	   emit error(Error::InvalidSpiritFile);
	   output->deleteLater();
	   return;
	}

	{
	   auto json_actions = meta.value("actions").toObject().keys();
	   auto archive_actions = frame_count.keys();

	   if(json_actions.size() != archive_actions.size()) {
	      emit error(Error::InvalidSpiritFile);
	      output->deleteLater();
	      return;
	   }

	   for(auto iter = json_actions.begin(),
		 end = json_actions.end();
		 iter != end;
		 ++iter) {
	      if(!archive_actions.contains(*iter)) {
		   emit error(Error::InvalidSpiritFile);
		   output->deleteLater();
		   return;
	      }
	   }

	   // Check frames and play now.
	   auto actions = meta.value("actions").toObject();

	   for(auto iter = json_actions.begin(),
		    end = json_actions.end();
		    iter != end;
		    ++iter){ 
		auto object = actions.value(*iter).toObject();

		if(object.isEmpty() || !object.contains("frames")) {
			emit error(Error::InvalidSpiritFile);
			output->deleteLater();
			return;
		}

		// Check audio file too.
		if(object.contains("play")) {
		   if(!audio_files.contains(object.value("play").toString() + ".mp3")) {
		      emit error(Error::InvalidSpiritFile);
		      output->deleteLater();
		      return;
		   } else {
		      audio_entry.insert(*iter, audio_buffers.value(object.value("play").toString() + ".mp3"));  
		   }
		}

		if(object.contains("loop")) {
		   if(object.value("loop").toBool()) {
			   loop_entry.insert(*iter, true);
		   } else {
			   if(*iter == "default") {
				   loop_entry.insert(*iter, true);
			   } else {
				   loop_entry.insert(*iter, false);
			   }
		   }
		}

		auto frames = object.value("frames").toArray();
		if(!frame_ranges.contains(*iter)) {
			frame_ranges.insert(*iter, QVector<QPair<int, int>>());
		}
		for(auto i = frames.begin(),
			 e = frames.end();
			 i != e;
			 ++i) {
			auto range = (*i).toString();
			auto rangeParts = range.split("-");
			bool ok = false;

			if(rangeParts.size() == 0) {
			   emit error(Error::InvalidSpiritFile);
			   output->deleteLater();
			   return;
			}

			if(rangeParts.size() == 1) {
			   int f = rangeParts.at(0).toInt(&ok);
			   if(!ok || f > frame_count.value(*iter)) {
			      emit error(Error::InvalidSpiritFile);
			      output->deleteLater();
			      return;
			   }
			} else {
			   bool er = false;
			   int f = rangeParts.at(0).toInt(&ok);
			   er = !ok;
			   int t = -1;
			   if(rangeParts.at(1) != "*") { 
			   	t = rangeParts.at(1).toInt(&ok);
			   	if(!er) {
					er = !ok;
			   	}
			   }

			   if(er || f > frame_count.value(*iter) || t > frame_count.value(*iter)) {
			      emit error(Error::InvalidSpiritFile);
			      output->deleteLater();
			      return;
			   }

			   frame_ranges[*iter].append(qMakePair<int, int>(f, t));
			}
		}
	   }
	}

	for(auto iter = m_Actions->begin(),
		    end = m_Actions->end();
		    iter != end;
		    ++iter) {
	       delete *iter;
	}
	m_Actions->clear();


	{
	   QHash<QString, QVector<QBuffer*>> frame_ac_buffers;
	   for(auto iter = frame_files.begin(),
		    end = frame_files.end();
		    iter != end;
		    ++iter) { 
	      iter.value().sort();

	      frame_ac_buffers.insert(iter.key(), QVector<QBuffer*>());

	      for(auto i = iter.value().begin(),
		       e = iter.value().end();
		       i != e;
		       ++i) {
	         frame_ac_buffers[iter.key()] << frame_buffers.value(*i);
	      }
	   }

	   for(auto iter = frame_ac_buffers.begin(),
		    end = frame_ac_buffers.end();
		    iter != end;
		    ++iter) {
	      auto a = new Action;
	      a->action  = iter.key();
	      if(audio_entry.contains(iter.key())) {
		      a->play = audio_entry.value(iter.key());
	      } else {
	              a->play = nullptr;
	      }
	      a->frames = iter.value();
	      a->frame_order = frame_ranges.value(iter.key());
	      a->loop = loop_entry.value(iter.key());
	      m_Actions->insert(iter.key(), a);
	   }
	}

	m_Extractor.reset(nullptr); // Free memory.
	m_Spirit.reset(output);

#if 0
	for(auto iter = m_Actions->begin(),
		 end = m_Actions->end();
		 iter != end;
		 ++iter) {
		qDebug() << iter.key() << ": ";
		if((iter.value())->play) {
			qDebug() << "Play size:: " << (iter.value())->play->size();
		}
		qDebug() << "Frames:: " << (iter.value())->frames.size();
		qDebug() << "Frame Order:: " << (iter.value())->frame_order;
		qDebug() << "Loop:: " << (iter.value())->loop;
		qDebug() << "---\n";
	}
#endif
	emit initialized();
}
