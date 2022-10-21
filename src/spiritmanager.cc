#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QFile>

#include "spiritmanager.hpp"

SpiritManager::SpiritManager(QApplication *app)
    : QObject(nullptr) {
    m_App = app;
    m_Manager = new QNetworkAccessManager(this);

    QObject::connect(m_Manager, &QNetworkAccessManager::finished,
                     this, &SpiritManager::handleAPIResponse);
}

SpiritManager::~SpiritManager() { }

// Starts this program with the argument
// 'daemon', the program should sense this to
// call the daemon method of this class.
void SpiritManager::init() {
    int port = getDaemonPort();

    if (port < 0) {
        emit initialized(false);
        return;
    } else if (port == 0) {
        emit initialized(startDaemonProcess());
        return;
    }

    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port)));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    req.setRawHeader("X-ReqCat", "INIT");

    m_Manager->get(req);
}

bool SpiritManager::daemon() {
    b_Daemon = true;
    m_SpiritApp = new SpiritApp(this);

    QObject::connect(m_SpiritApp, &SpiritApp::quit, m_App, &QApplication::quit);

    return m_SpiritApp->run();
}

void SpiritManager::reinit() {
    QObject::connect(this, &SpiritManager::deinitialized, this, &SpiritManager::handleReinit,
                     Qt::UniqueConnection);
    deinit();
}

void SpiritManager::deinit() {
    int port = getDaemonPort();
    if (port < 0) {
        emit deinitialized(false);
        return;
    } else if (port == 0) {
        emit deinitialized(true);
        return;
    }

    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/halt")));

    req.setRawHeader("X-ReqCat", "DEINIT");

    m_Manager->get(req);

}

bool SpiritManager::waitForInitialized() {
    QEventLoop loop;
    bool result = false;

    auto conn = QObject::connect(this, &SpiritManager::initialized,
    [&loop, &result](bool inits) {
        result = inits;
        loop.quit();
    });

    loop.exec();
    QObject::disconnect(conn);
    return result;
}

bool SpiritManager::waitForDeinitialized() {
    QEventLoop loop;
    bool result = false;

    auto conn = QObject::connect(this, &SpiritManager::deinitialized,
    [&loop, &result](bool deinits) {
        result = deinits;
        loop.quit();
    });

    loop.exec();
    QObject::disconnect(conn);
    return result;
}

void SpiritManager::load(QString file) {
    auto port = getDaemonPort();
    QFileInfo info(file);

    if (port < 1 ||
            !info.exists() ||
            !info.isFile() ||
            !info.isReadable()) {
        emit loaded(false);
        return;
    }

    auto apiRequest = QJsonObject {
        {"opt", "set"},
        {"file", info.absoluteFilePath()}
    };

    auto rawJson = QJsonDocument(apiRequest).toJson();

    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/load")));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-ReqCat", "LOADSET");



    m_Manager->post(req, rawJson);


}

bool SpiritManager::waitForLoaded() {
    QEventLoop loop;
    bool result = false;

    auto conn = QObject::connect(this, &SpiritManager::loaded,
    [&loop, &result](bool loads) {
        result = loads;
        loop.quit();
    });

    loop.exec();
    QObject::disconnect(conn);
    return result;
}

void SpiritManager::ghLoad(QString repo) {
    // TODO: Implement github load feature.
    return;
}

void SpiritManager::getQuirks() {
    auto port = getDaemonPort();
    if (port < 1) {
        emit quirks(false, QJsonObject {});
        return;
    }
    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/quirk")));

    req.setRawHeader("X-ReqCat", "QUIRKSGET");



    m_Manager->get(req);
}


void SpiritManager::getProperties() {
    auto port = getDaemonPort();
    if (port < 1) {
        emit properties(false, QJsonObject {});
        return;
    }
    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/property")));

    req.setRawHeader("X-ReqCat", "PROPERTYGET");



    m_Manager->get(req);
}

QJsonObject SpiritManager::waitForProperties() {
    QEventLoop loop;
    QJsonObject result = QJsonObject { };

    auto conn = QObject::connect(this, &SpiritManager::properties,
    [&loop, &result](bool success, QJsonObject res) {
        Q_UNUSED(success);
        result = res;
        loop.quit();
    });

    loop.exec();
    QObject::disconnect(conn);
    return result;
}

void SpiritManager::addQuirk(QJsonObject obj) {
    auto port = getDaemonPort();
    if (port < 1) {
        emit quirks(false, QJsonObject {});
        return;
    }

    if (!obj.contains("name")) {
        obj.insert("opt", "set");
    } else {
        obj.insert("opt", "add");
    }
    auto rawJson = QJsonDocument(obj).toJson();


    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/quirk")));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-ReqCat", "QUIRKADD");

    m_Manager->post(req, rawJson);
}

void SpiritManager::removeQuirk(QString name) {
    auto port = getDaemonPort();
    if (port < 1) {
        emit quirks(false, QJsonObject {});
        return;
    }

    QJsonObject obj {
        {"opt", "remove"},
        {"name", name}
    };

    auto rawJson = QJsonDocument(obj).toJson();

    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/quirk")));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-ReqCat", "QUIRKREMOVE");

    m_Manager->post(req, rawJson);
}


void SpiritManager::setProperties(QJsonObject obj) {

    auto port = getDaemonPort();
    if (port < 1) {
        emit updatedProperties(false, QJsonObject {});
        return;
    }


    obj.insert("opt", "set");
    auto rawJson = QJsonDocument(obj).toJson();


    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/property")));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-ReqCat", "PROPERTYSET");



    m_Manager->post(req, rawJson);
}

QJsonObject SpiritManager::waitForUpdatedProperties() {
    QEventLoop loop;
    QJsonObject result = QJsonObject { };

    auto conn = QObject::connect(this, &SpiritManager::updatedProperties,
    [&loop, &result](bool success, QJsonObject res) {
        Q_UNUSED(success);
        result = res;
        loop.quit();
    });

    loop.exec();
    QObject::disconnect(conn);
    return result;
}

void SpiritManager::resetProperty() {

    auto port = getDaemonPort();
    if (port < 1) {
        emit updatedProperties(false, QJsonObject {});
        return;
    }

    QJsonObject obj {
        {"opt", "reset"}
    };
    auto rawJson = QJsonDocument(obj).toJson();


    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/property")));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-ReqCat", "PROPERTYRESET");

    m_Manager->post(req, rawJson);
}


void SpiritManager::getLoadedSpiritInfo() {
    auto port = getDaemonPort();
    if (port < 1) {
        emit loadedSpiritInfo(false, QJsonObject {});
        return;
    }

    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/load")));

    req.setRawHeader("X-ReqCat", "LOADEDGET");

    m_Manager->get(req);
}

QJsonObject SpiritManager::waitForLoadedSpiritInfo() {
    QEventLoop loop;
    QJsonObject result = QJsonObject { };

    auto conn = QObject::connect(this, &SpiritManager::loadedSpiritInfo,
    [&loop, &result](bool success, QJsonObject res) {
        Q_UNUSED(success);
        result = res;
        loop.quit();
    });

    loop.exec();
    QObject::disconnect(conn);
    return result;
}

void SpiritManager::getActions() {
    auto port = getDaemonPort();
    if (port < 1) {
        emit properties(false, QJsonObject {});
        return;
    }
    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/action")));

    req.setRawHeader("X-ReqCat", "ACTIONSGET");



    m_Manager->get(req);
}


void SpiritManager::getAction() {
    auto port = getDaemonPort();
    if (port < 1) {
        emit properties(false, QJsonObject {});
        return;
    }
    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/action")));

    req.setRawHeader("X-ReqCat", "ACTIONGET");



    m_Manager->get(req);
}


void SpiritManager::setAction(QString act) {
    auto port = getDaemonPort();
    if (port < 1) {
        emit updatedProperties(false, QJsonObject {});
        return;
    }

    QJsonObject obj {
        {"opt", "set"},
        {"action", act}
    };

    auto rawJson = QJsonDocument(obj).toJson();


    QNetworkRequest req(QUrl(
                            QString("http://127.1:") + QString::number(port) + QString("/spirit/v1/action")));

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-ReqCat", "ACTIONSET");


    m_Manager->post(req, rawJson);
}



int SpiritManager::getPort() {
    return getDaemonPort();
}

int SpiritManager::exec() {
    if (b_Daemon) {
        return m_App->exec();
    }

    return 0;
}

// Private Slots
// ---

int SpiritManager::getDaemonPort() {
    auto tempFilePath = QDir::toNativeSeparators(
                            QDir::tempPath() + "/" + "com.github.antony-jr.spirit"
                        );

    if (QFile::exists(tempFilePath)) {
        QFile file(tempFilePath);
        if(!file.open(QIODevice::ReadOnly)) {
            return -1;
        }

        auto arr = file.readAll();
        QString contents(arr);
        file.close();

        contents = contents.trimmed();

        bool ok = true;
        int port = contents.toInt(&ok);

        if (ok) {
            return port;
        }
    }
    return 0;
}

bool SpiritManager::startDaemonProcess() {
#ifdef Q_OS_LINUX
    QString programPath;
    auto appimage = std::getenv("APPIMAGE");
    if(appimage) {
        programPath = QString::fromUtf8(appimage);
    } else {
        programPath = QCoreApplication::applicationFilePath();
    }
#else
    QString programPath = QCoreApplication::applicationFilePath();
#endif

#ifdef Q_OS_MAC
    QDir dir = QDir(programPath);
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();

    QString bundle = dir.absolutePath();
    QString configPath = QDir::toNativeSeparators(QDir::homePath() + "/.spirit");
    configPath = QDir::toNativeSeparators(configPath + "/daemon.app");

    QProcess::execute("rm", QStringList() << "-rf" << configPath);
    QProcess::execute("cp", QStringList() << "-r" << bundle
                      << configPath);
    QString infolist = QDir::toNativeSeparators(configPath + "/Contents/Info.plist");

    QFile source(":build_resources/Daemon-Info.plist");
    if (!source.open(QIODevice::ReadOnly)) {
        return false;
    }

    QFile file(infolist);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(source.readAll());

    source.close();
    file.close();

    programPath = QDir::toNativeSeparators(configPath + "/Contents/MacOS/spirit");
#endif

    QStringList arguments;
    arguments << "daemon";


    return QProcess::startDetached(programPath, arguments);

}

void SpiritManager::handleAPIResponse(QNetworkReply *reply) {
    auto request = reply->request();
    auto cat = QString(request.rawHeader("X-ReqCat"));

    if (cat == "INIT") {
        handleInitReply(reply);
    } else if (cat == "DEINIT") {
        handleDeinitReply(reply);
    } else if (cat == "LOADSET") {
        handleLoadSetReply(reply);
    } else if(cat == "QUIRKSGET" || cat == "QUIRKADD" || cat == "QUIRKREMOVE") {
        handleGetQuirksReply(reply);
    } else if (cat == "PROPERTYGET" || cat == "PROPERTYRESET") {
        handleGetPropertyReply(reply);
    } else if (cat == "PROPERTYSET") {
        handleSetPropertyReply(reply);
    } else if (cat == "LOADEDGET") {
        handleSpiritInfo(reply);
    } else if (cat == "ACTIONGET") {
        handleGetAction(reply);
    } else if (cat == "ACTIONSGET") {
        handleGetActions(reply);
    } else if (cat == "ACTIONSET") {
        handleGetAction(reply);
    } else {
        reply->deleteLater();
    }
}

void SpiritManager::handleReinit(bool success) {
    QObject::disconnect(this, &SpiritManager::deinitialized, this, &SpiritManager::handleReinit);
    if (!success) {
        emit initialized(false);
        return;
    }

    init();
}

void SpiritManager::handleDeinitReply(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit deinitialized(false);
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit deinitialized(false);
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit deinitialized(false);
        return;
    }

    auto stat = obj["status"].toString();

    reply->deleteLater();
    emit deinitialized(stat == "success");
}

void SpiritManager::handleInitReply(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit initialized(startDaemonProcess());
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit initialized(false);
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("app")) {
        reply->deleteLater();
        emit initialized(false);
        return;
    }

    auto app = obj["app"].toString();

    reply->deleteLater();
    emit initialized(app == "spirit");
}

void SpiritManager::handleLoadSetReply(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit loaded(false);
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit loaded(false);
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit loaded(false);
        return;
    }

    auto stat = obj["status"].toString();

    reply->deleteLater();
    emit loaded(stat == "success");
}


void SpiritManager::handleGetQuirksReply(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit quirks(false, QJsonObject {});
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit quirks(false, QJsonObject {});
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit quirks(false, QJsonObject {});
        return;
    }

    auto stat = obj["status"].toString();

    reply->deleteLater();
    emit quirks(stat == "success", obj);
}


void SpiritManager::handleGetPropertyReply(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit properties(false, QJsonObject {});
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit properties(false, QJsonObject {});
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit properties(false, QJsonObject {});
        return;
    }

    auto stat = obj["status"].toString();

    reply->deleteLater();
    emit properties(stat == "success", obj);
}


void SpiritManager::handleSetPropertyReply(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit updatedProperties(false, QJsonObject {});
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit updatedProperties(false, QJsonObject {});
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit updatedProperties(false, QJsonObject {});
        return;
    }

    auto stat = obj["status"].toString();

    reply->deleteLater();
    emit updatedProperties(stat == "success", obj);
}

void SpiritManager::handleSpiritInfo(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit loadedSpiritInfo(false, QJsonObject {});
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit loadedSpiritInfo(false, QJsonObject {});
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit loadedSpiritInfo(false, QJsonObject {});
        return;
    }

    auto stat = obj["status"].toString();

    reply->deleteLater();
    emit loadedSpiritInfo(stat == "success", obj);
}


void SpiritManager::handleGetAction(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit action(QString());
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit action(QString());
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit action(QString());
        return;
    }

    auto stat = obj["status"].toString();
    auto act = obj["action"].toString();

    reply->deleteLater();
    emit action(act);
}


void SpiritManager::handleGetActions(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit actions(QStringList(), QString());
        return;
    }

    auto arr = reply->readAll();
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        reply->deleteLater();
        emit actions(QStringList(), QString());
        return;
    }

    auto obj = json.object();

    if (obj.empty() ||
            !obj.contains("status")) {
        reply->deleteLater();
        emit actions(QStringList(), QString());
        return;
    }

    auto stat = obj["status"].toString();
    auto act = obj["action"].toString();
    auto action_arr = obj["actions"].toArray();

    reply->deleteLater();

    QStringList acts;

    for (auto action : action_arr) {
        acts << action.toString();
    }

    emit actions(acts, act);
}

