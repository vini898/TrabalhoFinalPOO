#include "../include/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Configuração da aplicação
    app.setApplicationName("Kanban Lite");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("POO Project");
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}