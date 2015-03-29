#ifndef STICKMAN_H
#define STICKMAN_H

#include <QGraphicsView>
#include <QTimer>

#include "player.h"

class StickMan : public QGraphicsView
{
public:
    StickMan(int fps, Player *player);

    void show();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QTimer m_timer;
    uint m_fps;

    Player *m_player;
};

#endif // STICKMAN_H
