#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include "Constants.h"

enum class PieceType
{
    NONE = 0,
    FROG = 1,
    SNAKE = 2,
    DONKEY = 3
};

enum class Player
{
    NONE,
    PLAYER_ONE,
    PLAYER_TWO
};

enum class GameState
{
    PLACEMENT,
    MOVEMENT,
    GAME_OVER
};

struct Piece
{
    PieceType type;
    Player owner;
    sf::CircleShape shape;

};

class Grid
{
public:
    Grid();
    ~Grid();

    void draw(sf::RenderWindow& t_window);
    bool getCellFromMouse(sf::Vector2f t_mousePos, int& t_row, int& t_col);
    void placePiece(int t_row, int t_col);
    void handleClick(int t_row, int t_col);
    void setSelectedPiece(PieceType t_type);
    void setCurrentPlayer(Player t_player);
    void loadFont(const sf::Font& t_font);

    Player getCurrentPlayer() const;
    GameState getGameState() const;
    int getRemainingPieces(Player t_player, PieceType t_type) const;
    int getTotalPiecesRemaining(Player t_player) const;
    PieceType getSelectedPiece() const;

    Player getWinner() const;
    void resetGame();

private:
    sf::RectangleShape m_cells[GRID_SIZE][GRID_SIZE];
    Piece m_board[GRID_SIZE][GRID_SIZE];
    sf::RectangleShape m_highlightCells[GRID_SIZE][GRID_SIZE];

    sf::Font* m_font;//helps with sfml 3.0 text setup
    sf::Text* m_pieceLabels[GRID_SIZE][GRID_SIZE];

    PieceType m_selectedPiece;
    Player m_currentPlayer;
    GameState m_gameState;
    Player m_winner;

    int m_selectedRow;
    int m_selectedCol;
    bool m_pieceSelected;

    // Track pieces by type
    int m_playerOnePieces[4];
    int m_playerTwoPieces[4];

    void setupGrid();

    void setupPiece(int t_row, int t_col, PieceType t_type, Player t_player);//All piece functions
    bool canPieceMove(PieceType t_type, int t_fromRow, int t_fromCol, int t_toRow, int t_toCol) const;
    void movePiece(int t_fromRow, int t_fromCol, int t_toRow, int t_toCol);
    void selectPiece(int t_row, int t_col);
    void deselectPiece();
    bool canPlacePiece(PieceType t_type) const;
    std::string getPieceLabel(PieceType t_type) const;
    int getMaxPiecesForType(PieceType t_type) const;
    int& getPieceCount(Player t_player, PieceType t_type);
    const int& getPieceCount(Player t_player, PieceType t_type) const;

    void switchPlayer();
    
    bool isValidPosition(int t_row, int t_col) const;

    void swapToMovement();
    bool isValidMove(int t_fromRow, int t_fromCol, int t_toRow, int t_toCol) const;
    
    void highlightAvailableMoves(int t_row, int t_col);
    void clearHighlights();

    bool checkForWin();
    bool checkLine(int t_startRow, int t_startCol, int t_rowDir, int t_colDir, Player t_player) const;
};

#endif