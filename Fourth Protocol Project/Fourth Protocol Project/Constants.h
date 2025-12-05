#pragma once
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

static const int GRID_SIZE = 5;
static const int CELL_SIZE = 100;

//player stuff
static const int MAX_FROGS_PER_PLAYER = 1;
static const int MAX_SNAKES_PER_PLAYER = 1;
static const int MAX_DONKEYS_PER_PLAYER = 3;

//ai stuff
static const int MAX_DEPTH = 3;  // Depth for minimax search
static const int WIN_SCORE = 10000;
static const int LOSE_SCORE = -10000;

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