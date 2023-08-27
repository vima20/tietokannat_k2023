#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QJsonObject>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class Register : public QWidget
{
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr);
    ~Register();

signals:
    void registrationSuccessful(const QString &username);

private slots:
    void onRegisterClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *pinEdit;
    QLabel *statusLabel;
};

#endif // REGISTER_H
