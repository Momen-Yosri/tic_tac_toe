#include "ui/loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

namespace tictactoe {

LoginWindow::LoginWindow(QWidget* parent)
    : QDialog(parent)
    , ui_(std::make_unique<Ui::LoginWindow>())
    , userManager_(std::make_unique<UserManager>())
{
    ui_->setupUi(this);
    setupConnections();
}

LoginWindow::~LoginWindow() = default;

void LoginWindow::setupConnections()
{
    connect(ui_->loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginButtonClicked);
    connect(ui_->registerButton, &QPushButton::clicked,
            this, &LoginWindow::onRegisterButtonClicked);
    connect(userManager_.get(), &UserManager::userLoggedIn,
            this, &LoginWindow::onUserLoggedIn);
    connect(userManager_.get(), &UserManager::loginFailed,
            this, &LoginWindow::onLoginFailed);
    connect(userManager_.get(), &UserManager::registrationFailed,
            this, &LoginWindow::onRegistrationFailed);
}

void LoginWindow::clearFields()
{
    ui_->usernameEdit->clear();
    ui_->passwordEdit->clear();
}

void LoginWindow::showError(const QString& message)
{
    QMessageBox::warning(this, "Error", message);
}

bool LoginWindow::validateInput() const
{
    if (ui_->usernameEdit->text().isEmpty()) {
        showError("Username cannot be empty");
        return false;
    }
    if (ui_->passwordEdit->text().isEmpty()) {
        showError("Password cannot be empty");
        return false;
    }
    return true;
}

void LoginWindow::onLoginButtonClicked()
{
    if (!validateInput()) {
        return;
    }

    std::string username = ui_->usernameEdit->text().toStdString();
    std::string password = ui_->passwordEdit->text().toStdString();

    if (userManager_->loginUser(username, password)) {
        accept();
    }
}

void LoginWindow::onRegisterButtonClicked()
{
    if (!validateInput()) {
        return;
    }

    std::string username = ui_->usernameEdit->text().toStdString();
    std::string password = ui_->passwordEdit->text().toStdString();

    if (userManager_->registerUser(username, password)) {
        accept();
    }
}

void LoginWindow::onUserLoggedIn(const User& user)
{
    clearFields();
}

void LoginWindow::onLoginFailed(const std::string& error)
{
    showError(QString::fromStdString(error));
}

void LoginWindow::onRegistrationFailed(const std::string& error)
{
    showError(QString::fromStdString(error));
}

} // namespace tictactoe 