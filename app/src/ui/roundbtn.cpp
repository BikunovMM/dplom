#include "roundbtn.h"

RoundBtn::RoundBtn(QWidget *parent)
    : QPushButton(parent)
{
    is_hovered = false;
    last_hovered = false;

    setMouseTracking(true);
}

RoundBtn::~RoundBtn()
{

}

void RoundBtn::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    if (isDown()) {
        painter.setBrush(QColor::fromRgb(61, 112, 102));
    }
    else if (is_hovered) {
        painter.setBrush(QColor::fromRgb(95, 172, 157));
    }
    else {
        painter.setBrush(QColor::fromRgb(119, 227, 206));
    }

    painter.drawEllipse(rect());

    drawContent(painter);
}

bool RoundBtn::hitButton(const QPoint &pos) const
{
    int cx = width()  / 2;
    int cy = height() / 2;
    int px = pos.x();
    int py = pos.y();

    return cx * cx >= (px - cx) * (px - cx) + (py - cy) * (py - cy);
}

void RoundBtn::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    int cx = width()  / 2;
    int cy = height() / 2;
    int px = pos.x();
    int py = pos.y();

    last_hovered = is_hovered;
    is_hovered = cx * cx >=
                 (px - cx) * (px - cx) + (py - cy) * (py - cy);

    if (is_hovered != last_hovered) {
        update();
    }
}

void RoundBtn::leaveEvent(QEvent *event)
{
    (void)event;

    if (is_hovered) {
        is_hovered = false;
        update();
        return;
    }
}
