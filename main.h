#ifndef MAIN_H
#define MAIN_H

#include <QApplication>
#include <QObject>
#include <QJsonObject>
#include <QString>
#include "login.h"
//#include "register.h"
#include "mainwindow.h"

class MyApp : public QObject
{
    Q_OBJECT

public:
    MyApp();
    ~MyApp();
    Login* getLoginScreen() const;

    QString getUsername() const { return username; }


signals:
    void loginSuccessful(const QJsonObject &jsonObj, const QString &username);

private slots:
    void onLoginSuccessful(const QJsonObject &jsonObj, const QString &username);

    //    void onRegistrationSuccessful(const QString &username);
    void showLoginScreen();
//    void showRegisterScreen();

private:
    void initializeUI();

    Login *loginScreen;
//    Register *registerScreen;
    MainWindow *mainWindow;
    QString username;

};

#endif // MAIN_H
