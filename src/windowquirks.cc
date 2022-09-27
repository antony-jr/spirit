#include <QtGlobal>
#include <QSysInfo>
#include <QString>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonParseError>

#include <QFile>
#include <QDir>

#include "windowquirks.hpp"

// Window Quirks helps to set error yoffset for specific programs
// since in linux every program acts very different so it is very
// hard to find the height of a program accurately, like in case
// of ubuntu where every program has window shadow which makes a 5
// pixel error in the height which makes the spirit look like it's
// hanging. And Qt programs in Ubuntu does not have this shadow
// making it even more weird for me to guess anything so quirk list
// to handle these cases as per the user wishes.

WindowQuirks::WindowQuirks(QObject *parent)
    : QObject(parent) {

    auto configPath = QDir::toNativeSeparators(QDir::homePath() + "/.spirit");
    m_QuirkFilePath = QDir::toNativeSeparators(configPath + "/quirks.json");

    if (QFile::exists(configPath)) {
        QFile::remove(configPath);
    }

    QDir configDir(configPath);
    if (!configDir.exists()) {
        configDir.cdUp();
        configDir.mkdir(".spirit");
        configDir.cd(".spirit");
    }

    if (!QFile::exists(m_QuirkFilePath)) {
        updateJson();
    }
}

WindowQuirks::~WindowQuirks() { }

bool WindowQuirks::read() {
    QFile file;
    file.setFileName(m_QuirkFilePath);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    auto arr = file.readAll();
    file.close();

    QJsonParseError error;

    auto json = QJsonDocument::fromJson(arr, &error);
    if (error.error != QJsonParseError::NoError || !json.isObject()) {
        updateJson();
        return read();
    }

    auto obj = json.object();
    if (obj.empty()) {
        updateJson();
        return read();
    }

    m_ProgramQuirks.clear();
    auto keys = obj.keys();
    for (auto key : keys) {
        auto value = obj[key].toObject();

        if (key == "global") {
            n_X = value["xoffset"].toInt(0);
            n_Y = value["yoffset"].toInt(0);
            n_BottomX = value["bottomXOffset"].toInt(0);
            n_BottomY = value["bottomYOffset"].toInt(0);
            continue;
        }

        Entry e;
        e.n_X = value["xoffset"].toInt(0);
        e.n_Y = value["yoffset"].toInt(0);
        e.n_BottomX = value["bottomXOffset"].toInt(0);
        e.n_BottomY = value["bottomYOffset"].toInt(0);

        e.m_VisibleName = value["visibleName"].toString();

        m_ProgramQuirks.insert(key, e);
    }

    return true;
}

bool WindowQuirks::setGlobalXOffset(int x1, int x2) {
    n_X = x1;
    n_BottomX = x2;
    return updateJson();
}

bool WindowQuirks::setGlobalYOffset(int y1, int y2) {
    n_Y = y1;
    n_BottomY = y2;
    return updateJson();
}

QJsonObject WindowQuirks::getQuirk(QString name) {
    if (name.isEmpty()) {
        return QJsonObject {
            {"xoffset", n_X},
            {"yoffset", n_Y},
            {"bottomXOffset", n_BottomX},
            {"bottomYOffset", n_BottomY},
            {"visibleName", ""},
            {"exists", false}
        };
    }

    auto iter = m_ProgramQuirks.find(name);
    if (iter == m_ProgramQuirks.end()) {
        return QJsonObject {
            {"xoffset", n_X},
            {"yoffset", n_Y},
            {"bottomXOffset", n_BottomX},
            {"bottomYOffset", n_BottomY},
            {"visibleName", ""},
            {"exists", false}
        };
    }

    auto e = iter.value();

    return QJsonObject {
        {"xoffset", e.n_X},
        {"yoffset", e.n_Y},
        {"bottomXOffset", e.n_BottomX},
        {"bottomYOffset", e.n_BottomY},
        {"visibleName", e.m_VisibleName},
        {"exists", true}
    };
}

void WindowQuirks::getQuirks() {
    QJsonObject r;

    r.insert("global", QJsonObject {
        {"xoffset", n_X},
        {"yoffset", n_Y},
        {"bottomXOffset", n_BottomX},
        {"bottomYOffset", n_BottomY}
    });

    for(auto iter = m_ProgramQuirks.constBegin();
            iter != m_ProgramQuirks.constEnd();
            ++iter) {
        auto e = iter.value();
        r.insert(iter.key(), QJsonObject {
            {"xoffset", e.n_X},
            {"yoffset", e.n_Y},
            {"bottomXOffset", e.n_BottomX},
            {"bottomYOffset", e.n_BottomY},
            {"visibleName", e.m_VisibleName}
        });
    }

    emit quirks(r);
}

bool WindowQuirks::addQuirk(QString name, int x, int y, int x2, int y2, QString visibleName) {
    Entry e;
    e.n_X = x;
    e.n_Y = y;
    e.n_BottomX = x2;
    e.n_BottomY = y2;
    e.m_VisibleName = visibleName;
    auto iter = m_ProgramQuirks.find(name);
    if (iter != m_ProgramQuirks.end()) {
        m_ProgramQuirks.erase(iter);
    }
    m_ProgramQuirks.insert(name, e);
    return updateJson();
}

bool WindowQuirks::removeQuirk(QString name) {
    auto iter = m_ProgramQuirks.find(name);
    if (iter != m_ProgramQuirks.end()) {
        m_ProgramQuirks.erase(iter);
    }

    return updateJson();
}

bool WindowQuirks::updateJson() {
    QFile file;
    file.setFileName(m_QuirkFilePath);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }


    QJsonObject r;

    r.insert("global", QJsonObject {
        {"xoffset", n_X},
        {"yoffset", n_Y},
        {"bottomXOffset", n_BottomX},
        {"bottomYOffset", n_BottomY}
    });

    for(auto iter = m_ProgramQuirks.constBegin();
            iter != m_ProgramQuirks.constEnd();
            ++iter) {
        auto e = iter.value();
        r.insert(iter.key(), QJsonObject {
            {"xoffset", e.n_X},
            {"yoffset", e.n_Y},
            {"bottomXOffset", e.n_BottomX},
            {"bottomYOffset", e.n_BottomY},
            {"visibleName", e.m_VisibleName}
        });
    }

    QJsonDocument doc(r);

    auto arr = doc.toJson(QJsonDocument::Indented);
    file.write(arr);
    file.close();

    return true;
}

