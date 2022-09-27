#include <QCoreApplication>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTcpServer>
#include <QDir>
#include <QFile>

#include "spiritdaemon_p.hpp"
#include "CivetServer.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static bool is_daemon_active(int port) {
    bool status = false;
    QNetworkAccessManager manager;
    QEventLoop loop;

    QObject::connect(&manager, &QNetworkAccessManager::finished,
    [&loop, &status](QNetworkReply *reply) {
        if(reply->error() != QNetworkReply::NoError) {
            loop.quit();
            reply->deleteLater();
            return;
        }

        auto arr = reply->readAll();
        QJsonParseError error;
        auto json = QJsonDocument::fromJson(arr, &error);
        if (error.error != QJsonParseError::NoError || !json.isObject()) {
            reply->deleteLater();
            loop.quit();
            return;
        }

        auto obj = json.object();

        if (obj.empty() ||
                !obj.contains("app")) {
            reply->deleteLater();
            loop.quit();
            return;
        }

        auto app = obj["app"].toString();

        if (app == "spirit") {
            status = true;
        }

        reply->deleteLater();
        loop.quit();
    });

    manager.get(QNetworkRequest(QUrl(QString("http://127.1:") + QString::number(port))));

    loop.exec();
    return status;
}

static QString get_free_port() {
    QTcpServer tcpServer;
    if(!tcpServer.listen()) {
        return "4499";
    }

    int randomPort = (int)tcpServer.serverPort();
    tcpServer.close();

    return QString::number(randomPort);
}

static void http_json_header(struct mg_connection *conn) {
    mg_printf(conn,
              "HTTP/1.1 200 OK\r\nContent-Type: "
              "application/json\r\nConnection: close\r\n\r\n");
}

static QJsonObject get_json_body(struct mg_connection *conn) {
    QByteArray body;
    std::vector<char> buffer(1025, 0);

    while(true) {
        int r = mg_read(conn, buffer.data(), 1024);
        if (r <= 0) {
            break;
        }
        body.append(buffer.data());
        std::fill(buffer.begin(), buffer.end(), 0);
    }

    QJsonParseError error;
    auto json = QJsonDocument::fromJson(body, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        return QJsonObject { };
    }

    return json.object();
}

static void write_json(QJsonObject json, struct mg_connection *conn) {
    QJsonDocument doc(json);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    mg_printf(conn, strJson.toUtf8().constData());
}

class IndexHandler : public CivetHandler {
  public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        http_json_header(conn);

        write_json(QJsonObject {
            {"status", "success"},
            {"app", "spirit"},
            {"version", SPIRIT_VERSION},
            {"commit", SPIRIT_COMMIT},
            {"build", SPIRIT_BUILD},
        }, conn);
        return true;
    }
};

class HaltHandler : public CivetHandler {
  public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        http_json_header(conn);

        emit obj->quit();

        write_json(QJsonObject {
            {"status", "success"},
        }, conn);

        return true;
    }
};

class LoadHandler : public CivetHandler {
  public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());

        http_json_header(conn);

        QJsonObject r = obj->m_Meta;

        r.insert("location", obj->m_Spirit);
        r.insert("status", "success");

        write_json(r, conn);
        return true;
    }

    bool handlePost(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        bool success = true;

        http_json_header(conn);

        auto body = get_json_body(conn);
        if(body.isEmpty() || !body.contains("opt")) {
            success = false;
        } else {
            auto operation = body["opt"].toString();

            if (operation == "set") {
                if (body.contains("file")) {
                    QEventLoop loop;

                    QObject::connect(obj, &SpiritDaemonPrivate::cachedSpiritMeta,
                                     &loop, &QEventLoop::quit, Qt::QueuedConnection);

                    emit obj->setSpirit(body["file"].toString());
                    loop.exec();

                } else if (body.contains("url")) {
                    // TODO: URL Downloads.
                } else {
                    success = false;
                }
            } else if (operation == "unset") {
                QEventLoop loop;
                QObject::connect(obj, &SpiritDaemonPrivate::cachedSpiritMeta,
                                 &loop, &QEventLoop::quit, Qt::QueuedConnection);


                emit obj->unsetSpirit();
                loop.exec();
            } else {
                success = false;
            }
        }

        QJsonObject r = obj->m_Meta;
        r.insert("location", obj->m_Spirit);
        r.insert("status", success ? "success" : "failed");
        write_json(r, conn);
        return true;
    }
};

class QuirkHandler : public CivetHandler {
  public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        http_json_header(conn);

        QEventLoop loop;

        QObject::connect(obj, &SpiritDaemonPrivate::cachedQuirks, &loop, &QEventLoop::quit);
        emit obj->requestQuirks();

        loop.exec();

        auto res = obj->m_Quirks;
        res.insert("status", "success");

        write_json(res, conn);
        return true;
    }

    bool handlePost(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        bool success = true;
        http_json_header(conn);

        auto body = get_json_body(conn);
        if(body.isEmpty() || !body.contains("opt")) {
            success = false;
        } else if(body["opt"].toString() == "set") {
            int x = obj->n_X,
                y = obj->n_Y,
                x2 = obj->n_BottomX,
                y2 = obj->n_BottomY;

            if (body.contains("xoffset")) {
                x = body["xoffset"].toInt(obj->n_X);
            }

            if (body.contains("yoffset")) {
                y = body["yoffset"].toInt(obj->n_Y);
            }

            if (body.contains("bottomXOffset")) {
                x2 = body["bottomXOffset"].toInt(obj->n_BottomX);
            }

            if (body.contains("bottomYOffset")) {
                y2 = body["bottomYOffset"].toInt(obj->n_BottomY);
            }

            emit obj->setGlobalOffsets(x, y, x2, y2);

        } else if (body["opt"].toString() == "add") {
            if (body.contains("name")) {
                int x = 0,
                    y = 0,
                    x2 = 0,
                    y2 = 0;
                QString vname;
                if (body.contains("xoffset")) {
                    x = body["xoffset"].toInt(0);
                }

                if (body.contains("yoffset")) {
                    y = body["yoffset"].toInt(0);
                }

                if (body.contains("bottomXOffset")) {
                    x2 = body["bottomXOffset"].toInt(obj->n_BottomX);
                }

                if (body.contains("bottomYOffset")) {
                    y2 = body["bottomYOffset"].toInt(obj->n_BottomY);
                }


                if (body.contains("visibleName")) {
                    vname = body["visibleName"].toString();
                }

                auto name = body["name"].toString();
                emit obj->addQuirk(name, x, y, x2, y2, vname);
            } else {
                success = false;
            }
        } else if (body["opt"].toString() == "remove") {
            if (body.contains("name")) {
                auto name = body["name"].toString();
                emit obj->removeQuirk(name);
            } else {
                success = false;
            }
        } else {
            success = false;
        }

        QEventLoop loop;

        QObject::connect(obj, &SpiritDaemonPrivate::cachedQuirks, &loop, &QEventLoop::quit);
        emit obj->requestQuirks();

        loop.exec();

        auto res = obj->m_Quirks;
        res.insert("status", "success");

        write_json(res, conn);
        return true;
    }
};

class PropertyHandler : public CivetHandler {
  public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        http_json_header(conn);

        QEventLoop loop;

        QObject::connect(obj, &SpiritDaemonPrivate::cachedProps, &loop, &QEventLoop::quit);
        emit obj->requestLatestProperties();

        loop.exec(); // wait for updateProps

        QString position = "topLeft";

        if (obj->m_Position == SpiritEnums::Spirit::Position::TopRight) {
            position = "topRight";
        } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomLeft) {
            position = "bottomLeft";
        } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomRight) {
            position = "bottomRight";
        }

        write_json(QJsonObject {
            {"status", "success"},
            {"position", position},
            {"scale", obj->n_Scale},
            {"speed", obj->n_Speed},
            {"topXOffset", obj->n_x1},
            {"bottomXOffset", obj->n_x2},
            {"topYOffset", obj->n_y1},
            {"bottomYOffset", obj->n_y2},
            {"signature", obj->m_Sign}
        }, conn);

        return true;
    }

    bool handlePost(CivetServer *server, struct mg_connection *conn) {

        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        bool success = true;
        http_json_header(conn);

        auto body = get_json_body(conn);
        if(body.isEmpty() || !body.contains("opt")) {
            success = false;
        } else if (body["opt"].toString() == "set") {
            if (body.contains("position")) {
                short pos = SpiritEnums::Spirit::Position::TopLeft;
                auto position = body["position"].toString();
                if (position == "topRight") {
                    pos = SpiritEnums::Spirit::Position::TopRight;
                } else if (position == "bottomLeft") {
                    pos = SpiritEnums::Spirit::Position::BottomLeft;
                } else if (position == "bottomRight") {
                    pos = SpiritEnums::Spirit::Position::BottomRight;
                }

                emit obj->setPosition(pos);
            }

            if (body.contains("scale")) {
                auto scale = body["scale"].toInt();
                if (scale > 0) {
                    emit obj->setScale(scale);
                }
            }

            if (body.contains("speed")) {
                auto speed = body["speed"].toInt();
                if (speed > 0) {
                    emit obj->setSpeed(speed);
                }
            }

            emit obj->setXOffset(
                body.contains("topXOffset") ?
                body["topXOffset"].toInt() : obj->n_x1,
                body.contains("bottomXOffset") ?
                body["bottomXOffset"].toInt() : obj->n_x2
            );

            emit obj->setYOffset(
                body.contains("topYOffset") ?
                body["topYOffset"].toInt() : obj->n_y1,
                body.contains("bottomYOffset") ?
                body["bottomYOffset"].toInt() : obj->n_y2
            );
        } else if (body["opt"].toString() == "reset") {
            emit obj->resetProperties();
        }

        QEventLoop loop;

        QObject::connect(obj, &SpiritDaemonPrivate::cachedProps, &loop, &QEventLoop::quit);
        emit obj->requestLatestProperties();

        loop.exec(); // wait for updateProps

        QString position = "topLeft";

        if (obj->m_Position == SpiritEnums::Spirit::Position::TopRight) {
            position = "topRight";
        } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomLeft) {
            position = "bottomLeft";
        } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomRight) {
            position = "bottomRight";
        }

        write_json(QJsonObject {
            {"status", success ? "success" : "failed"},
            {"position", position},
            {"scale", obj->n_Scale},
            {"speed", obj->n_Speed},
            {"topXOffset", obj->n_x1},
            {"bottomXOffset", obj->n_x2},
            {"topYOffset", obj->n_y1},
            {"bottomYOffset", obj->n_y2},
            {"signature", obj->m_Sign}
        }, conn);

        return true;
    }
};

class ActionHandler : public CivetHandler {
  public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        http_json_header(conn);

        write_json(QJsonObject {
            {"status", (obj->m_Action.isEmpty() || obj->m_ActionList.isEmpty()) ? "failed" : "success"},
            {"action", obj->m_Action},
            {"actions", QJsonArray::fromStringList(obj->m_ActionList)}
        }, conn);

        return true;
    }

    bool handlePost(CivetServer *server, struct mg_connection *conn) {
        SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
        http_json_header(conn);

        bool success = true;
        QString action = obj->m_Action;

        auto body = get_json_body(conn);

        if (body.isEmpty()) {
            success = false;
        } else {
            if(!body.contains("opt")) {
                success = false;
            } else {
                if(body["opt"].toString() == "set") {
                    if(!body.contains("action")) {
                        success = false;
                    } else {
                        QEventLoop loop;

                        QObject::connect(obj, &SpiritDaemonPrivate::cachedAction,
                                         &loop, &QEventLoop::quit, Qt::QueuedConnection);

                        emit obj->setAction(body["action"].toString());
                        loop.exec();
                        action = obj->m_Action;
                    }
                } else {
                    if(body["opt"].toString() == "reset") {
                        QEventLoop loop;

                        QObject::connect(obj, &SpiritDaemonPrivate::cachedAction,
                                         &loop, &QEventLoop::quit, Qt::QueuedConnection);

                        emit obj->resetAction();
                        loop.exec();
                        action = obj->m_Action;

                    }
                }
            }
        }

        write_json(QJsonObject {
            {"status", success ? "success" : "failed"},
            {"action", action},
            {"actions", QJsonArray::fromStringList(obj->m_ActionList)}
        }, conn);

        return true;
    }
};

SpiritDaemonPrivate::SpiritDaemonPrivate(QObject *parent)
    : QObject(parent),
      b_StopRequested(false) {

}

SpiritDaemonPrivate::~SpiritDaemonPrivate() { }

void SpiritDaemonPrivate::run() {
    mg_init_library(0);
    b_StopRequested = false;

    auto tempFilePath = QDir::toNativeSeparators(
                            QDir::tempPath() + "/" + "com.github.antony-jr.spirit"
                        );

    if (QFile::exists(tempFilePath)) {
        // Possible there is another instance so, let's
        // see if it's a valid one, if not we delete this
        // file.

        QFile file(tempFilePath);
        if(!file.open(QIODevice::ReadOnly)) {
            emit quit();
            return;
        }

        auto arr = file.readAll();
        QString contents(arr);
        file.close();

        contents = contents.trimmed();

        // Now let's request the server and see if it's
        // a valid spirit daemon instance.

        bool ok = true;
        int port = contents.toInt(&ok);

        if (ok) {
            if(is_daemon_active(port)) {
                emit quit();
                return;
            } else {
                file.remove();
            }
        } else {
            file.remove();
        }
    }

    QString port = get_free_port();
    QByteArray portArr = port.toUtf8();

    std::vector<std::string> opts;
    opts.push_back("listening_ports");
    opts.push_back(portArr.constData());

    CivetServer server(/*options*/opts, 0, /*user context*/(void*)this);

    // Write the port in use to the temporary file
    QFile file(tempFilePath);
    if(!file.open(QIODevice::WriteOnly)) {
        emit quit();
        return;
    }

    file.write(portArr);
    file.close();

    IndexHandler index;
    server.addHandler("", index);
    server.addHandler("/spirit", index);
    server.addHandler("/spirit/v1", index);

    HaltHandler halt;
    server.addHandler("/spirit/v1/halt", halt);

    LoadHandler load;
    server.addHandler("/spirit/v1/load", load);

    PropertyHandler prop;
    server.addHandler("/spirit/v1/property", prop);

    ActionHandler action;
    server.addHandler("/spirit/v1/action", action);

    QuirkHandler quirks;
    server.addHandler("/spirit/v1/quirk", quirks);

    emit started();

    while(true) {
        QCoreApplication::processEvents();
        if(b_StopRequested) {
            b_StopRequested = false;
            break;
        }
        QThread::sleep(5);
    }

    file.remove(); // Remove the temporary file
}

void SpiritDaemonPrivate::stop() {
    b_StopRequested = true;
}

void SpiritDaemonPrivate::updateAction(QString action, QList<QString> actions) {
    m_Action = action;
    m_ActionList = actions;
    emit cachedAction();
}

void SpiritDaemonPrivate::updateSpirit(QString loc) {
    m_Spirit = loc;
    emit cachedSpirit();
}

void SpiritDaemonPrivate::updateSpiritMeta(QJsonObject meta) {
    m_Meta = meta;
    emit cachedSpiritMeta();
}

void SpiritDaemonPrivate::updateProps(int x1, int x2,
                                      int y1, int y2,
                                      int scale, int speed,
                                      int position,
                                      QString sign) {
    n_x1 = x1;
    n_x2 = x2;
    n_y1 = y1;
    n_y2 = y2;
    n_Scale = scale;
    n_Speed = speed;
    m_Position = position;
    m_Sign = sign;
    emit cachedProps();
}

void SpiritDaemonPrivate::updateQuirks(QJsonObject obj) {
    m_Quirks = obj;
    auto global = m_Quirks["global"].toObject();

    n_X = global["xoffset"].toInt(0);
    n_Y = global["yoffset"].toInt(0);
    n_BottomX = global["bottomXOffset"].toInt(0);
    n_BottomY = global["bottomYOffset"].toInt(0);
    emit cachedQuirks();
}
