#pragma once

#include <cstdint>

namespace kanban {

class Board;

/**
 * IView: interface para visualizações (CLI/GUI).
 * Implementações futuras: CLIView, QtView, etc.
 */
class IView {
public:
    virtual ~IView() = default;

    // Renderiza o estado atual
    virtual void render(const Board& board) = 0;

    // Callback de movimento de cartão (para GUI)
    virtual void onMoveCard(std::uint64_t fromColumnId,
                            std::uint64_t toColumnId,
                            std::uint64_t cardId,
                            std::size_t toPos) = 0;
};

} // namespace kanban
