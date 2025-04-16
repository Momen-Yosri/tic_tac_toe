#include <gtest/gtest.h>
#include "game/gameengine.h"

namespace tictactoe {
namespace test {

class GameEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = std::make_unique<GameEngine>();
    }

    void TearDown() override {
        engine.reset();
    }

    std::unique_ptr<GameEngine> engine;
};

TEST_F(GameEngineTest, InitialState) {
    EXPECT_EQ(engine->getGameState(), GameState::IN_PROGRESS);
    EXPECT_EQ(engine->getCurrentPlayer(), Player::X);
    EXPECT_FALSE(engine->isGameOver());
}

TEST_F(GameEngineTest, ValidMove) {
    EXPECT_TRUE(engine->makeMove(0, 0));
    EXPECT_EQ(engine->getCurrentPlayer(), Player::O);
}

TEST_F(GameEngineTest, InvalidMove) {
    EXPECT_TRUE(engine->makeMove(0, 0));
    EXPECT_FALSE(engine->makeMove(0, 0)); // Same position
    EXPECT_FALSE(engine->makeMove(-1, 0)); // Invalid row
    EXPECT_FALSE(engine->makeMove(0, 3)); // Invalid column
}

TEST_F(GameEngineTest, WinCondition) {
    // X makes moves to win horizontally
    EXPECT_TRUE(engine->makeMove(0, 0)); // X
    EXPECT_TRUE(engine->makeMove(1, 0)); // O
    EXPECT_TRUE(engine->makeMove(0, 1)); // X
    EXPECT_TRUE(engine->makeMove(1, 1)); // O
    EXPECT_TRUE(engine->makeMove(0, 2)); // X

    EXPECT_EQ(engine->getGameState(), GameState::X_WON);
    EXPECT_TRUE(engine->isGameOver());
}

TEST_F(GameEngineTest, DrawCondition) {
    // Fill the board to create a draw
    EXPECT_TRUE(engine->makeMove(0, 0)); // X
    EXPECT_TRUE(engine->makeMove(0, 1)); // O
    EXPECT_TRUE(engine->makeMove(0, 2)); // X
    EXPECT_TRUE(engine->makeMove(1, 0)); // O
    EXPECT_TRUE(engine->makeMove(1, 1)); // X
    EXPECT_TRUE(engine->makeMove(1, 2)); // O
    EXPECT_TRUE(engine->makeMove(2, 0)); // X
    EXPECT_TRUE(engine->makeMove(2, 2)); // O
    EXPECT_TRUE(engine->makeMove(2, 1)); // X

    EXPECT_EQ(engine->getGameState(), GameState::DRAW);
    EXPECT_TRUE(engine->isGameOver());
}

TEST_F(GameEngineTest, ResetGame) {
    // Make some moves
    EXPECT_TRUE(engine->makeMove(0, 0));
    EXPECT_TRUE(engine->makeMove(1, 1));

    // Reset the game
    engine->resetGame();

    // Check initial state
    EXPECT_EQ(engine->getGameState(), GameState::IN_PROGRESS);
    EXPECT_EQ(engine->getCurrentPlayer(), Player::X);
    EXPECT_FALSE(engine->isGameOver());

    // Verify board is empty
    const auto& board = engine->getBoard();
    for (const auto& row : board) {
        for (const auto& cell : row) {
            EXPECT_EQ(cell, Player::NONE);
        }
    }
}

TEST_F(GameEngineTest, GameMode) {
    EXPECT_FALSE(engine->isVsAI());
    engine->setGameMode(true);
    EXPECT_TRUE(engine->isVsAI());
}

} // namespace test
} // namespace tictactoe 