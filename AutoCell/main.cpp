#include <QApplication>
#include <QDebug>
#include "cell.h"
#include "mainwindow.h"
#include <iostream>
int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    MainWindow w;
    w.show();

    std::string *s = new std::string;
    delete s;
    std::cout<<s;
    return 0;
    //return app.exec();

}
