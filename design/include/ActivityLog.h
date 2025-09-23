#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>
#include <optional>

namespace kanban {

class ActivityLog {
public:
    struct Entry {
        std::chrono::system_clock::time_point timestamp;
        std::optional<std::uint64_t> actorUserId;
        std::string action;        // ex.: "MOVE_CARD", "CREATE_CARD"
        std::string details;       // mensagem livre ou JSON leve
    };

    ActivityLog() = default;
    ActivityLog(const ActivityLog&) = default;
    ActivityLog(ActivityLog&&) noexcept = default;
    ActivityLog& operator=(const ActivityLog&) = default;
    ActivityLog& operator=(ActivityLog&&) noexcept = default;
    ~ActivityLog() = default;

    // Acrescenta uma entrada
    void append(Entry entry);

    // Leitura
    const std::vector<Entry>& entries() const noexcept;

private:
    std::vector<Entry> m_entries;
};

} // namespace kanban
