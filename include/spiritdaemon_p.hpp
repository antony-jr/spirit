#ifndef SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
#define SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
#include <QObject>

#include "activewindowtracker.hpp"
#include "spiritworker.hpp"
#include "spirit.hpp"

namespace httplib {
   class Server;
}

class SpiritDaemonPrivate : public QObject {
  Q_OBJECT
public:
   SpiritDaemonPrivate(QObject *parent = nullptr);
   ~SpiritDaemonPrivate();

public Q_SLOTS:
   
   void run();

private:
   httplib::Server *m_Server;

   ActiveWindowTracker *m_Tracker;
   SpiritWorker *m_Worker;
   Spirit *m_Spirit;   
}

#endif // SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
