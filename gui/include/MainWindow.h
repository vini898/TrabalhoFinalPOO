#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QFileDialog>
#include "BoardWidget.h"
#include "../../design/include/Board.h"
#include "CardDialog.h"
#include "JsonSerializer.h"  
#include <fstream>           

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

    void onNewBoard();
    void onOpenBoard();
    void onSaveBoard();
    void onAbout();

private:
    void setupUI();
    void setupMenus();
    void setupConnections();
    void setupDemoData();
    void onNewCard();


    kanban::Board m_board;
    BoardWidget* m_boardWidget;
    
    QAction* m_newBoardAction;
    QAction* m_openBoardAction;
    QAction* m_saveBoardAction;
    QAction* m_exitAction;
    QAction* m_aboutAction;
};