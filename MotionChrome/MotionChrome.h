#ifndef MOTIONCHROME_H
#define MOTIONCHROME_H

#include <QMainWindow>

namespace Ui {
class MotionChrome;
}

class MotionChrome : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MotionChrome(QWidget *parent = 0);
    ~MotionChrome();
    
private:
    Ui::MotionChrome *ui;
};

#endif // MOTIONCHROME_H
