#include <QRect>
#include <QPixmap>
#include <QPair>

#include "spirit.hpp"

Spirit::Spirit()
	: QWidget(nullptr, Qt::FramelessWindowHint |
			   Qt::Tool |
			   Qt::WindowStaysOnTopHint |
			   Qt::WindowTransparentForInput |
			   Qt::WindowDoesNotAcceptFocus | 
			   Qt::NoDropShadowWindowHint |
			   Qt::X11BypassWindowManagerHint) {
		
	setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setAttribute(Qt::WA_ShowWithoutActivating, true);
	setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
	setStyleSheet(QString::fromUtf8("background: transparent; border: none;"));
}

Spirit::Spirit(const QString &path)
	: Spirit() { 
	setPath(path);
}

Spirit::~Spirit() {
	hide();
}

void Spirit::setPath(const QString &path) {
	m_SpiritPath = path;
}

void Spirit::init() { 

}

void Spirit::clear() { 

}

void Spirit::update(QRect geometry) {
	auto point = geometry.topLeft();

	move(point.x() + 100, point.y() - (frameGeometry().height()) + 46);
	show();
}


/// Private Slots.
void Spirit::handleArchvieError(short code) {
	/// TODO: Handle error.
	emit error(Error::InvalidSpiritFile);
}

void Spirit::handleArchiveContents(QArchive::MemoryExtractorOutput *output) {
	/// TODO: Validate the output.
	
	m_Spirit.reset(output);
}


