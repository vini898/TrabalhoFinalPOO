#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include "../../design/include/Board.h"

class CardWidget;

class ColumnWidget : public QFrame {
    Q_OBJECT

public:
    ColumnWidget(kanban::Board& board, const kanban::Column& column, QWidget* parent = nullptr);
    
private:
    void setupUI();
    
    kanban::Board& m_board;
    const kanban::Column& m_column;
    QVBoxLayout* m_layout;
    QLabel* m_titleLabel;
};