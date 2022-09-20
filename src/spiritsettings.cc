#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "spiritsettings.hpp"

SpiritSettings::SpiritSettings(QObject *parent) :
    QObject(parent) {
    m_XOffset = qMakePair(0, 0);
    m_YOffset = qMakePair(0, 0);

    auto configPath = QDir::toNativeSeparators(QDir::homePath() + "/.spirit");
    m_SettingsDir = QDir::toNativeSeparators(configPath + "/settings");

    // Check if a file exists with the same
    // name, delete it first.

    if (QFile::exists(configPath)) {
        QFile::remove(configPath);
    }

    if (QFile::exists(m_SettingsDir)) {
        QFile::remove(m_SettingsDir);
    }

    QDir configDir(configPath);
    if (!configDir.exists()) {
        configDir.cdUp();
        configDir.mkdir(".spirit");
        configDir.cd(".spirit");
    }

    if (!QFile::exists(m_SettingsDir)) {
        configDir.mkdir("settings");
    }
}

SpiritSettings::~SpiritSettings() {  }


bool SpiritSettings::setSpiritSignature(QString sign,
                                        int x1, int x2,
                                        int y1, int y2,
                                        int scale,
                                        int speed,
                                        short position) {
    if (sign.isEmpty()) {
        return false;
    }

    if (m_Config.isOpen()) {
        m_Config.close();
    }

    auto fileName = QDir::toNativeSeparators(m_SettingsDir + "/" + sign + ".json");
    m_Config.setFileName(fileName);

    if (!m_Config.open(QIODevice::ReadWrite)) {
        return false;
    }

    auto arr = m_Config.readAll();
    m_Config.close();

    //QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr);

    //if (error.error != QJsonParseError::NoError || !json.isObject()) {
    //  return false;
    //}

    m_Sign = sign;
    auto obj = json.object();

    m_XOffset.first = obj["xoff"].toInt(x1);
    m_XOffset.second = obj["_xoff"].toInt(x2);
    m_YOffset.first = obj["yoff"].toInt(y1);
    m_YOffset.second = obj["_yoff"].toInt(y2);
    m_Scale = obj["scale"].toInt(scale);
    m_Speed = obj["speed"].toInt(speed);
    m_Position = obj["position"].toInt(position);

    auto def = obj["default"].toObject();
    if (def.isEmpty()) {
        m_DefaultValues = QJsonObject {
            {"xoff", x1},
            {"_xoff", x2},
            {"yoff", y1},
            {"_yoff", y2},
            {"scale", scale},
            {"speed", speed},
            {"position", position}
        };
    } else {
        m_DefaultValues = def;
    }

    /*m_DefaultValues = obj["default"].toObject(QJsonObject {
     {"xoff", x1},
     {"_xoff", x2},
     {"yoff", y1},
     {"_yoff", y2},
     {"scale", scale},
     {"speed", speed},
     {"position", position}
    });*/

    return updateJson();
}

bool SpiritSettings::resetDefaults() {
    if (m_Config.isOpen()) {
        m_Config.close();
    }

    auto fileName = QDir::toNativeSeparators(m_SettingsDir + "/" + m_Sign + ".json");
    m_Config.setFileName(fileName);

    if (!m_Config.open(QIODevice::ReadOnly)) {
        return false;
    }

    auto arr = m_Config.readAll();
    m_Config.close();

    QJsonParseError error;
    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        return false;
    }

    auto content = json.object();
    auto obj = content["default"].toObject();

    if (obj.empty()) {
        return false;
    }

    m_XOffset.first = obj["xoff"].toInt(0);
    m_XOffset.second = obj["_xoff"].toInt(0);
    m_YOffset.first = obj["yoff"].toInt(0);
    m_YOffset.second = obj["_yoff"].toInt(0);
    m_Scale = obj["scale"].toInt(100);
    m_Speed = obj["speed"].toInt(100);
    m_Position = obj["position"].toInt(0);
    m_DefaultValues = obj;

    return updateJson();
}

bool SpiritSettings::setXOffset(int x1, int x2) {
    m_XOffset.first = x1;
    if (x2 >= 0) {
        m_XOffset.second = x2;
    }

    return updateJson();
}

bool SpiritSettings::setYOffset(int y1, int y2) {
    m_YOffset.first = y1;
    if (y2 >= 0) {
        m_YOffset.second = y2;
    }

    return updateJson();
}

bool SpiritSettings::setScale(int scale) {
    m_Scale = scale;
    return updateJson();
}

bool SpiritSettings::setSpeed(int speed) {
    m_Speed = speed;
    return updateJson();
}

bool SpiritSettings::setPosition(short pos) {
    m_Position = pos;
    return updateJson();
}

QPair<int, int> SpiritSettings::getXOffset() {
    return m_XOffset;
}

QPair<int, int> SpiritSettings::getYOffset() {
    return m_YOffset;
}

int SpiritSettings::getScale() {
    return m_Scale;
}

int SpiritSettings::getSpeed() {
    return m_Speed;
}

short SpiritSettings::getPosition() {
    return m_Position;
}

// Private Methods
// ---

bool SpiritSettings::updateJson() {
    if (m_Config.isOpen()) {
        m_Config.close();
    }

    auto fileName = QDir::toNativeSeparators(m_SettingsDir + "/" + m_Sign + ".json");
    m_Config.setFileName(fileName);

    if(!m_Config.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(QJsonObject {
        {"xoff", m_XOffset.first},
        {"_xoff", m_XOffset.second},
        {"yoff", m_YOffset.first},
        {"_yoff", m_YOffset.second},
        {"scale", m_Scale},
        {"speed", m_Speed},
        {"position", m_Position},
        {"default", m_DefaultValues}
    });

    auto arr = doc.toJson(QJsonDocument::Indented);
    m_Config.write(arr);
    m_Config.close();

    return true;
}
