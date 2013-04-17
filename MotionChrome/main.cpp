#include "MotionChrome.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MotionChrome w;
    w.show();
    
    return a.exec();
}
