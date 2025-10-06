#include "../include/CardWidget.h"
#include <QHBoxLayout>
#include <iostream>

CardWidget::CardWidget(const kanban::Card& card, QWidget* parent) 
    : QFrame(parent), m_card(card) {
    std::cout << "CardWidget: Criando para cartão '" << card.title() << "'" << std::endl;
    setupUI();
    std::cout << "CardWidget: Setup completo" << std::endl;
}

void CardWidget::setupUI() {
    std::cout << "CardWidget: Configurando UI..." << std::endl;
    
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setLineWidth(1);
    setMinimumHeight(80);
    setMaximumHeight(120);
    setStyleSheet(
        "CardWidget { "
        "   background-color: #ffffff; "  // ← Mudei para branco sólido
        "   border-radius: 4px; "
        "   padding: 8px; "
        "   border: 2px solid #007acc; "  // ← Borda mais visível
        "}"
        "CardWidget:hover { "
        "   background-color: #f0f8ff; "
        "   border: 2px solid #005a9e; "
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(4);
    layout->setContentsMargins(6, 6, 6, 6);
    
    // Título do cartão - COM ESTILO FORTE
    QLabel* titleLabel = new QLabel(QString::fromStdString(m_card.title()), this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "   font-weight: bold; " 
        "   font-size: 14px; "
        "   color: #000000; "  // ← Preto para melhor contraste
        "   background-color: #e6f3ff; "
        "   padding: 4px; "
        "   border-radius: 2px; "
        "}"
    );
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel);
    
    std::cout << "CardWidget: Título adicionado: '" << m_card.title() << "'" << std::endl;
    
    // Descrição (se existir)
    if (!m_card.description().empty()) {
        QLabel* descLabel = new QLabel(QString::fromStdString(m_card.description()), this);
        descLabel->setStyleSheet(
            "QLabel { "
            "   color: #333333; "  // ← Cinza escuro
            "   font-size: 12px; "
            "   background-color: transparent; "
            "}"
        );
        descLabel->setWordWrap(true);
        descLabel->setMaximumHeight(40);
        layout->addWidget(descLabel);
        std::cout << "CardWidget: Descrição adicionada" << std::endl;
    }
    
    // Footer simples
    QHBoxLayout* footerLayout = new QHBoxLayout();
    
    // Prioridade (se existir)
    if (m_card.priority().has_value()) {
        QString priorityText;
        QString priorityColor;
        
        switch (*m_card.priority()) {
            case 1: priorityText = "🔥 Alta"; priorityColor = "#ff4444"; break;
            case 2: priorityText = "⚠️ Média"; priorityColor = "#ffaa00"; break;
            case 3: priorityText = "💚 Baixa"; priorityColor = "#44ff44"; break;
            default: priorityText = "📋 Normal"; priorityColor = "#888888"; break;
        }
        
        QLabel* priorityLabel = new QLabel(priorityText, this);
        priorityLabel->setStyleSheet(
            QString("QLabel { "
                   "   color: %1; "
                   "   font-size: 10px; "
                   "   font-weight: bold; "
                   "   background-color: #f8f8f8; "
                   "   padding: 2px 6px; "
                   "   border-radius: 3px; "
                   "}").arg(priorityColor)
        );
        footerLayout->addWidget(priorityLabel);
        std::cout << "CardWidget: Prioridade adicionada" << std::endl;
    }
    
    footerLayout->addStretch();
    layout->addLayout(footerLayout);
    
    std::cout << "CardWidget: UI completamente configurada" << std::endl;
}


void CardWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
    QFrame::mousePressEvent(event);
}

void CardWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }
    
    startDrag();
}

void CardWidget::startDrag() {
    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    
    // Passar dados do cartão via mime data
    mimeData->setText(QString::fromStdString(m_card.title()));
    mimeData->setData("application/x-kanban-card-id", 
                     QByteArray::number(m_card.id()));
    
    drag->setMimeData(mimeData);
    drag->setPixmap(grab()); // Captura visual do widget
    
    // Efeito visual durante o drag
    setStyleSheet(
        "CardWidget { "
        "   background-color: #f7f8f9; "
        "   border-radius: 4px; "
        "   padding: 8px; "
        "   border: 2px dashed #c1c7d0; "
        "   opacity: 0.7; "
        "}"
    );
    
    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
    
    // Restaurar estilo após o drag
    setStyleSheet(
        "CardWidget { "
        "   background-color: white; "
        "   border-radius: 4px; "
        "   padding: 8px; "
        "   border: 1px solid #dfe1e6; "
        "   cursor: grab; "
        "}"
        "CardWidget:hover { "
        "   background-color: #f7f8f9; "
        "   border: 1px solid #c1c7d0; "
        "}"
    );
}