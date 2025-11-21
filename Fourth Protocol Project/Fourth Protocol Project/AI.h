#ifndef AI_HPP
#define AI_HPP

#include "Grid.h"
#include <vector>
#include <utility>

// simple AI that uses random moves (minmax later)
class AI
{
public:
    AI();
    ~AI();

    void makeMove(Grid& t_grid);

private:
    // placement helpers
    void placePiece(Grid& t_grid);
    PieceType choosePieceType(Grid& t_grid, Player t_player);
    std::pair<int, int> choosePlacementPos(Grid& t_grid);

    // AI movement
    void movePiece(Grid& t_grid);
    std::pair<int, int> chooseRandomPiece(Grid& t_grid, Player t_player);
    std::pair<int, int> chooseRandomMove(Grid& t_grid, int t_fromRow, int t_fromCol);

    bool isCellEmpty(Grid& t_grid, int t_row, int t_col) const;
    std::vector<std::pair<int, int>> getValidMoves(Grid& t_grid, int t_fromRow, int t_fromCol);
};

#endif