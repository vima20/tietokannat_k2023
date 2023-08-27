#include "login.h"
#include <QApplication>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Login::Login(QWidget *parent) : QWidget(parent)
{
    initUI();
}

Login::~Login() // Destructor implementation
{
    // Clean up any resources or memory if needed
    // For example, delete any dynamically allocated objects
}

void Login::initUI()
{
    mainLayout = new QVBoxLayout(this);
    formLayout = new QFormLayout();

    QLabel *usernameLabel = new QLabel("Username:");
    usernameEdit = new QLineEdit(this);
    QLabel *pinLabel = new QLabel("PIN:");
    pinEdit = new QLineEdit(this);
    pinEdit->setEchoMode(QLineEdit::Password);

    QPushButton *loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &Login::onLoginClicked);

    statusLabel = new QLabel(this);

    formLayout->addRow(usernameLabel, usernameEdit);
    formLayout->addRow(pinLabel, pinEdit);

    tryAgainButton = new QPushButton("Try Again", this);
    connect(tryAgainButton, &QPushButton::clicked, this, &Login::onTryAgainClicked);
    tryAgainButton->setVisible(false);

    exitButton = new QPushButton("Exit", this);
    connect(exitButton, &QPushButton::clicked, this, &Login::onExitClicked);

    formLayout->addWidget(loginButton);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(tryAgainButton);
    mainLayout->addWidget(exitButton);

    setLayout(mainLayout);
}

void Login::setUsername(const QString &username)
{
    usernameEdit->setText(username);
}

void Login::onLoginClicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, &Login::handleNetworkReply);

    // URL for Android SDK emulator usage
    //QUrl url("http://10.0.2.2:3000/login");

    // url for Android mobile usage
    QUrl url("http://192.168.43.22:3000/login");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = usernameEdit->text();
    json["pin"] = pinEdit->text();

    QJsonDocument jsonDoc(json);
    manager->post(request, jsonDoc.toJson());
}

void Login::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        handleLoginResponse(reply->readAll());
    } else {
        handleNetworkError(reply->errorString());
    }

    reply->deleteLater();
}

void Login::handleLoginResponse(const QByteArray &response)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(response).object();

    if (jsonObj.contains("login") && jsonObj["login"].isBool() && jsonObj["login"].toBool()) {
        setStatusLabel("Login successful");

        // Emit the loginSuccessful signal with username
        emit loginSuccessful(jsonObj, usernameEdit->text());
    } else {
        qDebug() << "Login failed: Invalid username or PIN";
        setStatusLabel("Login failed: Invalid username or PIN.");
        setTryAgainButtonVisible(true);
    }
}

void Login::handleNetworkError(const QString &errorString)
{
    qDebug() << "Login failed: " << errorString;
    setStatusLabel("Login failed: " + errorString);
    setTryAgainButtonVisible(true);
}

void Login::setStatusLabel(const QString &message)
{
    statusLabel->setText(message);
}

void Login::setTryAgainButtonVisible(bool visible)
{
    tryAgainButton->setVisible(visible);
}

void Login::onTryAgainClicked()
{
    tryAgainButton->setVisible(false);
    statusLabel->clear();
}

void Login::onExitClicked()
{
    qApp->exit();
}
