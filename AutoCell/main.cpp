#include <QApplication>
#include <QDebug>
#include "cell.h"
#include "mainwindow.h"
#include "ruleeditor.h"
int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    app.setOrganizationName("LO21-project");
    app.setApplicationName("AutoCell");

    MainWindow w;
    w.show();

    return app.exec();

}
