#ifndef PLAYSTOPBTN_H
#define PLAYSTOPBTN_H

#include <array>

#include <QPainter>
#include <QPolygon>
#include <QPainterPath>

#include "roundbtn.h"

constexpr int ARROW_OFFSET = 11;
constexpr int ARROW_X_OFFSET = 2;
constexpr int RECT_OFFSET = 2;
constexpr int RECT_WIDTH = 8;
constexpr int RECT_HEIGHT = 20;

class PlayStopBtn : public RoundBtn
{
    Q_OBJECT

public:
    explicit PlayStopBtn(QWidget *parent = nullptr);
    ~PlayStopBtn();

    void setState(bool state);

signals:
    void playOrStop();

protected:
    void drawContent(QPainter &painter) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool is_on;
    int cur_path;
    std::array<QPainterPath, 2> paths;
};

#endif /* PLAYSTOPBTN_H */
