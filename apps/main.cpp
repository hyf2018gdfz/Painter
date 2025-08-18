#include <QApplication>
#include "../modules/vision/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/painter.ico"));

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}