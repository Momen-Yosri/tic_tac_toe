#include "auth/user_manager.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDateTime>

namespace tictactoe {

UserManager::UserManager(QObject* parent)
    : QObject(parent)
    , isLoggedIn_(false)
{
}

bool UserManager::registerUser(const std::string& username, const std::string& password)
{
    if (username.empty() || password.empty()) {
        emit registrationFailed("Username and password cannot be empty");
        return false;
    }

    std::string salt = generateSalt();
    std::string hashedPassword = hashPassword(password, salt);

    User newUser;
    newUser.username = username;
    newUser.passwordHash = hashedPassword;
    newUser.salt = salt;
    newUser.createdAt = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();

    // TODO: Save user to database
    // For now, just set as current user
    currentUser_ = newUser;
    isLoggedIn_ = true;
    emit userLoggedIn(currentUser_);
    return true;
}

bool UserManager::loginUser(const std::string& username, const std::string& password)
{
    if (username.empty() || password.empty()) {
        emit loginFailed("Username and password cannot be empty");
        return false;
    }

    // TODO: Get user from database
    // For now, just check if it's the current user
    if (currentUser_.username == username) {
        std::string hashedPassword = hashPassword(password, currentUser_.salt);
        if (hashedPassword == currentUser_.passwordHash) {
            isLoggedIn_ = true;
            emit userLoggedIn(currentUser_);
            return true;
        }
    }

    emit loginFailed("Invalid username or password");
    return false;
}

void UserManager::logoutUser()
{
    isLoggedIn_ = false;
    emit userLoggedOut();
}

bool UserManager::isUserLoggedIn() const
{
    return isLoggedIn_;
}

const User& UserManager::getCurrentUser() const
{
    return currentUser_;
}

bool UserManager::changePassword(const std::string& currentPassword, const std::string& newPassword)
{
    if (!isLoggedIn_) {
        return false;
    }

    std::string currentHash = hashPassword(currentPassword, currentUser_.salt);
    if (currentHash != currentUser_.passwordHash) {
        return false;
    }

    std::string newSalt = generateSalt();
    std::string newHash = hashPassword(newPassword, newSalt);

    currentUser_.passwordHash = newHash;
    currentUser_.salt = newSalt;

    // TODO: Update user in database
    return true;
}

std::string UserManager::generateSalt() const
{
    const int saltLength = 16;
    QByteArray salt;
    salt.resize(saltLength);
    for (int i = 0; i < saltLength; ++i) {
        salt[i] = QRandomGenerator::global()->bounded(256);
    }
    return salt.toBase64().toStdString();
}

std::string UserManager::hashPassword(const std::string& password, const std::string& salt) const
{
    QByteArray data;
    data.append(QString::fromStdString(password));
    data.append(QString::fromStdString(salt));
    
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return hash.toBase64().toStdString();
}

bool UserManager::validatePassword(const std::string& password, const std::string& hash, const std::string& salt) const
{
    std::string computedHash = hashPassword(password, salt);
    return computedHash == hash;
}

} // namespace tictactoe 