#pragma once

#include <QMainWindow>
#include <memory>
#include "../game/gameengine.h"
#include "../auth/user_manager.h"
#include "../database/db_manager.h"

namespace Ui {
class MainWindow;
}

namespace tictactoe {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onGameStateChanged(GameState newState);
    void onCurrentPlayerChanged(Player newPlayer);
    void onBoardChanged();
    void onGameOver(GameState finalState);
    void onUserLoggedIn(const User& user);
    void onUserLoggedOut();
    void onLoginFailed(const std::string& error);
    void onRegistrationFailed(const std::string& error);

private:
    void setupConnections();
    void updateUI();
    void showLoginDialog();
    void showGameBoard();
    void showGameHistory();
    void saveGameState();

    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<GameEngine> gameEngine_;
    std::unique_ptr<UserManager> userManager_;
    std::unique_ptr<DatabaseManager> dbManager_;
}; 