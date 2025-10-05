#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include "../../design/include/Board.h"

class BoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit BoardWidget(kanban::Board& board, QWidget* parent = nullptr);
    void refreshColumns();

private:
    void setupUI();
    
    kanban::Board& m_board;
    QHBoxLayout* m_layout;
    QScrollArea* m_scrollArea;
    QWidget* m_columnsContainer;
};