#include <QFileDialog>
#include <QTimer>
#include <QDir>

#include "guiapp.hpp"

GuiApp::GuiApp(QWidget *parent) :
    QMainWindow(parent) {
    m_UI = new Ui::SpiritMainWindow;
    m_SettingsDialogUI = new Ui::settingsDialog;
    m_Manager = new SpiritManager(qApp);
    m_Config = new SpiritConfig(this);
    m_SettingsDialog = new QDialog(this);


    m_UI->setupUi(this);
    m_SettingsDialogUI->setupUi(m_SettingsDialog);

    m_AllowedProgramsModel = new QStringListModel(this);
    m_SettingsDialogUI->programsListView->setModel(m_AllowedProgramsModel);

    // Populate our model
    // model->setStringList(List);


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

    connect(m_UI->actionSettings, &QAction::triggered,
            this, &GuiApp::handleSettings);

    connect(m_UI->actionLoad, &QAction::triggered,
            this, &GuiApp::handleLoad);

    connect(m_SettingsDialogUI->selectDefaultSpiritBtn, &QPushButton::pressed,
            this, &GuiApp::handleDefaultSpiritSelect);

    connect(m_Manager, &SpiritManager::loaded,
            m_Manager, &SpiritManager::getProperties);

    connect(m_UI->actionQuit, &QAction::triggered,
            m_Manager, &SpiritManager::deinit);

    connect(m_UI->scaleSlider, &QSlider::sliderReleased,
            this, &GuiApp::handleScaleChange, Qt::QueuedConnection);

    connect(m_UI->resetBtn, &QPushButton::pressed,
            m_Manager, &SpiritManager::resetProperty);

    connect(m_UI->actionComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &GuiApp::handleActionChange);

    connect(m_UI->positionComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &GuiApp::handlePositionChange);

    connect(m_UI->x1offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleX1Offset);

    connect(m_UI->x2offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleX2Offset);

    connect(m_UI->y1offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleY1Offset);

    connect(m_UI->y2offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleY2Offset);

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
    m_Manager->deleteLater();
}

void GuiApp::init() {
    m_Manager->init();
}

void GuiApp::handleInit(bool ok) {
    if (!ok) {
        emit quit();
        return;
    }

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
}

void GuiApp::handleProperties(bool ok, QJsonObject obj) {
    updateProperties(ok, obj);
    if (isHidden()) {
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

    disconnect(m_UI->x1offset,  QOverload<int>::of(&QSpinBox::valueChanged),
               this, &GuiApp::handleX1Offset);

    disconnect(m_UI->x2offset,  QOverload<int>::of(&QSpinBox::valueChanged),
               this, &GuiApp::handleX2Offset);

    disconnect(m_UI->y1offset,  QOverload<int>::of(&QSpinBox::valueChanged),
               this, &GuiApp::handleY1Offset);

    disconnect(m_UI->y2offset,  QOverload<int>::of(&QSpinBox::valueChanged),
               this, &GuiApp::handleY2Offset);



    m_UI->x1offset->setValue(obj["topXOffset"].toInt(m_UI->x1offset->value()));
    m_UI->x2offset->setValue(obj["bottomXOffset"].toInt(m_UI->x2offset->value()));

    m_UI->y1offset->setValue(obj["topYOffset"].toInt(m_UI->y1offset->value()));
    m_UI->y2offset->setValue(obj["bottomYOffset"].toInt(m_UI->y2offset->value()));


    connect(m_UI->x1offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleX1Offset);

    connect(m_UI->x2offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleX2Offset);

    connect(m_UI->y1offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleY1Offset);

    connect(m_UI->y2offset,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GuiApp::handleY2Offset);


}

void GuiApp::handlePositionChange(int index) {
    QString pos = "bottomRight";
    if (index == 0) {
        pos = "topLeft";
    } else if (index == 1) {
        pos = "topRight";
    } else if (index == 2) {
        pos = "bottomLeft";
    }

    m_Manager->setProperties(QJsonObject {
        {"position", pos}
    });
}

void GuiApp::handleX1Offset(int value) {
    m_Manager->setProperties(QJsonObject {
        {"topXOffset", value}
    });

}

void GuiApp::handleX2Offset(int value) {
    m_Manager->setProperties(QJsonObject {
        {"bottomXOffset", value}
    });

}


void GuiApp::handleY1Offset(int value) {
    m_Manager->setProperties(QJsonObject {
        {"topYOffset", value}
    });

}
void GuiApp::handleY2Offset(int value) {
    m_Manager->setProperties(QJsonObject {
        {"bottomYOffset", value}
    });

}



void GuiApp::handleActionChange(int index) {
    m_Manager->setAction(m_UI->actionComboBox->itemText(index));
}


void GuiApp::handleScaleChange() {
    m_Manager->setProperties(QJsonObject {
        {"scale", m_UI->scaleSlider->value()}
    });
}

void GuiApp::handleLoad() {
    auto fileName = QFileDialog::getOpenFileName(this,
                    tr("Open Spirit File"), QDir::homePath(), tr("Spirit Files (*.spirit)"));

    m_Manager->load(fileName);
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
            // TODO: Write To Specific Place to Run the
            // daemon at startup.
        }
    }
}
