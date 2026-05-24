#ifndef LINE_H
#define LINE_H

#include <QWidget>
#include <QPainter>

class Line : public QWidget
{
    Q_OBJECT

public:
    explicit Line(int x, int y, unsigned int length,
         unsigned int width,
         QColor color = Qt::lightGray,
         QWidget* parent = nullptr);
    ~Line();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int x;
    int y;
    unsigned int length;
    unsigned int width;
    QColor color;
};

#endif /* LINE_H */
