#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>

namespace kanban {

class Card;
class Column;
class User;
class ICardFilter;
class ActivityLog;

/**
 * Board: agrega colunas, cartões e usuários.
 * - Dono da coleção de Cards e Columns (armazenamento por valor na Etapa 2).
 * - Operações de alto nível: criar/mover/filtrar cartões, gerenciar colunas.
 */
class Board {
public:
    using Id = std::uint64_t;

    // Construtores
    Board(Id id, std::string title);
    Board(const Board&) = default;
    Board(Board&&) noexcept = default;
    Board& operator=(const Board&) = default;
    Board& operator=(Board&&) noexcept = default;
    ~Board() = default;

    // Identidade
    Id id() const noexcept;

    // Título
    const std::string& title() const noexcept;
    void setTitle(std::string title);

    // Usuários
    const std::vector<User>& users() const noexcept;
    User& addUser(User user);
    bool removeUser(std::uint64_t userId) noexcept;
    User* findUser(std::uint64_t userId) noexcept;
    const User* findUser(std::uint64_t userId) const noexcept;

    // Cartões
    const std::vector<Card>& cards() const noexcept;
    Card& createCard(std::string title);                    // gera id interno
    bool deleteCard(Card::Id cardId) noexcept;
    Card* findCard(Card::Id cardId) noexcept;
    const Card* findCard(Card::Id cardId) const noexcept;

    // Colunas
    const std::vector<Column>& columns() const noexcept;
    Column& addColumn(std::string name, std::uint32_t orderIndex = 0); // gera id
    bool removeColumn(Column::Id columnId) noexcept;
    Column* findColumn(Column::Id columnId) noexcept;
    const Column* findColumn(Column::Id columnId) const noexcept;
    bool reorderColumn(Column::Id columnId, std::uint32_t newOrder) noexcept;

    // Movimento de cartão entre colunas
    bool moveCard(Column::Id fromColumn, Column::Id toColumn, Card::Id cardId, std::size_t toPos);

    // Filtro polimórfico
    std::vector<const Card*> filterCards(const ICardFilter& filter) const;

    // Activity log
    const ActivityLog& activityLog() const noexcept;
    ActivityLog& activityLog() noexcept;

private:
    // Geração de ids simples
    Card::Id nextCardId() noexcept;
    Column::Id nextColumnId() noexcept;

private:
    Id m_id;
    std::string m_title;

    std::vector<User> m_users;
    std::vector<Card> m_cards;
    std::vector<Column> m_columns;

    // Índices auxiliares (planejados para Etapa 2)
    std::unordered_map<Card::Id, std::size_t> m_cardIndex;
    std::unordered_map<Column::Id, std::size_t> m_columnIndex;

    // Geradores
    Card::Id m_cardSeq{1};
    Column::Id m_columnSeq{1};

    // Log
    ActivityLog* m_log{nullptr}; // será gerenciado na implementação (Etapa 2)
};

} // namespace kanban
