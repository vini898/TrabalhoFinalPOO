#include "../include/BoardWidget.h"
#include "../include/CardWidget.h"
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <iostream>

BoardWidget::BoardWidget(kanban::Board& board, QWidget* parent) 
    : QWidget(parent), m_board(board), m_highlightedColumn(-1) {
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
    
    // Habilitar drop
    setAcceptDrops(true);
}

void BoardWidget::refreshColumns() {
    std::cout << "BoardWidget: Atualizando colunas..." << std::endl;
    
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
        std::cout << "Processando coluna: " << column.name() << " com " << column.cardIds().size() << " cartões" << std::endl;
        
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
        
        // Área para cartões REAIS
        QWidget* cardsArea = new QWidget(columnFrame);
        cardsArea->setStyleSheet("QWidget { background-color: transparent; }");
        QVBoxLayout* cardsLayout = new QVBoxLayout(cardsArea);
        cardsLayout->setAlignment(Qt::AlignTop);
        cardsLayout->setSpacing(6);
        
        // Adicionar cartões REAIS da coluna
        if (!column.cardIds().empty()) {
            std::cout << "Adicionando " << column.cardIds().size() << " cartões visíveis" << std::endl;
            for (auto cardId : column.cardIds()) {
                if (const auto* card = m_board.findCard(cardId)) {
                    std::cout << "Cartão: " << card->title() << " (ID: " << card->id() << ")" << std::endl;
                    CardWidget* cardWidget = new CardWidget(*card, cardsArea);
                    cardsLayout->addWidget(cardWidget);
                } else {
                    std::cout << "ERRO: Cartão ID " << cardId << " não encontrado!" << std::endl;
                }
            }
        } else {
            std::cout << "Coluna vazia" << std::endl;
            QLabel* emptyLabel = new QLabel("Vazio", cardsArea);
            emptyLabel->setStyleSheet("QLabel { color: #5e6c84; font-style: italic; padding: 20px; }");
            emptyLabel->setAlignment(Qt::AlignCenter);
            cardsLayout->addWidget(emptyLabel);
        }
        
        columnLayout->addWidget(cardsArea);
        columnLayout->addStretch();
        
        containerLayout->addWidget(columnFrame);
    }
    
    std::cout << "BoardWidget: Atualização completa" << std::endl;
}

void BoardWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-kanban-card-id")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void BoardWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (!event->mimeData()->hasFormat("application/x-kanban-card-id")) {
        event->ignore();
        return;
    }
    
    int columnIndex = findColumnAtPosition(event->position().toPoint());
    if (columnIndex != m_highlightedColumn) {
        // Remover highlight anterior
        if (m_highlightedColumn != -1) {
            highlightColumn(m_highlightedColumn, false);
        }
        
        // Aplicar novo highlight
        m_highlightedColumn = columnIndex;
        if (m_highlightedColumn != -1) {
            highlightColumn(m_highlightedColumn, true);
        }
    }
    
    event->acceptProposedAction();
}

void BoardWidget::dragLeaveEvent(QDragLeaveEvent* event) {
    if (m_highlightedColumn != -1) {
        highlightColumn(m_highlightedColumn, false);
        m_highlightedColumn = -1;
    }
    event->accept();
}

void BoardWidget::dropEvent(QDropEvent* event) {
    // Remover highlight
    if (m_highlightedColumn != -1) {
        highlightColumn(m_highlightedColumn, false);
        m_highlightedColumn = -1;
    }
    
    if (!event->mimeData()->hasFormat("application/x-kanban-card-id")) {
        event->ignore();
        return;
    }
    
    // Obter ID do cartão
    QByteArray itemData = event->mimeData()->data("application/x-kanban-card-id");
    kanban::Card::Id cardId = itemData.toULongLong();
    
    // Encontrar coluna destino
    int targetColumnIndex = findColumnAtPosition(event->position().toPoint());
    if (targetColumnIndex == -1) {
        event->ignore();
        return;
    }
    
    // Encontrar coluna origem e destino
    kanban::Column::Id fromColumnId = 0;
    kanban::Column::Id toColumnId = 0;
    
    int columnIndex = 0;
    for (const auto& col : m_board.columns()) {
        if (col.indexOf(cardId).has_value()) {
            fromColumnId = col.id();
        }
        if (columnIndex == targetColumnIndex) {
            toColumnId = col.id();
        }
        columnIndex++;
    }
    
    if (fromColumnId != 0 && toColumnId != 0 && fromColumnId != toColumnId) {
        // Mover cartão
        if (m_board.moveCard(fromColumnId, toColumnId, cardId, 0)) {
            refreshColumns();
            
            // Registrar no activity log
            kanban::ActivityLog::Entry entry;
            entry.timestamp = std::chrono::system_clock::now();
            entry.action = "MOVE_CARD";
            entry.details = "Cartão " + std::to_string(cardId) + 
                           " movido da coluna " + std::to_string(fromColumnId) + 
                           " para " + std::to_string(toColumnId);
            m_board.activityLog().append(std::move(entry));
            
            event->acceptProposedAction();
            return;
        }
    }
    
    event->ignore();
}

int BoardWidget::findColumnAtPosition(const QPoint& pos) {
    for (int i = 0; i < m_columnsContainer->layout()->count(); ++i) {
        QLayoutItem* item = m_columnsContainer->layout()->itemAt(i);
        if (item && item->widget()) {
            QWidget* columnWidget = item->widget();
            QRect columnRect = columnWidget->geometry();
            QPoint columnPos = m_columnsContainer->mapFromParent(pos);
            
            if (columnRect.contains(columnPos)) {
                return i;
            }
        }
    }
    return -1;
}

void BoardWidget::highlightColumn(int columnIndex, bool highlight) {
    QLayoutItem* item = m_columnsContainer->layout()->itemAt(columnIndex);
    if (item && item->widget()) {
        QFrame* columnFrame = qobject_cast<QFrame*>(item->widget());
        if (columnFrame) {
            if (highlight) {
                columnFrame->setStyleSheet(
                    "QFrame { "
                    "   background-color: #dbe1f1; "
                    "   border-radius: 8px; "
                    "   margin: 8px; "
                    "   padding: 8px; "
                    "   border: 2px solid #4c9aff; "
                    "}"
                );
            } else {
                columnFrame->setStyleSheet(
                    "QFrame { "
                    "   background-color: #ebecf0; "
                    "   border-radius: 8px; "
                    "   margin: 8px; "
                    "   padding: 8px; "
                    "   border: 1px solid #dfe1e6; "
                    "}"
                );
            }
        }
    }
}