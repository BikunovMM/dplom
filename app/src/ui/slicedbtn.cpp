#include "slicedbtn.h"

SlicedBtn::SlicedBtn(QString ltext, QString rtext, QWidget *parent)
    : QPushButton(parent)
{
    this->ltext   = ltext;
    this->rtext   = rtext;
    is_lhovered   = false;
    last_lhovered = false;
    is_rhovered   = false;
    last_rhovered = false;

    setMouseTracking(true);
}

SlicedBtn::~SlicedBtn()
{

}

void SlicedBtn::setText(const QString& left_text,
                        const QString& right_text)
{
    ltext = left_text;
    rtext = right_text;
}

void SlicedBtn::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPolygon btn_poly;
    int w = width();
    int h = height();

    painter.setPen(Qt::black);

    //
    //  LEFT_BTN
    //

    btn_poly << QPoint(0, 0) << QPoint(w / 2 + X_OFFSET, 0)
             << QPoint(w / 2 - X_OFFSET, h) << QPoint(0, h);

    if (isDown() && is_lhovered) {
        painter.setBrush(QColor::fromRgb(61, 112, 102));
    }
    else if (is_lhovered) {
        painter.setBrush(QColor::fromRgb(95, 172, 157));
    }
    else {
        painter.setBrush(QColor::fromRgb(119, 227, 206));
    }

    painter.drawPolygon(btn_poly);


    //
    //  RIGTH_BTN
    //

    btn_poly.clear();

    btn_poly << QPoint(w / 2 + X_OFFSET, 0) << QPoint(w, 0)
             << QPoint(w, h) << QPoint(w / 2 - X_OFFSET, h);

    if (isDown() && is_rhovered) {
        painter.setBrush(QColor::fromRgb(61, 112, 102));
    }
    else if (is_rhovered) {
        painter.setBrush(QColor::fromRgb(95, 172, 157));
    }
    else {
        painter.setBrush(QColor::fromRgb(110, 200, 182));
    }

    painter.drawPolygon(btn_poly);

    //
    //  LEFT_TEXT
    //

    painter.drawText(QRect(rect().x(), rect().y(),
                           rect().width() / 2, rect().height()),
        Qt::AlignCenter, ltext);

    painter.drawText(QRect(rect().x() + rect().width() / 2,
                           rect().y(), rect().width() / 2,
                           rect().height()),
                     Qt::AlignCenter, rtext);
}

void SlicedBtn::leaveEvent(QEvent *event)
{
    (void)event;

    is_lhovered = false;
    is_rhovered = false;
}

void SlicedBtn::mouseMoveEvent(QMouseEvent *event)
{
    int pos_x = event->pos().x();
    int pos_y = event->pos().y();
    int w = width();
    int h = height();

    last_lhovered = is_lhovered;
    last_rhovered = is_rhovered;

    if (pos_x <= w / 2 + X_OFFSET) { /* left_btn */
        is_lhovered = pos_x <= (w / 2 + X_OFFSET) - (X_OFFSET * 2)
                                                        * pos_y / h;
        is_rhovered = !is_lhovered;
    }
    else { /* right_btn */
        is_rhovered = true;
        is_lhovered = false;
    }

    if (is_lhovered != last_lhovered ||
        is_rhovered != last_rhovered)
    {
        update();
    }
}

void SlicedBtn::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;

    if (is_lhovered) {
        emit leftClicked();
    }
    else {
        emit rightClicked();
    }

    QPushButton::mouseReleaseEvent(event);
}
