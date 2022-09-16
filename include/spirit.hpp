#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QRect>
#include <QLabel>
#include <QMovie>
#include <QBuffer>
#include <QScopedPointer>

#include "spiritenums.hpp"

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
    /// Set the scale of the
    /// displayed animation.
    void setScale(int);

    /// Set the speed of the animation
    /// shown in this widget.
    void setSpeed(int);

    /// The position of the widget.
    void setPosition(short);

    // Moves the widget to the given rect.
    void update(QRect);

    // Animates a given action
    void animate(QString /*current action name*/,
                 QBuffer* /*current webp file*/,
                 QBuffer* /*current webp variant*/,
                 QBuffer* /*audio file if given*/,
                 bool /*loop*/,
                 int /*scale percentage*/,
                 int /*speed percentage*/,
                 QString /*next action if available*/,
                 QVector<int> /*offsets*/);

    void clear();

  private Q_SLOTS:
    void handleMovieStarted();
    void handleFrameChanged(int);
  Q_SIGNALS:
    // Request or set action for the current
    // animation cycle.
    void requestAction(QString);
    void cleared();

  private:
    QScopedPointer<QMovie> m_Movie;
    short n_Position = Position::TopLeft;
    int n_YOff = 0,
        n_XOff = 0,
        n__YOff = 0,
        n__XOff = 0;

    int n_Scale = 100,
        n_Speed = 100;
    bool b_Loop = true;
    bool b_Flipped = false;
    bool b_ClearRequested = false;
    QBuffer *m_Buffer = nullptr;
    QBuffer *m_Variant = nullptr;
    QString m_Action;
    QString m_Next;
};

#endif // SPIRIT_HPP_INCLUDED
