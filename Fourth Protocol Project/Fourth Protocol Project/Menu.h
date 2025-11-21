#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "Constants.h"

enum class GameMode
{
    NONE,
    TWO_PLAYER,
    PLAYER_VS_AI
};

class Menu
{
public:
    Menu(const sf::Font& t_font);
    ~Menu();

    void handleClick(sf::Vector2f t_mousePos);
    void draw(sf::RenderWindow& t_window);

    GameMode getSelectedMode() const;
    bool isModeSelected() const;

    void loadFont(const sf::Font& t_font);

private:
    sf::Font m_font;

    sf::Text m_titleText{m_font};

    sf::RectangleShape m_twoPlayerButton;
    sf::Text m_twoPlayerText{ m_font };

    sf::RectangleShape m_vsAIButton;
    sf::Text m_vsAIText{ m_font };

    GameMode m_selectedMode;

    void setupUI();
};

#endif