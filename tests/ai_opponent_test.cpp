#include <gtest/gtest.h>
#include "game/ai_opponent.h"

namespace tictactoe {
namespace test {

class AIOpponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        ai = std::make_unique<AIOpponent>();
    }

    void TearDown() override {
        ai.reset();
    }

    std::unique_ptr<AIOpponent> ai;
};

TEST_F(AIOpponentTest, EmptyBoard) {
    std::array<std::array<Player, 3>, 3> board;
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = Player::NONE;
        }
    }

    auto move = ai->calculateBestMove(board, Player::X);
    EXPECT_GE(move.first, 0);
    EXPECT_LT(move.first, 3);
    EXPECT_GE(move.second, 0);
    EXPECT_LT(move.second, 3);
}

TEST_F(AIOpponentTest, WinningMove) {
    std::array<std::array<Player, 3>, 3> board;
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = Player::NONE;
        }
    }

    // Set up a winning position for X
    board[0][0] = Player::X;
    board[0][1] = Player::X;
    board[0][2] = Player::NONE;

    auto move = ai->calculateBestMove(board, Player::X);
    EXPECT_EQ(move.first, 0);
    EXPECT_EQ(move.second, 2);
}

TEST_F(AIOpponentTest, BlockingMove) {
    std::array<std::array<Player, 3>, 3> board;
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = Player::NONE;
        }
    }

    // Set up a position where O needs to block X
    board[0][0] = Player::X;
    board[0][1] = Player::X;
    board[0][2] = Player::NONE;

    auto move = ai->calculateBestMove(board, Player::O);
    EXPECT_EQ(move.first, 0);
    EXPECT_EQ(move.second, 2);
}

TEST_F(AIOpponentTest, CenterMove) {
    std::array<std::array<Player, 3>, 3> board;
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = Player::NONE;
        }
    }

    // Empty board, should prefer center
    auto move = ai->calculateBestMove(board, Player::X);
    EXPECT_EQ(move.first, 1);
    EXPECT_EQ(move.second, 1);
}

TEST_F(AIOpponentTest, DrawPosition) {
    std::array<std::array<Player, 3>, 3> board;
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = Player::NONE;
        }
    }

    // Set up a position that leads to a draw
    board[0][0] = Player::X;
    board[0][1] = Player::O;
    board[0][2] = Player::X;
    board[1][0] = Player::O;
    board[1][1] = Player::X;
    board[1][2] = Player::O;
    board[2][0] = Player::X;
    board[2][2] = Player::O;
    board[2][1] = Player::NONE;

    auto move = ai->calculateBestMove(board, Player::X);
    EXPECT_EQ(move.first, 2);
    EXPECT_EQ(move.second, 1);
}

TEST_F(AIOpponentTest, EvaluateBoard) {
    std::array<std::array<Player, 3>, 3> board;
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = Player::NONE;
        }
    }

    // Test horizontal win
    board[0][0] = Player::X;
    board[0][1] = Player::X;
    board[0][2] = Player::X;
    EXPECT_EQ(ai->evaluateBoard(board, Player::X), 10);

    // Test vertical win
    board[0][0] = Player::O;
    board[1][0] = Player::O;
    board[2][0] = Player::O;
    EXPECT_EQ(ai->evaluateBoard(board, Player::O), 10);

    // Test diagonal win
    board[0][0] = Player::X;
    board[1][1] = Player::X;
    board[2][2] = Player::X;
    EXPECT_EQ(ai->evaluateBoard(board, Player::X), 10);
}

} // namespace test
} // namespace tictactoe 