#include "../include/CardWidget.h"
#include <QHBoxLayout>
#include <iostream>

CardWidget::CardWidget(const kanban::Card& card, QWidget* parent) 
    : QFrame(parent), m_card(card) {
    setupUI();
}

void CardWidget::setupUI() {
    // Frame estilo Trello
    this->setFrameStyle(QFrame::NoFrame);
    this->setMinimumSize(220, 80);
    this->setMaximumSize(280, 150);
    
    // CSS moderno - estilo Kanban
    this->setStyleSheet(
        "CardWidget {"
        "   background-color: #ffffff;"
        "   border: 1px solid #ddd;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   margin: 4px;"
        "   box-shadow: 0 1px 3px rgba(0,0,0,0.1);"
        "}"
        "CardWidget:hover {"
        "   background-color: #f9f9f9;"
        "   border: 1px solid #ccc;"
        "   box-shadow: 0 2px 5px rgba(0,0,0,0.15);"
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Título - estilo moderno
    QLabel* titleLabel = new QLabel(QString::fromStdString(m_card.title()), this);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   font-weight: 600;"
        "   font-size: 13px;"
        "   color: #172b4d;"
        "   background-color: transparent;"
        "   padding: 2px;"
        "   border: none;"
        "}"
    );
    titleLabel->setWordWrap(true);
    titleLabel->setMinimumHeight(18);
    layout->addWidget(titleLabel);
    
    // Descrição (se existir)
    if (!m_card.description().empty()) {
        QLabel* descLabel = new QLabel(QString::fromStdString(m_card.description()), this);
        descLabel->setStyleSheet(
            "QLabel {"
            "   color: #5e6c84;"
            "   font-size: 11px;"
            "   background-color: transparent;"
            "   padding: 1px 2px;"
            "   border: none;"
            "}"
        );
        descLabel->setWordWrap(true);
        descLabel->setMaximumHeight(32);
        layout->addWidget(descLabel);
    }
    
    // Container para tags e metadata
    QHBoxLayout* metaLayout = new QHBoxLayout();
    
    // Tags (se existirem)
    if (!m_card.tags().empty()) {
        for (const auto& tag : m_card.tags()) {
            QLabel* tagLabel = new QLabel(QString::fromStdString(tag), this);
            tagLabel->setStyleSheet(
                "QLabel {"
                "   background-color: #ebecf0;"
                "   color: #42526e;"
                "   border-radius: 4px;"
                "   padding: 2px 6px;"
                "   font-size: 10px;"
                "   font-weight: 500;"
                "   border: none;"
                "}"
            );
            metaLayout->addWidget(tagLabel);
        }
    }
    
    metaLayout->addStretch();
    
    // Indicadores à direita
    QHBoxLayout* indicatorsLayout = new QHBoxLayout();
    
    // Assignee indicator
    if (m_card.assigneeId().has_value()) {
        QLabel* assigneeLabel = new QLabel("👤", this);
        assigneeLabel->setStyleSheet("QLabel { font-size: 12px; color: #5e6c84; }");
        indicatorsLayout->addWidget(assigneeLabel);
    }
    
    // Priority indicator
    if (m_card.priority().has_value()) {
        QString emoji;
        QString color;
        
        switch (*m_card.priority()) {
            case 1: emoji = "🔴"; color = "#de350b"; break; // Alta - Vermelho
            case 2: emoji = "🟡"; color = "#ffab00"; break; // Média - Amarelo  
            case 3: emoji = "🟢"; color = "#36b37e"; break; // Baixa - Verde
            default: emoji = "⚪"; color = "#dfe1e6"; break; // Normal - Cinza
        }
        
        QLabel* priorityLabel = new QLabel(emoji, this);
        priorityLabel->setStyleSheet(QString("QLabel { font-size: 11px; color: %1; }").arg(color));
        indicatorsLayout->addWidget(priorityLabel);
    }
    
    metaLayout->addLayout(indicatorsLayout);
    layout->addLayout(metaLayout);
}

void CardWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
    QFrame::mousePressEvent(event);
}

void CardWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) return;
    
    startDrag();
}

void CardWidget::startDrag() {
    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    
    mimeData->setText(QString::fromStdString(m_card.title()));
    mimeData->setData("application/x-kanban-card-id", QByteArray::number(m_card.id()));
    
    drag->setMimeData(mimeData);
    drag->setPixmap(this->grab());
    
    // Efeito de drag
    this->setStyleSheet(
        "CardWidget {"
        "   background-color: #f8f9fa;"
        "   border: 2px dashed #4c9aff;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   margin: 4px;"
        "   opacity: 0.8;"
        "}"
    );
    
    drag->exec(Qt::MoveAction);
    
    // Restaurar estilo
    this->setStyleSheet(
        "CardWidget {"
        "   background-color: #ffffff;"
        "   border: 1px solid #ddd;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   margin: 4px;"
        "   box-shadow: 0 1px 3px rgba(0,0,0,0.1);"
        "}"
        "CardWidget:hover {"
        "   background-color: #f9f9f9;"
        "   border: 1px solid #ccc;"
        "   box-shadow: 0 2px 5px rgba(0,0,0,0.15);"
        "}"
    );
}