#pragma once

#include <string>
#include <vector>
#include <optional>
#include <chrono>
#include <cstdint>

namespace kanban {

// Forward declarations
class User;

/**
 * Card: item básico do quadro Kanban.
 * Regras:
 * - id é imutável após construção.
 * - title não pode ser vazio (validado na implementação na Etapa 2).
 * - dueDate e assignee são opcionais.
 */
class Card {
public:
    using Id = std::uint64_t;
    using TimePoint = std::chrono::system_clock::time_point;

    // Construtores
    Card(Id id, std::string title);
    Card(const Card&) = default;
    Card(Card&&) noexcept = default;
    Card& operator=(const Card&) = default;
    Card& operator=(Card&&) noexcept = default;
    ~Card() = default;

    // Identidade
    Id id() const noexcept;

    // Título
    const std::string& title() const noexcept;
    void setTitle(std::string title);

    // Descrição
    const std::string& description() const noexcept;
    void setDescription(std::string description);

    // Vencimento (opcional)
    const std::optional<TimePoint>& dueDate() const noexcept;
    void setDueDate(std::optional<TimePoint> due);

    // Tags
    const std::vector<std::string>& tags() const noexcept;
    void addTag(std::string tag);
    bool removeTag(const std::string& tag) noexcept;
    bool hasTag(const std::string& tag) const noexcept;

    // Responsável (opcional, somente referência por id para evitar acoplamento)
    const std::optional<std::uint64_t>& assigneeId() const noexcept;
    void setAssigneeId(std::optional<std::uint64_t> userId);

    // Prioridade opcional (exemplo de atributo ordenável)
    const std::optional<int>& priority() const noexcept;
    void setPriority(std::optional<int> prio);

    // Ordenação canônica (por dueDate asc, depois prioridade desc, depois título asc)
    bool operator<(const Card& other) const noexcept;

    // Igualdade por id
    bool operator==(const Card& other) const noexcept;

private:
    Id m_id;
    std::string m_title;
    std::string m_description;
    std::optional<TimePoint> m_dueDate;
    std::vector<std::string> m_tags;
    std::optional<std::uint64_t> m_assigneeId;
    std::optional<int> m_priority;
};

} // namespace kanban
