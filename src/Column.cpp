#include "Column.h"
#include <algorithm>
#include <iostream>

namespace kanban {

Column::Column(Id id, std::string name, std::uint32_t orderIndex) 
    : m_id(id), m_name(std::move(name)), m_orderIndex(orderIndex) {
}

Column::Id Column::id() const noexcept {
    return m_id;
}

std::uint32_t Column::orderIndex() const noexcept {
    return m_orderIndex;
}

void Column::setOrderIndex(std::uint32_t index) noexcept {
    m_orderIndex = index;
}

const std::string& Column::name() const noexcept {
    return m_name;
}

void Column::setName(std::string name) {
    m_name = std::move(name);
}

const std::vector<Card::Id>& Column::cardIds() const noexcept {
    return m_cards;
}

bool Column::addCard(Card::Id cardId) {
    std::cout << "=== Column::addCard ===" << std::endl;
    std::cout << "Coluna: " << m_name << " (ID: " << m_id << ")" << std::endl;
    std::cout << "Tentando adicionar cartão ID: " << cardId << std::endl;
    std::cout << "Cartões atuais na coluna: " << m_cards.size() << std::endl;
    
    // Verificar se o cartão já existe na coluna
    auto it = std::find(m_cards.begin(), m_cards.end(), cardId);
    if (it != m_cards.end()) {
        std::cout << "❌ Cartão já existe na coluna" << std::endl;
        return false;
    }
    
    std::cout << "✅ Cartão não existe, adicionando..." << std::endl;
    m_cards.push_back(cardId);
    std::cout << "✅ Cartão adicionado. Total agora: " << m_cards.size() << std::endl;
    std::cout << "=== FIM Column::addCard ===" << std::endl;
    
    return true;
}

bool Column::insertCard(Card::Id cardId, std::size_t pos) {
    if (std::find(m_cards.begin(), m_cards.end(), cardId) != m_cards.end()) {
        return false; // já existe
    }
    if (pos > m_cards.size()) {
        pos = m_cards.size();
    }
    m_cards.insert(m_cards.begin() + pos, cardId);
    return true;
}

bool Column::removeCard(Card::Id cardId) noexcept {
    auto it = std::find(m_cards.begin(), m_cards.end(), cardId);
    if (it != m_cards.end()) {
        m_cards.erase(it);
        return true;
    }
    return false;
}

bool Column::moveCardWithin(std::size_t from, std::size_t to) noexcept {
    if (from >= m_cards.size() || to >= m_cards.size()) {
        return false;
    }
    if (from == to) return true;
    
    Card::Id cardId = m_cards[from];
    m_cards.erase(m_cards.begin() + from);
    m_cards.insert(m_cards.begin() + (to > from ? to - 1 : to), cardId);
    return true;
}

std::optional<std::size_t> Column::indexOf(Card::Id cardId) const noexcept {
    auto it = std::find(m_cards.begin(), m_cards.end(), cardId);
    if (it != m_cards.end()) {
        return std::distance(m_cards.begin(), it);
    }
    return std::nullopt;
}

bool Column::operator<(const Column& other) const noexcept {
    if (m_orderIndex != other.m_orderIndex) {
        return m_orderIndex < other.m_orderIndex;
    }
    return m_name < other.m_name;
}

bool Column::operator==(const Column& other) const noexcept {
    return m_id == other.m_id;
}

} // namespace kanban