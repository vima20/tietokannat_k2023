// mainwindow.cpp

#include "mainwindow.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnetworkrequest.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QFont>

MainWindow::MainWindow(const QString &username, QWidget *parent) : QMainWindow(parent), username(username), isMeasuring(false), startPointSet(false)
{
    positionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
    if (positionInfoSource)
    {
        // Set the preferred positioning methods before starting updates
        positionInfoSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);

        positionInfoSource->setUpdateInterval(1000); // Set the update interval to 1000 milliseconds (1 update per 1 seconds)
    }
    else
    {
        qCritical() << "Position information source not available.";
    }

    initializeUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeUI()
{

    qDebug() << "New measurement!";

    setWindowTitle("Driville");
    setMinimumSize(0, 0);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(20);

    QLabel *headerLabel = new QLabel("Drive", centralWidget);
    headerLabel->setFont(QFont("Arial", 26, QFont::Bold));
    layout->addWidget(headerLabel);

    // Create the distance widget (QLCDNumber)
    distanceWidget = new QLCDNumber(this);
    layout->addWidget(distanceWidget);

    startButton = new QPushButton("Start Measurement", centralWidget);
    layout->addWidget(startButton);

    QHBoxLayout *positionButtonsLayout = new QHBoxLayout;
    QPushButton *leftButton = new QPushButton("Left");
    QPushButton *fairwayButton = new QPushButton("Fairway");
    QPushButton *rightButton = new QPushButton("Right");
    positionButtonsLayout->addWidget(leftButton);
    positionButtonsLayout->addWidget(fairwayButton);
    positionButtonsLayout->addWidget(rightButton);
    layout->addLayout(positionButtonsLayout);

    // Create the cancel button
    cancelButton = new QPushButton("Cancel", centralWidget);
    layout->addWidget(cancelButton);

    // Create the exit button
    QPushButton *exitButton = new QPushButton("Exit", centralWidget);
    layout->addWidget(exitButton);

    // Connect signals and slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startMeasurement);
    connect(leftButton, &QPushButton::clicked, this, &MainWindow::handleLeftButtonClicked);
    connect(rightButton, &QPushButton::clicked, this, &MainWindow::handleRightButtonClicked);
    connect(fairwayButton, &QPushButton::clicked, this, &MainWindow::handleFairwayButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::handleCancelButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(positionInfoSource, &QGeoPositionInfoSource::positionUpdated, this, &MainWindow::onPositionUpdated);

    // Connect Fetch signals and get avg, max and min values per username
    connect(&fetch, &Fetch::getAvg, this, [&](double avg) {
        avgDistance = avg;
    });

    connect(&fetch, &Fetch::getMax, this, [&](double max) {
        maxDistance = max;
    });

    connect(&fetch, &Fetch::getMin, this, [&](double min) {
        minDistance = min;
    });

    // Fetch and display avg, max and min distance for the current user
    fetch.fetchAvg(username);
    fetch.fetchMax(username);
    fetch.fetchMin(username);

}

void MainWindow::startMeasurement()
{
    // Check if positionInfoSource is valid
    if (!positionInfoSource)
    {
        qCritical() << "Position information source not available.";
        return;
    }

    // If the signal is connected, the measurement can start
    qDebug() << "Position update signal is connected!";

    // Emit the positionUpdateConnected signal
    emit positionUpdateConnected();

    // Set the flag to indicate that we are starting a new measurement
    isMeasuring = true;
    startPointSet = false;

    // Start the position updates
    positionInfoSource->startUpdates();
}

void MainWindow::onPositionUpdated(const QGeoPositionInfo &positionInfo)
{

    if (positionInfo.isValid())
    {
        QGeoCoordinate currentPosition = positionInfo.coordinate();
        qreal latitude = currentPosition.latitude();
        qreal longitude = currentPosition.longitude();

        // Check if the coordinates are valid
        if (qIsFinite(latitude) && qIsFinite(longitude))
        {
            // Update UI or perform actions based on the new position
            qDebug() << "New position: Latitude=" << latitude << ", Longitude=" << longitude;

            // Check if the measurement has started and the starting point has not been set
            if (isMeasuring && !startPointSet)
            {
                // Set the starting point to the current position
                startingPoint = currentPosition;
                qDebug() << "Starting point: " << startingPoint;

                // Set the flag to indicate that the starting point has been set
                startPointSet = true;
            }
            else if (isMeasuring && startPointSet)
            {
                // Set the ending point to the current position
                checkPoint = currentPosition;
                qDebug() << "Check point: " << checkPoint;

                // Calculate the distance between the starting and check points
                double distance = startingPoint.distanceTo(checkPoint);
                qDebug() << "Updated distance: " << distance;

                // Update the distance value in the distance widget (QLCDNumber)
                distanceWidget->display(QString::number(distance, 'f', 1)); // Display distance with two decimal places

                // Display the drive distance and position in a message box
                qInfo() << "Current Distance: " << distance << " meters";
            }
            else if (!isMeasuring && startPointSet)
            {
                // Stop the measurement
                positionInfoSource->stopUpdates();

                // Show a message or perform any necessary cleanup
                qInfo() << "The measurement has been stopped.";

                // Set the ending point to the current position
                endingPoint = currentPosition;
                qDebug() << "Ending point: " << endingPoint;

                // Calculate the distance between the starting and ending points
                double distance = startingPoint.distanceTo(endingPoint);
                qDebug() << "Distance: " << distance << "position: " << position;

                // Update the distance value in the distance widget (QLCDNumber)
                distanceWidget->display(QString::number(distance, 'f', 1)); // Display distance with one decimal

                // Display the drive distance, position, avg, max, and min in a message box
                QString message = QString("Drive Distance: %1 meters\nPosition: %2\nAverage: %3 meters\nMaximum: %4 meters\nMinimum: %5 meters")
                                      .arg(distance)
                                      .arg(position)
                                      .arg(avgDistance)
                                      .arg(maxDistance)
                                      .arg(minDistance);

                QMessageBox::information(this, "Drive Summary", message);

                // Add REST API to store data to the database
                QNetworkAccessManager *manager = new QNetworkAccessManager(this);

                QJsonObject driveData;

                driveData["username"] = username;
                driveData["time"] = QDateTime::currentDateTime().toString(Qt::ISODate);
                driveData["distance"] = distance;
                driveData["position"] = position;
                driveData["gpsStartLAT"] = startingPoint.latitude();
                driveData["gpsStartLNG"] = startingPoint.longitude();
                driveData["gpsEndLAT"] = currentPosition.latitude();
                driveData["gpsEndLNG"] = currentPosition.longitude();

                QJsonDocument driveDocument(driveData);
                QByteArray driveJson = driveDocument.toJson();

                qDebug() << "driveJson: " + driveJson;

                // URL for Android SDK emulator usage
                //QNetworkRequest request(QUrl("http://10.0.2.2:3000/drive"));

                // URL for Android mobile usage
                QNetworkRequest request(QUrl("http://https://192.168.0.30://drive"));

                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QNetworkReply *reply = manager->post(request, driveJson);
                connect(reply, &QNetworkReply::finished, this, [reply]() {
                    if (reply->error() == QNetworkReply::NoError) {
                        qDebug() << "Drive data posted successfully";
                    } else {
                        qDebug() << "Error posting drive data:" << reply->errorString();
                    }
                    reply->deleteLater();
                });

                // Reset UI and the flags for the next measurement

                isMeasuring = false;
                startPointSet = false;

                initializeUI();

            }
        }
        else
        {
            // Invalid coordinates; handle the error
            qWarning() << "Invalid GPS coordinates: Latitude=" << latitude << ", Longitude=" << longitude;
        }
    }
    else
    {
        // Position info is not valid; handle the error or absence of GPS data
        qWarning() << "Invalid position data or GPS not available.";
    }
}

void MainWindow::handleLeftButtonClicked()
{
    position = "Left";
    isMeasuring = false;
}

void MainWindow::handleRightButtonClicked()
{
    position = "Right";
    isMeasuring = false;
}

void MainWindow::handleFairwayButtonClicked()
{
    position = "Fairway";
    isMeasuring = false;
}

void MainWindow::handleCancelButtonClicked()
{
    // Stop the measurement and perform cleanup
    if (positionInfoSource)
    {
        positionInfoSource->stopUpdates();
    }

    // Show a message to the user
    QMessageBox::information(this, "Measurement Canceled", "The measurement has been canceled.");

    // Perform any other necessary cleanup or UI updates
    initializeUI();
}

