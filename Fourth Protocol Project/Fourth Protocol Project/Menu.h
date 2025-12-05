#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "Constants.h"

enum class GameMode
{
    NONE,
    TWO_PLAYER,
    PLAYER_VS_AI,
    AI_VS_AI
};

class Menu
{
public:
    Menu(const sf::Font& t_font);
    ~Menu();

    void handleClick(sf::Vector2f t_mousePos);
    void handleMouseMove(sf::Vector2f t_mousePos);
    void draw(sf::RenderWindow& t_window);

    GameMode getSelectedMode() const;
    bool isModeSelected() const;

    void loadFont(const sf::Font& t_font);

private:
    sf::Font m_font;

    sf::Text m_titleText{m_font};
    sf::Text m_subtitleText{m_font};

    sf::RectangleShape m_twoPlayerButton;
    sf::Text m_twoPlayerText{ m_font };

    sf::RectangleShape m_vsAIButton;
    sf::Text m_vsAIText{ m_font };

    sf::RectangleShape m_aiVsAIButton;
    sf::Text m_aiVsAIText{ m_font };

    sf::RectangleShape m_background;
    sf::CircleShape m_menuCircles[5];

    GameMode m_selectedMode;
    int m_hoveredButton;

    void setupUI();
    void updateButtonStates();
};

#endif