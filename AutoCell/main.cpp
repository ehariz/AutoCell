#include <QApplication>
#include <QDebug>
#include "cell.h"
#include "mainwindow.h"
<<<<<<< HEAD
//#include <iostream>
=======
#include "ruleeditor.h"
>>>>>>> master
int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    MainWindow w;
    w.show();

<<<<<<< HEAD
    /*std::string *s = new std::string;
    delete s;
    std::cout<<s;
    return 0;*/
=======
>>>>>>> master
    return app.exec();

}
