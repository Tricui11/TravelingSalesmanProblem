#include <QtWidgets/QGraphicsTextItem>

class NumberedTextItem : public QGraphicsTextItem
{
public:
    NumberedTextItem(const QString& text, int x, int y) : QGraphicsTextItem(text)
    {
        setPos(x, y);
    }
};
