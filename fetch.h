#ifndef FETCH_H
#define FETCH_H

#include <QObject>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>

class Fetch : public QObject
{
    Q_OBJECT

public:
    explicit Fetch(QObject *parent = nullptr);
    ~Fetch();

    void fetchAvg(const QString &username);
    void fetchMax(const QString &username);
    void fetchMin(const QString &username);

signals:
    void getAvg(double avg);
    void getMax(double max);
    void getMin(double min);


private:
    QNetworkAccessManager *networkManager;

    void handleDistanceResponse(const QByteArray &response, const QString &type);
};

#endif // FETCH_H
