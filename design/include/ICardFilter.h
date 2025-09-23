#pragma once

namespace kanban {

class Card;

/**
 * ICardFilter: interface para filtros de cartões.
 * Implementações típicas: TagFilter, AssigneeFilter.
 */
class ICardFilter {
public:
    virtual ~ICardFilter() = default;
    virtual bool matches(const Card& card) const = 0;
};

} // namespace kanban
