#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGeoPositionInfoSource>
#include <QLCDNumber>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QFont>

#include "fetch.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void positionUpdateConnected();

private slots:
    void initializeUI();
    void startMeasurement();
    void onPositionUpdated(const QGeoPositionInfo &positionInfo);
    void handleLeftButtonClicked();
    void handleRightButtonClicked();
    void handleFairwayButtonClicked();
    void handleCancelButtonClicked();
    /*
    void handleSummary(double avg);
    void handleAvg(double avg);
    void handleMax(double max);
    void handleMin(double min);
*/

private:
    QString username;
    QGeoPositionInfoSource *positionInfoSource;
    bool isMeasuring;
    bool startPointSet;
    QGeoCoordinate startingPoint;
    QGeoCoordinate checkPoint;
    QGeoCoordinate endingPoint;
    QString position;

    QLCDNumber *distanceWidget;
    QPushButton *startButton;
    QPushButton *cancelButton;

    Fetch fetch; // Create an instance of the Fetch class

    double avgDistance;
    double maxDistance;
    double minDistance;

};

#endif // MAINWINDOW_H
