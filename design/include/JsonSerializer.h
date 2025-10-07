#pragma once

#include "ISerializer.h"
#include <string>
#include <ostream>
#include <istream>

namespace kanban {

class JsonSerializer : public ISerializer {
public:
    std::string serialize(const Board& board) const override;
    void deserialize(const std::string& data, Board& outBoard) const override;

private:
    void serializeToStream(const Board& board, std::ostream& os) const;
    void deserializeFromStream(std::istream& is, Board& outBoard) const;
};

} // namespace kanban