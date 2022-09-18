#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QDebug>

#include "spirit.hpp"

struct Coordinates {
    int x = 0,
        y = 0;
};

static Coordinates getCoordinates(short pos, QRect geometry, QRect frame, int w, int h, int yoff, int _yoff) {
    int offset = frame.width() / 10;
    w += offset;

    auto point = geometry.topLeft();
    auto x = point.x();
    auto y = point.y();

    if(pos == Spirit::Position::TopRight) {
        point = geometry.topRight();
        x = point.x();
        y = point.y();

        x = x + frame.width() - (w*2);
        y = y - frame.height() + (yoff < 0 ? 0 : yoff);
    } else if(pos == Spirit::Position::BottomLeft) {
        point = geometry.bottomLeft();
        x = point.x();
        y = point.y();

        x = x - frame.width() + w;
        y = y - frame.height() - (_yoff < 0 ? 0 : _yoff);

    } else if(pos == Spirit::Position::BottomRight) {
        point = geometry.bottomRight();
        x = point.x();
        y = point.y();

        x = x - frame.width() - (w/4);
        y = y - frame.height() - (_yoff < 0 ? 0 : _yoff);
    } else { // TopLeft
        x += w;
        y = y - frame.height() + (yoff <= 0 ? 0 : yoff);
    }

    Coordinates r;
    r.x = x;
    r.y = y;

    return r;
}

Spirit::Spirit()
    : QLabel(nullptr, Qt::FramelessWindowHint |
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
    setScaledContents(true);

    m_Player.reset(new QMediaPlayer);
}

Spirit::~Spirit() {
    disconnect();
    hide();
}

void Spirit::setScale(int scale) {
    n_Scale = scale;
    emit requestUpdate();
}

void Spirit::setSpeed(int speed) {
    n_Speed = speed;
    emit requestUpdate();
}

void Spirit::setPosition(short pos) {
    n_Position = pos;
    emit requestUpdate();
}

void Spirit::update(QRect geometry) {
    if (b_Paused || m_Movie.isNull() || b_ClearRequested) {
        return;
    }

    auto pos = getCoordinates(n_Position, geometry, frameGeometry(), width(), height(), n_YOff, n__YOff);
    auto x = pos.x;
    auto y = pos.y;

    if ( y < 0 || x < 0 ||
            ((n_Position == Position::BottomRight || n_Position == Position::BottomLeft) && m_Variant)) {
        if(!b_Flipped && m_Variant != nullptr) {
            b_Flipped = true;
            hide();
            m_Movie->stop();
            m_Movie.reset(new QMovie);
            setMovie(m_Movie.data());

            m_Variant->reset();
            m_Movie->setDevice(m_Variant);
            m_Movie->setSpeed(n_Speed);


            QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged);
            m_Movie->start();
        }
    } else {
        if(b_Flipped) {
            b_Flipped = false;
            hide();
            m_Movie->stop();
            m_Movie.reset(new QMovie);
            setMovie(m_Movie.data());

            m_Buffer->reset();
            m_Movie->setDevice(m_Buffer);
            m_Movie->setSpeed(n_Speed);

            QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged);
            m_Movie->start();
        }
    }

    auto percentage = n_Scale / 100.0;
    if (n_OrigWidth && n_OrigHeight && n_OrigWidthVar && n_OrigHeightVar) {
        resize((b_Flipped ? n_OrigWidthVar : n_OrigWidth) * percentage,
               (b_Flipped ? n_OrigHeightVar : n_OrigHeight) * percentage);

        qDebug() << "Percentage: " << percentage;
    }

    pos = getCoordinates(b_Flipped ? Position::BottomRight : n_Position, geometry, frameGeometry(), width(), height(), n_YOff * percentage, n__YOff * percentage);

    x = pos.x;
    y = pos.y;

    move(x,y);
    show();
}

void Spirit::animate(QString action,
                     QBuffer *buffer,
                     QBuffer *_buffer,
                     QBuffer *play,
                     bool loop,
                     int scale,
                     int speed,
                     QString nxt,
                     QVector<int> offsets) {
    hide();
    b_Paused = true;
    n_XOff = offsets[0];
    n_YOff = offsets[1];
    n__XOff = offsets[2];
    n__YOff =   offsets[3];

    b_Loop = loop;
    n_Scale = scale;
    n_Speed = speed;
    m_Action = action;
    m_Next = nxt;

    if(m_Buffer) {
        m_Buffer->close();
    }

    if(m_Variant) {
        m_Variant->close();
    }

    m_Buffer = buffer;
    m_Variant = _buffer;

    if(m_Buffer && !m_Buffer->isOpen()) {
        m_Buffer->open(QIODevice::ReadOnly);
    }

    if(m_Variant && !m_Variant->isOpen()) {
        m_Variant->open(QIODevice::ReadOnly);
    }

    m_Buffer->reset();
    if(m_Variant) {
        m_Variant->reset();
    }

    if (m_Variant) {
        m_Movie.reset(new QMovie);
        setMovie(m_Movie.data());

        m_Variant->reset();
        m_Movie->setDevice(m_Variant);
        m_Movie->start();

        auto pixmap = m_Movie->currentPixmap();
        n_OrigWidthVar = pixmap.width();
        n_OrigHeightVar = pixmap.height();
    }

    if(!m_Movie.isNull()) {
        m_Movie->stop();
        if (m_Variant) {
            m_Variant->reset();
        }
    }
    m_Movie.reset(new QMovie);
    setMovie(m_Movie.data());

    QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged);

    if(n_Position == Position::TopRight ||
            n_Position == Position::TopLeft) {
        m_Movie->setDevice(m_Buffer);
    } else {
        if(m_Variant != nullptr) {
            m_Movie->setDevice(m_Variant);
        } else {
            m_Movie->setDevice(m_Buffer);
        }
    }

    m_Movie->setSpeed(n_Speed);
    m_Movie->start();

    auto pixmap = m_Movie->currentPixmap();
    n_OrigWidth = pixmap.width();
    n_OrigHeight = pixmap.height();

    qDebug() << "Width: " << n_OrigWidth;
    qDebug() << "Height: " << n_OrigHeight;
    qDebug() << "vWidth: " << n_OrigWidthVar;
    qDebug() << "vHeight: " << n_OrigHeightVar;

    b_Paused = false;

    emit requestUpdate();

    if(play) {
       if(!play->isOpen()) {
	  play->open(QIODevice::ReadOnly);
       }
       play->reset();
       m_Player->setMedia(QUrl("audio/mp3"), (QIODevice*)play);
       m_Player->setVolume(100);
       m_Player->play();
    }
}

void Spirit::clear() {
    b_ClearRequested = true;
    hide();
    m_Player->stop();
    m_Player->setMedia(QUrl("audio/mp3"), nullptr);
    b_Paused = true;
    b_Loop = false;
    n_Scale = n_Speed = 100;
    n_Position = Position::TopLeft;
    m_Action = "default";
    m_Next.clear();

    n_OrigWidth = n_OrigHeight = 0;

    if(m_Buffer) {
        m_Buffer->close();
    }

    if(m_Variant) {
        m_Variant->close();
    }

    m_Buffer = m_Variant = nullptr;
    if(!m_Movie.isNull()) {
        m_Movie->stop();
    }

    m_Movie.reset(new QMovie);
    setMovie(m_Movie.data());

    b_ClearRequested = false;
    emit cleared();
}

void Spirit::handleMovieStarted() {

}

void Spirit::handleFrameChanged(int frame) {
    if(m_Movie.isNull()) {
        return;
    }

    if(frame + 1 == m_Movie->frameCount()) {
        if(!b_Loop) {
	    if (m_Player->state() == QMediaPlayer::PlayingState) {
	       return;
	    }
            emit requestAction(m_Next);
        }
    }
}
