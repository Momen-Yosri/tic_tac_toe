#include <QApplication>
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    tictactoe::MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
} 