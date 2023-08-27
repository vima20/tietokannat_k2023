#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccessful(const QJsonObject &jsonObj, const QString &username);

private slots:
    void onLoginClicked();
    void handleNetworkReply(QNetworkReply *reply);
    void handleLoginResponse(const QByteArray &response);
    void handleNetworkError(const QString &errorString);
    void setStatusLabel(const QString &message);
    void setTryAgainButtonVisible(bool visible);
    void onTryAgainClicked();
    void onExitClicked();

private:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *statusLabel;
    QPushButton *tryAgainButton;
    QLineEdit *usernameEdit;
    QLineEdit *pinEdit;
    QPushButton *exitButton;

    void initUI();
    void setUsername(const QString &username);
};

#endif // LOGIN_H
