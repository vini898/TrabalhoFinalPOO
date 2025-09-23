#pragma once

#include "ISerializer.h"

namespace kanban {

/**
 * JsonSerializer: implementação de ISerializer usando JSON.
 * A implementação usará uma biblioteca JSON na Etapa 2 (ex.: nlohmann/json).
 */
class JsonSerializer final : public ISerializer {
public:
    JsonSerializer() = default;
    ~JsonSerializer() override = default;

    std::string serialize(const Board& board) const override;
    void serializeToStream(const Board& board, std::ostream& os) const override;
    void deserialize(const std::string& data, Board& outBoard) const override;
    void deserializeFromStream(std::istream& is, Board& outBoard) const override;
};

} // namespace kanban
