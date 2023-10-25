#include "CustomBar.hpp"
#include <QPainter>

CustomBar::CustomBar(QWidget *parent,
                     double smoothThresholdIn) : QWidget(parent), value(0.0)
{
    setValue(0.0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    smoothing=smoothThresholdIn;

}

double CustomBar::smoothValue(double oldValue,double newValue) const{
    if (oldValue==0.0){
        return newValue;
    } else{
        return newValue*(1-smoothing)+oldValue*smoothing;
    }
}

void CustomBar::setValue(double valueIn)
{
    value=smoothValue(value,valueIn);
    update(); // Request a repaint of the widget
}

void CustomBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set the background color
    painter.fillRect(rect(), Qt::lightGray);

    // Define the margins for the bar
    int leftMargin = 50;
    int rightMargin = 50;

    // Calculate the available width for the bar (excluding margins)
    int availableWidth = width() - leftMargin - rightMargin;

    // Calculate the width of the bar
    int barWidth = static_cast<int>(availableWidth * value);

    // Fill the left and right margins with white color
    painter.fillRect(0, 0, leftMargin, height(), Qt::black);
    painter.fillRect(width() - rightMargin, 0, rightMargin, height(), Qt::black);

    // Set the border color and width
    QPen borderPen(Qt::black);
    int borderThickness = 2; // Adjust this value to set the desired thickness
    borderPen.setWidth(borderThickness);
    painter.setPen(borderPen);

    // Draw the border around everything between the margins
    painter.drawRect(leftMargin, 0, availableWidth, height());

    // Determine the color for the progress bar based on the current progress
    QColor progressBarColor;
    if (value < 0.7)
    {
        // Red color for progress below 0.7
        progressBarColor = Qt::red;
    }
    else if (value < 0.8)
    {
        // Orange color for progress between 0.7 and 0.8
        progressBarColor = QColor(255, 165, 0); // Orange color
    }
    else
    {
        // Green color for progress equal to or above 0.8
        progressBarColor = Qt::green;
    }

    // Set the progress bar color
    painter.setBrush(progressBarColor);

    // Draw the progress bar with margins
    painter.drawRect(leftMargin, 0, barWidth, height());
}
