#include "ActivityLog.h"

namespace kanban {

void ActivityLog::append(Entry entry) {
    m_entries.push_back(std::move(entry));
}

const std::vector<ActivityLog::Entry>& ActivityLog::entries() const noexcept {
    return m_entries;
}

} // namespace kanban