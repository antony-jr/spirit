#include <QRect>
#include <QPixmap>
#include <QPair>
#include <QJsonParseError>
#include <QJsonArray>
#include <QCoreApplication>

#include "spiritenums.hpp"
#include "spiritworker_p.hpp"
#include "helpers_p.hpp"

SpiritWorkerPrivate::SpiritWorkerPrivate()
    : QObject() {
    m_Actions.reset(new QHash<QString, Action*>);
    m_Extractor.reset(new QArchive::MemoryExtractor(/*parent=*/nullptr, /*single threaded=*/false));

    connect(m_Extractor.data(), &QArchive::MemoryExtractor::progress,
            this, &SpiritWorkerPrivate::handleProgress);

    connect(m_Extractor.data(), &QArchive::MemoryExtractor::started,
            this, &SpiritWorkerPrivate::handleArchiveStarted);

    connect(m_Extractor.data(), &QArchive::MemoryExtractor::error,
            this, &SpiritWorkerPrivate::handleArchiveError);

    connect(m_Extractor.data(), &QArchive::MemoryExtractor::canceled,
            this, &SpiritWorkerPrivate::handleArchiveCancel);

    connect(m_Extractor.data(), &QArchive::MemoryExtractor::finished,
            this, &SpiritWorkerPrivate::handleArchiveContents);
}

SpiritWorkerPrivate::~SpiritWorkerPrivate() {
    clear(false);
}

void SpiritWorkerPrivate::setSpiritFile(const QString &file) {
    if(n_Status != SpiritEnums::Spirit::Status::Idle &&
            n_Status != SpiritEnums::Spirit::Status::Canceled) {
        emit error(SpiritEnums::Spirit::Error::SpiritNotCanceled);
        return;
    }

    clear(false);
    m_SpiritPath = file;
}

void SpiritWorkerPrivate::setAction(const QString &ac) {
   auto keys = m_Actions->keys();
   if(!keys.contains(ac)) {
      return;
   }
   n_Frame = 0;
   m_CurrentAction = ac;
   emit actionChanged(m_CurrentAction);
}

void SpiritWorkerPrivate::getActions() {
   emit actions(m_Actions->keys());
}

void SpiritWorkerPrivate::getCurrentAction() {
   emit action(m_CurrentAction.isEmpty() ? "default" : 
	       m_CurrentAction);
}

void SpiritWorkerPrivate::getInfo() {
   emit info(m_Meta);
}

void SpiritWorkerPrivate::start() {
    if(n_Status == SpiritEnums::Spirit::Status::Loading ||
            n_Status == SpiritEnums::Spirit::Status::Parsing ||
            n_Status == SpiritEnums::Spirit::Status::Animating) {
        return;
    }

    if(m_SpiritPath.isEmpty()) {
        emit error(SpiritEnums::Spirit::Error::NoSpiritFile);
        return;
    }

    m_Extractor->setArchive(m_SpiritPath);
    m_Extractor->start();
    n_Status = SpiritEnums::Spirit::Status::Loading;
    emit status(n_Status);
}

void SpiritWorkerPrivate::cancel() {
    if(b_CancelRequested || n_Status == SpiritEnums::Spirit::Status::Idle ||
            n_Status == SpiritEnums::Spirit::Status::Canceled) {
        return;
    }
    if(n_Status == SpiritEnums::Spirit::Status::Stopped || n_Status == SpiritEnums::Spirit::Status::Errored) {
        clear();
        return;
    }
    b_CancelRequested = true;
}

void SpiritWorkerPrivate::clear(bool emitCanceled) {
    b_CancelRequested = false;
    b_IsExtractorRunning = false;
    n_Status = SpiritEnums::Spirit::Status::Idle;
    n_Frame = 0;
    n_FrameUnit = 0;
    m_AnimationTimer.reset(nullptr);
    m_Spirit.reset(nullptr);
    m_Extractor->clear();
    m_SpiritPath.clear();
    if(!m_Actions->isEmpty()) {
        for(auto iter = m_Actions->begin(),
	         end = m_Actions->end();
                 iter != end;
                ++iter) {
            delete iter.value();
        }
        m_Actions->clear();
    }

    if(emitCanceled) {
        n_Status = SpiritEnums::Spirit::Status::Canceled;
        emit status(n_Status);
    }
}


/// ----
/// Private Slots.
/// ----
void SpiritWorkerPrivate::handleProgress() {
    if(b_CancelRequested) {
        m_Extractor->cancel();
    }
}

void SpiritWorkerPrivate::handleArchiveStarted() {
    b_IsExtractorRunning = true;
}

void SpiritWorkerPrivate::handleArchiveCancel() {
    b_IsExtractorRunning = false; // Idle
    clear();
}

void SpiritWorkerPrivate::handleArchiveError(short code) {
    /// TODO: Handle error more accurately.
    if(b_CancelRequested) {
        clear();
        return;
    }
    b_IsExtractorRunning = false;
    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
}

void SpiritWorkerPrivate::handleArchiveContents(QArchive::MemoryExtractorOutput *output) {
    b_IsExtractorRunning = false;
    n_Status = SpiritEnums::Spirit::Status::Parsing;
    emit status(n_Status);

    /// Check if cancel was requested.
    if(b_CancelRequested) {
        clear();
        return;
    }

    auto animate = false;
    auto files = output->getFiles();
    
    // meta.json and then find the edition of the 
    // spirit file to handle.
    for(auto iter = files.begin(),
	  end = files.end();
	  iter != end;
	  ++iter) {
       if(b_CancelRequested) {
	  clear();
	  return;
       }

       auto info = (*iter).fileInformation();
       auto filename = info.value("FileName").toString();

       if(filename == "meta.json") {
	  /// Parse the json file.
          QBuffer *buffer = (*iter).buffer();

          if(!buffer->open(QIODevice::ReadOnly)) {
             n_Status = SpiritEnums::Spirit::Status::Errored;
	     emit status(n_Status);

	     emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
             output->deleteLater();
             return;
          }

          QJsonParseError e;
          auto document = QJsonDocument::fromJson(buffer->readAll(), &e);
	  
	  if(e.error != QJsonParseError::NoError || !document.isObject()) {
               n_Status = SpiritEnums::Spirit::Status::Errored;
	       emit status(n_Status);

	       emit error(SpiritEnums::Spirit::Error::CannotParseMetaFile);
               output->deleteLater();
               return;
          }
           
	  buffer->close();

          auto meta = document.object();
	  if(!meta.contains("edition")) {
	     n_Status = SpiritEnums::Spirit::Status::Errored;
	     emit status(n_Status);

	     emit error(SpiritEnums::Spirit::Error::InvalidSpiritEdition);
	     output->deleteLater();
	     return;
	  }

	  auto edition = meta.value("edition").toString().toLower(); 
	  
	  /// Use specific handlers for parsing and checking specific edition
	  /// of the .spirit file.
	  if(edition == "2021") { 
	     /// The very first edition .spirit (aka. v0.1.0)
	     animate = parseEdition2021(files);
	     break;
	  } else {
	     n_Status = SpiritEnums::Spirit::Status::Errored;
	     emit status(n_Status);
 
	     emit error(SpiritEnums::Spirit::Error::InvalidSpiritEdition);
	     output->deleteLater();
	     return;
	  }
	  break;
       }
    }

    if(animate) {
       //// Store the Spirit file in 
       //// memory since we will be using it later on.
       m_Spirit.reset(output);


       //// Start Animation.
       // Get the current action to 
       // animate.

       Action *currentAction = nullptr;

       if(!m_CurrentAction.isEmpty()) {
	 currentAction = m_Actions->value(m_CurrentAction, nullptr);
       }

       if(currentAction == nullptr) {
	  auto v = m_Actions->value("default", nullptr);
	  if(v) {
	     currentAction = v;
	     m_CurrentAction = QString::fromUtf8("default");
	  } else {
	     /// Cannot get default action.
	     /// Very unlikely.
	     n_Status = SpiritEnums::Spirit::Status::Errored;
	     emit status(n_Status);

	     emit error(SpiritEnums::Spirit::Error::CannotGetDefaultAction);
	     return;
	  }
         }

       m_AnimationTimer.reset(new QTimer);
       m_AnimationTimer->setInterval(currentAction->interval);
       m_AnimationTimer->setSingleShot(false);

       connect(m_AnimationTimer.data(), &QTimer::timeout,
	        this, &SpiritWorkerPrivate::animationLoop);


       n_FrameUnit = 0;
       n_Frame = 0;
       m_AnimationTimer->start();
       
       n_Status = SpiritEnums::Spirit::Status::Animating;
       emit status(n_Status);
       emit started(m_Meta);
       return;
    } else {
       output->deleteLater();
    }

    /// No valid meta.json file found in the archive.
    if(n_Status != SpiritEnums::Spirit::Status::Errored &&
       n_Status != SpiritEnums::Spirit::Status::Canceled) {
     /// Only emit error in case no error was emitted previously. 
     n_Status = SpiritEnums::Spirit::Status::Errored;
     emit status(n_Status);
     emit error(SpiritEnums::Spirit::Error::CannotFindMetaFile);
    }

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
}

/// Animation Loop
//---
void SpiritWorkerPrivate::animationLoop() {
   auto currentAction = m_Actions->value(m_CurrentAction, nullptr);
   if(!currentAction) {
      m_AnimationTimer->stop();
      n_Status = SpiritEnums::Spirit::Status::Errored;
      emit status(n_Status);
      emit error(SpiritEnums::Spirit::Error::CannotGetAction);
      return;
   }

   if(n_FrameUnit < (currentAction->frame_order).size()) {
      auto range = (currentAction->frame_order).at(n_FrameUnit); 
      auto currentFrame = range.first + n_Frame;

      if(currentFrame > range.second && range.second != -1) {
	 n_Frame = 0;
	 ++n_FrameUnit;
	 return;
      }

      qDebug() << currentFrame;

      if(currentFrame > (currentAction->frames).size()) {
	 if(range.second == -1) {
	    n_Frame = 0;
	    return;
	 }
	 m_AnimationTimer->stop();
      	 n_Status = SpiritEnums::Spirit::Status::Errored;
      	 emit status(n_Status);
         emit error(SpiritEnums::Spirit::Error::CannotGetAction);
	 return;
      }

      QBuffer *buffer = (currentAction->frames).at(currentFrame);
      emit frame(buffer);

      ++n_Frame;
   } else {
      if(currentAction->loop) {
	 n_Frame = 0;
	 n_FrameUnit = 0;
	 return;
      }

      if(!((currentAction->next_action).isEmpty()) && 
	    m_Actions->contains(currentAction->next_action)) {
	setAction(currentAction->next_action);
	return;
      }

      m_AnimationTimer->stop();
      n_Status = SpiritEnums::Spirit::Status::Stopped;
      emit status(n_Status);
      return;
   }
}


//// Parse Method for Different Editions of .spirit file
//---

/// Edition 2021 (v0.1.0 spirit file spec)
bool SpiritWorkerPrivate::parseEdition2021(const QVector<QArchive::MemoryFile> &files) {
    QHash<QString, int> frame_count;
    QHash<QString, QBuffer*> audio_entry;
    QHash<QString, QBuffer*> audio_buffers;
    QHash<QString, bool> loop_entry;
    QHash<QString, int> interval_entry;
    QHash<QString, QString> next_entry;
    QHash<QString, QVector<QPair<int, int>>> frame_ranges;
    QHash<QString, QStringList> frame_files;
    QHash<QString, QBuffer*> frame_buffers;
    QVector<QString> audio_files;

    for(auto iter = files.begin(),
            end = files.end();
            iter != end;
            ++iter) {
        QCoreApplication::processEvents();
        if(b_CancelRequested) {
            clear();
            return false;
        }
        auto info = (*iter).fileInformation();
        auto fileName = info.value("FileName").toString();
        auto fileNameParts = fileName.split("/");

        if(fileNameParts.size() == 0) { // Highly unlikely event.
            n_Status = SpiritEnums::Spirit::Status::Errored;
	    emit status(n_Status);
	    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
            return false;
        }

        /// If it's compressed by ritual then there should be atmost
        /// three parts but we should not assume this.
        if(fileNameParts.size() == 1) {
            /// It should be meta.json file
            if(fileNameParts.at(0) != "meta.json") {
                n_Status = SpiritEnums::Spirit::Status::Errored;
	    	emit status(n_Status);
	        emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                return false;
            } else {
                /// Parse the json file. 
	        QBuffer *buffer = (*iter).buffer();

                if(!buffer->open(QIODevice::ReadOnly)) {
                    n_Status = SpiritEnums::Spirit::Status::Errored;
	    	    emit status(n_Status);
		    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                    return false;
                }

                QJsonParseError e;
                auto document = QJsonDocument::fromJson(buffer->readAll(), &e);

                if(e.error != QJsonParseError::NoError || !document.isObject()) {
                    n_Status = SpiritEnums::Spirit::Status::Errored;
		    emit status(n_Status); 
		    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                    return false;
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
	       	n_Status = SpiritEnums::Spirit::Status::Errored;
		emit status(n_Status);

                emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                return false;
            }
        }
    }

    /// Store the meta information 
    /// for later use.
    QJsonObject meta_head {
       { "name" , meta.value("name").toString() },
       { "edition", "2021" },
       { "version", meta.value("version").toString() },
       { "author", meta.value("author").toString() }, 
       { "copyright", meta.value("copyright").toString() },
       { "positions", meta.value("positions").toObject() }
    };
    m_Meta = meta_head;

    /// Now check the json file.
    if(!meta.contains("name") ||
       !meta.contains("version") ||
       !meta.contains("positions") ||
       !meta.contains("actions")) {
        n_Status = SpiritEnums::Spirit::Status::Errored;
	emit status(n_Status);

        emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
        return false;
    }

    {
        auto json_actions = meta.value("actions").toObject().keys();
        auto archive_actions = frame_count.keys();

        if(json_actions.size() != archive_actions.size()) {
            n_Status = SpiritEnums::Spirit::Status::Errored;
	    emit status(n_Status);

	    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
            return false;
        }

        for(auto iter = json_actions.begin(),
                end = json_actions.end();
                iter != end;
                ++iter) {
	    QCoreApplication::processEvents();
            if(b_CancelRequested) {
                clear();
                return false;
            }
            if(!archive_actions.contains(*iter)) {
                n_Status = SpiritEnums::Spirit::Status::Errored;
	    	emit status(n_Status);

	        emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                return false;
            }
        }

        // Check frames and play now.
        auto actions = meta.value("actions").toObject();

        for(auto iter = json_actions.begin(),
                end = json_actions.end();
                iter != end;
                ++iter) {
            // Check if cancel requested
	    QCoreApplication::processEvents();
            if(b_CancelRequested) {
                clear();
                return false;
            }

            auto object = actions.value(*iter).toObject();

            if(object.isEmpty() || !object.contains("frames")) {
                n_Status = SpiritEnums::Spirit::Status::Errored;
	    	emit status(n_Status);

	       	emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                return false;
            }

            // Check audio file too.
            if(object.contains("play")) {
                if(!audio_files.contains(object.value("play").toString() + ".mp3")) {
                    n_Status = SpiritEnums::Spirit::Status::Errored;
	    	    emit status(n_Status);

		    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                    return false;
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

	    if(object.contains("interval")) {
	       int msecs = object.value("interval").toInt();
	       if(msecs > 0) {
		  interval_entry.insert(*iter, msecs);
	       } else {
		  interval_entry.insert(*iter, 200);
	       }
	    } else {
	       interval_entry.insert(*iter, 200);
	    }

	    if(object.contains("next")) {
	       next_entry.insert(*iter, object.value("next").toString());
	    }

            auto frames = object.value("frames").toArray();
            if(!frame_ranges.contains(*iter)) {
                frame_ranges.insert(*iter, QVector<QPair<int, int>>());
            }
            for(auto i = frames.begin(),
                    e = frames.end();
                    i != e;
                    ++i) {
	        QCoreApplication::processEvents();
                if(b_CancelRequested) {
                    clear();
                    return false;
                }
                auto range = (*i).toString();
                auto rangeParts = range.split("-");
                bool ok = false;

                if(rangeParts.size() == 0) {
                    n_Status = SpiritEnums::Spirit::Status::Errored;
		    emit status(n_Status);

		    emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                    return false;
                }

                if(rangeParts.size() == 1) {
                    int f = rangeParts.at(0).toInt(&ok);
                    if(!ok || f > frame_count.value(*iter)) {
                        n_Status = SpiritEnums::Spirit::Status::Errored;
	    		emit status(n_Status);

		        emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                        return false;
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
                        n_Status = SpiritEnums::Spirit::Status::Errored;
	    		emit status(n_Status);

		        emit error(SpiritEnums::Spirit::Error::InvalidSpiritFile);
                        return false;
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
	    QCoreApplication::processEvents();
            if(b_CancelRequested) {
                clear();
                return false;
            }
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
	    QCoreApplication::processEvents();
            if(b_CancelRequested) {
                clear();
                return false;
            }
            auto a = new Action;
            a->action  = iter.key();
	    a->next_action= next_entry.value(iter.key());
            if(audio_entry.contains(iter.key())) {
                a->play = audio_entry.value(iter.key());
            } else {
                a->play = nullptr;
            }
            a->frames = iter.value();
            a->frame_order = frame_ranges.value(iter.key());
            a->interval = interval_entry.value(iter.key());
	    a->loop = loop_entry.value(iter.key());
            m_Actions->insert(iter.key(), a);
        }
    }

    return true;
}
