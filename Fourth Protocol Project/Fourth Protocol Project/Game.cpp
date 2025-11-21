#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{WINDOW_WIDTH, WINDOW_HEIGHT}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{ false },
	m_menu(m_jerseyFont),
	m_gameMode(GameMode::NONE),
	m_showMenu(true),
	m_aiWaiting(false),
	m_aiDelaySeconds(1.0f)
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
	if (sf::Keyboard::Key::R == newKeypress->code)//resets all texts and players
	{
		if (m_grid.getGameState() == GameState::GAME_OVER)
		{
			m_grid.resetGame();
			updateAllUI();
			m_grid.clearHighlights();
		}
	}
	if (sf::Keyboard::Key::M == newKeypress->code)
	{
		if (m_grid.getGameState() == GameState::GAME_OVER)
		{
			m_grid.resetGame();
			m_gameMode = GameMode::NONE;
			m_showMenu = true;
			m_aiWaiting = false;

			updateAllUI();
			m_grid.clearHighlights();
		}
	}
	if (!m_aiWaiting && m_grid.getGameState() != GameState::GAME_OVER)
	{
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

}

void Game::processMouse(const std::optional<sf::Event> t_event)
{
	const sf::Event::MouseButtonPressed* mouseClick = t_event->getIf<sf::Event::MouseButtonPressed>();
	if (sf::Mouse::Button::Left == mouseClick->button)
	{
		sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseClick->position);

		if (m_showMenu)
		{
			m_menu.handleClick(mousePos);
			if (m_menu.isModeSelected())
			{
				m_gameMode = m_menu.getSelectedMode();
				m_showMenu = false;
			}
		}
		else
		{
			bool canClick = !m_aiWaiting && m_grid.getGameState() != GameState::GAME_OVER;
			
			if (m_gameMode == GameMode::PLAYER_VS_AI)
			{
				// only allow clicks when it's Player 1 turn
				canClick = canClick && (m_grid.getCurrentPlayer() == Player::PLAYER_ONE);
			}
			if (canClick)
			{
				int row, col;
				if (m_grid.getCellFromMouse(mousePos, row, col))
				{
					m_grid.handleClick(row, col);//clicking pieces + movement
					updateAllUI();

					if (m_gameMode == GameMode::PLAYER_VS_AI && m_grid.getCurrentPlayer() == Player::PLAYER_TWO && m_grid.getGameState() != GameState::GAME_OVER)
					{
						m_aiWaiting = true;
						m_aiClock.restart();
					}
				}
			}
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

	if (m_aiWaiting && m_aiClock.getElapsedTime().asSeconds() >= m_aiDelaySeconds)
	{
		m_aiWaiting = false;
		handleAITurn();
	}
}


void Game::render()
{
	m_window.clear(ULTRAMARINE);

	if (m_showMenu)
	{
		m_menu.draw(m_window);
	}
	else
	{
		m_grid.draw(m_window);
		m_window.draw(m_titleText);
		m_window.draw(m_playerText);
		m_window.draw(m_pieceCountText);
		m_window.draw(m_selectedPieceText);
		m_window.draw(m_gameStateText);

		if (m_grid.getGameState() == GameState::GAME_OVER)
		{
			m_window.draw(m_winnerText);
			m_window.draw(m_restartText);
			m_window.draw(m_menuText);
		}
	}

	m_window.display();
}

void Game::handleAITurn()
{
	m_ai.makeMove(m_grid);

	// update UI
	updateAllUI();
}


void Game::setupTexts()
{
	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "problem loading Jersey20 font" << std::endl;
	}

	// Setup title text at top
	m_titleText.setString("The Fourth Protocol");
	m_titleText.setCharacterSize(45U);
	m_titleText.setOutlineColor(sf::Color::Black);
	m_titleText.setFillColor(sf::Color::White);
	m_titleText.setOutlineThickness(2.0f);

	sf::FloatRect titleBounds = m_titleText.getLocalBounds();
	m_titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f));
	m_titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 30.0f));

	// Setup player text at bottom
	m_playerText.setFillColor(sf::Color::White);
	m_playerText.setCharacterSize(25U);
	m_playerText.setOutlineColor(sf::Color::Black);
	m_playerText.setOutlineThickness(2.0f);
	updatePlayerText();
	sf::FloatRect playerBounds = m_playerText.getLocalBounds();
	m_playerText.setOrigin(sf::Vector2f(playerBounds.size.x / 2.0f, playerBounds.size.y / 2.0f));
	m_playerText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 40.0f));

	// Setup piece count text on left side
	m_pieceCountText.setPosition(sf::Vector2f(10.0f, 80.0f));
	m_pieceCountText.setFillColor(sf::Color::White);
	m_pieceCountText.setCharacterSize(25U);
	m_pieceCountText.setOutlineColor(sf::Color::Black);
	m_pieceCountText.setOutlineThickness(2.0f);
	updatePieceCountText();

	// Shows what piece is selected
	m_selectedPieceText.setPosition(sf::Vector2f(10.0f, 450.0f));
	m_selectedPieceText.setFillColor(sf::Color::Yellow);
	m_selectedPieceText.setCharacterSize(25U);
	m_selectedPieceText.setOutlineColor(sf::Color::Black);
	m_selectedPieceText.setOutlineThickness(2.0f);
	updateSelectedPieceText();

	// State text
	m_gameStateText.setPosition(sf::Vector2f(WINDOW_WIDTH - 250.0f, 80.0f));
	m_gameStateText.setFillColor(sf::Color::Cyan);
	m_gameStateText.setCharacterSize(25U);
	m_gameStateText.setOutlineColor(sf::Color::Black);
	m_gameStateText.setOutlineThickness(2.0f);
	updateGameStateText();

	m_winnerText.setCharacterSize(60U);//displays whoever won
	m_winnerText.setFillColor(sf::Color::Yellow);
	m_winnerText.setOutlineColor(sf::Color::Black);
	m_winnerText.setOutlineThickness(3.0f);
	updateWinnerText();

	// Setup restart text for the endgame
	m_restartText.setString("Press R to Restart");
	m_restartText.setCharacterSize(30U);
	m_restartText.setFillColor(sf::Color::White);
	m_restartText.setOutlineColor(sf::Color::Black);
	m_restartText.setOutlineThickness(2.0f);
	sf::FloatRect restartBounds = m_restartText.getLocalBounds();
	m_restartText.setOrigin(sf::Vector2f(restartBounds.size.x / 2.0f, restartBounds.size.y / 2.0f));
	m_restartText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 100.0f));

	m_menuText.setString("Press M to Return to Menu");
	m_menuText.setCharacterSize(30U);
	m_menuText.setFillColor(sf::Color::White);
	m_menuText.setOutlineColor(sf::Color::Black);
	m_menuText.setOutlineThickness(2.0f);
	sf::FloatRect menuBounds = m_menuText.getLocalBounds();
	m_menuText.setOrigin(sf::Vector2f(menuBounds.size.x / 2.0f, menuBounds.size.y / 2.0f));
	m_menuText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 150.0f));
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

void Game::updateGameStateText()
{
	if (m_grid.getGameState() == GameState::PLACEMENT)
	{
		m_gameStateText.setString("PLACEMENT PHASE");
	}
	else if (m_grid.getGameState() == GameState::MOVEMENT)
	{
		m_gameStateText.setString("MOVEMENT PHASE");
	}
	else
	{
		m_gameStateText.setString("GAME OVER");
	}
}

void Game::updateWinnerText()
{
	if (m_grid.getGameState() == GameState::GAME_OVER)
	{
		Player winner = m_grid.getWinner();
		std::string winnerStr;
		sf::Color winnerColor;

		if (winner == Player::PLAYER_ONE)
		{
			winnerStr = "PLAYER 1 (RED) WINS!";
			winnerColor = sf::Color::Red;
		}
		else if (winner == Player::PLAYER_TWO)
		{
			winnerStr = "PLAYER 2 (BLUE) WINS!";
			winnerColor = sf::Color::Blue;
		}
		else
		{
			winnerStr = "TIE!";
			winnerColor = sf::Color::Yellow;
		}

		m_winnerText.setString(winnerStr);
		m_winnerText.setFillColor(winnerColor);

		sf::FloatRect winnerBounds = m_winnerText.getLocalBounds();
		m_winnerText.setOrigin(sf::Vector2f(winnerBounds.size.x / 2.0f, winnerBounds.size.y / 2.0f));
		m_winnerText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
	}
}

void Game::updateAllUI()
{
	updatePlayerText();
	updatePieceCountText();
	updateGameStateText();
	updateWinnerText();
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