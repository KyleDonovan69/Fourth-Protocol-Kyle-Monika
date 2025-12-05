/**
 * @file AI.h
 * @brief Artificial Intelligence using minimax algorithm
 * @authors: Kyle & Monika
 */

#ifndef AI_HPP
#define AI_HPP

#include "Grid.h"
#include <vector>
#include <utility>
#include <functional>
#include "Constants.h"

/**
 * @class AI
 * @brief AI player that uses minimax algorithm with alpha-beta pruning
 * 
 * This class implements a basic AI that can play at different difficulty levels.
 * It uses the minimax algorithm with alpha-beta pruning for move evaluation during
 * the movement phase, and heuristic-based placement during the placement phase.
 * 
 * Difficulty levels affect the search depth:
 * - Easy: 1 move (shallow search)
 * - Medium: 3 moves (balanced)
 * - Hard: 5 moves (deep search)
 */
class AI
{
public:
    /**
     * @brief Default constructor
     * Initializes AI with medium difficulty
     */
    AI();
    
    /**
     * @brief Destructor
     */
    ~AI();

    /**
     * @brief Makes the AI perform a move on the grid
     * @param t_grid Reference to the grid
     * 
     * Calls either placePiece() or movePiece() depending on game state
     */
    void makeMove(Grid& t_grid);
    
    /**
     * @brief Sets the AI difficulty level
     * @param t_difficulty The difficulty level to set
     * 
     * Changes the search depth used by the minimax algorithm
     */
    void setDifficulty(Difficulty t_difficulty);
    
    /**
     * @brief Gets the current AI difficulty level
     * @return The current Difficulty setting
     */
    Difficulty getDifficulty() const;
    
    /**
     * @brief Gets the last evaluated moves for visualisation
     * @return Vector of AIvisualisation structs containing move scores
     * 
     * Used to display what moves the AI considered and their scores
     */
    std::vector<AIVisualisation> getLastCheckedMoves() const { return m_lastCheckedMoves; }

private:
    /**
     * @struct Move
     * @brief Represents a potential move with its score
     */
    struct Move
    {
        int fromRow;    ///< Source row
        int fromCol;    ///< Source column
        int toRow;      ///< Destination row
        int toCol;      ///< Destination column
        int score;      ///< Evaluated score of this move
    };
    
    std::vector<AIVisualisation> m_lastCheckedMoves;    ///< Last evaluated moves for visualisation
    Difficulty m_difficulty;                            ///< Current difficulty level
    int m_maxDepth;                                     ///< Maximum search depth for minimax
    
    // Placement phase methods
    
    /**
     * @brief Handles piece placement during placement phase
     * @param t_grid Reference to the grid
     */
    void placePiece(Grid& t_grid);
    
    /**
     * @brief Chooses which piece type to place
     * @param t_grid Reference to the grid
     * @param t_player The player to choose for
     * @return The selected PieceType
     */
    PieceType choosePieceType(Grid& t_grid, Player t_player);
    
    /**
     * @brief Chooses where to place a piece
     * @param t_grid Reference to the grid
     * @return Pair of (row, col) coordinates
     */
    std::pair<int, int> choosePlacementPos(Grid& t_grid);

    // Movement phase methods
    
    /**
     * @brief Handles piece movement during movement phase
     * @param t_grid Reference to the grid
     */
    void movePiece(Grid& t_grid);
    
    /**
     * @brief Randomly selects one of the AI's pieces
     * @param t_grid Reference to the grid
     * @param t_player The player to select for
     * @return Pair of (row, col) coordinates
     */
    std::pair<int, int> chooseRandomPiece(Grid& t_grid, Player t_player);
    
    /**
     * @brief Randomly selects a valid move
     * @param t_grid Reference to the grid
     * @param t_fromRow Source row
     * @param t_fromCol Source column
     * @return Pair of (row, col) destination coordinates
     */
    std::pair<int, int> chooseRandomMove(Grid& t_grid, int t_fromRow, int t_fromCol);

    // Minimax algorithm methods
    
    /**
     * @brief Gets all possible moves for a player
     * @param t_grid Reference to the grid
     * @param t_player The player to get moves for
     * @return Vector of all possible Move structs
     */
    std::vector<Move> getAllPossibleMoves(Grid& t_grid, Player t_player);
    
    /**
     * @brief Orders moves by likelihood of being good
     * @param t_moves Vector of moves to sort
     * @param t_grid Reference to the grid
     * @param t_player The player making the moves
     */
    void orderMoves(std::vector<Move>& t_moves, Grid& t_grid, Player t_player);
    
    /**
     * @brief Gets all valid moves for a specific piece
     * @param t_grid Reference to the grid
     * @param t_fromRow Source row
     * @param t_fromCol Source column
     * @return Vector of (row, col) pairs of valid destinations
     */
    std::vector<std::pair<int, int>> getValidMoves(Grid& t_grid, int t_fromRow, int t_fromCol);
    
    /**
     * @brief Checks if a cell is empty
     * @param t_grid Reference to the grid
     * @param t_row Row to check
     * @param t_col Column to check
     * @return True if cell is empty
     */
    bool isCellEmpty(Grid& t_grid, int t_row, int t_col) const;
    
    /**
     * @brief Finds the best move using minimax algorithm
     * @param t_grid Reference to the grid
     * @param t_player The player to find best move for
     * @return The best Move found
     */
    Move findBestMove(Grid& t_grid, Player t_player);
    
    /**
     * @brief Minimax algorithm with alpha-beta pruning
     * @param t_grid Reference to the grid
     * @param t_depth Current search depth
     * @param t_isMaximizing Whether this is a maximizing node
     * @param t_aiPlayer The AI player
     * @param t_alpha Alpha value for pruning
     * @param t_beta Beta value for pruning
     * @return Evaluated score of the position
     */
    int minimax(Grid& t_grid, int t_depth, bool t_isMaximizing, Player t_aiPlayer, int t_alpha, int t_beta);
    
    /**
     * @brief Evaluates the current board state
     * @param t_grid Reference to the grid
     * @param t_aiPlayer The AI player
     * @return Heuristic score of the board
     */
    int evaluateBoard(Grid& t_grid, Player t_aiPlayer);

    /**
     * @brief Temporarily applies a move to test it
     * @param t_grid Reference to the grid
     * @param t_move The move to test
     */
    void testMove(Grid& t_grid, const Move& t_move);
    
    /**
     * @brief Undoes a tested move
     * @param t_grid Reference to the grid
     * @param t_move The move to undo
     * @param t_capturedType Type of piece that was at destination
     * @param t_capturedOwner Owner of piece that was at destination
     */
    void undoMove(Grid& t_grid, const Move& t_move, PieceType t_capturedType, Player t_capturedOwner);

    /**
     * @brief Checks if a move would cause a win
     * @param t_grid Reference to the grid
     * @param row Row to check
     * @param col Column to check
     * @param player Player to check for
     * @return True if placing a piece here would win
     */
    bool doesMoveCauseWin(Grid& t_grid, int row, int col, Player player);

    /**
     * @brief Counts number of 4-in-a-row lines for a player
     * @param t_grid Reference to the grid
     * @param t_player Player to count for
     * @return Number of 4-in-a-row lines
     */
    int count4InARow(Grid& t_grid, Player t_player);
    
    /**
     * @brief Counts number of 3-in-a-row threats for a player
     * @param t_grid Reference to the grid
     * @param t_player Player to count for
     * @return Number of 3-in-a-row lines with one empty space
     */
    int count3InARow(Grid& t_grid, Player t_player);
    
    /**
     * @brief Counts potential winning opportunities (2+ in a row)
     * @param t_grid Reference to the grid
     * @param t_player Player to count for
     * @return Number of potential winning lines
     */
    int countPotentialWins(Grid& t_grid, Player t_player);
};

#endif