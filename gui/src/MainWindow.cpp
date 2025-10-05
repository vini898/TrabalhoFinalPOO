#include "../include/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent),
      m_board(1, "Meu Quadro Kanban"),  // ← INICIALIZE O BOARD AQUI!
      m_newBoardAction(nullptr),
      m_openBoardAction(nullptr),
      m_saveBoardAction(nullptr),
      m_exitAction(nullptr),
      m_aboutAction(nullptr)
{
    setupUI();
    setupMenus();
    setupConnections();
    
    statusBar()->showMessage("Pronto - Kanban Lite");
}

void MainWindow::setupUI() {
    setWindowTitle("Kanban Lite");
    setMinimumSize(800, 600);
    setCentralWidget(new QWidget(this));
}

void MainWindow::setupMenus() {
    QMenu* fileMenu = menuBar()->addMenu("&Arquivo");
    m_newBoardAction = fileMenu->addAction("&Novo Quadro");
    m_openBoardAction = fileMenu->addAction("&Abrir...");
    m_saveBoardAction = fileMenu->addAction("&Salvar");
    fileMenu->addSeparator();
    m_exitAction = fileMenu->addAction("&Sair");
    
    QMenu* helpMenu = menuBar()->addMenu("&Ajuda");
    m_aboutAction = helpMenu->addAction("&Sobre");
}

void MainWindow::setupConnections() {
    // Conectar usando lambda functions
    connect(m_newBoardAction, &QAction::triggered, [this]() { onNewBoard(); });
    connect(m_openBoardAction, &QAction::triggered, [this]() { onOpenBoard(); });
    connect(m_saveBoardAction, &QAction::triggered, [this]() { onSaveBoard(); });
    connect(m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_aboutAction, &QAction::triggered, [this]() { onAbout(); });
}

void MainWindow::onNewBoard() {
    QMessageBox::information(this, "Novo Quadro", "Criando novo quadro...");
    // TODO: Implementar criação de novo quadro
}

void MainWindow::onOpenBoard() {
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir Quadro", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Abrir", "Abrindo: " + fileName);
        // TODO: Implementar carregamento
    }
}

void MainWindow::onSaveBoard() {
    QString fileName = QFileDialog::getSaveFileName(this, "Salvar Quadro", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Salvar", "Salvando: " + fileName);
        // TODO: Implementar salvamento
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "Sobre Kanban Lite", 
        "Kanban Lite v1.0\n\n"
        "Sistema de gerenciamento de tarefas colaborativas\n"
        "Desenvolvido para a disciplina de POO - C++\n"
        "Qt 6.9.3");
}