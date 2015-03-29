#include "stickman.h"

#include <QKeyEvent>

/**
 * @brief StickMan::StickMan    Creates a new StickMan/GraphicsView object.
 * @param fps   Frames-per-Second. This is used to calculate the exact time interval between frames.
 */
StickMan::StickMan(int fps, Player *player)
    : QGraphicsView()
{
    m_fps = fps;

    m_player = player;
}

/**
 * @brief StickMan::show    Show the graphics view/window.
 */
void StickMan::show() {
    QGraphicsView::show();

    QObject::connect(&m_timer, SIGNAL(timeout()), scene(), SLOT(advance()));
    m_timer.start(1000 / m_fps);
}

/**
 * @brief StickMan::resizeEvent This method is called whenever the window has been resized.
 * @param event
 */
void StickMan::resizeEvent(QResizeEvent *event) {
    // Stretch window contents to match the new window size.
    fitInView(0, 0, 1024, 768, Qt::IgnoreAspectRatio);

    QGraphicsView::resizeEvent(event);
}

/**
 * @brief StickMan::keyPressEvent   This method is called whenever a user has pressed a key.
 * @param event
 */
void StickMan::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        // Pause the game.
        case Qt::Key_P: {
            if(m_timer.timerId() == -1)
                m_timer.start(1000 / m_fps);
            else
                m_timer.stop();

            break;
        }

        // Quit the game.
        case Qt::Key_Escape: {
            exit(0);
        }

        case Qt::Key_Up: {
            m_player->setY(m_player->y() - 15);
            break;
        }

    case Qt::Key_Down: {
        m_player->setY(m_player->y() + 15);
        break;
    }
    }

    QGraphicsView::keyPressEvent(event);
}
