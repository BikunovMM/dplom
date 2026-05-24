#include "skipbtn.h"

SkipBtn::SkipBtn(bool isright, QWidget *parent)
    : RoundBtn(parent), is_right(isright)
{

}

SkipBtn::~SkipBtn()
{

}

void SkipBtn::drawContent(QPainter& painter)
{
    painter.fillPath(arrows_ph0, Qt::black);
}

void SkipBtn::resizeEvent(QResizeEvent *event)
{
    (void)event;

    int rhw = width() / 2 + R_ARROW_X_OFFSET;
    int lhw = rhw - L_ARROW_X_OFFSET;
    int hh = height() / 2;

    arrows_ph0.clear();

    if (is_right) {
        arrows_ph0.moveTo(rhw - ARROWS_OFFSET, hh - ARROWS_OFFSET);
        arrows_ph0.lineTo(rhw, hh);
        arrows_ph0.lineTo(rhw- ARROWS_OFFSET, hh + ARROWS_OFFSET);

        arrows_ph0.moveTo(rhw, hh - ARROWS_OFFSET);
        arrows_ph0.lineTo(rhw + ARROWS_OFFSET, hh);
        arrows_ph0.lineTo(rhw, hh + ARROWS_OFFSET);
        arrows_ph0.closeSubpath();
    }
    else {
        arrows_ph0.moveTo(lhw - ARROWS_OFFSET, hh);
        arrows_ph0.lineTo(lhw, hh - ARROWS_OFFSET);
        arrows_ph0.lineTo(lhw, hh + ARROWS_OFFSET);

        arrows_ph0.moveTo(lhw, hh);
        arrows_ph0.lineTo(lhw + ARROWS_OFFSET, hh - ARROWS_OFFSET);
        arrows_ph0.lineTo(lhw + ARROWS_OFFSET, hh + ARROWS_OFFSET);
        arrows_ph0.closeSubpath();
    }
}
