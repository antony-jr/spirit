#include <QCoreApplication>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "spiritdaemon_p.hpp"
#include "CivetServer.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static void http_json_header(struct mg_connection *conn) {
   mg_printf(conn,
	 "HTTP/1.1 200 OK\r\nContent-Type: "
	 "application/json\r\nConnection: close\r\n\r\n");	
}

static QJsonObject get_json_body(struct mg_connection *conn) {
   QByteArray body;
   std::vector<char> buffer(1025, 0);

   while(true) {
      int r = mg_read(conn, buffer.data(), 1024);
      if (r <= 0) {
	 break;
      }
      body.append(buffer.data());
      std::fill(buffer.begin(), buffer.end(), 0);
   }

   QJsonParseError error;	
   auto json = QJsonDocument::fromJson(body, &error);
   if (error.error != QJsonParseError::NoError || !json.isObject()) {
      return QJsonObject { };
   }

   return json.object();
}

static void write_json(QJsonObject json, struct mg_connection *conn) {
   QJsonDocument doc(json);
   QString strJson(doc.toJson(QJsonDocument::Compact));
   mg_printf(conn, strJson.toUtf8().constData());
}

class IndexHandler : public CivetHandler
{
  public:
	bool handleGet(CivetServer *server, struct mg_connection *conn) {
		SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
		http_json_header(conn);

		// TODO: Embed Version String here.
		write_json(QJsonObject {
		   {"status", "success"},
		   {"app", "spirit"},
		   {"version", "1"},
		   {"commit", "dev"},
		   {"build", 1},
		}, conn);
		return true;
	}
};

class HaltHandler : public CivetHandler {
   public:
      bool handleGet(CivetServer *server, struct mg_connection *conn) {
	 SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());	   
	 http_json_header(conn);

	 emit obj->quit(); 
	 
	 write_json(QJsonObject {
		   {"status", "success"},
		}, conn);

	 return true;
      }
}; 

class LoadHandler : public CivetHandler
{
   public:
      bool handleGet(CivetServer *server, struct mg_connection *conn) {
	 SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());

	 http_json_header(conn);

	 QJsonObject r = obj->m_Meta;
	 
	 r.insert("location", obj->m_Spirit);
	 r.insert("status", "success");

	 write_json(r, conn);
	 return true;
      }

      bool handlePost(CivetServer *server, struct mg_connection *conn) {
	 SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
	 bool success = true;

	 http_json_header(conn);

	 auto body = get_json_body(conn);
	 if(body.isEmpty() || !body.contains("opt")) {
	    success = false;
	 } else {
	    auto operation = body["opt"].toString();

	    if (operation == "set") {
	       if (body.contains("file")) {
		  QEventLoop loop;

		  QObject::connect(obj, &SpiritDaemonPrivate::cachedSpiritMeta,
		       		   &loop, &QEventLoop::quit, Qt::QueuedConnection);

		  emit obj->setSpirit(body["file"].toString());
		  loop.exec();

	       } else if (body.contains("url")) {
		  // TODO: URL Downloads.
	       } else {
		  success = false;
	       }
	    } else if (operation == "unset") {
	       QEventLoop loop;
     	       QObject::connect(obj, &SpiritDaemonPrivate::cachedSpiritMeta,
		       		   &loop, &QEventLoop::quit, Qt::QueuedConnection);


       		emit obj->unsetSpirit();	       
       	       loop.exec();	       
	    } else {
	       success = false;
	    }
	 }

	 QJsonObject r = obj->m_Meta;
	 r.insert("location", obj->m_Spirit);	
	 r.insert("status", success ? "success" : "failed");
	 write_json(r, conn);
	 return true;
      } 
};

class PropertyHandler : public CivetHandler
{
   public:
      bool handleGet(CivetServer *server, struct mg_connection *conn) {
	  SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
	   http_json_header(conn);

	   QString position = "topLeft";

	   if (obj->m_Position == SpiritEnums::Spirit::Position::TopRight) {
	     position = "topRight"; 
	   } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomLeft) {
	     position = "bottomLeft";
	   } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomRight) {
	     position = "bottomRight";
	   }

   	  write_json(QJsonObject {
	      {"status", "success"},
	      {"position", position},
	      {"scale", obj->n_Scale},
	      {"speed", obj->n_Speed} 
	  }, conn);

	  return true;
      }

      bool handlePost(CivetServer *server, struct mg_connection *conn) {
	 
	  SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
	  bool success = true; 
	  http_json_header(conn);

	   auto body = get_json_body(conn);
	   if(body.isEmpty()) {
	      success = false;
	   } else {
	      if (body.contains("position")) {
		 short pos = SpiritEnums::Spirit::Position::TopLeft;
		 auto position = body["position"].toString();
		 if (position == "topRight") {
		    pos = SpiritEnums::Spirit::Position::TopRight;
		 } else if (position == "bottomLeft") {
		    pos = SpiritEnums::Spirit::Position::BottomLeft;
		 } else if (position == "bottomRight") {
		    pos = SpiritEnums::Spirit::Position::BottomRight;
		 }

		 emit obj->setPosition(pos);
		 obj->m_Position = pos;
	      }

	      if (body.contains("scale")) {
		 auto scale = body["scale"].toInt();
		 if (scale > 0) {
		    emit obj->setScale(scale);
		    obj->n_Scale = scale;
		 }
	      }

	      if (body.contains("speed")) {
		 auto speed = body["speed"].toInt();
		 if (speed > 0) {
		    emit obj->setSpeed(speed);
		    obj->n_Speed = speed;
		 }
	      }
	   }

	   QString position = "topLeft";

	   if (obj->m_Position == SpiritEnums::Spirit::Position::TopRight) {
	     position = "topRight"; 
	   } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomLeft) {
	     position = "bottomLeft";
	   } else if(obj->m_Position == SpiritEnums::Spirit::Position::BottomRight) {
	     position = "bottomRight";
	   }

   	  write_json(QJsonObject {
	      {"status", success ? "success" : "failed"},
	      {"position", position},
	      {"scale", obj->n_Scale},
	      {"speed", obj->n_Speed} 
	  }, conn);


	   return true;
      } 
};

class ActionHandler : public CivetHandler
{
  public:
        bool handleGet(CivetServer *server, struct mg_connection *conn) {
   	   SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());
	   http_json_header(conn);

   	   write_json(QJsonObject {
	      {"status", "success"},
	      {"action", "default"}
	   }, conn);

	   return true;
	}

	bool handlePost(CivetServer *server, struct mg_connection *conn) {
		SpiritDaemonPrivate *obj = qobject_cast<SpiritDaemonPrivate*>((QObject*)server->getUserContext());	   
	 	http_json_header(conn);

		bool success = true;
		QString action = "default";

		auto body = get_json_body(conn);
		
		if (body.isEmpty()) {
		   success = false;
		} else {
		     if(!body.contains("opt")) {
			success = false;
		     } else {
			if(body["opt"].toString() == "set") {
			   if(!body.contains("action")) {
			      success = false;
			   } else {
			      qDebug() << "Set Action:: " << body["action"].toString();

			      QEventLoop loop;

			      QObject::connect(obj, &SpiritDaemonPrivate::cachedAction, 
				    	       &loop, &QEventLoop::quit, Qt::QueuedConnection);

			      emit obj->setAction(body["action"].toString());
			      loop.exec(); 
			   }
			} else {
			   if(body["opt"].toString() == "reset") {
			      QEventLoop loop;

			      QObject::connect(obj, &SpiritDaemonPrivate::cachedAction, 
				    	       &loop, &QEventLoop::quit, Qt::QueuedConnection);

			      emit obj->resetAction();
			      loop.exec(); 
			  
			   }
			}
		     }
		}

	 	write_json(QJsonObject {
		   {"status", success ? "success" : "failed"},
	           {"action", action}	      
		}, conn);

	   return true;
	}
};

SpiritDaemonPrivate::SpiritDaemonPrivate(QObject *parent)
   : QObject(parent),
     b_StopRequested(false) {
  
}

SpiritDaemonPrivate::~SpiritDaemonPrivate() { }

void SpiritDaemonPrivate::run() {
   mg_init_library(0);
   b_StopRequested = false; 
   const char *options[] = { "listening_ports", "4499", 0}; 
   std::vector<std::string> cpp_options;
   for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
      cpp_options.push_back(options[i]);
   }
   
   CivetServer server(cpp_options, 0, /*user context*/(void*)this);

   IndexHandler index;
   server.addHandler("", index);
   server.addHandler("/spirit", index);
   server.addHandler("/spirit/v1", index);

   HaltHandler halt;
   server.addHandler("/spirit/v1/halt", halt);

   LoadHandler load;
   server.addHandler("/spirit/v1/load", load);

   PropertyHandler prop;
   server.addHandler("/spirit/v1/property", prop);

   ActionHandler action;
   server.addHandler("/spirit/v1/action", action);

   while(true) {
      QCoreApplication::processEvents();
      if(b_StopRequested) {
	 b_StopRequested = false;
	 break;
      }
      QThread::sleep(5);
   }
}

void SpiritDaemonPrivate::stop() {
   b_StopRequested = true;
}

void SpiritDaemonPrivate::updateAction(QString action) {
   m_Action = action;
   emit cachedAction();
}

void SpiritDaemonPrivate::updateSpirit(QString loc) {
   m_Spirit = loc;
   emit cachedSpirit();
}

void SpiritDaemonPrivate::updateSpiritMeta(QJsonObject meta) {
   m_Meta = meta;
   emit cachedSpiritMeta();
}
