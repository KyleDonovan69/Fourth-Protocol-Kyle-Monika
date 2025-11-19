#include "Grid.h"

Grid::Grid() :
    m_selectedPiece(PieceType::FROG),
    m_currentPlayer(Player::PLAYER_ONE),
    m_font(nullptr)
{
    for (int i = 0; i < 4; ++i)
    {
        m_playerOnePieces[i] = 0;
        m_playerTwoPieces[i] = 0;
    }

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            m_pieceLabels[row][col] = nullptr;
        }
    }

    setupGrid();

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            m_board[row][col].type = PieceType::NONE;
            m_board[row][col].owner = Player::NONE;
        }
    }
}

Grid::~Grid()//Deconstructor
{
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            delete m_pieceLabels[row][col];
        }
    }
}

void Grid::setupGrid()
{
    const float GRID_TOTAL_SIZE = GRID_SIZE * CELL_SIZE;
    const float START_X = (WINDOW_WIDTH - GRID_TOTAL_SIZE) / 2.0f;
    const float START_Y = (WINDOW_HEIGHT - GRID_TOTAL_SIZE) / 2.0f;

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            m_cells[row][col].setSize(sf::Vector2f(CELL_SIZE - 2.0f, CELL_SIZE - 2.0f));
            m_cells[row][col].setPosition(sf::Vector2f( START_X + col * CELL_SIZE, START_Y + row * CELL_SIZE ));
            m_cells[row][col].setFillColor(sf::Color(240, 240, 240));
            m_cells[row][col].setOutlineThickness(2.0f);
            m_cells[row][col].setOutlineColor(sf::Color(100, 100, 100));
        }
    }
}

void Grid::loadFont(const sf::Font& t_font)
{
    m_font = const_cast<sf::Font*>(&t_font);

    // Create text objects for all grid positions
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            m_pieceLabels[row][col] = new sf::Text(*m_font);
            m_pieceLabels[row][col]->setCharacterSize(50U);
            m_pieceLabels[row][col]->setFillColor(sf::Color::White);
            m_pieceLabels[row][col]->setOutlineColor(sf::Color::Black);
            m_pieceLabels[row][col]->setOutlineThickness(2.0f);
        }
    }
}

int Grid::getMaxPiecesForType(PieceType t_type) const
{
    switch (t_type)
    {
    case PieceType::FROG:
        return MAX_FROGS_PER_PLAYER;
    case PieceType::SNAKE:
        return MAX_SNAKES_PER_PLAYER;
    case PieceType::DONKEY:
        return MAX_DONKEYS_PER_PLAYER;
    default:
        return 0;
    }
}

int& Grid::getPieceCount(Player t_player, PieceType t_type)
{
    int index = static_cast<int>(t_type);
    if (t_player == Player::PLAYER_ONE)
    {
        return m_playerOnePieces[index];
    }
    else
    {
        return m_playerTwoPieces[index];
    }
}

const int& Grid::getPieceCount(Player t_player, PieceType t_type) const
{
    int index = static_cast<int>(t_type);
    if (t_player == Player::PLAYER_ONE)
    {
        return m_playerOnePieces[index];
    }
    else
    {
        return m_playerTwoPieces[index];
    }
}

std::string Grid::getPieceLabel(PieceType t_type) const
{
    switch (t_type)
    {
    case PieceType::FROG:
        return "F";
    case PieceType::SNAKE:
        return "S";
    case PieceType::DONKEY:
        return "D";
    default:
        return "";
    }
}

void Grid::setupPiece(int t_row, int t_col, PieceType t_type, Player t_player)
{
    m_board[t_row][t_col].type = t_type;
    m_board[t_row][t_col].owner = t_player;

    // Setup circle shape
    m_board[t_row][t_col].shape.setRadius(35.0f);
    sf::Vector2f cellPos = m_cells[t_row][t_col].getPosition();
    m_board[t_row][t_col].shape.setPosition(sf::Vector2f(cellPos.x + 15.0f, cellPos.y + 15.0f));

    if (t_player == Player::PLAYER_ONE)
    {
        m_board[t_row][t_col].shape.setFillColor(sf::Color::Red);
    }
    else
    {
        m_board[t_row][t_col].shape.setFillColor(sf::Color::Blue);
    }

    m_board[t_row][t_col].shape.setOutlineThickness(3.0f);
    m_board[t_row][t_col].shape.setOutlineColor(sf::Color::Black);

    // Setup text label
    if (m_font != nullptr && m_pieceLabels[t_row][t_col] != nullptr)
    {
        m_pieceLabels[t_row][t_col]->setString(getPieceLabel(t_type));

        sf::FloatRect textBounds = m_pieceLabels[t_row][t_col]->getLocalBounds();
        m_pieceLabels[t_row][t_col]->setOrigin(sf::Vector2f( textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f ));
        m_pieceLabels[t_row][t_col]->setPosition(sf::Vector2f( cellPos.x + CELL_SIZE / 2.0f, cellPos.y + CELL_SIZE / 2.0f ));
    }
}

bool Grid::canPlacePiece(PieceType t_type) const
{
    if (t_type == PieceType::NONE)
    {
        return false;
    }

    int piecesPlaced = getPieceCount(m_currentPlayer, t_type);
    int maxPieces = getMaxPiecesForType(t_type);

    return piecesPlaced < maxPieces;
}

bool Grid::isValidPosition(int t_row, int t_col) const
{
    return (t_row >= 0 && t_row < GRID_SIZE && t_col >= 0 && t_col < GRID_SIZE);
}

void Grid::placePiece(int t_row, int t_col)
{
    if (!isValidPosition(t_row, t_col))
    {
        return;
    }

    if (m_board[t_row][t_col].type != PieceType::NONE)
    {
        return;
    }

    if (!canPlacePiece(m_selectedPiece))
    {
        return;
    }

    setupPiece(t_row, t_col, m_selectedPiece, m_currentPlayer);
    getPieceCount(m_currentPlayer, m_selectedPiece)++;
    switchPlayer();
}

void Grid::switchPlayer()
{
    if (m_currentPlayer == Player::PLAYER_ONE)
    {
        m_currentPlayer = Player::PLAYER_TWO;
    }
    else
    {
        m_currentPlayer = Player::PLAYER_ONE;
    }
}

void Grid::setSelectedPiece(PieceType t_type)
{
    m_selectedPiece = t_type;
}

void Grid::setCurrentPlayer(Player t_player)
{
    m_currentPlayer = t_player;
}

Player Grid::getCurrentPlayer() const
{
    return m_currentPlayer;
}

PieceType Grid::getSelectedPiece() const
{
    return m_selectedPiece;
}

int Grid::getRemainingPieces(Player t_player, PieceType t_type) const
{
    if (t_type == PieceType::NONE)
    {
        return 0;
    }

    int piecesPlaced = getPieceCount(t_player, t_type);
    int maxPieces = getMaxPiecesForType(t_type);

    return maxPieces - piecesPlaced;
}

int Grid::getTotalPiecesRemaining(Player t_player) const
{
    int total = 0;
    total += getRemainingPieces(t_player, PieceType::FROG);
    total += getRemainingPieces(t_player, PieceType::SNAKE);
    total += getRemainingPieces(t_player, PieceType::DONKEY);
    return total;
}

void Grid::draw(sf::RenderWindow& t_window)
{
    // Draw grid cells
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            t_window.draw(m_cells[row][col]);
        }
    }

    // Draw pieces and labels
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (m_board[row][col].type != PieceType::NONE)
            {
                t_window.draw(m_board[row][col].shape);
                if (m_pieceLabels[row][col] != nullptr)
                {
                    t_window.draw(*m_pieceLabels[row][col]);
                }
            }
        }
    }
}

bool Grid::getCellFromMouse(sf::Vector2f t_mousePos, int& t_row, int& t_col)
{
    const float GRID_TOTAL_SIZE = GRID_SIZE * CELL_SIZE;
    const float START_X = (WINDOW_WIDTH - GRID_TOTAL_SIZE) / 2.0f;
    const float START_Y = (WINDOW_HEIGHT - GRID_TOTAL_SIZE) / 2.0f;

    t_col = static_cast<int>((t_mousePos.x - START_X) / CELL_SIZE);
    t_row = static_cast<int>((t_mousePos.y - START_Y) / CELL_SIZE);

    return (t_row >= 0 && t_row < GRID_SIZE && t_col >= 0 && t_col < GRID_SIZE);
}