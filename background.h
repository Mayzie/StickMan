#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QGraphicsItem>

class Background : public QGraphicsPixmapItem
{
public:
    Background(const QString &fileName, const int xVelocity);

protected:
    void advance(int step);

private:
    int m_xVelocity;
};

#endif // BACKGROUND_H
