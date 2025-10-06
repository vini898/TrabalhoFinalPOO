#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QListWidget>
#include <QDialogButtonBox>
#include "../../design/include/Board.h"

class CardDialog : public QDialog {
    Q_OBJECT

public:
    CardDialog(kanban::Board& board, QWidget* parent = nullptr);
    
    QString getTitle() const;
    QString getDescription() const;
    std::vector<std::string> getTags() const;
    std::optional<std::uint64_t> getAssigneeId() const;
    std::optional<int> getPriority() const;

private:
    void setupUI();
    
    kanban::Board& m_board;
    QLineEdit* m_titleEdit;
    QTextEdit* m_descriptionEdit;
    QLineEdit* m_tagsEdit;
    QComboBox* m_assigneeCombo;
    QComboBox* m_priorityCombo;
};