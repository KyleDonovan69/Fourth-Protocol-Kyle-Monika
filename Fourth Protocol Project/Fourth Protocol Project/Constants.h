#pragma once

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