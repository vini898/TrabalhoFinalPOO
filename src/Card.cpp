#include "Card.h"
#include <algorithm>  // ← ADICIONE ESTE INCLUDE

namespace kanban {

Card::Card(Id id, std::string title) 
    : m_id(id), m_title(std::move(title)) {
}

Card::Id Card::id() const noexcept {
    return m_id;
}

const std::string& Card::title() const noexcept {
    return m_title;
}

void Card::setTitle(std::string title) {
    m_title = std::move(title);
}

const std::string& Card::description() const noexcept {
    return m_description;
}

void Card::setDescription(std::string description) {
    m_description = std::move(description);
}

const std::optional<Card::TimePoint>& Card::dueDate() const noexcept {
    return m_dueDate;
}

void Card::setDueDate(std::optional<TimePoint> due) {
    m_dueDate = std::move(due);
}

const std::vector<std::string>& Card::tags() const noexcept {
    return m_tags;
}

void Card::addTag(std::string tag) {
    m_tags.push_back(std::move(tag));
}

bool Card::removeTag(const std::string& tag) noexcept {
    auto it = std::find(m_tags.begin(), m_tags.end(), tag);
    if (it != m_tags.end()) {
        m_tags.erase(it);
        return true;
    }
    return false;
}

bool Card::hasTag(const std::string& tag) const noexcept {
    return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
}

const std::optional<std::uint64_t>& Card::assigneeId() const noexcept {
    return m_assigneeId;
}

void Card::setAssigneeId(std::optional<std::uint64_t> userId) {
    m_assigneeId = userId;
}

const std::optional<int>& Card::priority() const noexcept {
    return m_priority;
}

void Card::setPriority(std::optional<int> prio) {
    m_priority = prio;
}

bool Card::operator<(const Card& other) const noexcept {
    // Ordenação: dueDate asc, prioridade desc, título asc
    if (m_dueDate != other.m_dueDate) {
        if (!m_dueDate) return false;
        if (!other.m_dueDate) return true;
        return *m_dueDate < *other.m_dueDate;
    }
    
    if (m_priority != other.m_priority) {
        if (!m_priority) return false;
        if (!other.m_priority) return true;
        return *m_priority > *other.m_priority; // desc
    }
    
    return m_title < other.m_title;
}

bool Card::operator==(const Card& other) const noexcept {
    return m_id == other.m_id;
}

} // namespace kanban