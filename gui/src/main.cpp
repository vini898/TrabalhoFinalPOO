#include "MainWindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "=== KANBAN LITE - APLICAÇÃO COMPLETA ===" << std::endl;
    
    QApplication app(argc, argv);
    app.setApplicationName("Kanban Lite");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("POO Project");
    
    try {
        std::cout << "Iniciando Kanban Lite..." << std::endl;
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
        
    } catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
        return 1;
    }
}