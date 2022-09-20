#include <QDir>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "spiritconfig.hpp"

SpiritConfig::SpiritConfig(QObject *parent) :
    QObject(parent) {
    auto configPath = QDir::toNativeSeparators(QDir::homePath() + "/.spirit");
    m_ConfigPath = QDir::toNativeSeparators(configPath + "/config.json");

    if (QFile::exists(configPath)) {
        QFile::remove(configPath);
    }

    QDir configDir(configPath);
    if (!configDir.exists()) {
        configDir.cdUp();
        configDir.mkdir(".spirit");
        configDir.cd(".spirit");
    }

    if (!QFile::exists(m_ConfigPath)) {
        updateJson();
    }
}

SpiritConfig::~SpiritConfig() { }

bool SpiritConfig::read() {
    QFile file;
    file.setFileName(m_ConfigPath);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    auto arr = file.readAll();
    file.close();

    QJsonParseError error;

    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        return false;
    }

    auto obj = json.object();
    if (obj.empty()) {
        updateJson();
        return read();
    }

    b_RunOnStartup = obj["runOnStartup"].toBool(false);
    m_DefaultSpiritFile = obj["defaultSpirit"].toString(":default.spirit");

    m_AllowedPrograms.clear();
    auto allowedArray = obj["allowedPrograms"].toArray();

    for (auto value : allowedArray) {
        m_AllowedPrograms << value.toString();
    }

    return true;
}

bool SpiritConfig::isRunOnStartup() {
    return b_RunOnStartup;
}

QString SpiritConfig::getDefaultSpiritFile() {
    return m_DefaultSpiritFile;
}

QStringList SpiritConfig::getAllowedPrograms() {
    return m_AllowedPrograms;
}


bool SpiritConfig::setDefaultSpiritFile(QString file) {
    m_DefaultSpiritFile = file;
    return updateJson();
}

bool SpiritConfig::setRunOnStartup(bool ch) {
    b_RunOnStartup = ch;
    return updateJson();
}

bool SpiritConfig::addAllowedProgram(QString program) {
    m_AllowedPrograms << program;
    return updateJson();
}

bool SpiritConfig::removeAllowedProgram(int indx) {
    m_AllowedPrograms.removeAt(indx);
    return updateJson();
}

bool SpiritConfig::updateJson() {
    QFile file;
    file.setFileName(m_ConfigPath);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(QJsonObject {
        {"runOnStartup", b_RunOnStartup},
        {"defaultSpirit", m_DefaultSpiritFile},
        {"allowedPrograms", QJsonArray::fromStringList(m_AllowedPrograms)}
    });

    auto arr = doc.toJson(QJsonDocument::Indented);
    file.write(arr);
    file.close();

    return true;
}

