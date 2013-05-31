#ifndef LAYOUTDESIGN_H
#define LAYOUTDESIGN_H
#include <QBoxLayout>

class LayoutDesign
{
public:
    LayoutDesign();

    static const QBoxLayout* designMainLayout()
    {
        QVBoxLayout* mainLayout = new QVBoxLayout;
        return mainLayout;
    }
};

#endif // LAYOUTDESIGN_H
