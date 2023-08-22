#include <marquee_line_edit.h>
#include <QTimer>

MarqueeLineEdit::MarqueeLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MarqueeLineEdit::moveText);
    timer->start(300);
}

void MarqueeLineEdit::moveText()
{
    QString currentText = text();

    if (!currentText.isEmpty())
    {
        currentText = !movingRight ? currentText.right(1) + currentText.left(currentText.length() - 1) : currentText.mid(1) + currentText.at(0);
        setText(currentText);
    }
}

void MarqueeLineEdit::focusInEvent(QFocusEvent *event)
{
    timer->stop();
    QLineEdit::focusInEvent(event);
}

void MarqueeLineEdit::focusOutEvent(QFocusEvent *event)
{
    timer->start();
    QLineEdit::focusOutEvent(event);
}

void MarqueeLineEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text());
    int visibleWidth = width();
    if (textWidth > visibleWidth)
    {
        timer->start();
    }
    else
    {
        timer->stop();
        setText(text());
    }
}
