#include "../include/ColumnWidget.h"
#include "../include/CardWidget.h"  // ADD THIS
#include <QStyle>
#include <iostream>  // ADD FOR DEBUG

ColumnWidget::ColumnWidget(kanban::Board& board, const kanban::Column& column, QWidget* parent) 
    : QFrame(parent), m_board(board), m_column(column) {
    setupUI();
}

void ColumnWidget::setupUI() {
    // Estilo da coluna - ATUALIZADO
    setFrameStyle(QFrame::StyledPanel);
    setLineWidth(1);
    setMinimumWidth(280);
    setMaximumWidth(320);
    setStyleSheet(
        "QFrame { "
        "   background-color: #ebecf0; "
        "   border-radius: 8px; "
        "   margin: 8px; "
        "   padding: 8px; "
        "   border: 1px solid #dfe1e6; "
        "}"
    );
    
    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->setSpacing(8);
    
    // Título da coluna - ATUALIZADO
    m_titleLabel = new QLabel(QString::fromStdString(m_column.name()), this);
    m_titleLabel->setStyleSheet(
        "QLabel { "
        "   font-weight: bold; "
        "   font-size: 16px; "
        "   padding: 8px; "
        "   background-color: #dfe1e6; "
        "   border-radius: 4px; "
        "}"
    );
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(m_titleLabel);
    
    // Contador de cartões - NOVO
    QLabel* countLabel = new QLabel(
        QString("%1 cartões").arg(m_column.cardIds().size()), 
        this
    );
    countLabel->setStyleSheet("QLabel { color: #5e6c84; font-size: 12px; padding: 4px; }");
    countLabel->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(countLabel);
    
    // Separador
    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { color: #c1c7d0; }");
    m_layout->addWidget(separator);
    
    // Área para cartões REAIS - IMPLEMENTADO
    QWidget* cardsContainer = new QWidget(this);
    cardsContainer->setStyleSheet("QWidget { background-color: transparent; }");
    QVBoxLayout* cardsLayout = new QVBoxLayout(cardsContainer);
    cardsLayout->setAlignment(Qt::AlignTop);
    cardsLayout->setSpacing(6);
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    
    std::cout << "ColumnWidget '" << m_column.name() << "': " 
              << m_column.cardIds().size() << " cartões para renderizar" << std::endl;
    
    // Adicionar cartões REAIS da coluna
    if (!m_column.cardIds().empty()) {
        for (auto cardId : m_column.cardIds()) {
            if (const auto* card = m_board.findCard(cardId)) {
                std::cout << "  - Renderizando cartão: " << card->title() << std::endl;
                CardWidget* cardWidget = new CardWidget(*card, cardsContainer);
                cardsLayout->addWidget(cardWidget);
            } else {
                std::cout << "  - ERRO: Cartão ID " << cardId << " não encontrado!" << std::endl;
            }
        }
    } else {
        QLabel* emptyLabel = new QLabel("Vazio", cardsContainer);
        emptyLabel->setStyleSheet("QLabel { color: #5e6c84; font-style: italic; padding: 20px; }");
        emptyLabel->setAlignment(Qt::AlignCenter);
        cardsLayout->addWidget(emptyLabel);
    }
    
    cardsLayout->addStretch();
    m_layout->addWidget(cardsContainer);
}