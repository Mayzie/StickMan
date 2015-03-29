#include "player.h"

/**
 * @brief Player::Player    Creates a new Player object.
 * @param fps   The Frames-Per-Second the game is running at. This determines the animation speed.
 */
Player::Player(uint fps)
    : QGraphicsPixmapItem()
{
    m_index = 0;
    m_currFrame = 0;
    m_fps = fps;
    m_changeAt = m_fps;
}

/**
 * @brief Player::advance Called when the scene is about to advance by a frame, and when the scene is advancing by a frame.
 * @param step  0 if about to advance, 1 if the scene is advancing.
 */
void Player::advance(int step) {
    if(step) {
        // This is here to prevent it changing the image each and every frame (which is really fast, it would just look like a blur).
        if(m_currFrame++ % m_changeAt == 0) {
            m_index = m_index % m_animations.length();

            setPixmap(m_animations[m_index++]);
        }
    }
}

/**
 * @brief Player::addAnimationImage Adds a new image/sprite for the animation.
 * @param fileName  Path (absolute or relative) to the image file.
 */
void Player::addAnimationImage(const QString &fileName) {
    m_animations.append(QPixmap(fileName));
    // If no image has been added previously, we need this because we need a bounding rectangle.
    if(m_animations.length() == 1)
        setPixmap(m_animations[0]);

    m_changeAt = m_fps / m_animations.length();
}
