#include "Menu.h"

Menu::Menu(const sf::Font& t_font) :
    m_selectedMode(GameMode::NONE)
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

    m_titleText.setFont(m_font);
    m_titleText.setString("The Fourth Protocol");
    m_titleText.setCharacterSize(55U);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setOutlineColor(sf::Color::Black);
    m_titleText.setOutlineThickness(3.0f);
    
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f));
    m_titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 150.0f));
    
    m_twoPlayerButton.setSize(sf::Vector2f(400.0f, 80.0f));
    m_twoPlayerButton.setPosition(sf::Vector2f((WINDOW_WIDTH - 400.0f) / 2.0f, 300.0f));
    m_twoPlayerButton.setFillColor(sf::Color(100, 150, 200));
    m_twoPlayerButton.setOutlineThickness(3.0f);
    m_twoPlayerButton.setOutlineColor(sf::Color::White);
    
    m_twoPlayerText.setFont(m_font);
    m_twoPlayerText.setString("2 Player");
    m_twoPlayerText.setCharacterSize(40U);
    m_twoPlayerText.setFillColor(sf::Color::White);
    
    sf::FloatRect twoPlayerBounds = m_twoPlayerText.getLocalBounds();
    m_twoPlayerText.setOrigin(sf::Vector2f(twoPlayerBounds.size.x / 2.0f, twoPlayerBounds.size.y / 2.0f));
    m_twoPlayerText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 340.0f));

    m_vsAIButton.setSize(sf::Vector2f(400.0f, 80.0f));
    m_vsAIButton.setPosition(sf::Vector2f((WINDOW_WIDTH - 400.0f) / 2.0f, 420.0f));
    m_vsAIButton.setFillColor(sf::Color(100, 200, 150));
    m_vsAIButton.setOutlineThickness(3.0f);
    m_vsAIButton.setOutlineColor(sf::Color::White);
    
    m_vsAIText.setFont(m_font);
    m_vsAIText.setString("Player vs AI");
    m_vsAIText.setCharacterSize(40U);
    m_vsAIText.setFillColor(sf::Color::White);
    
    sf::FloatRect vsAIBounds = m_vsAIText.getLocalBounds();
    m_vsAIText.setOrigin(sf::Vector2f(vsAIBounds.size.x / 2.0f, vsAIBounds.size.y / 2.0f));
    m_vsAIText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 460.0f));
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
}

void Menu::draw(sf::RenderWindow& t_window)
{
    t_window.draw(m_titleText);
    t_window.draw(m_twoPlayerButton);
    t_window.draw(m_twoPlayerText);
    t_window.draw(m_vsAIButton);
    t_window.draw(m_vsAIText);
}

GameMode Menu::getSelectedMode() const
{
    return m_selectedMode;
}

bool Menu::isModeSelected() const
{
    return m_selectedMode != GameMode::NONE;
}