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
            if (t_grid.isCellEmpty(row, col)) 
            {
                emptyCells.push_back({ row, col });
            }


        }
    }
    Player opponent = (t_grid.getCurrentPlayer() == Player::PLAYER_ONE)
        ? Player::PLAYER_TWO
        : Player::PLAYER_ONE;

    // BLOCK opponent winning placement
        for (auto cell : emptyCells)
        {
            if (doesMoveCauseWin(t_grid, cell.first, cell.second, opponent))
            {
                return cell; // place here to block
            }
        }

    // Try to form our own winning line
    for (auto cell : emptyCells)
    {
        if (doesMoveCauseWin(t_grid, cell.first, cell.second, t_grid.getCurrentPlayer()))
        {
            return cell; // place aggressively
        }
    }

    //  Otherwise choose center-ish
    if (t_grid.isCellEmpty(2, 2))
        return { 2,2 };

    //fallback to random choice
    int randomIndex = rand() % emptyCells.size();
    return emptyCells[randomIndex];
}



void AI::movePiece(Grid& t_grid)
{
    Player currentPlayer = t_grid.getCurrentPlayer();

	Move bestMove = findBestMove(t_grid, currentPlayer);//use minimax to find the best move

    if (bestMove.fromRow != -1)//if a valid move, do it
    {
        t_grid.handleClick(bestMove.fromRow, bestMove.fromCol);
        t_grid.handleClick(bestMove.toRow, bestMove.toCol);
    }
}

std::pair<int, int> AI::chooseRandomPiece(Grid& t_grid, Player t_player)//not used at the minute
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

std::pair<int, int> AI::chooseRandomMove(Grid& t_grid, int t_fromRow, int t_fromCol)//also not used anymore
{
    std::vector<std::pair<int, int>> validMoves = getValidMoves(t_grid, t_fromRow, t_fromCol);

    if (!validMoves.empty())
    {
        int randomIndex = rand() % validMoves.size();
        return validMoves[randomIndex];
    }

    return std::make_pair(-1, -1);
}

bool AI::isCellEmpty(Grid& t_grid, int t_row, int t_col) const//checks if the cell is empty
{
    return t_grid.isCellEmpty(t_row, t_col);
}

std::vector<std::pair<int, int>> AI::getValidMoves(Grid& t_grid, int t_fromRow, int t_fromCol)
{
    std::vector<std::pair<int, int>> validMoves;

    // check if the cell belongs to current player 
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

AI::Move AI::findBestMove(Grid& t_grid, Player t_player)
{
    std::vector<Move> allMoves = getAllPossibleMoves(t_grid, t_player);//gets all possible moves

    if (allMoves.empty())
    {
		return { -1, -1, -1, -1, 0 };//none available
    }

    Move bestMove = allMoves[0];//sets the best move as the first
    bestMove.score = std::numeric_limits<int>::min();

    for (Move& move : allMoves)//try all the moves
    {
		// keep what was in the target cell
        PieceType capturedType = t_grid.getPieceType(move.toRow, move.toCol);
        Player capturedOwner = t_grid.getCellOwner(move.toRow, move.toCol);

        // chance the move
        testMove(t_grid, move);

        // Check if this move wins the game
        if (t_grid.getGameState() == GameState::GAME_OVER && t_grid.getWinner() == t_player)
        {
            undoMove(t_grid, move, capturedType, capturedOwner);
            return move;
        }

        // use minimax to check how good the move is
        int score = minimax(t_grid, MAX_DEPTH - 1, false, t_player, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        move.score = score;

        // undo the move to check the next
        undoMove(t_grid, move, capturedType, capturedOwner);

        if (score > bestMove.score)
        {
            bestMove = move;
        }
    }

    return bestMove;
}

int AI::minimax(Grid& t_grid, int t_depth, bool t_isMaximizing, Player t_aiPlayer, int t_alpha, int t_beta)
{
    if (t_grid.getGameState() == GameState::GAME_OVER)
    {
        Player winner = t_grid.getWinner();
        if (winner == t_aiPlayer)
            return WIN_SCORE + t_depth;  // high depth = faster win
        else if (winner != Player::NONE)
            return LOSE_SCORE - t_depth;  // lower loss score by taking longer to lose
        else
            return 0;  // Tie
    }

	if (t_depth == 0)//if its gone to the depth, evaluate the board
    {
        return evaluateBoard(t_grid, t_aiPlayer);
    }

    Player currentPlayer = t_grid.getCurrentPlayer();//get all moves for whichever player
    std::vector<Move> moves = getAllPossibleMoves(t_grid, currentPlayer);

    if (moves.empty())
    {
        return evaluateBoard(t_grid, t_aiPlayer);
    }

    if (t_isMaximizing)//go for the highest score for ai
    {
        int maxEval = std::numeric_limits<int>::min();

        for (const Move& move : moves)
        {
            PieceType capturedType = t_grid.getPieceType(move.toRow, move.toCol);//saves target cell info
            Player capturedOwner = t_grid.getCellOwner(move.toRow, move.toCol);

            testMove(t_grid, move);//chance the move
            int eval = minimax(t_grid, t_depth - 1, false, t_aiPlayer, t_alpha, t_beta);
            undoMove(t_grid, move, capturedType, capturedOwner);

            maxEval = std::max(maxEval, eval);
            t_alpha = std::max(t_alpha, eval);
            if (t_beta <= t_alpha)//skips checking rest of the moves after cutoff
                break;
        }
        return maxEval;
    }
    else//minimizing, checks for lowest score for opponent
    {
        int minEval = std::numeric_limits<int>::max();
        for (const Move& move : moves)
        {
            PieceType capturedType = t_grid.getPieceType(move.toRow, move.toCol);
            Player capturedOwner = t_grid.getCellOwner(move.toRow, move.toCol);

            testMove(t_grid, move);
            int eval = minimax(t_grid, t_depth - 1, true, t_aiPlayer, t_alpha, t_beta);
            undoMove(t_grid, move, capturedType, capturedOwner);

            minEval = std::min(minEval, eval);
            t_beta = std::min(t_beta, eval);
            if (t_beta <= t_alpha)
                break;
        }
        return minEval;
    }
}

bool AI::doesMoveCauseWin(Grid& t_grid, int row, int col, Player player)
{
    // Temporarily place a piece
    PieceType temp = PieceType::DONKEY; // type doesn't matter for win check
    Player oldOwner = t_grid.getCellOwner(row, col);
    PieceType oldType = t_grid.getPieceType(row, col);

    t_grid.setPiece(row, col, temp, player);

    bool win = false;

    // Check if this creates 4 in a row
    if (count4InARow(t_grid, player) > 0)
        win = true;

    // Undo change
    if (oldType == PieceType::NONE)
        t_grid.clearCell(row, col);
    else
        t_grid.setPiece(row, col, oldType, oldOwner);

    return win;
}

int AI::evaluateBoard(Grid& t_grid, Player t_aiPlayer)
{
    Player opponent = (t_aiPlayer == Player::PLAYER_ONE) ? Player::PLAYER_TWO : Player::PLAYER_ONE;//figure out who is who

    int score = 0;

    //if ai has a 4 in a row 
    score += count4InARow(t_grid, t_aiPlayer) * 10000;
    //if opponent has a 4 in a row loose score - minmax avoids at all costs
    score -= count4InARow(t_grid, opponent) * 12000;


    // checks which line can be made into 4 in a row
    score += countPotentialWins(t_grid, t_aiPlayer) * 80;
    score -= countPotentialWins(t_grid, opponent) * 60;  // Weight opponent threats higher to not lose

    // Count 3-in-a-rows to set as a threat
    score += count3InARow(t_grid, t_aiPlayer) * 800;
    score -= count3InARow(t_grid, opponent) * 5000;//oppenent threats = bad >:(


    return score;
}

std::vector<AI::Move> AI::getAllPossibleMoves(Grid& t_grid, Player t_player)
{
    std::vector<Move> allMoves;

    for (int fromRow = 0; fromRow < GRID_SIZE; ++fromRow)
    {
        for (int fromCol = 0; fromCol < GRID_SIZE; ++fromCol)
        {
            if (t_grid.getCellOwner(fromRow, fromCol) == t_player)//check for our pieces
            {
                std::vector<std::pair<int, int>> validMoves = getValidMoves(t_grid, fromRow, fromCol);//checks all valid moves

                for (const auto& movePair : validMoves)//and adds the, to a list
                {
                    allMoves.push_back({ fromRow, fromCol, movePair.first, movePair.second, 0 });
                }
            }
        }
    }

    return allMoves;
}

void AI::testMove(Grid& t_grid, const Move& t_move)//this just tests moves, doesnt actually do them
{
    PieceType type = t_grid.getPieceType(t_move.fromRow, t_move.fromCol);
    Player owner = t_grid.getCellOwner(t_move.fromRow, t_move.fromCol);

    t_grid.clearCell(t_move.fromRow, t_move.fromCol);
    t_grid.setPiece(t_move.toRow, t_move.toCol, type, owner);
}

void AI::undoMove(Grid& t_grid, const Move& t_move, PieceType t_capturedType, Player t_capturedOwner)
{
    PieceType type = t_grid.getPieceType(t_move.toRow, t_move.toCol);//finds the moved piece
    Player owner = t_grid.getCellOwner(t_move.toRow, t_move.toCol);

    t_grid.setPiece(t_move.fromRow, t_move.fromCol, type, owner);//sets it back to old position

    if (t_capturedType != PieceType::NONE)//changes back the new cell too
    {
        t_grid.setPiece(t_move.toRow, t_move.toCol, t_capturedType, t_capturedOwner);
    }
    else
    {
        t_grid.clearCell(t_move.toRow, t_move.toCol);
    }
}

int AI::count3InARow(Grid& t_grid, Player t_player)
{
    int threats = 0;

    // Check all directions for 3-in-a-row with an empty space
    int directions[4][4] = {
        {0, 1, GRID_SIZE, GRID_SIZE - 3},     // Horizontal
        {1, 0, GRID_SIZE - 3, GRID_SIZE},     // Vertical
        {1, 1, GRID_SIZE - 3, GRID_SIZE - 3}, // Diagonal left
        {1, -1, GRID_SIZE - 3, 3}             // Diagonal right
    };

    for (auto& dir : directions)//checks the 4 cells in a row for possible wins
    {
        int rowDir = dir[0];
        int colDir = dir[1];
        int maxRow = dir[2];
        int maxCol = dir[3];

        for (int row = 0; row < maxRow; ++row)
        {
            for (int col = (colDir == -1 ? 3 : 0); col < maxCol; ++col)
            {
                int count = 0;
                int empty = 0;

                for (int i = 0; i < 4; ++i)
                {
                    int checkRow = row + i * rowDir;
                    int checkCol = col + i * colDir;

                    if (t_grid.getCellOwner(checkRow, checkCol) == t_player)//makes sure its the enemy pieces
                        count++;
                    else if (t_grid.isCellEmpty(checkRow, checkCol))
                        empty++;
                }

                if (count == 3 && empty == 1)//if 3 in a row and an empty space, mark it
                    threats++;
            }
        }
    }

    return threats;
}

//counts every full 4 in a row line for the given player
int AI::count4InARow(Grid& t_grid, Player t_player)
{
    int wins = 0;

    // horizontal
    for (int row = 0; row < GRID_SIZE; row++)
        for (int col = 0; col <= GRID_SIZE - 4; col++)
            if (t_grid.getCellOwner(row, col) == t_player &&
                t_grid.getCellOwner(row, col + 1) == t_player &&
                t_grid.getCellOwner(row, col + 2) == t_player &&
                t_grid.getCellOwner(row, col + 3) == t_player)
                wins++;    //found a full horizontl line

    // vertical
    for (int row = 0; row <= GRID_SIZE - 4; row++)
        for (int col = 0; col < GRID_SIZE; col++)
            if (t_grid.getCellOwner(row, col) == t_player &&
                t_grid.getCellOwner(row + 1, col) == t_player &&
                t_grid.getCellOwner(row + 2, col) == t_player &&
                t_grid.getCellOwner(row + 3, col) == t_player)
                wins++; //found a vertical line

    // diagonal
    for (int row = 0; row <= GRID_SIZE - 4; row++)
        for (int col = 0; col <= GRID_SIZE - 4; col++)
            if (t_grid.getCellOwner(row, col) == t_player &&
                t_grid.getCellOwner(row + 1, col + 1) == t_player &&
                t_grid.getCellOwner(row + 2, col + 2) == t_player &&
                t_grid.getCellOwner(row + 3, col + 3) == t_player)
                wins++; //found a diagonal top left line

    // anti-diagonal
    for (int row = 0; row <= GRID_SIZE - 4; row++)
        for (int col = 3; col < GRID_SIZE; col++)
            if (t_grid.getCellOwner(row, col) == t_player &&
                t_grid.getCellOwner(row + 1, col - 1) == t_player &&
                t_grid.getCellOwner(row + 2, col - 2) == t_player &&
                t_grid.getCellOwner(row + 3, col - 3) == t_player)
                wins++;  //found a diagonal top right line
   
    return wins;
}

int AI::countPotentialWins(Grid& t_grid, Player t_player)//same as threats but checks 2+ in a row
{
    int potential = 0;
    Player opponent = (t_player == Player::PLAYER_ONE) ? Player::PLAYER_TWO : Player::PLAYER_ONE;

    // Check all directions for 2+ in a row
    int directions[4][4] = {
        {0, 1, GRID_SIZE, GRID_SIZE - 3}, // Horizontal
        {1, 0, GRID_SIZE - 3, GRID_SIZE}, // Vertical
        {1, 1, GRID_SIZE - 3, GRID_SIZE - 3}, // Diagonal left
        {1, -1, GRID_SIZE - 3, 3} // Diagonal right
    };

    for (auto& dir : directions)
    {
        int rowDir = dir[0];
        int colDir = dir[1];
        int maxRow = dir[2];
        int maxCol = dir[3];

        for (int row = 0; row < maxRow; ++row)
        {
            for (int col = (colDir == -1 ? 3 : 0); col < maxCol; ++col)
            {
                int count = 0;
                int empty = 0;
                bool hasOpponent = false;

                for (int i = 0; i < 4; ++i)
                {
                    int checkRow = row + i * rowDir;
                    int checkCol = col + i * colDir;

                    Player cellOwner = t_grid.getCellOwner(checkRow, checkCol);

                    if (cellOwner == t_player)
                        count++;
                    else if (t_grid.isCellEmpty(checkRow, checkCol))
                        empty++;
                    else if (cellOwner == opponent)
                        hasOpponent = true;
                }

                if (count >= 2 && !hasOpponent && empty >= 1)
                    potential++;
            }
        }
    }

    return potential;
}