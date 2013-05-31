#ifndef MOTIONCHROME_H
#define MOTIONCHROME_H

#include <QMainWindow>
#include <OpenSceneGraph/MainScene.h>




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

    MainScene* mainScene;

};

#endif // MOTIONCHROME_H
