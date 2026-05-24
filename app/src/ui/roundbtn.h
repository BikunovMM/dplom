#ifndef ROUNDBTN_H
#define ROUNDBTN_H

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

class RoundBtn : public QPushButton
{
    Q_OBJECT

public:
    explicit RoundBtn(QWidget *parent = nullptr);
    ~RoundBtn();

protected:
    virtual void drawContent(QPainter& painter) = 0;
    void paintEvent(QPaintEvent *event) override;
    bool hitButton(const QPoint &pos) const override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    bool is_hovered;
    bool last_hovered;
};

#endif /* ROUNDBTN_H */
