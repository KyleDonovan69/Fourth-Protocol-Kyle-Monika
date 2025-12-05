===============================================================
    FOURTH PROTOCOL
    by Kyle & Monika
    AI for Games
===============================================================

This is our Fourth Protocol project - a mix of Chess and Connect 4.
It starts with 

===============================================================
    HOW TO RUN THIS THING
===============================================================

1. Open "Fourth Protocol Project" Folder
2. Either:
    - Open the Visual Studio solution file (.sln) to build and run
   OR
    - Navigate to the "Build" folder and run either:
        - "FourthProtocol_Debug.exe" for debug version
        - "FourthProtocol_Release.exe" for release version (Both should be the same)
3. Enjoy the game, try not to lose

IMPORTANT: All the SFML libraries are included in the project.
You don't need to download or install SFML separately. 
Everything you need should be right here.

===============================================================
    TECHNICAL STUFF (For the Video/Grading)
===============================================================

Language & Standards:
- C++14 (as required)
- Compiled with Visual Studio 2022
- Documented with comments throughout

External Libraries Used:
- SFML 3.0
  * Used for graphics, window management, and audio
  * All .lib files are auto-linked via #pragma in main.cpp
  * DLLs are in the output directory
  * Environment variable not needed - libs are part of the project

Project Structure:
- main.cpp: initializes the game
- Game.cpp/h: Main game loop, handles all states & UI
- Grid.cpp/h: The game board logic, piece placement/movement, win detection
- Menu.cpp/h: Main menu and game mode selection
- AI.cpp/h: Minimax algorithm with alpha-beta pruning
- Constants.h: All game constants and enums (Leaves it easy to change)

Key Features Implemented:
- Full placement and movement phases
- Win detection (4 in a row - horizontal/vertical/diagonal)
- AI with 3 difficulty levels using minimax algorithm
- Alpha-beta pruning for efficiency
- Move validation for each piece type
- Visual highlighting of valid moves
- AI move visualization - shows AI's thinking process
- Clean UI with player indicators
- Restart and menu navigation

Grid System:
- 5x5 board
- Cells are 100x100 pixels each
- Custom colors for better visibility plus I dont like some of SFML's defaults(Too bright)
- Clear visual indicators for each piece type

AI Implementation:
- MINIMAX ALGORITHM with alpha-beta pruning implemented
- Depth varies by difficulty (handles large search space):
  * Easy: depth 1 (looks 1 move ahead)
  * Medium: depth 3 (looks 3 moves ahead)
  * Hard: depth 5 (looks 5 moves ahead)
- EVALUATION FUNCTION considers:
  * Potential four-in-a-row opportunities for both players
  * Piece positioning and board control
  * Immediate win/loss detection
  * Blocking opponent's winning moves
- ALPHA-BETA PRUNING drastically reduces search space
- AI handles both placement and movement phases

BONUS FEATURES:
- AI vs AI mode - watch two AIs play against each other, betting will start soon
- AI Decision Visualizer - Press V to see:
  * Which moves the AI considered
  * The score for each potential move
  This shows the AI's decision-making as it goes

===============================================================
    CONTROLS
===============================================================

In Menu:
- Click buttons to select game mode (PvP, PvAI, AIvAI)
- Select AI difficulty (Easy/Medium/Hard)

During Placement Phase:
- Number keys (1-3): Select piece type to place
  * 1 = Frog, 2 = Snake, 3 = Donkey
- Mouse Click: Place selected piece on empty cell
- Auto-switches to next available piece type

During Movement Phase:
- Mouse Click: Select your piece, then click where to move it
- Valid moves are highlighted in green
- Pieces follow their specific movement rules
- R: Restart game, works after win/loss
- V: Toggle AI move visualization on/off

===============================================================
    NOTES ON THE CODE
===============================================================

- The AI can take longer on Hard difficulty (this is because its checking so many moves ahead so it slows down a bit)
- We added a slight delay to AI moves so you can actually see what it's doing (otherwise it's instant and confusing)
- Alpha beta pruning was necessary as without it the AI would take forever to do literally anything

===============================================================
    TEAM CONTRIBUTION
===============================================================

Team Members: Kyle & Monika
Github Repo for commit history: https://github.com/KyleDonovan69/Fourth-Protocol-Kyle-Monika

Contribution Split:
- Kyle: 50%
- Monika: 50%

We worked together throughout the project, pair programming for most of the AI implementation. 
Both contributed equally to design, coding, testing, and documentation.

===============================================================
    FINAL NOTES
===============================================================

The project should just work out of the box using either the debug or release EXE file. 
Everything needed is included - you don't need to install anything.

Thanks for checking out our project! It was honestly a good bit of craic to make and figure out how the AI works from scratch

===============================================================
