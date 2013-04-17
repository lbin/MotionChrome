#include "MotionChrome.h"
#include "ui_MotionChromee.h"

MotionChrome::MotionChrome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MotionChrome)
{
    ui->setupUi(this);
}

MotionChrome::~MotionChrome()
{
    delete ui;
}
