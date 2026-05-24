#ifndef TRAPLABEL_H
#define TRAPLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

constexpr int Y_OFFSET = 12;

class TrapLabel : public QWidget
{
    Q_OBJECT

public:
    explicit TrapLabel(QString str, bool isright = true,
                       QWidget *parent = nullptr);
    ~TrapLabel();

    void setText(const QString& str);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString text;
    bool is_right;
};

#endif /* TRAPLABEL_H */
