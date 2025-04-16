#pragma once

#include <array>
#include <string>
#include <memory>
#include <QObject>

namespace tictactoe {

enum class Player {
    NONE,
    X,
    O
};

enum class GameState {
    IN_PROGRESS,
    X_WON,
    O_WON,
    DRAW
};

class GameEngine : public QObject {
    Q_OBJECT

public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine() = default;

    // Game control
    bool makeMove(int row, int col);
    void resetGame();
    void setGameMode(bool isVsAI);

    // Game state
    GameState getGameState() const;
    Player getCurrentPlayer() const;
    const std::array<std::array<Player, 3>, 3>& getBoard() const;
    bool isGameOver() const;

signals:
    void gameStateChanged(GameState newState);
    void currentPlayerChanged(Player newPlayer);
    void boardChanged();
    void gameOver(GameState finalState);

private:
    bool checkWin() const;
    bool checkDraw() const;
    void switchPlayer();
    bool isValidMove(int row, int col) const;

    std::array<std::array<Player, 3>, 3> board_;
    Player currentPlayer_;
    GameState gameState_;
    bool isVsAI_;
}; 