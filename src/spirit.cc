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
    QObject::connect(m_Player.data(), &QMediaPlayer::stateChanged,
                     this, &Spirit::handlePlayerStateChanged, Qt::QueuedConnection);

}

Spirit::~Spirit() {
    disconnect();
    hide();
}

void Spirit::resetDefaults() {
    m_Settings.resetDefaults();
}

void Spirit::setXOffset(int x1, int x2) {
    m_Settings.setXOffset(x1, x2);
}

void Spirit::setYOffset(int y1, int y2) {
    m_Settings.setYOffset(y1, y2);
}

void Spirit::setScale(int scale) {
    m_Settings.setScale(scale);
    emit requestUpdate();
}

void Spirit::setSpeed(int speed) {
    m_Settings.setSpeed(speed);
    emit requestUpdate();
}

void Spirit::setPosition(short pos) {
    n_Position = pos;
    m_Settings.setPosition(pos);
    emit requestUpdate();
}

void Spirit::update(QRect geometry) {
    if (b_Paused || m_Movie.isNull() || b_ClearRequested) {
        return;
    }

    auto xoffsets = m_Settings.getXOffset();
    auto xoff = xoffsets.first;
    auto _xoff = xoffsets.second;

    auto yoffsets = m_Settings.getYOffset();
    auto yoff = yoffsets.first;
    auto _yoff = yoffsets.second;

    auto pos = getCoordinates(n_Position, geometry, frameGeometry(), width(), height(), yoff, _yoff);
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
            m_Movie->setSpeed(m_Settings.getSpeed());


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
            m_Movie->setSpeed(m_Settings.getSpeed());

            QObject::connect(m_Movie.data(), &QMovie::frameChanged, this, &Spirit::handleFrameChanged);
            m_Movie->start();
        }
    }

    auto percentage = m_Settings.getScale() / 100.0;
    if (n_OrigWidth && n_OrigHeight && n_OrigWidthVar && n_OrigHeightVar) {
        resize((b_Flipped ? n_OrigWidthVar : n_OrigWidth) * percentage,
               (b_Flipped ? n_OrigHeightVar : n_OrigHeight) * percentage);
    }

    pos = getCoordinates(b_Flipped ?
                         (n_Position == Position::BottomLeft ?
                          Position::BottomLeft : Position::BottomRight)
                         : n_Position,
                         geometry,
                         frameGeometry(), width(), height(),
                         yoff * percentage, _yoff * percentage);

    x = pos.x;
    y = pos.y;

    x += b_Flipped ? xoff * percentage : _xoff * percentage;

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
                     QVector<int> offsets,
                     QString signature) {
    hide();
    b_Paused = true;
    b_Loop = loop;
    m_Action = action;
    m_Next = nxt;
    m_Sign = signature;

    auto ok = m_Settings.setSpiritSignature(
                  signature,
                  /*x1=*/offsets[0], /*x2=*/offsets[2],
                  /*y1=*/offsets[1], /*y2=*/offsets[3],
                  scale,
                  speed,
                  n_Position
              );

    if (!ok) {
        // TODO: Handle this better.
        return;
    }

    n_Position = m_Settings.getPosition();

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

    m_Movie->setSpeed(m_Settings.getSpeed());
    m_Movie->start();

    auto pixmap = m_Movie->currentPixmap();
    n_OrigWidth = pixmap.width();
    n_OrigHeight = pixmap.height();

    b_Paused = false;

    emit requestUpdate();

    if(play) {
        if (play == m_Player->mediaStream()) {
            return;
        }
        if(!play->isOpen()) {
            play->open(QIODevice::ReadOnly);
        }
        play->reset();
        m_Player->setMedia(QUrl("audio/mp3"), (QIODevice*)play);
        m_Player->setVolume(100);
        m_Player->play();
    } else {
        m_Player->stop();
        m_Player->setMedia(QUrl("audio/mp3"), nullptr);
    }
}

void Spirit::getProperties() {
    auto xoffset = m_Settings.getXOffset();
    auto yoffset = m_Settings.getYOffset();
    emit properties(
        xoffset.first, xoffset.second,
        yoffset.first, yoffset.second,
        m_Settings.getScale(), m_Settings.getSpeed(),
        m_Settings.getPosition(),
        m_Sign
    );
    return;
}

void Spirit::clear() {
    b_ClearRequested = true;
    hide();
    m_Player->stop();
    m_Player->setMedia(QUrl("audio/mp3"), nullptr);
    b_Paused = true;
    b_Loop = false;
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

void Spirit::handlePlayerStateChanged(QMediaPlayer::State state) {
    if (b_Loop && m_Player->state() == QMediaPlayer::StoppedState) {
        m_Player->play();
    }
}
