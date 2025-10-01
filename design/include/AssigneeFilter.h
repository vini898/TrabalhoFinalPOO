#pragma once

#include "ICardFilter.h"
#include "Card.h"
#include <cstdint>
#include <optional>

namespace kanban {

class AssigneeFilter final : public ICardFilter {
public:
    explicit AssigneeFilter(std::uint64_t userId);
    bool matches(const Card& card) const override;

private:
    std::uint64_t m_userId;
};

} // namespace kanban
