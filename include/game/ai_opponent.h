#pragma once

#include "gameengine.h"
#include <utility>

namespace tictactoe {

class AIOpponent {
public:
    AIOpponent() = default;
    ~AIOpponent() = default;

    // Calculate the best move using minimax with alpha-beta pruning
    std::pair<int, int> calculateBestMove(const std::array<std::array<Player, 3>, 3>& board, Player aiPlayer);

private:
    // Minimax algorithm with alpha-beta pruning
    int minimax(std::array<std::array<Player, 3>, 3> board, 
                int depth, 
                bool isMaximizing, 
                int alpha, 
                int beta, 
                Player aiPlayer);

    // Evaluate the current board state
    int evaluateBoard(const std::array<std::array<Player, 3>, 3>& board, Player aiPlayer) const;

    // Check if there are any empty cells left
    bool hasEmptyCells(const std::array<std::array<Player, 3>, 3>& board) const;

    // Get the opponent player
    Player getOpponent(Player player) const;
};
} 