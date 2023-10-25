#ifndef CUSTOMPROGRESSBAR_H
#define CUSTOMPROGRESSBAR_H

#include <QWidget>

class CustomBar : public QWidget
{
Q_OBJECT

public:
    explicit CustomBar(QWidget *parent = nullptr,
                       double smoothing = 0.95);

    void setValue(double progress);

protected:
    void paintEvent(QPaintEvent *event) override;
    double smoothValue(double oldValue,double newValue) const;

private:
    double value;
    double smoothing;
};

#endif // CUSTOMPROGRESSBAR_H