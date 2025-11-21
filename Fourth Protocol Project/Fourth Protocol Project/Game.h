#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Grid.h"
#include "Constants.h"
#include "Menu.h"
#include "AI.h"

const sf::Color ULTRAMARINE{ 5, 55, 242, 255 };

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void processMouse(const std::optional<sf::Event> t_event);
	void checkKeyboardState();

	void update(sf::Time t_deltaTime);
	void render();

	void handleAITurn();

	void setupTexts();
	void updatePlayerText();
	void updatePieceCountText();
	void updateSelectedPieceText();
	void updateGameStateText();
	void updateWinnerText();

	void updateAllUI();

	std::string getSelectedPieceName() const;

	sf::RenderWindow m_window;
	sf::Font m_jerseyFont;
	sf::Text m_titleText{ m_jerseyFont };
	sf::Text m_playerText{ m_jerseyFont };
	sf::Text m_pieceCountText{ m_jerseyFont };
	sf::Text m_selectedPieceText{ m_jerseyFont };
	sf::Text m_gameStateText{ m_jerseyFont };
	sf::Text m_winnerText{ m_jerseyFont };
	sf::Text m_restartText{ m_jerseyFont };
	sf::Text m_menuText{ m_jerseyFont };

	bool m_DELETEexitGame;
	Grid m_grid;
	Menu m_menu;

	AI m_ai;
	sf::Clock m_aiClock;
	bool m_aiWaiting;
	float m_aiDelaySeconds;

	GameMode m_gameMode;
	bool m_showMenu;
};

#pragma warning( pop ) 
#endif // !GAME_HPP