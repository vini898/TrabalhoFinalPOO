#pragma once

#include <iosfwd>
#include <string>

namespace kanban {

class Board;

/**
 * ISerializer: contrato para salvar/carregar um Board.
 * Implementações podem serializar para JSON, XML, etc.
 */
class ISerializer {
public:
    virtual ~ISerializer() = default;

    // Serializa para uma string
    virtual std::string serialize(const Board& board) const = 0;

    // Serializa para um stream de saída
    virtual void serializeToStream(const Board& board, std::ostream& os) const = 0;

    // Carrega de uma string (substitui conteúdo do destino)
    virtual void deserialize(const std::string& data, Board& outBoard) const = 0;

    // Carrega de um stream de entrada
    virtual void deserializeFromStream(std::istream& is, Board& outBoard) const = 0;
};

} // namespace kanban
