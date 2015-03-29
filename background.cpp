#include "background.h"

/**
 * @brief Background::Background    Constructor for Background class
 * @param fileName  The filename of the background image file.
 * @param xVelocity Speed at which the background should move.
 */
Background::Background(const QString &fileName, const int xVelocity)
    : QGraphicsPixmapItem(QPixmap(fileName))
{
    m_xVelocity = xVelocity;
}

/**
 * @brief Background::advance Called when the scene is about to advance by a frame, and when the scene is advancing by a frame.
 * @param step  0 if about to advance, 1 if the scene is advancing.
 */
void Background::advance(int step)
{
    if(step) {
        setX(x() + m_xVelocity);

        // Check if the background is out of bounds. If it is, move it to the far right.
        if(x() <= -boundingRect().width()) {
            if(m_xVelocity < 0)
                setX(boundingRect().width() + m_xVelocity);
            else
                setX(boundingRect().width() - m_xVelocity);
        }
    }
}
