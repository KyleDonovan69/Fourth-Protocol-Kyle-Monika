#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Grid.h"
#include "Constants.h"

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

	void setupTexts();
	void updatePlayerText();
	void updatePieceCountText();
	void updateSelectedPieceText();

	std::string getSelectedPieceName() const;

	sf::RenderWindow m_window;
	sf::Font m_jerseyFont;
	sf::Text m_titleText{ m_jerseyFont };
	sf::Text m_playerText{ m_jerseyFont };
	sf::Text m_pieceCountText{ m_jerseyFont };
	sf::Text m_selectedPieceText{ m_jerseyFont };

	bool m_DELETEexitGame;
	Grid m_grid;
};

#pragma warning( pop ) 
#endif // !GAME_HPP