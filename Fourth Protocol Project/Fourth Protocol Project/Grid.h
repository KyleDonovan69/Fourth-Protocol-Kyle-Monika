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
    void setSelectedPiece(PieceType t_type);
    void setCurrentPlayer(Player t_player);
    void loadFont(const sf::Font& t_font);

    Player getCurrentPlayer() const;
    int getRemainingPieces(Player t_player, PieceType t_type) const;
    int getTotalPiecesRemaining(Player t_player) const;
    PieceType getSelectedPiece() const;

private:
    sf::RectangleShape m_cells[GRID_SIZE][GRID_SIZE];
    Piece m_board[GRID_SIZE][GRID_SIZE];

    sf::Font* m_font;//helps with sfml 3.0 text setup
    sf::Text* m_pieceLabels[GRID_SIZE][GRID_SIZE];

    PieceType m_selectedPiece;
    Player m_currentPlayer;

    // Track pieces by type
    int m_playerOnePieces[4];
    int m_playerTwoPieces[4];

    void setupGrid();
    void setupPiece(int t_row, int t_col, PieceType t_type, Player t_player);
    void switchPlayer();
    bool canPlacePiece(PieceType t_type) const;
    bool isValidPosition(int t_row, int t_col) const;
    std::string getPieceLabel(PieceType t_type) const;
    int getMaxPiecesForType(PieceType t_type) const;
    int& getPieceCount(Player t_player, PieceType t_type);
    const int& getPieceCount(Player t_player, PieceType t_type) const;
};

#endif