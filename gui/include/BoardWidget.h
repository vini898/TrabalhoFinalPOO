#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include "../../design/include/Board.h"

class CardWidget;

class BoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit BoardWidget(kanban::Board& board, QWidget* parent = nullptr);
    void refreshColumns();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    void setupUI();
    void highlightColumn(int columnIndex, bool highlight);
    int findColumnAtPosition(const QPoint& pos);
    
    kanban::Board& m_board;
    QHBoxLayout* m_layout;
    QScrollArea* m_scrollArea;
    QWidget* m_columnsContainer;
    int m_highlightedColumn;
};