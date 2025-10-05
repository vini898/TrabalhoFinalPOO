#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QFileDialog>
#include "../../design/include/Board.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

    // Métodos públicos em vez de slots por enquanto
    void onNewBoard();
    void onOpenBoard();
    void onSaveBoard();
    void onAbout();

private:
    void setupUI();
    void setupMenus();
    void setupConnections();

    kanban::Board m_board;
    
    // Actions
    QAction* m_newBoardAction;
    QAction* m_openBoardAction;
    QAction* m_saveBoardAction;
    QAction* m_exitAction;
    QAction* m_aboutAction;
};