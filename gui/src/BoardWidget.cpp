#include "../include/BoardWidget.h"
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

BoardWidget::BoardWidget(kanban::Board& board, QWidget* parent) 
    : QWidget(parent), m_board(board) {
    setupUI();
    refreshColumns();
}

void BoardWidget::setupUI() {
    m_layout = new QHBoxLayout(this);
    
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_columnsContainer = new QWidget();
    m_columnsContainer->setLayout(new QHBoxLayout());
    m_columnsContainer->layout()->setAlignment(Qt::AlignLeft);
    m_columnsContainer->layout()->setSpacing(10);
    
    m_scrollArea->setWidget(m_columnsContainer);
    m_layout->addWidget(m_scrollArea);
}

void BoardWidget::refreshColumns() {
    // Limpar colunas existentes
    QLayout* containerLayout = m_columnsContainer->layout();
    while (auto item = containerLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    
    // Se não há colunas, mostrar mensagem
    if (m_board.columns().empty()) {
        QLabel* emptyLabel = new QLabel("Nenhuma coluna disponível. Use Arquivo → Novo Quadro.", m_columnsContainer);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("QLabel { color: #666; font-size: 12pt; padding: 20px; }");
        containerLayout->addWidget(emptyLabel);
        return;
    }
    
    // Adicionar colunas do board
    for (const auto& column : m_board.columns()) {
        QFrame* columnFrame = new QFrame(m_columnsContainer);
        columnFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        columnFrame->setLineWidth(1);
        columnFrame->setMinimumWidth(280);
        columnFrame->setMaximumWidth(320);
        columnFrame->setMinimumHeight(500);
        columnFrame->setStyleSheet(
            "QFrame { "
            "   background-color: #ebecf0; "
            "   border-radius: 8px; "
            "   margin: 8px; "
            "   padding: 8px; "
            "}"
        );
        
        QVBoxLayout* columnLayout = new QVBoxLayout(columnFrame);
        columnLayout->setSpacing(8);
        
        // Título da coluna
        QLabel* titleLabel = new QLabel(QString::fromStdString(column.name()), columnFrame);
        titleLabel->setStyleSheet(
            "QLabel { "
            "   font-weight: bold; "
            "   font-size: 16px; "
            "   padding: 8px; "
            "   background-color: #dfe1e6; "
            "   border-radius: 4px; "
            "}"
        );
        titleLabel->setAlignment(Qt::AlignCenter);
        columnLayout->addWidget(titleLabel);
        
        // Contador de cartões
        QLabel* countLabel = new QLabel(
            QString("%1 cartões").arg(column.cardIds().size()), 
            columnFrame
        );
        countLabel->setStyleSheet("QLabel { color: #5e6c84; font-size: 12px; padding: 4px; }");
        countLabel->setAlignment(Qt::AlignCenter);
        columnLayout->addWidget(countLabel);
        
        // Separador
        QFrame* separator = new QFrame(columnFrame);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { color: #c1c7d0; }");
        columnLayout->addWidget(separator);
        
        // Área para cartões (vazia por enquanto)
        QWidget* cardsArea = new QWidget(columnFrame);
        cardsArea->setStyleSheet("QWidget { background-color: transparent; }");
        QVBoxLayout* cardsLayout = new QVBoxLayout(cardsArea);
        cardsLayout->setAlignment(Qt::AlignTop);
        cardsLayout->setSpacing(6);
        
        // Cartão de exemplo (remover depois)
        if (!column.cardIds().empty()) {
            QFrame* sampleCard = new QFrame(cardsArea);
            sampleCard->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
            sampleCard->setLineWidth(1);
            sampleCard->setStyleSheet(
                "QFrame { "
                "   background-color: white; "
                "   border-radius: 4px; "
                "   padding: 8px; "
                "   border: 1px solid #dfe1e6; "
                "}"
            );
            
            QVBoxLayout* cardLayout = new QVBoxLayout(sampleCard);
            
            QLabel* cardTitle = new QLabel("Cartão de Exemplo", sampleCard);
            cardTitle->setStyleSheet("QLabel { font-weight: bold; }");
            cardLayout->addWidget(cardTitle);
            
            QLabel* cardInfo = new QLabel("Arraste para mover", sampleCard);
            cardInfo->setStyleSheet("QLabel { color: #5e6c84; font-size: 11px; }");
            cardLayout->addWidget(cardInfo);
            
            cardsLayout->addWidget(sampleCard);
        } else {
            QLabel* emptyLabel = new QLabel("Vazio", cardsArea);
            emptyLabel->setStyleSheet("QLabel { color: #5e6c84; font-style: italic; padding: 20px; }");
            emptyLabel->setAlignment(Qt::AlignCenter);
            cardsLayout->addWidget(emptyLabel);
        }
        
        columnLayout->addWidget(cardsArea);
        columnLayout->addStretch(); // Empurra tudo para o topo
        
        containerLayout->addWidget(columnFrame);
    }
}