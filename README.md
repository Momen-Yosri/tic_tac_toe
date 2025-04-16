# Tic Tac Toe Game

A feature-rich Tic Tac Toe game built with C++ and Qt, featuring user authentication, AI opponent, and game history tracking.

## Features

- User Authentication
  - Secure login/registration with password hashing
  - Session management
  - Password change functionality

- Game Modes
  - Player vs Player
  - Player vs AI (using minimax algorithm with alpha-beta pruning)

- Game Features
  - Interactive 3x3 game board
  - Real-time game state updates
  - Win/tie detection
  - Game history tracking
  - Move replay functionality

- Database
  - SQLite database for user profiles and game history
  - Secure password storage with salt
  - Game statistics tracking

## Requirements

- C++17 or later
- Qt 6.0 or later
- SQLite3
- CMake 3.16 or later
- Google Test (for testing)

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/Momen-Yosri/tic-tac-toe.git
cd tic-tac-toe
```

2. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```

3. Configure and build the project:
```bash
cmake ..
cmake --build .
```

## Project Structure

```
tic-tac-toe/
├── include/
│   ├── game/
│   │   ├── gameengine.h
│   │   └── ai_opponent.h
│   ├── auth/
│   │   └── user_manager.h
│   ├── database/
│   │   └── db_manager.h
│   └── ui/
│       ├── mainwindow.h
│       ├── loginwindow.h
│       └── gameboard.h
├── src/
│   ├── game/
│   ├── auth/
│   ├── database/
│   └── ui/
├── ui/
│   ├── mainwindow.ui
│   ├── loginwindow.ui
│   └── gameboard.ui
├── tests/
├── CMakeLists.txt
└── README.md
```

## Testing

Run the tests using:
```bash
cd build
ctest
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details. 
