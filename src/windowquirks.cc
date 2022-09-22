#include <QtGlobal>
#include <QSysInfo>
#include <QString>
#include <QDebug>

#include "windowquirks.hpp"

// Window Quirks helps to get over some quirks in window
// sizes in different linux distros and window managers.
// Although this covers only popular distros and wms, for
// other wms and distros the user can adjust their yoffset
// themselves.

WindowQuirks::WindowQuirks() {
#ifdef Q_OS_LINUX
   auto wm = std::getenv("XDG_CURRENT_DESKTOP");
   if (wm == NULL) {
      return;
   }
   auto wmStr = QString(wm);
   auto productType = QSysInfo::productType();
   auto productVersion = QSysInfo::productVersion(); 

#if 0
   qDebug() << "Product: " << productType;
   qDebug() << "Product Version: " << productVersion;
   qDebug() << "Window Manager: " << wmStr;
#endif

  // KDE at the time does not have any quirks
  // with frame geometry.

  if (wmStr == "ubuntu:GNOME") {
     n_Y = 60;
  } 
     
  if (productType == "ubuntu") { 
      if (productVersion.contains("20.04")) {
	// Any specific addition quirks 
	// for specific versions of ubuntu. 
      }
   }
#endif // LINUX
}

WindowQuirks::~WindowQuirks() { }

int WindowQuirks::yoffset() {
   return n_Y;
}

int WindowQuirks::xoffset() {
   return n_X;
}
