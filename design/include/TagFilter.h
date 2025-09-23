#pragma once

#include "ICardFilter.h"
#include <string>

namespace kanban {

class TagFilter final : public ICardFilter {
public:
    explicit TagFilter(std::string tag);
    bool matches(const Card& card) const override;

private:
    std::string m_tag;
};

} // namespace kanban
