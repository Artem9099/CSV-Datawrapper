#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <cmath>
#include <shlobj.h>
#include "datahandling.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sbResolution_valueChanged(int value);
    void on_sbResolution_actionTriggered(int action);
    void on_sbResolution_sliderReleased();
    void on_sbResolution_sliderMoved(int position);
    void on_nbxResolution_valueChanged(int arg1);
    void on_sbZoom_sliderReleased();
    void on_sbZoom_valueChanged(int value);
    void on_cbxShowNullAxis_stateChanged(int arg1);
    void on_nbxMeasureAxesXVal_valueChanged(double arg1);
    void on_cbxShowMeasureAxesY_stateChanged(int arg1);
    void on_nbxMeasureAxesYVal_valueChanged(double arg1);
    void on_cbxShowMeasureAxesX_stateChanged(int arg1);
    void on_actionClose_triggered();
    void on_actionExportImage_triggered();
    void on_actionOpen_triggered();
    void on_btnboxPreferences_accepted();
    void on_btnboxPreferences_rejected();
    void on_actionGraphik_triggered();
    void on_actionPreferences_triggered();
    void on_nbxZoom_valueChanged(int arg1);
    void on_actionAbout_triggered();
    void showModified(const QString& str)
    {
        Q_UNUSED(str)
        QMessageBox::information(this, "Directory Modified", "Your Directory is modified");
    }

    void on_actionVersion_triggered();

private:
    Ui::MainWindow *ui;
    DataHandling dataHandler;
    void DrawLineChart(int resolution, bool setAnimations);
    QChartView *chartView = new QChartView();
    void PopulateThemeBox();
    void PopulateLegendBox();
    void UpdateUi();
    void LoadConfigs();
    void SetSingleStep();
    void SetDelimiter();
    QString sChartTitle;
    //QFileSystemWatcher watcher;

};
#endif // MAINWINDOW_H
