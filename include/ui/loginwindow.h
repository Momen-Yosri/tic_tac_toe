#pragma once

#include <QDialog>
#include <memory>
#include "../auth/user_manager.h"

namespace Ui {
class LoginWindow;
}

namespace tictactoe {

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onUserLoggedIn(const User& user);
    void onLoginFailed(const std::string& error);
    void onRegistrationFailed(const std::string& error);

private:
    void setupConnections();
    void clearFields();
    void showError(const QString& message);
    bool validateInput() const;

    std::unique_ptr<Ui::LoginWindow> ui_;
    std::unique_ptr<UserManager> userManager_;
};
} 