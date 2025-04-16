#include "ui/gameboard.h"
#include "ui_gameboard.h"
#include <QGridLayout>
#include <QPushButton>
#include <QStyle>

namespace tictactoe {

GameBoard::GameBoard(QWidget* parent)
    : QWidget(parent)
    , ui_(std::make_unique<Ui::GameBoard>())
    , gameEngine_(nullptr)
{
    ui_->setupUi(this);
    setupBoard();
}

GameBoard::~GameBoard() = default;

void GameBoard::setGameEngine(GameEngine* engine)
{
    gameEngine_ = engine;
    if (gameEngine_) {
        setupConnections();
        updateBoard();
    }
}

void GameBoard::resetBoard()
{
    if (gameEngine_) {
        gameEngine_->resetGame();
    }
}

void GameBoard::updateBoard()
{
    if (!gameEngine_) {
        return;
    }

    const auto& board = gameEngine_->getBoard();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            updateCellStyle(cells_[i][j], board[i][j]);
        }
    }
}

void GameBoard::setupBoard()
{
    auto layout = new QGridLayout(this);
    layout->setSpacing(5);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto button = new QPushButton(this);
            button->setFixedSize(100, 100);
            button->setFont(QFont("Arial", 24, QFont::Bold));
            button->setProperty("row", i);
            button->setProperty("col", j);
            cells_[i][j] = button;
            layout->addWidget(button, i, j);
        }
    }

    setLayout(layout);
}

void GameBoard::setupConnections()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            connect(cells_[i][j], &QPushButton::clicked,
                    this, &GameBoard::onCellClicked);
        }
    }

    connect(gameEngine_, &GameEngine::gameStateChanged,
            this, &GameBoard::onGameStateChanged);
    connect(gameEngine_, &GameEngine::currentPlayerChanged,
            this, &GameBoard::onCurrentPlayerChanged);
    connect(gameEngine_, &GameEngine::boardChanged,
            this, &GameBoard::onBoardChanged);
}

void GameBoard::updateCellStyle(QPushButton* button, Player player)
{
    QString text;
    QString style;

    switch (player) {
        case Player::X:
            text = "X";
            style = "background-color: #FFB6C1; color: #000000;";
            break;
        case Player::O:
            text = "O";
            style = "background-color: #87CEEB; color: #000000;";
            break;
        default:
            text = "";
            style = "background-color: #FFFFFF; color: #000000;";
            break;
    }

    button->setText(text);
    button->setStyleSheet(style);
}

void GameBoard::disableBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells_[i][j]->setEnabled(false);
        }
    }
}

void GameBoard::enableBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells_[i][j]->setEnabled(true);
        }
    }
}

QString GameBoard::getPlayerSymbol(Player player) const
{
    return (player == Player::X) ? "X" : "O";
}

void GameBoard::onCellClicked()
{
    if (!gameEngine_) {
        return;
    }

    auto button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    if (gameEngine_->makeMove(row, col)) {
        if (gameEngine_->isGameOver()) {
            disableBoard();
        }
    }
}

void GameBoard::onGameStateChanged(GameState newState)
{
    if (newState != GameState::IN_PROGRESS) {
        disableBoard();
    } else {
        enableBoard();
    }
}

void GameBoard::onCurrentPlayerChanged(Player newPlayer)
{
    // Update UI to show current player
    ui_->currentPlayerLabel->setText("Current player: " + getPlayerSymbol(newPlayer));
}

void GameBoard::onBoardChanged()
{
    updateBoard();
}

} // namespace tictactoe 