#include "ui/mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/loginwindow.h"
#include "ui/gameboard.h"
#include <QMessageBox>
#include <QDateTime>

namespace tictactoe {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>())
    , gameEngine_(std::make_unique<GameEngine>())
    , userManager_(std::make_unique<UserManager>())
    , dbManager_(std::make_unique<DatabaseManager>())
{
    ui_->setupUi(this);
    setupConnections();

    if (!dbManager_->initialize()) {
        QMessageBox::critical(this, "Error", "Failed to initialize database");
        return;
    }

    showLoginDialog();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupConnections()
{
    // Game engine connections
    connect(gameEngine_.get(), &GameEngine::gameStateChanged,
            this, &MainWindow::onGameStateChanged);
    connect(gameEngine_.get(), &GameEngine::currentPlayerChanged,
            this, &MainWindow::onCurrentPlayerChanged);
    connect(gameEngine_.get(), &GameEngine::boardChanged,
            this, &MainWindow::onBoardChanged);
    connect(gameEngine_.get(), &GameEngine::gameOver,
            this, &MainWindow::onGameOver);

    // User manager connections
    connect(userManager_.get(), &UserManager::userLoggedIn,
            this, &MainWindow::onUserLoggedIn);
    connect(userManager_.get(), &UserManager::userLoggedOut,
            this, &MainWindow::onUserLoggedOut);
    connect(userManager_.get(), &UserManager::loginFailed,
            this, &MainWindow::onLoginFailed);
    connect(userManager_.get(), &UserManager::registrationFailed,
            this, &MainWindow::onRegistrationFailed);
}

void MainWindow::updateUI()
{
    // Update window title with current user
    if (userManager_->isUserLoggedIn()) {
        setWindowTitle("Tic Tac Toe - " + QString::fromStdString(userManager_->getCurrentUser().username));
    } else {
        setWindowTitle("Tic Tac Toe");
    }

    // Update game status
    QString status;
    switch (gameEngine_->getGameState()) {
        case GameState::IN_PROGRESS:
            status = "Current player: " + QString(getCurrentPlayer() == Player::X ? "X" : "O");
            break;
        case GameState::X_WON:
            status = "Player X won!";
            break;
        case GameState::O_WON:
            status = "Player O won!";
            break;
        case GameState::DRAW:
            status = "Game ended in a draw!";
            break;
    }
    ui_->statusLabel->setText(status);
}

void MainWindow::showLoginDialog()
{
    LoginWindow loginDialog(this);
    if (loginDialog.exec() != QDialog::Accepted) {
        close();
    }
}

void MainWindow::showGameBoard()
{
    // Create and show game board
    auto gameBoard = new GameBoard(this);
    gameBoard->setGameEngine(gameEngine_.get());
    setCentralWidget(gameBoard);
}

void MainWindow::showGameHistory()
{
    if (!userManager_->isUserLoggedIn()) {
        return;
    }

    auto history = dbManager_->getUserGameHistory(userManager_->getCurrentUser().id);
    QString historyText;
    for (const auto& record : history) {
        historyText += QString::fromStdString(record.timestamp) + " - " +
                      QString::fromStdString(record.result) + "\n";
    }
    QMessageBox::information(this, "Game History", historyText);
}

void MainWindow::saveGameState()
{
    if (!userManager_->isUserLoggedIn()) {
        return;
    }

    GameRecord record;
    record.userId = userManager_->getCurrentUser().id;
    record.timestamp = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();

    switch (gameEngine_->getGameState()) {
        case GameState::X_WON:
            record.result = "WIN";
            break;
        case GameState::O_WON:
            record.result = "LOSS";
            break;
        case GameState::DRAW:
            record.result = "DRAW";
            break;
        default:
            return;
    }

    // TODO: Save moves
    record.moves = "";

    dbManager_->saveGameRecord(record);
}

void MainWindow::onGameStateChanged(GameState newState)
{
    updateUI();
    if (newState != GameState::IN_PROGRESS) {
        saveGameState();
    }
}

void MainWindow::onCurrentPlayerChanged(Player newPlayer)
{
    updateUI();
}

void MainWindow::onBoardChanged()
{
    updateUI();
}

void MainWindow::onGameOver(GameState finalState)
{
    QString message;
    switch (finalState) {
        case GameState::X_WON:
            message = "Player X won!";
            break;
        case GameState::O_WON:
            message = "Player O won!";
            break;
        case GameState::DRAW:
            message = "Game ended in a draw!";
            break;
        default:
            return;
    }

    QMessageBox::information(this, "Game Over", message);
}

void MainWindow::onUserLoggedIn(const User& user)
{
    updateUI();
    showGameBoard();
}

void MainWindow::onUserLoggedOut()
{
    updateUI();
    showLoginDialog();
}

void MainWindow::onLoginFailed(const std::string& error)
{
    QMessageBox::warning(this, "Login Failed", QString::fromStdString(error));
}

void MainWindow::onRegistrationFailed(const std::string& error)
{
    QMessageBox::warning(this, "Registration Failed", QString::fromStdString(error));
}

} // namespace tictactoe 