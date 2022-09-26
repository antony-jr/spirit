#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QRect>
#include <QLabel>
#include <QMovie>
#include <QMediaPlayer>
#include <QBuffer>
#include <QScopedPointer>

#include "spiritenums.hpp"
#include "spiritsettings.hpp"

/// This is the GUI Widget for
/// the spirit.
/// This runs in the main thread and all
/// heavy lifting is done in the spirit worker which
/// runs all calculations and parsing in a separate
/// thread.
class Spirit : public QLabel {
    Q_OBJECT
  public:
    struct Position : public SpiritEnums::Spirit::Position { };

    Spirit();
    ~Spirit();
  public Q_SLOTS:
    void resetDefaults();

    void setXOffset(int, int);
    void setYOffset(int, int);

    /// Set the scale of the
    /// displayed animation.
    void setScale(int);

    /// Set the speed of the animation
    /// shown in this widget.
    void setSpeed(int);

    /// The position of the widget.
    void setPosition(short);

    // Moves the widget to the given rect.
    void update(QRect, int, int, int, int);

    // Animates a given action
    void animate(QString /*current action name*/,
                 QBuffer* /*current webp file*/,
                 QBuffer* /*current webp variant*/,
                 QBuffer* /*audio file if given*/,
                 bool /*loop*/,
                 int /*scale percentage*/,
                 int /*speed percentage*/,
                 QString /*next action if available*/,
                 QVector<int> /*offsets*/,
                 QString /*md5 hash of the spirit file*/);

    void getProperties();

    void clear();

  private Q_SLOTS:
    void handleMovieStarted();
    void handleFrameChanged(int);
    void handlePlayerStateChanged(QMediaPlayer::State);
  Q_SIGNALS:
    // Request or set action for the current
    // animation cycle.
    void properties(int, int, int, int, int, int, int, QString);
    void requestAction(QString);
    void requestUpdate();
    void cleared();

  private:
    QScopedPointer<QMediaPlayer> m_Player;
    QScopedPointer<QMovie> m_Movie;
    SpiritSettings m_Settings;
    short n_Position = Position::TopLeft;

    int n_OrigWidth = 0,
        n_OrigHeight = 0;
    int n_OrigWidthVar = 0,
        n_OrigHeightVar = 0;

    bool b_Loop = true;
    bool b_Flipped = false;
    bool b_Paused = true;
    bool b_ClearRequested = false;
    QBuffer *m_Buffer = nullptr;
    QBuffer *m_Variant = nullptr;
    QString m_Action;
    QString m_Next;
    QString m_Sign;
};

#endif // SPIRIT_HPP_INCLUDED
