#include "traplabel.h"

TrapLabel::TrapLabel(QString str, bool isright, QWidget *parent)
    : QWidget(parent)
{
    text = str;
    is_right = isright;
}

TrapLabel::~TrapLabel()
{

}

void TrapLabel::setText(const QString& str)
{
    text = str;
}

void TrapLabel::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPolygon bg_poly;
    int w = width();
    int h = height();

    painter.setPen(QColor::fromRgb(110, 200, 182));//QColor::fromRgb(61, 112, 102));
    //painter.setFont()

    //
    //  TRAPEZOID BG
    //

    if (is_right) {
        bg_poly << QPoint(0, 0) << QPoint(w, 0)
            << QPoint(w, h) << QPoint(0, h  - Y_OFFSET);
    }
    else {
        bg_poly << QPoint(0, 0) << QPoint(w, 0)
            << QPoint(w, h - Y_OFFSET) << QPoint(0, h);
    }

    painter.setBrush(QColor::fromRgb(110, 200, 182));
    painter.drawPolygon(bg_poly);

    //
    //  TEXT
    //

    painter.setPen(Qt::black);
    painter.drawText(QRect(rect().x(), rect().y(), rect().width(),
                           rect().height() - Y_OFFSET),
                     Qt::AlignCenter, text);
}
