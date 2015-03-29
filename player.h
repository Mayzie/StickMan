#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>

class Player : public QGraphicsPixmapItem
{
public:
    Player(uint fps);

    void addAnimationImage(const QString &fileName);
protected:
    void advance(int step);

private:
    QList<QPixmap> m_animations;    // Contains the frame/step-by-step images of the player animation.
    int m_index;        // Current image we are showing.
    uint m_currFrame;   // Current frame we are up to.
    uint m_fps;         // Number of frames per second the game is running at.
    uint m_changeAt;    // Relative to the number of animation images, this is how often we switch these images.
};

#endif // PLAYER_H
