#include <QRect>
#include <QPixmap>
#include <QPair>
#include <QFile>
#include <QJsonParseError>
#include <QJsonArray>
#include <QCoreApplication>
#include <QCryptographicHash>

#include "spiritenums.hpp"
#include "spiritworker_p.hpp"
#include "helpers_p.hpp"


static QString hashFile(QString fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QCryptographicHash hasher(QCryptographicHash::Md5);
    while(!file.atEnd()) {
        hasher.addData(file.read(1024));
    }

    return QString(hasher.result().toHex());
}

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
    if(n_Status != SpiritEnums::SpiritFile::Status::Idle &&
            n_Status != SpiritEnums::SpiritFile::Status::Canceled) {
        emit error(SpiritEnums::SpiritFile::Error::SpiritNotCanceled);
        return;
    }

    clear(false);
    m_SpiritPath = file;
}

void SpiritWorkerPrivate::setAction(const QString &ac) {
    auto keys = m_Actions->keys();
    if(!keys.contains(ac)) {
        getCurrentAction();
        return;
    }
    m_CurrentActionId = ac;
    m_CurrentAction = m_Actions->value(m_CurrentActionId, m_CurrentAction);
    if(m_CurrentActionId != m_CurrentAction->action) {
        m_CurrentActionId = m_CurrentAction->action;
    }

    // Emit action when current action is changed.
    getCurrentAction();
}

void SpiritWorkerPrivate::getActions() {
    emit actions(m_Actions->keys());
}

void SpiritWorkerPrivate::getCurrentAction() {
    if(!m_CurrentAction) {
        return;
    }

    auto uniqueSign = m_Signature + m_CurrentAction->action;
    auto sign = QString(QCryptographicHash::hash(uniqueSign.toUtf8(), QCryptographicHash::Md5).toHex());
    emit action(m_CurrentAction->action,
                m_CurrentAction->buffer,
                m_CurrentAction->_buffer,
                m_CurrentAction->play,
                m_CurrentAction->loop,
                m_CurrentAction->scale,
                m_CurrentAction->speed,
                m_CurrentAction->next_action,
                m_CurrentAction->offsets,
                sign);
}

void SpiritWorkerPrivate::getInfo() {
    emit info(m_Meta);
}

void SpiritWorkerPrivate::init() {
    if(n_Status == SpiritEnums::SpiritFile::Status::Loading ||
            n_Status == SpiritEnums::SpiritFile::Status::Parsing ||
            n_Status == SpiritEnums::SpiritFile::Status::Loaded) {
        return;
    }

    if(m_SpiritPath.isEmpty()) {
        emit error(SpiritEnums::SpiritFile::Error::NoSpiritFile);
        return;
    }

    m_Signature = hashFile(m_SpiritPath);
    m_Extractor->setArchive(m_SpiritPath);
    m_Extractor->start();
    n_Status = SpiritEnums::SpiritFile::Status::Loading;
    emit status(n_Status);
}

void SpiritWorkerPrivate::cancel() {
    if(b_CancelRequested || n_Status == SpiritEnums::SpiritFile::Status::Idle ||
            n_Status == SpiritEnums::SpiritFile::Status::Canceled) {
        return;
    }
    if(n_Status == SpiritEnums::SpiritFile::Status::Errored  ||
            n_Status == SpiritEnums::SpiritFile::Status::Loaded) {
        clear();
        return;
    }
    b_CancelRequested = true;
}

void SpiritWorkerPrivate::clear(bool emitCanceled) {
    b_CancelRequested = false;
    b_IsExtractorRunning = false;
    n_Status = SpiritEnums::SpiritFile::Status::Idle;
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
        n_Status = SpiritEnums::SpiritFile::Status::Canceled;
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
    emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
}

void SpiritWorkerPrivate::handleArchiveContents(QArchive::MemoryExtractorOutput *output) {
    b_IsExtractorRunning = false;
    n_Status = SpiritEnums::SpiritFile::Status::Parsing;
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
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                output->deleteLater();
                return;
            }

            QJsonParseError e;
            auto document = QJsonDocument::fromJson(buffer->readAll(), &e);

            if(e.error != QJsonParseError::NoError || !document.isObject()) {
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::CannotParseMetaFile);
                output->deleteLater();
                return;
            }

            buffer->close();

            auto meta = document.object();
            if(!meta.contains("edition")) {
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritEdition);
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
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritEdition);
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

        // Get the current action to
        // animate.

        m_CurrentAction = nullptr;

        if(!m_CurrentActionId.isEmpty()) {
            m_CurrentAction = m_Actions->value(m_CurrentActionId, nullptr);
        }

        if(m_CurrentAction == nullptr) {
            auto v = m_Actions->value("default", nullptr);
            if(v) {
                m_CurrentAction = v;
                m_CurrentActionId = QString::fromUtf8("default");
            } else {
                /// Cannot get default action.
                /// Very unlikely.
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::CannotGetDefaultAction);
                return;
            }
        }

        n_Status = SpiritEnums::SpiritFile::Status::Loaded;
        emit status(n_Status);
        emit initialized(m_Meta);

        getCurrentAction();
        return;
    } else {
        output->deleteLater();
    }

    /// No valid meta.json file found in the archive.
    if(n_Status != SpiritEnums::SpiritFile::Status::Errored &&
            n_Status != SpiritEnums::SpiritFile::Status::Canceled) {
        /// Only emit error in case no error was emitted previously.
        n_Status = SpiritEnums::SpiritFile::Status::Errored;
        emit status(n_Status);
        emit error(SpiritEnums::SpiritFile::Error::CannotFindMetaFile);
    }
}

//// Parse Method for Different Editions of .spirit file
//---

/// Edition 2021 (v0.1.0 spirit file spec)
bool SpiritWorkerPrivate::parseEdition2021(const QVector<QArchive::MemoryFile> &files) {
    QVector<QString> audio_files;
    QHash<QString, QBuffer*> audio_buffers;
    QVector<QString> action_files;
    QHash<QString, QBuffer*> action_buffers;

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
            n_Status = SpiritEnums::SpiritFile::Status::Errored;
            emit status(n_Status);
            emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
            return false;
        }

        /// If it's compressed by ritual then there should be atmost
        /// three parts but we should not assume this.
        if(fileNameParts.size() == 1) {
            /// It should be meta.json file
            if(fileNameParts.at(0) != "meta.json") {
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);
                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                return false;
            } else {
                /// Parse the json file.
                QBuffer *buffer = (*iter).buffer();

                if(!buffer->open(QIODevice::ReadOnly)) {
                    n_Status = SpiritEnums::SpiritFile::Status::Errored;
                    emit status(n_Status);
                    emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                    return false;
                }

                QJsonParseError e;
                auto document = QJsonDocument::fromJson(buffer->readAll(), &e);

                if(e.error != QJsonParseError::NoError || !document.isObject()) {
                    n_Status = SpiritEnums::SpiritFile::Status::Errored;
                    emit status(n_Status);
                    emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                    return false;
                }
                buffer->close();

                meta = document.object();
            }
        } else {
            if(fileNameParts.at(0) == "actions" && fileNameParts.size() == 2) {
                if(!action_files.contains(fileNameParts.at(1))) {
                    action_files.append(fileNameParts.at(1));
                    action_buffers.insert(fileNameParts.at(1), (*iter).buffer());
                }
            } else if(fileNameParts.at(0) == "audio" && fileNameParts.size() == 2) {
                if(!audio_files.contains(fileNameParts.at(1))) {
                    audio_files.append(fileNameParts.at(1));
                    audio_buffers.insert(fileNameParts.at(1), (*iter).buffer());
                }
            } else {
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                return false;
            }
        }
    }

    /// Store the meta information
    /// for later use.
    QJsonObject meta_head {
        { "name", meta.value("name").toString() },
        { "edition", "2021" },
        { "version", meta.value("version").toString() },
        { "author", meta.value("author").toString() },
        { "copyright", meta.value("copyright").toString() },
        { "signature", m_Signature }
    };
    m_Meta = meta_head;

    /// Now check the json file.
    if(!meta.contains("name") ||
            !meta.contains("version") ||
            !meta.contains("actions")) {
        n_Status = SpiritEnums::SpiritFile::Status::Errored;
        emit status(n_Status);

        emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
        return false;
    }

    /// Free anything that was left over.
    for(auto iter = m_Actions->begin(),
            end = m_Actions->end();
            iter != end;
            ++iter) {
        delete *iter;
    }
    m_Actions->clear();

    {
        auto json_actions = meta.value("actions").toObject().keys();

        if(json_actions.size() > action_files.size()) {
            n_Status = SpiritEnums::SpiritFile::Status::Errored;
            emit status(n_Status);

            emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
            return false;
        }

        // Check options to each action.
        auto actions = meta.value("actions").toObject();

        if(actions.isEmpty()) {
            n_Status = SpiritEnums::SpiritFile::Status::Errored;
            emit status(n_Status);

            emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
            return false;
        }

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
            QBuffer *play = nullptr;
            bool loop = *iter == "default" ? true : false;
            int speed = 100;
            int scale = 100;
            int yoff = 0;
            int xoff = 0;
            int _yoff = 0;
            int _xoff = 0;
            QString next_action;

            // Get the buffer
            // for the webp file.
            if(!action_files.contains(*iter + ".webp")) {
                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                return false;
            }

            // Check audio file too.
            if(object.contains("play")) {
                if(!audio_files.contains(object.value("play").toString() + ".mp3")) {
                    n_Status = SpiritEnums::SpiritFile::Status::Errored;
                    emit status(n_Status);

                    emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                    return false;
                } else {
                    play = audio_buffers.value(object.value("play").toString() + ".mp3");
                }
            }

            if(object.contains("loop")) {
                loop = object.value("loop").toBool();
            }

            if(object.contains("yoff")) {
                int pixels = object.value("yoff").toInt();
                yoff = pixels;
            }

            if(object.contains("_yoff")) {
                int pixels = object.value("_yoff").toInt();
                _yoff = pixels;
            }

            if(object.contains("xoff")) {
                int pixels = object.value("xoff").toInt();
                xoff = pixels;
            }

            if(object.contains("_xoff")) {
                int pixels = object.value("_xoff").toInt();
                _xoff = pixels;
            }

            if(object.contains("scale")) {
                int percent = object.value("scale").toInt();
                if(percent > 0) {
                    scale = percent;
                }
            }

            if(object.contains("speed")) {
                int percent = object.value("speed").toInt();
                if(percent > 0) {
                    speed = percent;
                }
            }

            if(object.contains("next")) {
                next_action = object.value("next").toString();
            }

            auto action_obj = new Action;
            action_obj->action = *iter;
            action_obj->next_action = next_action;
            action_obj->buffer = action_buffers.value(*iter + ".webp", nullptr);
            action_obj->_buffer = action_buffers.value("_" + *iter + ".webp", nullptr);
            if(action_obj->buffer == nullptr) {
                delete action_obj;

                n_Status = SpiritEnums::SpiritFile::Status::Errored;
                emit status(n_Status);

                emit error(SpiritEnums::SpiritFile::Error::InvalidSpiritFile);
                return false;

            }
            action_obj->play = play;
            action_obj->scale = scale;
            action_obj->speed = speed;
            action_obj->loop = loop;
            action_obj->offsets[0] = xoff;
            action_obj->offsets[1] = yoff;
            action_obj->offsets[2] = _xoff;
            action_obj->offsets[3] = _yoff;

            m_Actions->insert(*iter, action_obj);
        }
    }

    return true;
}
