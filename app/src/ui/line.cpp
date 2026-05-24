#include "line.h"

Line::Line(int x, int y, unsigned int length,
           unsigned int width, QColor color,
           QWidget *parent)
    : QWidget(parent),
    x(x), y(y), length(length), width(width), color(color)
{
    setMinimumSize(length, width);
    setMaximumSize(length, width);
}

Line::~Line()
{

}

void Line::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);

    painter.drawRect(x, y, length, width);
}
