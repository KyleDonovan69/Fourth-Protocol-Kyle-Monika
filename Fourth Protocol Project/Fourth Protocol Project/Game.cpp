#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{WINDOW_WIDTH, WINDOW_HEIGHT}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{ false }
{
	setupTexts();
	m_grid.loadFont(m_jerseyFont); // Share font with grid
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if (newEvent->is<sf::Event::Closed>())
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>())
		{
			processKeys(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			processMouse(newEvent);
		}
	}
}


void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed* newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true;
	}
	if (sf::Keyboard::Key::Num1 == newKeypress->code)
	{
		m_grid.setSelectedPiece(PieceType::FROG);
		updateSelectedPieceText();
	}
	if (sf::Keyboard::Key::Num2 == newKeypress->code)
	{
		m_grid.setSelectedPiece(PieceType::SNAKE);
		updateSelectedPieceText();
	}
	if (sf::Keyboard::Key::Num3 == newKeypress->code)
	{
		m_grid.setSelectedPiece(PieceType::DONKEY);
		updateSelectedPieceText();
	}

}

void Game::processMouse(const std::optional<sf::Event> t_event)
{
	const sf::Event::MouseButtonPressed* mouseClick = t_event->getIf<sf::Event::MouseButtonPressed>();
	if (sf::Mouse::Button::Left == mouseClick->button)
	{
		sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseClick->position);
		int row, col;
		if (m_grid.getCellFromMouse(mousePos, row, col))
		{
			m_grid.placePiece(row, col);
			updatePlayerText();
			updatePieceCountText();
		}
	}
}


void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true;
	}
}


void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}


void Game::render()
{
	m_window.clear(ULTRAMARINE);

	m_grid.draw(m_window);
	m_window.draw(m_titleText);
	m_window.draw(m_playerText);
	m_window.draw(m_pieceCountText);
	m_window.draw(m_selectedPieceText);

	m_window.display();
}


void Game::setupTexts()
{
	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "problem loading Jersey20 font" << std::endl;
	}

	// Setup title text
	m_titleText.setString("The Fourth Protocol");
	m_titleText.setCharacterSize(40U);
	m_titleText.setOutlineColor(sf::Color::Black);
	m_titleText.setFillColor(sf::Color::White);
	m_titleText.setOutlineThickness(2.0f);

	sf::FloatRect titleBounds = m_titleText.getLocalBounds();
	m_titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f));
	m_titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 25.0f));

	// Setup player text
	m_playerText.setFillColor(sf::Color::White);
	m_playerText.setCharacterSize(30U);
	updatePlayerText();
	sf::FloatRect playerBounds = m_playerText.getLocalBounds();
	m_playerText.setOrigin(sf::Vector2f(playerBounds.size.x / 2.0f, playerBounds.size.y / 2.0f));
	m_playerText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.0f));

	// Setup piece count text
	m_pieceCountText.setPosition(sf::Vector2f(10.0f, 80.0f));
	m_pieceCountText.setFillColor(sf::Color::White);
	m_pieceCountText.setCharacterSize(25U);
	updatePieceCountText();

	// Setup selected piece text
	m_selectedPieceText.setPosition(sf::Vector2f(10.0f, 250.0f));
	m_selectedPieceText.setFillColor(sf::Color::Yellow);
	m_selectedPieceText.setCharacterSize(25U);
	updateSelectedPieceText();
}

void Game::updatePlayerText()
{
	if (m_grid.getCurrentPlayer() == Player::PLAYER_ONE)
	{
		m_playerText.setString("Current Player: 1 (Red)");
		m_playerText.setFillColor(sf::Color::Red);
	}
	else
	{
		m_playerText.setString("Current Player: 2 (Blue)");
		m_playerText.setFillColor(sf::Color::Blue);
	}
	sf::FloatRect playerBounds = m_playerText.getLocalBounds();
	m_playerText.setOrigin(sf::Vector2f(playerBounds.size.x / 2.0f, playerBounds.size.y / 2.0f));
	m_playerText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.0f));
}

void Game::updatePieceCountText()
{
	std::string countText = "Player 1 Pieces:\n";
	countText += "  Frogs: " + std::to_string(m_grid.getRemainingPieces(Player::PLAYER_ONE, PieceType::FROG)) + "/1\n";
	countText += "  Snakes: " + std::to_string(m_grid.getRemainingPieces(Player::PLAYER_ONE, PieceType::SNAKE)) + "/1\n";
	countText += "  Donkeys: " + std::to_string(m_grid.getRemainingPieces(Player::PLAYER_ONE, PieceType::DONKEY)) + "/3\n\n";

	countText += "Player 2 Pieces:\n";
	countText += "  Frogs: " + std::to_string(m_grid.getRemainingPieces(Player::PLAYER_TWO, PieceType::FROG)) + "/1\n";
	countText += "  Snakes: " + std::to_string(m_grid.getRemainingPieces(Player::PLAYER_TWO, PieceType::SNAKE)) + "/1\n";
	countText += "  Donkeys: " + std::to_string(m_grid.getRemainingPieces(Player::PLAYER_TWO, PieceType::DONKEY)) + "/3";

	m_pieceCountText.setString(countText);
}

void Game::updateSelectedPieceText()
{
	std::string selectedName;
	PieceType selected = m_grid.getSelectedPiece();

	switch (selected)
	{
	case PieceType::FROG:
		selectedName = "Frog (1)";
		break;
	case PieceType::SNAKE:
		selectedName = "Snake (2)";
		break;
	case PieceType::DONKEY:
		selectedName = "Donkey (3)";
		break;
	default:
		selectedName = "None";
		break;
	}

	m_selectedPieceText.setString("Selected: " + selectedName);
}

std::string Game::getSelectedPieceName() const
{
	PieceType selected = m_grid.getSelectedPiece();

	switch (selected)
	{
	case PieceType::FROG:
		return "Frog";
	case PieceType::SNAKE:
		return "Snake";
	case PieceType::DONKEY:
		return "Donkey";
	default:
		return "None";
	}
}