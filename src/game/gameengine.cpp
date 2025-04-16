#include "game/gameengine.h"
#include <algorithm>

namespace tictactoe {

GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
    , currentPlayer_(Player::X)
    , gameState_(GameState::IN_PROGRESS)
    , isVsAI_(false)
{
    resetGame();
}

bool GameEngine::makeMove(int row, int col)
{
    if (!isValidMove(row, col)) {
        return false;
    }

    board_[row][col] = currentPlayer_;
    emit boardChanged();

    if (checkWin()) {
        gameState_ = (currentPlayer_ == Player::X) ? GameState::X_WON : GameState::O_WON;
        emit gameStateChanged(gameState_);
        emit gameOver(gameState_);
        return true;
    }

    if (checkDraw()) {
        gameState_ = GameState::DRAW;
        emit gameStateChanged(gameState_);
        emit gameOver(gameState_);
        return true;
    }

    switchPlayer();
    return true;
}

void GameEngine::resetGame()
{
    for (auto& row : board_) {
        std::fill(row.begin(), row.end(), Player::NONE);
    }
    currentPlayer_ = Player::X;
    gameState_ = GameState::IN_PROGRESS;
    emit boardChanged();
    emit currentPlayerChanged(currentPlayer_);
    emit gameStateChanged(gameState_);
}

void GameEngine::setGameMode(bool isVsAI)
{
    isVsAI_ = isVsAI;
}

GameState GameEngine::getGameState() const
{
    return gameState_;
}

Player GameEngine::getCurrentPlayer() const
{
    return currentPlayer_;
}

const std::array<std::array<Player, 3>, 3>& GameEngine::getBoard() const
{
    return board_;
}

bool GameEngine::isGameOver() const
{
    return gameState_ != GameState::IN_PROGRESS;
}

bool GameEngine::checkWin() const
{
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board_[i][0] != Player::NONE &&
            board_[i][0] == board_[i][1] &&
            board_[i][1] == board_[i][2]) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board_[0][i] != Player::NONE &&
            board_[0][i] == board_[1][i] &&
            board_[1][i] == board_[2][i]) {
            return true;
        }
    }

    // Check diagonals
    if (board_[0][0] != Player::NONE &&
        board_[0][0] == board_[1][1] &&
        board_[1][1] == board_[2][2]) {
        return true;
    }

    if (board_[0][2] != Player::NONE &&
        board_[0][2] == board_[1][1] &&
        board_[1][1] == board_[2][0]) {
        return true;
    }

    return false;
}

bool GameEngine::checkDraw() const
{
    for (const auto& row : board_) {
        for (const auto& cell : row) {
            if (cell == Player::NONE) {
                return false;
            }
        }
    }
    return true;
}

void GameEngine::switchPlayer()
{
    currentPlayer_ = (currentPlayer_ == Player::X) ? Player::O : Player::X;
    emit currentPlayerChanged(currentPlayer_);
}

bool GameEngine::isValidMove(int row, int col) const
{
    return row >= 0 && row < 3 && col >= 0 && col < 3 && board_[row][col] == Player::NONE;
}

} // namespace tictactoe 