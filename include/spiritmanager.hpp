#ifndef SPIRIT_MANAGER_HPP_INCLUDED
#define SPIRIT_MANAGER_HPP_INCLUDED
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

#include "spiritapp.hpp"

class SpiritManager : public QObject {
    Q_OBJECT
  public:

    SpiritManager(QApplication *app);
    ~SpiritManager();

  public Q_SLOTS:
    void init();
    bool waitForInitialized();

    bool daemon();
    void reinit();

    void deinit();
    bool waitForDeinitialized();

    void load(QString file);
    bool waitForLoaded();

    void ghLoad(QString repo);

    void getActions();
    void getAction();
    void setAction(QString);

    void getQuirks();
    void addQuirk(QJsonObject);
    void removeQuirk(QString);

    void getProperties();
    QJsonObject waitForProperties();
    void setProperties(QJsonObject);
    QJsonObject waitForUpdatedProperties();
    void resetProperty();

    void getLoadedSpiritInfo();
    QJsonObject waitForLoadedSpiritInfo();

    int getPort();
    int exec();

  private Q_SLOTS:
    bool startDaemonProcess();
    void handleAPIResponse(QNetworkReply*);
    void handleReinit(bool);
    void handleInitReply(QNetworkReply*);
    void handleDeinitReply(QNetworkReply*);
    void handleLoadSetReply(QNetworkReply*);
    void handleGetPropertyReply(QNetworkReply*);
    void handleSetPropertyReply(QNetworkReply*);
    void handleSpiritInfo(QNetworkReply*);
    void handleGetAction(QNetworkReply*);
    void handleGetActions(QNetworkReply*);
    void handleGetQuirksReply(QNetworkReply*);

  private:
    int getDaemonPort();

  Q_SIGNALS:
    void initialized(bool);
    void deinitialized(bool);
    void loaded(bool);
    void ghLoaded(bool);
    void quirks(bool, QJsonObject);
    void properties(bool, QJsonObject);
    void updatedProperties(bool, QJsonObject);
    void loadedSpiritInfo(bool, QJsonObject);
    void updatedRunOnStartup(bool);
    void updatedDefaultSpiritFile(QString);
    void allowedPrograms(QStringList);
    void actions(QStringList);
    void action(QString);


  private:
    bool b_Daemon = false;

    QApplication *m_App;
    QNetworkAccessManager *m_Manager;
    SpiritApp *m_SpiritApp;
};

#endif
