/**
 * @file Menu.h
 * @brief Main menu system for game mode and difficulty selection
 * @authors:  Kyle & Monika
 */

#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "Constants.h"

/**
 * @enum GameMode
 * @brief Defines the different game modes available
 */
enum class GameMode
{
    NONE,           ///< No mode selected
    TWO_PLAYER,     ///< Two human players
    PLAYER_VS_AI,   ///< Human vs AI
    AI_VS_AI        ///< AI vs AI (watch mode)
};

/**
 * @class Menu
 * @brief Handles the main menu interface (game mode and difficulty)
 * 
 * This class manages the menu system with two screens:
 * 1. Game mode selection (Two Player, Player vs AI, AI vs AI)
 * 2. Difficulty selection (Easy, Medium, Hard) - only for AI modes
 */
class Menu
{
public:
    /**
     * @brief Builds the menu with the specified font
     * @param t_font Has to refer to the font to use for text rendering
     */
    Menu(const sf::Font& t_font);
    
    /**
     * @brief Destructor
     */
    ~Menu();

    /**
     * @brief Handles mouse click events on menu buttons
     * @param t_mousePos Position of the mouse click
     */
    void handleClick(sf::Vector2f t_mousePos);
    
    /**
     * @brief Handles mouse movement for button hover effects
     * @param t_mousePos Current position of the mouse
     */
    void handleMouseMove(sf::Vector2f t_mousePos);
    
    /**
     * @brief Renders the menu to the window
     * @param t_window Reference to the render window
     */
    void draw(sf::RenderWindow& t_window);

    /**
     * @brief Gets the currently selected game mode
     * @return The selected GameMode
     */
    GameMode getSelectedMode() const;
    
    /**
     * @brief Checks if a game mode has been fully selected (including difficulty if needed)
     * @return True if mode selection is complete
     */
    bool isModeSelected() const;
    
    /**
     * @brief Gets the selected AI difficulty level
     * @return The selected Difficulty
     */
    Difficulty getSelectedDifficulty() const;
    
    /**
     * @brief Checks if difficulty has been selected
     * @return True if difficulty selection is complete
     */
    bool isDifficultySelected() const;

    /**
     * @brief Loads the font for menu text
     * @param t_font Reference to the font to load
     */
    void loadFont(const sf::Font& t_font);

private:
    sf::Font m_font;  ///< Font used for all menu text

    // Title elements
    sf::Text m_titleText{m_font};       ///< Main title text
    sf::Text m_subtitleText{m_font};    ///< Subtitle text

    // Game mode selection buttons
    sf::RectangleShape m_twoPlayerButton;   ///< Two player mode button
    sf::Text m_twoPlayerText{ m_font };     ///< Two player button text

    sf::RectangleShape m_vsAIButton;        ///< Player vs AI mode button
    sf::Text m_vsAIText{ m_font };          ///< Player vs AI button text

    sf::RectangleShape m_aiVsAIButton;      ///< AI vs AI mode button
    sf::Text m_aiVsAIText{ m_font };        ///< AI vs AI button text
    
    // Difficulty selection UI
    sf::Text m_difficultyTitleText{ m_font };   ///< Difficulty selection title
    sf::RectangleShape m_easyButton;            ///< Easy difficulty button
    sf::Text m_easyText{ m_font };              ///< Easy button text
    sf::RectangleShape m_mediumButton;          ///< Medium difficulty button
    sf::Text m_mediumText{ m_font };            ///< Medium button text
    sf::RectangleShape m_hardButton;            ///< Hard difficulty button
    sf::Text m_hardText{ m_font };              ///< Hard button text

    // Background elements
    sf::RectangleShape m_background;        ///< Background rectangle
    sf::CircleShape m_menuCircles[5];       ///< Decorative circles

    GameMode m_selectedMode;                ///< Currently selected game mode
    Difficulty m_selectedDifficulty;        ///< Currently selected difficulty
    bool m_showDifficultyScreen;            ///< Whether to show difficulty screen
    int m_hoveredButton;                    ///< Index of currently hovered button (-1 if none)

    /**
     * @brief Sets up the main menu UI elements
     */
    void setupUI();
    
    /**
     * @brief Sets up the difficulty selection UI elements
     */
    void setupDifficultyUI();
    
    /**
     * @brief Updates button visual states based on hover
     */
    void updateButtonStates();
    
    /**
     * @brief Updates difficulty button visual states based on hover
     */
    void updateDifficultyButtonStates();
};

#endif