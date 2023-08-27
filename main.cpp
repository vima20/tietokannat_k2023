#include "main.h"

MyApp::MyApp()
{
    initializeUI();

    // Connect signals and slots
    QObject::connect(loginScreen, &Login::loginSuccessful, this, &MyApp::onLoginSuccessful);
    //QObject::connect(registerScreen, &Register::registrationSuccessful, this, &MyApp::onRegistrationSuccessful);

}

MyApp::~MyApp()
{
    delete loginScreen;
    //delete registerScreen;
    delete mainWindow;
}

void MyApp::initializeUI()
{
    loginScreen = new Login;
    //registerScreen = new Register;
    //mainWindow = new MainWindow();

    loginScreen->show();
}

/*
void Login::onRegisterClicked()
{
    // Handle the registration process here
    // You can show the registration screen or perform other actions as needed
    // For example:
    registerScreen->show();
}
*/

void MyApp::onLoginSuccessful(const QJsonObject &, const QString &username)
{
    mainWindow = new MainWindow(username);
    mainWindow->show();
}

/*
void MyApp::onRegistrationSuccessful(const QString &username)
{
    loggedInUsername = username;
    loginScreen->setUsername(loggedInUsername);
    loginScreen->show();
}
*/

void MyApp::showLoginScreen()
{
    loginScreen->show();
}

/*
void MyApp::showRegisterScreen()
{
    registerScreen->show();
}
*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyApp myApp;

    return app.exec();
}
