#pragma once

#include <QWidget>
#include <memory>
#include <QPushButton>
#include "../game/gameengine.h"

namespace Ui {
class GameBoard;
}

namespace tictactoe {

class GameBoard : public QWidget {
    Q_OBJECT

public:
    explicit GameBoard(QWidget* parent = nullptr);
    ~GameBoard();

    void setGameEngine(GameEngine* engine);
    void resetBoard();
    void updateBoard();

private slots:
    void onCellClicked();
    void onGameStateChanged(GameState newState);
    void onCurrentPlayerChanged(Player newPlayer);
    void onBoardChanged();

private:
    void setupBoard();
    void setupConnections();
    void updateCellStyle(QPushButton* button, Player player);
    void disableBoard();
    void enableBoard();
    QString getPlayerSymbol(Player player) const;

    std::unique_ptr<Ui::GameBoard> ui_;
    GameEngine* gameEngine_;
    QPushButton* cells_[3][3];
}; 