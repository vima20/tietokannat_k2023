#include "register.h"
#include "register.h"
#include <QJsonDocument> // Include the required header file


Register::Register(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *usernameLabel = new QLabel("Username:");
    usernameEdit = new QLineEdit(this);
    QLabel *pinLabel = new QLabel("PIN:");
    pinEdit = new QLineEdit(this);
    pinEdit->setEchoMode(QLineEdit::Password);

    QPushButton *registerButton = new QPushButton("Register", this);
    connect(registerButton, &QPushButton::clicked, this, &Register::onRegisterClicked);

    statusLabel = new QLabel(this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(pinLabel);
    layout->addWidget(pinEdit);
    layout->addWidget(registerButton);
    layout->addWidget(statusLabel);
}

Register::~Register() {}

void Register::onRegisterClicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Registration successful
            emit registrationSuccessful(usernameEdit->text());
        } else {
            // Registration failed
            statusLabel->setText("Registration failed: " + reply->errorString());
        }
        reply->deleteLater();
    });

    QUrl url("http://10.0.2.2:3000/user");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = usernameEdit->text();
    json["pin"] = pinEdit->text();

    manager->post(request, QJsonDocument(json).toJson());
}
