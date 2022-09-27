#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QTimer>
#include <QDir>

#include "guiapp.hpp"

GuiApp::GuiApp(QWidget *parent) :
    QMainWindow(parent) {
    m_UI = new Ui::SpiritMainWindow;
    m_SettingsDialogUI = new Ui::settingsDialog;
    m_QuirksDialogUI = new Ui::quirksDialog;
    m_Manager = new SpiritManager(qApp);
    m_Config = new SpiritConfig(this);
    m_SettingsDialog = new QDialog(this);
    m_QuirksDialog = new QDialog(this);
    m_ProgressDialog = new QProgressDialog(this);
    m_ProgressDialog->setMinimum(0);
    m_ProgressDialog->setMaximum(0);
    m_ProgressDialog->setMinimumWidth(400);
    m_ProgressDialog->setMinimumHeight(120);

    m_UI->setupUi(this);
    m_SettingsDialogUI->setupUi(m_SettingsDialog);
    m_QuirksDialogUI->setupUi(m_QuirksDialog);

    m_AllowedProgramsModel = new QStringListModel(this);
    m_QuirksModel = new QStringListModel(this);


    m_SettingsDialogUI->programsListView->setModel(m_AllowedProgramsModel);
    m_QuirksDialogUI->quirksListView->setModel(m_QuirksModel);

    connect(m_ProgressDialog, &QProgressDialog::canceled,
            this, &GuiApp::handleProgressCancel);

    connect(m_Manager, &SpiritManager::quirks,
            this, &GuiApp::handleQuirkData);

    connect(m_QuirksDialogUI->quirksListView, &QListView::activated,
            this, &GuiApp::handleQuirkSelected);

    connect(m_QuirksDialogUI->addBtn, &QPushButton::pressed,
            this, &GuiApp::handleAddQuirk);

    connect(m_QuirksDialogUI->removeBtn, &QPushButton::pressed,
            this, &GuiApp::handleRemoveQuirk);

    connect(m_SettingsDialogUI->programsListView, &QListView::activated,
            this, &GuiApp::handleAllowedProgramSelected);

    connect(m_SettingsDialogUI->addBtn, &QPushButton::pressed,
            this, &GuiApp::handleAddAllowedProgram);

    connect(m_SettingsDialogUI->removeBtn, &QPushButton::pressed,
            this, &GuiApp::handleRemoveAllowedProgram);


    connect(m_SettingsDialog, &QDialog::finished,
            this, &GuiApp::handleSettingsFinished);

    connect(m_UI->quitBtn, &QPushButton::pressed,
            m_Manager, &SpiritManager::deinit);

    connect(m_UI->actionQuirks, &QAction::triggered,
            this, &GuiApp::handleQuirks);

    connect(m_UI->actionSettings, &QAction::triggered,
            this, &GuiApp::handleSettings);

    connect(m_UI->actionLoad, &QAction::triggered,
            this, &GuiApp::handleLoad);

    connect(m_UI->actionAbout, &QAction::triggered,
            this, &GuiApp::handleDocs);

    connect(m_UI->actionAbout_2, &QAction::triggered,
            this, &GuiApp::handleAbout);

    connect(m_SettingsDialogUI->selectDefaultSpiritBtn, &QPushButton::pressed,
            this, &GuiApp::handleDefaultSpiritSelect);

    connect(m_Manager, &SpiritManager::loaded,
            m_Manager, &SpiritManager::getProperties);

    connect(m_UI->actionQuit, &QAction::triggered,
            m_Manager, &SpiritManager::deinit);

    connect(m_UI->applyBtn, &QPushButton::pressed,
            this, &GuiApp::handleApplyBtn);

    connect(m_UI->resetBtn, &QPushButton::pressed,
            this, &GuiApp::handleResetBtn);

    connect(m_Manager, &SpiritManager::loadedSpiritInfo,
            this, &GuiApp::handleSpiritInfo);

    connect(m_Manager, &SpiritManager::initialized,
            this, &GuiApp::handleInit);

    connect(m_Manager, &SpiritManager::deinitialized,
            this, &GuiApp::handleDeinit);

    connect(m_Manager, &SpiritManager::actions,
            this, &GuiApp::handleActions);

    connect(m_Manager, &SpiritManager::properties,
            this, &GuiApp::handleProperties);

    connect(m_Manager, &SpiritManager::updatedProperties,
            this, &GuiApp::updateProperties, Qt::QueuedConnection);
}

GuiApp::~GuiApp() {
    delete m_UI;
    delete m_SettingsDialogUI;
    delete m_QuirksDialogUI;
    m_Manager->deleteLater();
}

void GuiApp::init() {
    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Initializing Spirit");
    m_ProgressDialog->open();
    m_Manager->init();
}

void GuiApp::handleInit(bool ok) {
    if (!ok) {
        emit quit();
        return;
    }

    // m_ProgressDialog->setLabelText("Fetching Information");

    m_Config->read();
    m_Manager->getProperties();
}

void GuiApp::handleDeinit(bool ok) {
    Q_UNUSED(ok);
    emit quit();
    return;
}

void GuiApp::handleActions(QStringList actions, QString action) {
    int selected = 0;
    int c = 0;

    m_UI->actionComboBox->clear();

    for (auto act : actions) {
        m_UI->actionComboBox->insertItem(c, act);
        if (act == action) {
            selected = c;
        }
        ++c;
    }

    m_UI->actionComboBox->setCurrentIndex(selected);
}

void GuiApp::handleSpiritInfo(bool ok, QJsonObject obj) {
    if(!ok) {
        return;
    }

    m_UI->spiritName->setText(obj["name"].toString());
    m_UI->copyrightLbl->setText(obj["copyright"].toString());
    m_ProgressDialog->cancel();
}

void GuiApp::handleProperties(bool ok, QJsonObject obj) {
    updateProperties(ok, obj);
    if (isHidden() && ok) {
        show();
    }
}

void GuiApp::updateProperties(bool ok, QJsonObject obj) {
    if (!ok) {

        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(500);

        connect(timer, &QTimer::timeout, m_Manager, &SpiritManager::getProperties);

        timer->start();

        return ;
    }

    m_Manager->getActions();
    m_Manager->getLoadedSpiritInfo();

    m_UI->scaleSlider->setValue(obj["scale"].toInt(m_UI->scaleSlider->value()));

    auto pos = obj["position"].toString();

    if (pos == "topLeft") {
        m_UI->positionComboBox->setCurrentIndex(0);
    } else if (pos == "topRight") {
        m_UI->positionComboBox->setCurrentIndex(1);
    } else if (pos == "bottomLeft") {
        m_UI->positionComboBox->setCurrentIndex(2);
    } else {
        m_UI->positionComboBox->setCurrentIndex(3);
    }

    m_UI->x1offset->setValue(obj["topXOffset"].toInt(m_UI->x1offset->value()));
    m_UI->x2offset->setValue(obj["bottomXOffset"].toInt(m_UI->x2offset->value()));

    m_UI->y1offset->setValue(obj["topYOffset"].toInt(m_UI->y1offset->value()));
    m_UI->y2offset->setValue(obj["bottomYOffset"].toInt(m_UI->y2offset->value()));
}

void GuiApp::handleLoad() {
    auto fileName = QFileDialog::getOpenFileName(this,
                    tr("Open Spirit File"), QDir::homePath(), tr("Spirit Files (*.spirit)"));

    if (fileName.isEmpty()) {
        return;
    }

    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Loading Spirit File");
    m_ProgressDialog->open();

    m_Manager->load(fileName);
}

void GuiApp::handleDocs() {
    QDesktopServices::openUrl(QUrl("https://antonyjr.in/spirit"));
}

void GuiApp::handleAbout() {
    QStringList s;
    s << "Spirit "
      << SPIRIT_VERSION
      << " commit-"
      << SPIRIT_COMMIT
      << " (build "
      << SPIRIT_BUILD
      << "),\n"
      << "Copyright (C) 2021, D. Antony J.R <antonyjr@pm.me>.\n\n"
      << "This program is free software: you can redistribute it and/or modify "
      << "it under the terms of the GNU General Public License as published by "
      << "the Free Software Foundation, either version 3 of the License, or "
      << "any later version.\n";

    QMessageBox::about(this, "About Spirit", s.join(""));
}

void GuiApp::handleDefaultSpiritSelect() {
    auto fileName = QFileDialog::getOpenFileName(m_SettingsDialog,
                    tr("Open Spirit File"), QDir::homePath(), tr("Spirit Files (*.spirit)"));

    if (fileName.isEmpty()) {
        return;
    }

    m_DefaultSpirit = fileName;
    m_SettingsDialogUI->defaultSpiritText->setText(fileName);
}

void GuiApp::handleAllowedProgramSelected(QModelIndex index) {
    if (!index.isValid()) {
        return;
    }

    int i = index.row();
    n_AllowedProgramRow = i;

    auto progs = m_AllowedProgramsModel->stringList();

    m_SettingsDialogUI->lineEdit->setText(progs.at(i));
    m_SettingsDialogUI->removeBtn->setEnabled(true);
}

void GuiApp::handleRemoveAllowedProgram() {
    auto progs = m_AllowedProgramsModel->stringList();
    progs.removeAt(n_AllowedProgramRow);
    m_AllowedProgramsModel->setStringList(progs);
    m_Config->removeAllowedProgram(n_AllowedProgramRow);
}

void GuiApp::handleAddAllowedProgram() {
    m_RecentAllowed << m_SettingsDialogUI->lineEdit->text();
    auto progs = m_AllowedProgramsModel->stringList();
    progs << m_SettingsDialogUI->lineEdit->text();
    m_AllowedProgramsModel->setStringList(progs);
}

void GuiApp::handleSettings() {
    m_RecentAllowed.clear();
    m_DefaultSpirit.clear();

    n_AllowedProgramRow = 0;


    m_DefaultSpirit = m_Config->getDefaultSpiritFile();
    m_SettingsDialogUI->defaultSpiritText->setText(m_DefaultSpirit);
    m_SettingsDialogUI->runOnStartupCheckbox->setCheckState(
        m_Config->isRunOnStartup() == true ? Qt::Checked : Qt::Unchecked
    );
    m_AllowedProgramsModel->setStringList(m_Config->getAllowedPrograms());
    m_SettingsDialog->open();
}

void GuiApp::handleSettingsFinished(int result) {
    if (result == QDialog::Accepted) {
        m_Config->setDefaultSpiritFile(m_DefaultSpirit);
        for (auto prog : m_RecentAllowed) {
            m_Config->addAllowedProgram(prog);
        }
        m_Config->setRunOnStartup(m_SettingsDialogUI->runOnStartupCheckbox->checkState()
                                  == Qt::Checked ? true  : false);

        if(m_Config->isRunOnStartup()) {
#ifdef Q_OS_LINUX

            QStringList desktopFile;
            QString programPath;
            auto appimage = std::getenv("APPIMAGE");
            if(appimage) {
                programPath = QString::fromUtf8(appimage);
            } else {
                programPath = QCoreApplication::applicationFilePath();
            }

            desktopFile << "[Desktop Entry]\n"
                        << "Name=Spirit\n"
                        << "Type=Application\n"
                        << "Exec="
                        << programPath
                        << " daemon"
                        << "\n"
                        << "Terminal=false\n";

            QFile entryFile(
                QDir::homePath() +
                QString::fromUtf8("/.config/autostart/Spirit.desktop"));
            if(!entryFile.open(QIODevice::WriteOnly)) {
                return;
            }
            entryFile.write(desktopFile.join("").toLatin1());
            entryFile.close();
#endif // LINUX
        } else {
#ifdef Q_OS_LINUX
            QFile::remove(
                QDir::homePath() +
                QString::fromUtf8("/.config/autostart/Spirit.desktop"));

#endif // LINUX
        }
    }
}

void GuiApp::handleQuirkSelected(QModelIndex index) {
    if (!index.isValid()) {
        return;
    }

    int i = index.row();
    auto lst = m_QuirksModel->stringList();
    auto key = lst.at(i);
    m_LastSelectedQuirk = key;

    auto value = m_QuirkData[key].toObject();
    m_QuirksDialogUI->programLineEdit->setText(key == "global" ? "" : key);
    m_QuirksDialogUI->vnameLineEdit->setText(value["visibleName"].toString());
    m_QuirksDialogUI->xoffset->setValue(value["xoffset"].toInt());
    m_QuirksDialogUI->bottomXOffset->setValue(value["bottomXOffset"].toInt());
    m_QuirksDialogUI->yoffset->setValue(value["yoffset"].toInt());
    m_QuirksDialogUI->bottomYOffset->setValue(value["bottomYOffset"].toInt());
}

void GuiApp::handleAddQuirk() {
    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Updating Quirks");
    m_ProgressDialog->open();

    QString name = m_QuirksDialogUI->programLineEdit->text(),
            vname =   m_QuirksDialogUI->vnameLineEdit->text();

    m_LastSelectedQuirk = name;

    m_Manager->addQuirk(QJsonObject {
        {"name", name.isEmpty() ? "global" : name},
        {"visibleName", vname},
        {"xoffset", m_QuirksDialogUI->xoffset->value()},
        {"bottomXOffset", m_QuirksDialogUI->bottomXOffset->value()},
        {"yoffset", m_QuirksDialogUI->yoffset->value()},
        {"bottomYOffset", m_QuirksDialogUI->bottomYOffset->value()}
    });
}

void GuiApp::handleRemoveQuirk() {
    auto lst = m_QuirksModel->stringList();
    auto key = lst.at(n_QuirkSelected);
    m_LastSelectedQuirk = "global";

    if (key == "global") {
        return;
    }

    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Removing Quirk");
    m_ProgressDialog->open();


    m_Manager->removeQuirk(key);
}

void GuiApp::handleQuirkData(bool ok, QJsonObject obj) {
    if (!ok) {
        m_QuirksModel->setStringList(QStringList());
        m_ProgressDialog->cancel();
        return;
    }

    Q_UNUSED(obj.take("status"));
    m_QuirkData = obj;
    auto lst = obj.keys();
    m_QuirksModel->setStringList(lst);

    QString showKey = "global";
    if (obj.contains(m_LastSelectedQuirk)) {
        showKey = m_LastSelectedQuirk;
    }

    auto value = obj[showKey].toObject();
    m_QuirksDialogUI->programLineEdit->setText(showKey == "global" ? "" : showKey);
    m_QuirksDialogUI->vnameLineEdit->setText(value["visibleName"].toString());
    m_QuirksDialogUI->xoffset->setValue(value["xoffset"].toInt());
    m_QuirksDialogUI->bottomXOffset->setValue(value["bottomXOffset"].toInt());
    m_QuirksDialogUI->yoffset->setValue(value["yoffset"].toInt());
    m_QuirksDialogUI->bottomYOffset->setValue(value["bottomYOffset"].toInt());

    m_ProgressDialog->cancel();
    m_QuirksDialog->open();
}

void GuiApp::handleQuirks() {
    m_LastSelectedQuirk = "global";
    m_QuirksModel->setStringList(QStringList());
    m_QuirksDialogUI->programLineEdit->setText("");
    m_QuirksDialogUI->vnameLineEdit->setText("");

    m_Manager->getQuirks();
    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Fetching Quirks");
    m_ProgressDialog->open();
}

void GuiApp::handleApplyBtn() {

    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Applying Properties");
    m_ProgressDialog->open();

    auto index = m_UI->positionComboBox->currentIndex();
    QString pos = "bottomRight";
    if (index == 0) {
        pos = "topLeft";
    } else if (index == 1) {
        pos = "topRight";
    } else if (index == 2) {
        pos = "bottomLeft";
    }

    m_Manager->setAction(m_UI->actionComboBox->itemText(m_UI->actionComboBox->currentIndex()));

    m_Manager->setProperties(QJsonObject {
        {"position", pos},
        {"topXOffset", m_UI->x1offset->value()},
        {"bottomXOffset", m_UI->x2offset->value()},
        {"topYOffset", m_UI->y1offset->value()},
        {"bottomYOffset", m_UI->y2offset->value()},
        {"scale", m_UI->scaleSlider->value()}
    });
}

void GuiApp::handleResetBtn() {

    centerDialog(m_ProgressDialog);
    m_ProgressDialog->reset();
    m_ProgressDialog->setLabelText("Applying Properties");
    m_ProgressDialog->open();

    m_Manager->resetProperty();
}

void GuiApp::handleProgressCancel() {

}

void GuiApp::centerDialog(QDialog *d) {
    auto rec = QGuiApplication::screenAt(this->pos())->geometry();
    auto size = d->size();
    auto topLeft = QPoint((rec.width() / 2) - (size.width() / 2), (rec.height() / 2) - (size.height() / 2));
    d->setGeometry(QRect(topLeft, size));
}
