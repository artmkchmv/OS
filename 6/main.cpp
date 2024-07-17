#include "mywindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWindow window;
    window.setWindowTitle("Lab. 6 (GUI for Lab. 5)");
    window.setWindowIcon(QIcon(":/icon.png"));
    window.resize(600, 700);
    window.setStyleSheet("background-color: #01020c;");
    window.show();
    return a.exec();
}
