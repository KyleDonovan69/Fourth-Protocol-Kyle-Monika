#include "Menu.h"
#include <cmath>

Menu::Menu(const sf::Font& t_font) :
    m_font(t_font),
    m_selectedMode(GameMode::NONE),
    m_hoveredButton(-1)
{
    setupUI();
}

Menu::~Menu()
{
}

void Menu::setupUI()
{
    if (!m_font.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
    {
    }
    m_titleText.setString("THE FOURTH PROTOCOL");
    m_titleText.setCharacterSize(70U);
    m_titleText.setFillColor(CYAN);
    m_titleText.setOutlineColor(sf::Color::Black);
    m_titleText.setOutlineThickness(5.0f);
    m_titleText.setLetterSpacing(1.2f);
    m_titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f));
    m_titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 120.0f));

    m_subtitleText.setString("MAY THE FOURTH BE WITH YOU");
    m_subtitleText.setCharacterSize(30U);
    m_subtitleText.setFillColor(sf::Color::White);
    m_subtitleText.setStyle(sf::Text::Italic);

    sf::FloatRect subtitleBounds = m_subtitleText.getLocalBounds();
    m_subtitleText.setOrigin(sf::Vector2f(subtitleBounds.size.x / 2.0f, subtitleBounds.size.y / 2.0f));
    m_subtitleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 180.0f));

    float buttonWidth = 350.0f;
    float buttonHeight = 80.0f;
    float startY = 280.0f;
    float spacing = 100.0f;

    m_twoPlayerButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_twoPlayerButton.setPosition(sf::Vector2f((WINDOW_WIDTH - buttonWidth) / 2.0f, startY));
    m_twoPlayerButton.setFillColor(sf::Color(50, 50, 120));
    m_twoPlayerButton.setOutlineThickness(4.0f);
    m_twoPlayerButton.setOutlineColor(CYAN);

    m_twoPlayerText.setString("TWO PLAYER");
    m_twoPlayerText.setCharacterSize(38U);
    m_twoPlayerText.setFillColor(sf::Color::White);
    m_twoPlayerText.setOutlineColor(sf::Color::Black);
    m_twoPlayerText.setOutlineThickness(3.0f);
    m_twoPlayerText.setStyle(sf::Text::Bold);

    sf::FloatRect twoPlayerBounds = m_twoPlayerText.getLocalBounds();
    m_twoPlayerText.setOrigin(sf::Vector2f(twoPlayerBounds.size.x / 2.0f, twoPlayerBounds.size.y / 2.0f));
    m_twoPlayerText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, startY + buttonHeight / 2.0f));

    m_vsAIButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_vsAIButton.setPosition(sf::Vector2f((WINDOW_WIDTH - buttonWidth) / 2.0f, startY + spacing));
    m_vsAIButton.setFillColor(sf::Color(50, 50, 120));
    m_vsAIButton.setOutlineThickness(4.0f);
    m_vsAIButton.setOutlineColor(CYAN);

    m_vsAIText.setString("PLAYER vs AI");
    m_vsAIText.setCharacterSize(38U);
    m_vsAIText.setFillColor(sf::Color::White);
    m_vsAIText.setOutlineColor(sf::Color::Black);
    m_vsAIText.setOutlineThickness(3.0f);
    m_vsAIText.setStyle(sf::Text::Bold);

    sf::FloatRect vsAIBounds = m_vsAIText.getLocalBounds();
    m_vsAIText.setOrigin(sf::Vector2f(vsAIBounds.size.x / 2.0f, vsAIBounds.size.y / 2.0f));
    m_vsAIText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, startY + spacing + buttonHeight / 2.0f));

    m_aiVsAIButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_aiVsAIButton.setPosition(sf::Vector2f((WINDOW_WIDTH - buttonWidth) / 2.0f, startY + spacing * 2));
    m_aiVsAIButton.setFillColor(sf::Color(50, 50, 120));
    m_aiVsAIButton.setOutlineThickness(4.0f);
    m_aiVsAIButton.setOutlineColor(CYAN);

    m_aiVsAIText.setString("AI vs AI");
    m_aiVsAIText.setCharacterSize(38U);
    m_aiVsAIText.setFillColor(sf::Color::White);
    m_aiVsAIText.setOutlineColor(sf::Color::Black);
    m_aiVsAIText.setOutlineThickness(3.0f);
    m_aiVsAIText.setStyle(sf::Text::Bold);

    sf::FloatRect aiVsAIBounds = m_aiVsAIText.getLocalBounds();
    m_aiVsAIText.setOrigin(sf::Vector2f(aiVsAIBounds.size.x / 2.0f, aiVsAIBounds.size.y / 2.0f));
    m_aiVsAIText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, startY + spacing * 2 + buttonHeight / 2.0f));

    m_background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_background.setFillColor(DARK_BLUE);

    for (int i = 0; i < 5; ++i)//Main menu circles
    {
        m_menuCircles[i].setRadius(50.0f + i * 30.0f);
        int alpha = 50 - i * 8;
        m_menuCircles[i].setFillColor(sf::Color(120, 60, 190, static_cast<uint8_t>(alpha)));
        m_menuCircles[i].setPosition(sf::Vector2f( 100.0f + i * 150.0f, 600.0f - i * 50.0f
        ));
    }
}

void Menu::updateButtonStates()//for highlighting buttons when hovering
{
    m_twoPlayerButton.setFillColor(sf::Color(50, 50, 120));
    m_vsAIButton.setFillColor(sf::Color(50, 50, 120));
    m_aiVsAIButton.setFillColor(sf::Color(50, 50, 120));

    m_twoPlayerButton.setOutlineColor(CYAN);
    m_vsAIButton.setOutlineColor(CYAN);
    m_aiVsAIButton.setOutlineColor(CYAN);

    if (m_hoveredButton == 0)
    {
        m_twoPlayerButton.setFillColor(sf::Color(80, 100, 180));
        m_twoPlayerButton.setOutlineColor(BRIGHT_YELLOW);
        m_twoPlayerButton.setOutlineThickness(5.0f);
    }
    else if (m_hoveredButton == 1)
    {
        m_vsAIButton.setFillColor(sf::Color(80, 100, 180));
        m_vsAIButton.setOutlineColor(BRIGHT_YELLOW);
        m_vsAIButton.setOutlineThickness(5.0f);
    }
    else if (m_hoveredButton == 2)
    {
        m_aiVsAIButton.setFillColor(sf::Color(80, 100, 180));
        m_aiVsAIButton.setOutlineColor(BRIGHT_YELLOW);
        m_aiVsAIButton.setOutlineThickness(5.0f);
    }
}

void Menu::handleClick(sf::Vector2f t_mousePos)
{
    if (m_twoPlayerButton.getGlobalBounds().contains(t_mousePos))
    {
        m_selectedMode = GameMode::TWO_PLAYER;
    }
    else if (m_vsAIButton.getGlobalBounds().contains(t_mousePos))
    {
        m_selectedMode = GameMode::PLAYER_VS_AI;
    }
    else if (m_aiVsAIButton.getGlobalBounds().contains(t_mousePos))
    {
        m_selectedMode = GameMode::AI_VS_AI;
    }
}

void Menu::handleMouseMove(sf::Vector2f t_mousePos)//for hovering the mouse
{
    m_hoveredButton = -1;

    if (m_twoPlayerButton.getGlobalBounds().contains(t_mousePos))
    {
        m_hoveredButton = 0;
    }
    else if (m_vsAIButton.getGlobalBounds().contains(t_mousePos))
    {
        m_hoveredButton = 1;
    }
    else if (m_aiVsAIButton.getGlobalBounds().contains(t_mousePos))
    {
        m_hoveredButton = 2;
    }

    updateButtonStates();
}

void Menu::draw(sf::RenderWindow& t_window)
{
    t_window.draw(m_background);

    for (int i = 0; i < 5; ++i)
    {
        t_window.draw(m_menuCircles[i]);
    }

    t_window.draw(m_twoPlayerButton);
    t_window.draw(m_twoPlayerText);

    t_window.draw(m_vsAIButton);
    t_window.draw(m_vsAIText);

    t_window.draw(m_aiVsAIButton);
    t_window.draw(m_aiVsAIText);

    t_window.draw(m_titleText);
    t_window.draw(m_subtitleText);
}

GameMode Menu::getSelectedMode() const
{
    return m_selectedMode;
}

bool Menu::isModeSelected() const
{
    return m_selectedMode != GameMode::NONE;
}