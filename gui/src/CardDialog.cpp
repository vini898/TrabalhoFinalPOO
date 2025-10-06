#include "../include/CardDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

CardDialog::CardDialog(kanban::Board& board, QWidget* parent) 
    : QDialog(parent), m_board(board) {
    setupUI();
    setWindowTitle("Novo Cartão");
    setMinimumSize(400, 300);
}

void CardDialog::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Título
    layout->addWidget(new QLabel("Título:"));
    m_titleEdit = new QLineEdit(this);
    layout->addWidget(m_titleEdit);
    
    // Descrição
    layout->addWidget(new QLabel("Descrição:"));
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setMaximumHeight(80);
    layout->addWidget(m_descriptionEdit);
    
    // Tags
    layout->addWidget(new QLabel("Tags (separadas por vírgula):"));
    m_tagsEdit = new QLineEdit(this);
    layout->addWidget(m_tagsEdit);
    
    // Assignee
    QHBoxLayout* assigneeLayout = new QHBoxLayout();
    assigneeLayout->addWidget(new QLabel("Responsável:"));
    m_assigneeCombo = new QComboBox(this);
    m_assigneeCombo->addItem("Nenhum", QVariant());
    for (const auto& user : m_board.users()) {
        m_assigneeCombo->addItem(
            QString::fromStdString(user.name()), 
            QVariant::fromValue(user.id())
        );
    }
    assigneeLayout->addWidget(m_assigneeCombo);
    assigneeLayout->addStretch();
    layout->addLayout(assigneeLayout);
    
    // Prioridade
    QHBoxLayout* priorityLayout = new QHBoxLayout();
    priorityLayout->addWidget(new QLabel("Prioridade:"));
    m_priorityCombo = new QComboBox(this);
    m_priorityCombo->addItem("Alta", 1);
    m_priorityCombo->addItem("Média", 2);
    m_priorityCombo->addItem("Baixa", 3);
    m_priorityCombo->addItem("Normal", 0);
    priorityLayout->addWidget(m_priorityCombo);
    priorityLayout->addStretch();
    layout->addLayout(priorityLayout);
    
    // Botões
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, 
        this
    );
    layout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString CardDialog::getTitle() const { return m_titleEdit->text(); }
QString CardDialog::getDescription() const { return m_descriptionEdit->toPlainText(); }

std::vector<std::string> CardDialog::getTags() const {
    std::vector<std::string> tags;
    QStringList tagList = m_tagsEdit->text().split(',', Qt::SkipEmptyParts);
    for (const auto& tag : tagList) {
        tags.push_back(tag.trimmed().toStdString());
    }
    return tags;
}

std::optional<std::uint64_t> CardDialog::getAssigneeId() const {
    QVariant data = m_assigneeCombo->currentData();
    return data.isValid() ? std::optional<std::uint64_t>(data.toULongLong()) : std::nullopt;
}

std::optional<int> CardDialog::getPriority() const {
    QVariant data = m_priorityCombo->currentData();
    return data.isValid() ? std::optional<int>(data.toInt()) : std::nullopt;
}