#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include "../../design/include/Card.h"

class CardWidget : public QFrame {
    Q_OBJECT

public:
    CardWidget(const kanban::Card& card, QWidget* parent = nullptr);
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    
private:
    void setupUI();
    void startDrag();
    
    const kanban::Card& m_card;
    QPoint m_dragStartPosition;
};
