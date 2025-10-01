#include "JsonSerializer.h"
#include "Board.h"
#include <sstream>

namespace kanban {

std::string JsonSerializer::serialize(const Board& board) const {
    // Implementação simplificada para Etapa 2
    std::ostringstream oss;
    serializeToStream(board, oss);
    return oss.str();
}

void JsonSerializer::serializeToStream(const Board& board, std::ostream& os) const {
    // Serialização básica - será expandida na Etapa 3
    os << "{\"board_id\": " << board.id() << ", \"title\": \"" << board.title() << "\"}";
}

void JsonSerializer::deserialize(const std::string& data, Board& outBoard) const {
    std::istringstream iss(data);
    deserializeFromStream(iss, outBoard);
}

void JsonSerializer::deserializeFromStream(std::istream& is, Board& outBoard) const {
    // Desserialização básica - será expandida na Etapa 3
    // Por enquanto, apenas um placeholder
    std::string line;
    while (std::getline(is, line)) {
        // Processamento básico
    }
}

} // namespace kanban