#include "../include/ColumnWidget.h"
#include <QStyle>

ColumnWidget::ColumnWidget(kanban::Board& board, const kanban::Column& column, QWidget* parent) 
    : QFrame(parent), m_board(board), m_column(column) {
    setupUI();
}

void ColumnWidget::setupUI() {
    // Estilo da coluna
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setLineWidth(2);
    setMinimumWidth(250);
    setMaximumWidth(350);
    setStyleSheet("QFrame { background-color: #f0f0f0; margin: 5px; padding: 5px; }");
    
    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignTop);
    
    // Título da coluna
    m_titleLabel = new QLabel(QString::fromStdString(m_column.name()), this);
    m_titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14pt; padding: 5px; }");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(m_titleLabel);
    
    // Separador
    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    m_layout->addWidget(separator);
    
    // TODO: Adicionar CardWidgets depois
}