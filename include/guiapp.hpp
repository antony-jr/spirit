#ifndef GUI_APP_HPP_INCLUDED
#define GUI_APP_HPP_INCLUDED
#include <QMainWindow>
#include <QStringListModel>
#include <QProgressDialog>
#include <QDialog>

// Import UIC generated header
// files.
#include "ui/ui_spiritgui.h"
#include "ui/ui_spiritsettings.h"
#include "ui/ui_spiritquirks.h"

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
    void centerDialog(QDialog*);

    void handleInit(bool);
    void handleDeinit(bool);

    void handleLoad();
    void handleDocs();
    void handleAbout();
    void handleSpiritInfo(bool, QJsonObject);
    void handleProperties(bool, QJsonObject);
    void updateProperties(bool, QJsonObject);
    void handleActions(QStringList, QString);
    void handleApplyBtn();
    void handleResetBtn();

    // Settings
    void handleSettings();
    void handleDefaultSpiritSelect();
    void handleAllowedProgramSelected(QModelIndex);
    void handleAddAllowedProgram();
    void handleRemoveAllowedProgram();
    void handleSettingsFinished(int);

    // Quirks
    void handleQuirks();
    void handleQuirkSelected(QModelIndex);
    void handleAddQuirk();
    void handleRemoveQuirk();
    void handleQuirkData(bool, QJsonObject);

    void handleProgressCancel();
  Q_SIGNALS:
    void quit();

  private:
    Ui::SpiritMainWindow *m_UI;
    Ui::settingsDialog *m_SettingsDialogUI;
    Ui::quirksDialog *m_QuirksDialogUI;

    QDialog *m_SettingsDialog;
    QDialog *m_QuirksDialog;
    QProgressDialog *m_ProgressDialog;

    SpiritManager *m_Manager;
    SpiritConfig *m_Config;

    QStringListModel *m_AllowedProgramsModel;
    QStringList m_RecentAllowed;
    int n_AllowedProgramRow = 0;
    QString m_DefaultSpirit;

    QStringListModel *m_QuirksModel;
    int n_QuirkSelected = 0;
    QString m_LastSelectedQuirk;
    QJsonObject m_QuirkData;
};

#endif // GUI_APP_HPP_INCLUDED
