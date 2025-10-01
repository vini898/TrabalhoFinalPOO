#include "TagFilter.h"
#include "Card.h"

namespace kanban {

TagFilter::TagFilter(std::string tag)
    : m_tag(std::move(tag)) {
}

bool TagFilter::matches(const Card& card) const {
    return card.hasTag(m_tag);
}

} // namespace kanban