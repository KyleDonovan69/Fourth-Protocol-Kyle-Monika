#include "AI.h"
#include <cstdlib>
#include <ctime>

AI::AI()
{
    srand(static_cast<unsigned>(time(nullptr)));
}

AI::~AI()
{
}

void AI::makeMove(Grid& t_grid)
{
    // check the state for actions
    if (t_grid.getGameState() == GameState::PLACEMENT)
    {
        placePiece(t_grid);
    }
    else if (t_grid.getGameState() == GameState::MOVEMENT)
    {
        movePiece(t_grid);
    }
}

void AI::placePiece(Grid& t_grid)
{
    Player currentPlayer = t_grid.getCurrentPlayer();

    // pick a piece type thats left
    PieceType pieceType = choosePieceType(t_grid, currentPlayer);
    t_grid.setSelectedPiece(pieceType);

    // find an empty spot
    std::pair<int, int> position = choosePlacementPos(t_grid);

    // place the piece
    t_grid.placePiece(position.first, position.second);
}

PieceType AI::choosePieceType(Grid& t_grid, Player t_player)
{
    // just pick the first piece type it has available
    if (t_grid.getRemainingPieces(t_player, PieceType::FROG) > 0)
    {
        return PieceType::FROG;
    }
    else if (t_grid.getRemainingPieces(t_player, PieceType::SNAKE) > 0)
    {
        return PieceType::SNAKE;
    }
    else if (t_grid.getRemainingPieces(t_player, PieceType::DONKEY) > 0)
    {
        return PieceType::DONKEY;
    }

    return PieceType::FROG; // fallback
}

std::pair<int, int> AI::choosePlacementPos(Grid& t_grid)
{
    // find all empty cells
    std::vector<std::pair<int, int>> emptyCells;

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (isCellEmpty(t_grid, row, col))
            {
                emptyCells.push_back(std::make_pair(row, col));//push them for the ai to check
            }
        }
    }

    // pick a random empty cell from list
    if (!emptyCells.empty())
    {
        int randomIndex = rand() % emptyCells.size();
        return emptyCells[randomIndex];
    }

    // fallback to center just in case
    return std::make_pair(2, 2);
}

void AI::movePiece(Grid& t_grid)
{
    Player currentPlayer = t_grid.getCurrentPlayer();

    // pick a random piece thats placed
    std::pair<int, int> piecePos = chooseRandomPiece(t_grid, currentPlayer);

    if (piecePos.first == -1)
    {
        return; // no pieces found
    }

    // get valid moves for said piece
    std::vector<std::pair<int, int>> validMoves = getValidMoves(t_grid, piecePos.first, piecePos.second);

    int amountChecked = 0;
    if (validMoves.empty())
    {
        // if the piece can't move, pick another
        if (amountChecked < 4)
        {
            std::pair<int, int> piecePos = chooseRandomPiece(t_grid, currentPlayer);
            std::vector<std::pair<int, int>> validMoves = getValidMoves(t_grid, piecePos.first, piecePos.second);
            amountChecked++;//should prevent infinite loop
        }
    }

    // pick a random valid move
    int randomIndex = rand() % validMoves.size();
    std::pair<int, int> targetPos = validMoves[randomIndex];

    // mimics clicking to move
    t_grid.handleClick(piecePos.first, piecePos.second); // select piece
    t_grid.handleClick(targetPos.first, targetPos.second); // move it
}

std::pair<int, int> AI::chooseRandomPiece(Grid& t_grid, Player t_player)
{
    // find all our pieces
    std::vector<std::pair<int, int>> ourPieces;

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            // check if this cell has our piece
            std::vector<std::pair<int, int>> moves = getValidMoves(t_grid, row, col);
            if (!moves.empty())
            {
                ourPieces.push_back(std::make_pair(row, col));
            }
        }
    }

    if (!ourPieces.empty())
    {
        int randomIndex = rand() % ourPieces.size();
        return ourPieces[randomIndex];
    }

    return std::make_pair(-1, -1); // no pieces found
}

std::pair<int, int> AI::chooseRandomMove(Grid& t_grid, int t_fromRow, int t_fromCol)
{
    std::vector<std::pair<int, int>> validMoves = getValidMoves(t_grid, t_fromRow, t_fromCol);

    if (!validMoves.empty())
    {
        int randomIndex = rand() % validMoves.size();
        return validMoves[randomIndex];
    }

    return std::make_pair(-1, -1);
}

bool AI::isCellEmpty(Grid& t_grid, int t_row, int t_col) const
{
    return t_grid.isCellEmpty(t_row, t_col);
}

std::vector<std::pair<int, int>> AI::getValidMoves(Grid& t_grid, int t_fromRow, int t_fromCol)
{
    std::vector<std::pair<int, int>> validMoves;

    // check if this cell has a piece belonging to current player
    Player currentPlayer = t_grid.getCurrentPlayer();
    Player cellOwner = t_grid.getCellOwner(t_fromRow, t_fromCol);

    if (cellOwner != currentPlayer)
    {
        return validMoves; // not our piece
    }

    // try every cell on the board to check for possible moves
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (t_grid.canPieceMoveTo(t_fromRow, t_fromCol, row, col))
            {
                validMoves.push_back(std::make_pair(row, col));
            }
        }
    }

    return validMoves;
}