#include "AssigneeFilter.h"
#include "Card.h"

namespace kanban {

AssigneeFilter::AssigneeFilter(std::uint64_t userId) 
    : m_userId(userId) {
}

bool AssigneeFilter::matches(const Card& card) const {
    return card.assigneeId() == m_userId;
}

} // namespace kanban