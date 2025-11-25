#include "Grid.h"

Grid::Grid() :
    m_selectedPiece(PieceType::FROG),
    m_currentPlayer(Player::PLAYER_ONE),
    m_font(nullptr),
    m_gameState(GameState::PLACEMENT),
    m_selectedRow(-1),
    m_selectedCol(-1),
    m_pieceSelected(false)
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
            m_cells[row][col].setPosition(sf::Vector2f(START_X + col * CELL_SIZE, START_Y + row * CELL_SIZE));

            // Checkerboard look to it now
            if ((row + col) % 2 == 0)
            {
                m_cells[row][col].setFillColor(sf::Color(245, 245, 245));
            }
            else
            {
                m_cells[row][col].setFillColor(sf::Color(220, 220, 220));
            }

            m_cells[row][col].setOutlineThickness(1.0f);
            m_cells[row][col].setOutlineColor(sf::Color(180, 180, 180));

            // highlights any squares for moves
            m_highlightCells[row][col].setSize(sf::Vector2f(CELL_SIZE - 2.0f, CELL_SIZE - 2.0f));
            m_highlightCells[row][col].setPosition(sf::Vector2f(START_X + col * CELL_SIZE, START_Y + row * CELL_SIZE));
            m_highlightCells[row][col].setFillColor(sf::Color(100, 255, 100, 0));
        }
    }
}

void Grid::loadFont(const sf::Font& t_font)
{
    m_font = const_cast<sf::Font*>(&t_font);

    // Create texts for pieces
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
    m_board[t_row][t_col].shape.setRadius(38.0f);
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

    if (checkForWin())
    {
        return;
    }

    switchPlayer();
    swapToMovement();
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
    clearHighlights();
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

GameState Grid::getGameState() const
{
    return m_gameState;
}

PieceType Grid::getSelectedPiece() const
{
    return m_selectedPiece;
}

Player Grid::getWinner() const
{
    return m_winner;
}

void Grid::resetGame()
{
    m_gameState = GameState::PLACEMENT;
    m_winner = Player::NONE;
    m_currentPlayer = Player::PLAYER_ONE;
    m_selectedPiece = PieceType::FROG;
    m_pieceSelected = false;
    m_selectedRow = -1;
    m_selectedCol = -1;

    // Reset piece counters
    for (int i = 0; i < 4; ++i)
    {
        m_playerOnePieces[i] = 0;
        m_playerTwoPieces[i] = 0;
    }

    // Clear board
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            m_board[row][col].type = PieceType::NONE;
            m_board[row][col].owner = Player::NONE;
        }
    }
}

bool Grid::isCellEmpty(int t_row, int t_col) const
{
    if (!isValidPosition(t_row, t_col))
    {
        return false;
    }
    return m_board[t_row][t_col].type == PieceType::NONE;
}

Player Grid::getCellOwner(int t_row, int t_col) const
{
    if (!isValidPosition(t_row, t_col))
    {
        return Player::NONE;
    }
    return m_board[t_row][t_col].owner;
}

PieceType Grid::getPieceType(int t_row, int t_col) const
{
    if (!isValidPosition(t_row, t_col))
    {
        return PieceType::NONE;
    }
    return m_board[t_row][t_col].type;
}

bool Grid::canPieceMoveTo(int t_fromRow, int t_fromCol, int t_toRow, int t_toCol) const
{
    return isValidMove(t_fromRow, t_fromCol, t_toRow, t_toCol);
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
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            t_window.draw(m_highlightCells[row][col]);
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
    const float GRID_TOTAL_SIZE = GRID_SIZE * CELL_SIZE;//finds cell clicked
    const float START_X = (WINDOW_WIDTH - GRID_TOTAL_SIZE) / 2.0f;
    const float START_Y = (WINDOW_HEIGHT - GRID_TOTAL_SIZE) / 2.0f;

    t_col = static_cast<int>((t_mousePos.x - START_X) / CELL_SIZE);
    t_row = static_cast<int>((t_mousePos.y - START_Y) / CELL_SIZE);

    return (t_row >= 0 && t_row < GRID_SIZE && t_col >= 0 && t_col < GRID_SIZE);
}

void Grid::swapToMovement()
{
    // Check if all pieces have been placed
    if (getTotalPiecesRemaining(Player::PLAYER_ONE) == 0 &&
        getTotalPiecesRemaining(Player::PLAYER_TWO) == 0)
    {
        m_gameState = GameState::MOVEMENT;
    }
}

void Grid::handleClick(int t_row, int t_col)
{
    if (m_gameState == GameState::GAME_OVER)
    {
        return; // No more moves allowed
    }

    if (m_gameState == GameState::PLACEMENT)
    {
        placePiece(t_row, t_col);
    }
    else if (m_gameState == GameState::MOVEMENT)
    {
        if (m_pieceSelected)
        {
            // Try to move selected piece
            if (isValidMove(m_selectedRow, m_selectedCol, t_row, t_col))
            {
                movePiece(m_selectedRow, m_selectedCol, t_row, t_col);
                deselectPiece();
                if (m_gameState != GameState::GAME_OVER)
                {
                    switchPlayer();
                }
            }
            else
            {
                // cant do that move, try another
                deselectPiece();
                selectPiece(t_row, t_col);
            }
        }
        else
        {
            selectPiece(t_row, t_col);
        }
    }
}

void Grid::selectPiece(int t_row, int t_col)
{
    if (!isValidPosition(t_row, t_col))
    {
        return;
    }

    // Check if there's a piece at this position
    if (m_board[t_row][t_col].type != PieceType::NONE &&
        m_board[t_row][t_col].owner == m_currentPlayer)
    {
        m_selectedRow = t_row;
        m_selectedCol = t_col;
        m_pieceSelected = true;

        // Highlight selected piece
        m_board[t_row][t_col].shape.setOutlineColor(sf::Color::Yellow);
        m_board[t_row][t_col].shape.setOutlineThickness(5.0f);

        highlightAvailableMoves(t_row, t_col);
    }
}

void Grid::deselectPiece()
{
    if (m_pieceSelected)
    {
        // Reset outline
        m_board[m_selectedRow][m_selectedCol].shape.setOutlineColor(sf::Color::Black);
        m_board[m_selectedRow][m_selectedCol].shape.setOutlineThickness(3.0f);

        m_pieceSelected = false;
        m_selectedRow = -1;
        m_selectedCol = -1;
    }
}

void Grid::highlightAvailableMoves(int t_row, int t_col)
{
    clearHighlights();

    // Check all cells on the board for valid moves
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (isValidMove(t_row, t_col, row, col))
            {
                // Highlight these cells
                m_highlightCells[row][col].setFillColor(sf::Color(100, 255, 100, 120));
            }
        }
    }
}

void Grid::clearHighlights()
{
    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            m_highlightCells[row][col].setFillColor(sf::Color(0, 0, 0, 0)); // Fully transparent
        }
    }
}

void Grid::clearCell(int t_row, int t_col)
{
    if (!isValidPosition(t_row, t_col))
    {
        return;
    }

    m_board[t_row][t_col].type = PieceType::NONE;
    m_board[t_row][t_col].owner = Player::NONE;
}

void Grid::setPiece(int t_row, int t_col, PieceType t_type, Player t_owner)
{
    if (!isValidPosition(t_row, t_col))
    {
        return;
    }

    setupPiece(t_row, t_col, t_type, t_owner);
}

bool Grid::checkForWin()//4 in a row check
{
    Player playerToCheck = m_currentPlayer;

    for (int row = 0; row < GRID_SIZE; ++row)//horizontal lines
    {
        for (int col = 0; col <= GRID_SIZE - 4; ++col)
        {
            if (checkLine(row, col, 0, 1, playerToCheck))
            {
                m_winner = playerToCheck;
                m_gameState = GameState::GAME_OVER;
                return true;
            }
        }
    }

    
    for (int row = 0; row <= GRID_SIZE - 4; ++row)// vertical lines
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (checkLine(row, col, 1, 0, playerToCheck))
            {
                m_winner = playerToCheck;
                m_gameState = GameState::GAME_OVER;
                return true;
            }
        }
    }

    // Check diagonal lines (top-left to bottom-right)
    for (int row = 0; row <= GRID_SIZE - 4; ++row)
    {
        for (int col = 0; col <= GRID_SIZE - 4; ++col)
        {
            if (checkLine(row, col, 1, 1, playerToCheck))
            {
                m_winner = playerToCheck;
                m_gameState = GameState::GAME_OVER;
                return true;
            }
        }
    }

    // Check diagonal lines (top-right to bottom-left)
    for (int row = 0; row <= GRID_SIZE - 4; ++row)
    {
        for (int col = 3; col < GRID_SIZE; ++col)
        {
            if (checkLine(row, col, 1, -1, playerToCheck))
            {
                m_winner = playerToCheck;
                m_gameState = GameState::GAME_OVER;
                return true;
            }
        }
    }

    return false;
}

bool Grid::checkLine(int t_startRow, int t_startCol, int t_rowDir, int t_colDir, Player t_player) const
{
    for (int i = 0; i < 4; ++i)//checks the 4 in a row to see if its all one players
    {
        int row = t_startRow + i * t_rowDir;
        int col = t_startCol + i * t_colDir;

        if (m_board[row][col].owner != t_player)
        {
            return false;
        }
    }
    return true;
}

void Grid::movePiece(int t_fromRow, int t_fromCol, int t_toRow, int t_toCol)
{
    PieceType type = m_board[t_fromRow][t_fromCol].type;//copies piece data
    Player owner = m_board[t_fromRow][t_fromCol].owner;

    m_board[t_fromRow][t_fromCol].type = PieceType::NONE;
    m_board[t_fromRow][t_fromCol].owner = Player::NONE;

    // show it in new pos
    setupPiece(t_toRow, t_toCol, type, owner);

    checkForWin();
}

bool Grid::isValidMove(int t_fromRow, int t_fromCol, int t_toRow, int t_toCol) const
{
    if (!isValidPosition(t_fromRow, t_fromCol) || !isValidPosition(t_toRow, t_toCol))
    {
        return false;
    }

    // Can't move to occupied square
    if (m_board[t_toRow][t_toCol].type != PieceType::NONE)
    {
        return false;
    }

    // Check piece-specific movements
    PieceType pieceType = m_board[t_fromRow][t_fromCol].type;

    return canPieceMove(pieceType, t_fromRow, t_fromCol, t_toRow, t_toCol);
}


bool Grid::canPieceMove(PieceType t_type, int t_fromRow, int t_fromCol, int t_toRow, int t_toCol) const
{
    int rowDiff = abs(t_toRow - t_fromRow);
    int colDiff = abs(t_toCol - t_fromCol);

    switch (t_type)
    {
    case PieceType::DONKEY:
        // Donkey moves 1 space in cardinal directions
        return (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1);

    case PieceType::SNAKE:
        // Snake moves 1 space in any direction
        return (rowDiff <= 1 && colDiff <= 1) && (rowDiff + colDiff > 0);

    case PieceType::FROG:
    {
        // Frog can move 1 space in any direction
        if ((rowDiff <= 1 && colDiff <= 1) && (rowDiff + colDiff > 0))
        {
            return true;
        }

        // Jump must be straight or diagonal
        if (!(rowDiff == 0 || colDiff == 0 || rowDiff == colDiff))
        {
            return false;
        }

        int rowStep = (t_toRow > t_fromRow) ? 1 : ((t_toRow < t_fromRow) ? -1 : 0);
        int colStep = (t_toCol > t_fromCol) ? 1 : ((t_toCol < t_fromCol) ? -1 : 0);

        int currentRow = t_fromRow + rowStep;
        int currentCol = t_fromCol + colStep;


        bool foundFirstPiece = false;

        // Move along the line until reaching target
        while (currentRow != t_toRow || currentCol != t_toCol)
        {
            if (currentRow == t_toRow && currentCol == t_toCol)
            {
                break;
            }

            if (m_board[currentRow][currentCol].type != PieceType::NONE)
            {
                //hit a piece
                foundFirstPiece = true;
            }
            else
            {
                // Empty space before a piece breaks the chain / no jump
                if (!foundFirstPiece)
                    return false;

                // Empty space AFTER pieces but before target / frog must stop there
                if (currentRow != t_toRow || currentCol != t_toCol)
                    return false;
            }

            currentRow += rowStep;
            currentCol += colStep;
        }

        // Landing square has to be empty
        if (m_board[t_toRow][t_toCol].type != PieceType::NONE)
        {
            return false;
        }

        // Must have jumped over at least one piece
        return foundFirstPiece;
    }
    }
}
