/**
 * @file Constants.h
 * @brief Global constants and enums for the Fourth Protocol game
 * @authors: Kyle & Monika
 */

#pragma once
#include <SFML/Graphics.hpp>

// Window dimensions
const int WINDOW_WIDTH = 1000;   ///< Window width in pixels
const int WINDOW_HEIGHT = 800;   ///< Window height in pixels

// Grid dimensions
static const int GRID_SIZE = 5;      ///< Size of the game grid (5x5)
static const int CELL_SIZE = 100;    ///< Size of each grid cell in pixels

// Player piece limits
static const int MAX_FROGS_PER_PLAYER = 1;     ///< Maximum frogs per player
static const int MAX_SNAKES_PER_PLAYER = 1;    ///< Maximum snakes per player
static const int MAX_DONKEYS_PER_PLAYER = 3;   ///< Maximum donkeys per player

/**
 * @enum Difficulty
 * @brief AI difficulty levels
 */
enum class Difficulty
{
    EASY,       ///< Easy difficulty (depth 1)
    MEDIUM,     ///< Medium difficulty (depth 3)
    HARD        ///< Hard difficulty (depth 5)
};

// AI configuration constants
static const int MAX_DEPTH_EASY = 1;      ///< Minimax depth for easy AI
static const int MAX_DEPTH_MEDIUM = 3;    ///< Minimax depth for medium AI
static const int MAX_DEPTH_HARD = 5;      ///< Minimax depth for hard AI
static const int MAX_DEPTH = 3;           ///< Default minimax search depth
static const int WIN_SCORE = 10000;       ///< Score value for winning position
static const int LOSE_SCORE = -10000;     ///< Score value for losing position

// custom colours for the overhaul
static const sf::Color DARK_BLUE = sf::Color(15, 25, 50);     
static const sf::Color DARK_PURPLE = sf::Color(120, 60, 190);
static const sf::Color CYAN = sf::Color(0, 255, 255);
static const sf::Color BRIGHT_YELLOW = sf::Color(255, 230, 0);
static const sf::Color BRIGHT_RED = sf::Color(255, 50, 80);   
static const sf::Color BRIGHT_BLUE = sf::Color(0, 180, 255);  

// grid colours so i dont mix them up
static const sf::Color GRID_LIGHT = sf::Color(255, 255, 255);
static const sf::Color GRID_DARK = sf::Color(180, 200, 220); 
static const sf::Color GRID_BORDER = sf::Color(60, 80, 120); 

// ui colour stuff
static const sf::Color UI_BACKGROUND = sf::Color(10, 10, 30, 240);
static const sf::Color HIGHLIGHT_GREEN = sf::Color(0, 255, 100, 180);