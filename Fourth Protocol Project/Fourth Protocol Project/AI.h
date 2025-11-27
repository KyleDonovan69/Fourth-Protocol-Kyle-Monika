#ifndef AI_HPP
#define AI_HPP

#include "Grid.h"
#include <vector>
#include <utility>
#include "Constants.h"

// simple AI that now uses minimax
class AI
{
public:
    AI();
    ~AI();

    void makeMove(Grid& t_grid);

private:
    struct Move//store move info
    {
        int fromRow;
        int fromCol;
        int toRow;
        int toCol;
        int score;
    };
    // placement helpers
    void placePiece(Grid& t_grid);
    PieceType choosePieceType(Grid& t_grid, Player t_player);
    std::pair<int, int> choosePlacementPos(Grid& t_grid);

    // AI movement(random ones)
    void movePiece(Grid& t_grid);
    std::pair<int, int> chooseRandomPiece(Grid& t_grid, Player t_player);
    std::pair<int, int> chooseRandomMove(Grid& t_grid, int t_fromRow, int t_fromCol);

    //minimax moves
    std::vector<Move> getAllPossibleMoves(Grid& t_grid, Player t_player);
    std::vector<std::pair<int, int>> getValidMoves(Grid& t_grid, int t_fromRow, int t_fromCol);
    bool isCellEmpty(Grid& t_grid, int t_row, int t_col) const;
    Move findBestMove(Grid& t_grid, Player t_player);
    int minimax(Grid& t_grid, int t_depth, bool t_isMaximizing, Player t_aiPlayer, int t_alpha, int t_beta);
    int evaluateBoard(Grid& t_grid, Player t_aiPlayer);

    void testMove(Grid& t_grid, const Move& t_move);//helps ai test moves first before deciding
    void undoMove(Grid& t_grid, const Move& t_move, PieceType t_capturedType, Player t_capturedOwner);

    int count4InARow(Grid& t_grid, Player t_player);
    int count3InARow(Grid& t_grid, Player t_player);//checks the board for rows of 3
    int countPotentialWins(Grid& t_grid, Player t_player);

};

#endif