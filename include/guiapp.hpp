#ifndef GUI_APP_HPP_INCLUDED
#define GUI_APP_HPP_INCLUDED
#include <QMainWindow>
#include <QStringListModel>
#include <QDialog>

// Import UIC generated header
// files.
#include "ui/ui_spiritgui.h"
#include "ui/ui_spiritsettings.h"

#include "spiritmanager.hpp"
#include "spiritconfig.hpp"

class GuiApp : public QMainWindow {
    Q_OBJECT
  public:
    GuiApp(QWidget *parent = nullptr);
    ~GuiApp();

  public Q_SLOTS:
    void init();

  private Q_SLOTS:
    void handleInit(bool);
    void handleDeinit(bool);

    void handleLoad();
    void handleSpiritInfo(bool, QJsonObject);
    void handleProperties(bool, QJsonObject);
    void updateProperties(bool, QJsonObject);
    void handleScaleChange();
    void handlePositionChange(int);
    void handleActionChange(int);
    void handleActions(QStringList, QString);
    void handleX1Offset(int);
    void handleX2Offset(int);
    void handleY1Offset(int);
    void handleY2Offset(int);

    // Settings
    void handleSettings();
    void handleDefaultSpiritSelect();
    void handleAllowedProgramSelected(QModelIndex);
    void handleAddAllowedProgram();
    void handleRemoveAllowedProgram();
    void handleSettingsFinished(int);

  Q_SIGNALS:
    void quit();

  private:
    Ui::SpiritMainWindow *m_UI;
    Ui::settingsDialog *m_SettingsDialogUI;
    QDialog *m_SettingsDialog;
    SpiritManager *m_Manager;
    SpiritConfig *m_Config;

    QStringListModel *m_AllowedProgramsModel;
    QStringList m_RecentAllowed;
    int n_AllowedProgramRow = 0;
    QString m_DefaultSpirit;
};

#endif // GUI_APP_HPP_INCLUDED
