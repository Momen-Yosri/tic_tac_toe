#include "game/ai_opponent.h"
#include <limits>

namespace tictactoe {

std::pair<int, int> AIOpponent::calculateBestMove(const std::array<std::array<Player, 3>, 3>& board, Player aiPlayer)
{
    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == Player::NONE) {
                auto tempBoard = board;
                tempBoard[i][j] = aiPlayer;
                int score = minimax(tempBoard, 0, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), aiPlayer);
                
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }

    return bestMove;
}

int AIOpponent::minimax(std::array<std::array<Player, 3>, 3> board,
                       int depth,
                       bool isMaximizing,
                       int alpha,
                       int beta,
                       Player aiPlayer)
{
    int score = evaluateBoard(board, aiPlayer);

    if (score != 0) {
        return score;
    }

    if (!hasEmptyCells(board)) {
        return 0;
    }

    if (isMaximizing) {
        int bestScore = std::numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == Player::NONE) {
                    board[i][j] = aiPlayer;
                    int score = minimax(board, depth + 1, false, alpha, beta, aiPlayer);
                    board[i][j] = Player::NONE;
                    bestScore = std::max(score, bestScore);
                    alpha = std::max(alpha, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = std::numeric_limits<int>::max();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == Player::NONE) {
                    board[i][j] = getOpponent(aiPlayer);
                    int score = minimax(board, depth + 1, true, alpha, beta, aiPlayer);
                    board[i][j] = Player::NONE;
                    bestScore = std::min(score, bestScore);
                    beta = std::min(beta, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
}

int AIOpponent::evaluateBoard(const std::array<std::array<Player, 3>, 3>& board, Player aiPlayer) const
{
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != Player::NONE &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) {
            return (board[i][0] == aiPlayer) ? 10 : -10;
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] != Player::NONE &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) {
            return (board[0][i] == aiPlayer) ? 10 : -10;
        }
    }

    // Check diagonals
    if (board[0][0] != Player::NONE &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return (board[0][0] == aiPlayer) ? 10 : -10;
    }

    if (board[0][2] != Player::NONE &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return (board[0][2] == aiPlayer) ? 10 : -10;
    }

    return 0;
}

bool AIOpponent::hasEmptyCells(const std::array<std::array<Player, 3>, 3>& board) const
{
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == Player::NONE) {
                return true;
            }
        }
    }
    return false;
}

Player AIOpponent::getOpponent(Player player) const
{
    return (player == Player::X) ? Player::O : Player::X;
}

} // namespace tictactoe 