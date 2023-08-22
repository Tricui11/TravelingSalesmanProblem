#include <QLineEdit>
#include <QTimer>

class MarqueeLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    MarqueeLineEdit(QWidget *parent = nullptr);

protected slots:
    void moveText();

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QTimer *timer;
    bool movingRight;
};
