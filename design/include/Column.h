#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include "Card.h"

namespace kanban {

/**
 * Column: lista de cartões dentro de um Board.
 * Regras:
 * - name não pode ser vazio (validação na Etapa 2).
 * - Mantém a ordenação dos cartões via índice.
 * - Não gerencia a vida de Card nesta etapa (armazenamento por valor no futuro ou ids).
 */
class Column {
public:
    using Id = std::uint64_t;

    // Construtores
    Column(Id id, std::string name, std::uint32_t orderIndex = 0);
    Column(const Column&) = default;
    Column(Column&&) noexcept = default;
    Column& operator=(const Column&) = default;
    Column& operator=(Column&&) noexcept = default;
    ~Column() = default;

    // Identidade e posição
    Id id() const noexcept;
    std::uint32_t orderIndex() const noexcept;
    void setOrderIndex(std::uint32_t index) noexcept;

    // Nome
    const std::string& name() const noexcept;
    void setName(std::string name);

    // Cartões (armazenados por id para baixo acoplamento entre Column e Card)
    const std::vector<Card::Id>& cardIds() const noexcept;

    // Operações sobre cartões na coluna
    bool addCard(Card::Id cardId);                       // adiciona ao fim se não existir
    bool insertCard(Card::Id cardId, std::size_t pos);   // insere em posição específica
    bool removeCard(Card::Id cardId) noexcept;           // remove se existir
    bool moveCardWithin(std::size_t from, std::size_t to) noexcept; // reordena internamente

    // Consulta
    std::optional<std::size_t> indexOf(Card::Id cardId) const noexcept;

    // Ordenação de colunas por orderIndex asc, depois nome asc
    bool operator<(const Column& other) const noexcept;

    // Igualdade por id
    bool operator==(const Column& other) const noexcept;

private:
    Id m_id;
    std::string m_name;
    std::uint32_t m_orderIndex;
    std::vector<Card::Id> m_cards;
};

} // namespace kanban
