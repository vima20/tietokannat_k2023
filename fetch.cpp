#include "fetch.h"

Fetch::Fetch(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

Fetch::~Fetch()
{
    networkManager->deleteLater();
}

void Fetch::fetchAvg(const QString &username)
{
    QNetworkRequest request;

    // URL for Android SDK emulator usage
    //QString apiUrl = "http://10.0.2.2:3000/drive/avg/" + username;

    // url for Android mobile usage
    QString apiUrl = "http://192.168.0.30::3000/drive/avg/" + username;

    request.setUrl(QUrl(apiUrl));

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            handleDistanceResponse(responseData, "average");
        } else {
            qDebug() << "Error fetching average distance:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void Fetch::fetchMax(const QString &username)
{
    QNetworkRequest request;

    // URL for Android SDK emulator usage
    //QString apiUrl = "http://10.0.2.2:3000/drive/max/" + username;

    // url for Android mobile usage
    QString apiUrl = "http://192.168.0.30:3000/drive/max/" + username;

    request.setUrl(QUrl(apiUrl));

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            handleDistanceResponse(responseData, "maximum");
        } else {
            qDebug() << "Error fetching max distance:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void Fetch::fetchMin(const QString &username)
{
    QNetworkRequest request;

    // URL for Android SDK emulator usage
    //QString apiUrl = "http://10.0.2.2:3000/drive/min/" + username;

    // url for Android mobile usage
    QString apiUrl = "http://192.168.0.30::3000/drive/min/" + username;

    request.setUrl(QUrl(apiUrl));

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            handleDistanceResponse(responseData, "minimum");
        } else {
            qDebug() << "Error fetching min distance:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void Fetch::handleDistanceResponse(const QByteArray &response, const QString &type)
{

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (!jsonDoc.isArray()) {
        qDebug() << "JSON response is not an array";
        return;
    }

    QJsonArray jsonArray = jsonDoc.array();
    if (jsonArray.isEmpty()) {
        qDebug() << "Empty JSON array in response";
        return;
    }

    QJsonObject jsonObject = jsonArray[0].toObject();

    if (jsonObject.contains(type) && jsonObject[type].isDouble()) {
        double distance = jsonObject[type].toDouble();
        if (type == "average") {
            emit getAvg(distance);
        } else if (type == "maximum") {
            emit getMax(distance);
        } else if (type == "minimum") {
            emit getMin(distance);
        }
    } else {
        qDebug() << "Key '" << type << "' not found in response";
    }
}
