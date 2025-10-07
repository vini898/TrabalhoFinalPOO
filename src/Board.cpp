#include "Board.h"
#include "Card.h"
#include "Column.h"
#include "User.h"
#include "ActivityLog.h"
#include "ICardFilter.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>  

namespace kanban {

Board::Board(Id id, std::string title)
    : m_id(id), m_title(std::move(title)), m_cardSeq(1), m_columnSeq(1) {
    // Construtor - inicialização já está feita na lista de inicialização
}

Board::Id Board::id() const noexcept {
    return m_id;
}

const std::string& Board::title() const noexcept {
    return m_title;
}

void Board::setTitle(std::string title) {
    m_title = std::move(title);
}

const std::vector<User>& Board::users() const noexcept {
    return m_users;
}

User& Board::addUser(User user) {
    // Verifica se usuário já existe
    if (findUser(user.id())) {
        throw std::invalid_argument("User ID already exists");
    }
    m_users.push_back(std::move(user));
    return m_users.back();
}

bool Board::removeUser(std::uint64_t userId) noexcept {
    auto it = std::find_if(m_users.begin(), m_users.end(),
        [userId](const User& user) { return user.id() == userId; });
    
    if (it != m_users.end()) {
        m_users.erase(it);
        
        // Remove assignee dos cartões
        for (auto& card : m_cards) {
            if (card.assigneeId() == userId) {
                card.setAssigneeId(std::nullopt);
            }
        }
        return true;
    }
    return false;
}

User* Board::findUser(std::uint64_t userId) noexcept {
    auto it = std::find_if(m_users.begin(), m_users.end(),
        [userId](const User& user) { return user.id() == userId; });
    return it != m_users.end() ? &(*it) : nullptr;
}

const User* Board::findUser(std::uint64_t userId) const noexcept {
    return const_cast<Board*>(this)->findUser(userId);
}

const std::vector<Card>& Board::cards() const noexcept {
    return m_cards;
}

Card& Board::createCard(std::string title) {
    static Card::Id manualId = 1;
    Card::Id newId = manualId++;
    
    std::cout << "CRIANDO CARTÃO: " << title << " (ID: " << newId << ")" << std::endl;
    
    m_cards.emplace_back(newId, std::move(title));
    
    // Atualiza índice
    m_cardIndex[newId] = m_cards.size() - 1;
    
    std::cout << "Total de cartões: " << m_cards.size() << std::endl;
    
    // Verificar se a referência é válida
    Card& card = m_cards.back();
    std::cout << "Referência do cartão - ID: " << card.id() << ", Título: " << card.title() << std::endl;
    
    return card;
}

bool Board::deleteCard(Card::Id cardId) noexcept {
    auto indexIt = m_cardIndex.find(cardId);
    if (indexIt == m_cardIndex.end()) {
        return false;
    }
    
    std::size_t index = indexIt->second;
    m_cards.erase(m_cards.begin() + index);
    m_cardIndex.erase(indexIt);
    
    // Atualiza índices restantes
    for (auto& pair : m_cardIndex) {
        if (pair.second > index) {
            pair.second--;
        }
    }
    
    // Remove das colunas
    for (auto& column : m_columns) {
        column.removeCard(cardId);
    }
    
    return true;
}

Card* Board::findCard(Card::Id cardId) noexcept {
    auto it = m_cardIndex.find(cardId);
    return it != m_cardIndex.end() ? &m_cards[it->second] : nullptr;
}

const Card* Board::findCard(Card::Id cardId) const noexcept {
    return const_cast<Board*>(this)->findCard(cardId);
}

const std::vector<Column>& Board::columns() const noexcept {
    return m_columns;
}

Column& Board::addColumn(std::string name, std::uint32_t orderIndex) {
    static Column::Id manualId = 1;
    Column::Id newId = manualId++;
    
    std::cout << "CRIANDO COLUNA: " << name << " (ID: " << newId << ")" << std::endl;
    
    m_columns.emplace_back(newId, std::move(name), orderIndex);
    
    // Atualiza índice
    m_columnIndex[newId] = m_columns.size() - 1;
    
    std::cout << "Total de colunas: " << m_columns.size() << std::endl;
    
    // NÃO retornar referência - pode se tornar inválida
    // return m_columns.back();
    
    return m_columns.back(); // Vamos tentar uma última vez com debug
}

bool Board::removeColumn(Column::Id columnId) noexcept {
    auto indexIt = m_columnIndex.find(columnId);
    if (indexIt == m_columnIndex.end()) {
        return false;
    }
    
    std::size_t index = indexIt->second;
    m_columns.erase(m_columns.begin() + index);
    m_columnIndex.erase(indexIt);
    
    // Atualiza índices restantes
    for (auto& pair : m_columnIndex) {
        if (pair.second > index) {
            pair.second--;
        }
    }
    
    return true;
}

Column* Board::findColumn(Column::Id columnId) noexcept {
    auto it = m_columnIndex.find(columnId);
    return it != m_columnIndex.end() ? &m_columns[it->second] : nullptr;
}

const Column* Board::findColumn(Column::Id columnId) const noexcept {
    return const_cast<Board*>(this)->findColumn(columnId);
}

bool Board::reorderColumn(Column::Id columnId, std::uint32_t newOrder) noexcept {
    Column* column = findColumn(columnId);
    if (!column) {
        return false;
    }
    column->setOrderIndex(newOrder);
    return true;
}

bool Board::moveCard(Column::Id fromColumn, Column::Id toColumn, Card::Id cardId, std::size_t toPos) {
    Column* from = findColumn(fromColumn);
    Column* to = findColumn(toColumn);
    Card* card = findCard(cardId);
    
    if (!from || !to || !card) {
        return false;
    }
    
    // Remove da coluna origem
    if (!from->removeCard(cardId)) {
        return false;
    }
    
    // Adiciona na coluna destino
    return to->insertCard(cardId, toPos);
}

std::vector<const Card*> Board::filterCards(const ICardFilter& filter) const {
    std::vector<const Card*> result;
    for (const auto& card : m_cards) {
        if (filter.matches(card)) {
            result.push_back(&card);
        }
    }
    return result;
}

const ActivityLog& Board::activityLog() const noexcept {
    static ActivityLog defaultLog; // Retorna log padrão se não houver
    return m_log ? *m_log : defaultLog;
}

ActivityLog& Board::activityLog() noexcept {
    if (!m_log) {
        m_log = new ActivityLog(); // Gerenciamento simplificado
    }
    return *m_log;
}

Card::Id Board::nextCardId() noexcept {
    return m_cardSeq++;
}

Column::Id Board::nextColumnId() noexcept {
    return m_columnSeq++;
}

} // namespace kanban