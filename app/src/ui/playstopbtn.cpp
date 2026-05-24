#include "playstopbtn.h"

PlayStopBtn::PlayStopBtn(QWidget *parent)
    : RoundBtn(parent), cur_path(0)
{

}

PlayStopBtn::~PlayStopBtn()
{

}

void PlayStopBtn::setState(bool state) {
    cur_path = state; /* implicit casting bool to int (f: 0, t: 1) */
    update();
}

void PlayStopBtn::drawContent(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.fillPath(paths[cur_path], Qt::black);
}

void PlayStopBtn::mouseReleaseEvent(QMouseEvent *event)
{
    cur_path = cur_path == 0 ? 1 : 0;
    update();

    emit playOrStop();

    RoundBtn::mouseReleaseEvent(event);
}

void PlayStopBtn::resizeEvent(QResizeEvent *event)
{
    (void)event;

    int hw = width()  / 2;
    int hh = height() / 2;

    paths[0].clear();

    paths[0].moveTo(hw - ARROW_OFFSET + ARROW_X_OFFSET,
        hh - ARROW_OFFSET);
    paths[0].lineTo(hw + ARROW_OFFSET + ARROW_X_OFFSET, hh);
    paths[0].lineTo(hw - ARROW_OFFSET + ARROW_X_OFFSET,
        hh + ARROW_OFFSET);
    paths[0].closeSubpath();

    paths[1].clear();

    paths[1].addRect(hw - RECT_WIDTH - RECT_OFFSET,
                 hh - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT);
    paths[1].addRect(hw + RECT_OFFSET,
                 hh - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT);
}
