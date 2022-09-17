#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CSV-Datawrapper (Viewer)");

    PopulateThemeBox();
    PopulateLegendBox();

    iResolution = ui->sbResolution->value();
    ui->nbxResolution->setValue(ui->sbResolution->value());

    LoadConfigs();
    UpdateUi();

    // Open file via doubleclick
    QStringList filenames = QCoreApplication::arguments();
    if (filenames.size() > 0) {
        for (int i=0; i < filenames.size(); i++) {
            sFilePath = filenames.at(i);
            ui->statusbar->showMessage(sFilePath);
        }
        if(sFilePath.right(3).toLower() == "csv") {
            csv.LoadFileContent();
            ui->stackedWidget->setCurrentWidget(ui->pageMain);
            DrawLineChart(ui->sbResolution->value(), false);
        }
        else {
            ui->stackedWidget->setCurrentWidget(ui->pageHome);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawLineChart(int resolution, bool setAnimations) {
    QLineSeries *series = new QLineSeries();
    QLineSeries *nullSeries = new QLineSeries();
    QLineSeries *xAxesSeries = new QLineSeries();
    QLineSeries *yAxesSeries = new QLineSeries();

    series->setName("CAT");
    nullSeries->setName("Nullachse");
    xAxesSeries->setName("Messachse X/T");
    yAxesSeries->setName("Messachse Y");

    csv.SetValueList(resolution);

    unsigned int i = dlTimeList.count();

    series->clear();
    for (unsigned int var = 0; var < i; var++) {
        series->append(dlTimeList[var], dlPressureList[var]);
        //*series << QPointF(dlTimeList[var], dlPressureList[var]);
    }

    // If Y-Axis enable then extension the X-Axis line
    double setYMinVal = dMinVal, setYMaxVal = dMaxVal;
    if(ui->cbxShowMeasureAxesY->isChecked()) {
        if(dMinVal > ui->nbxMeasureAxesYVal->value()) {
            setYMinVal = ui->nbxMeasureAxesYVal->value();
        }
        else {
            setYMinVal = dMinVal;
        }
        if(dMaxVal < ui->nbxMeasureAxesYVal->value()) {
            setYMaxVal = ui->nbxMeasureAxesYVal->value();
        }
        else {
            setYMaxVal = dMaxVal;
        }
    }

    if(i > 1) {
        nullSeries->append(dlTimeList[0], 0);
        nullSeries->append(dlTimeList[i - 1], 0);
        xAxesSeries->append(ui->nbxMeasureAxesXVal->value(), setYMinVal);
        xAxesSeries->append(ui->nbxMeasureAxesXVal->value(), setYMaxVal);
        yAxesSeries->append(dlTimeList[0], ui->nbxMeasureAxesYVal->value());
        yAxesSeries->append(dlTimeList[i - 1], ui->nbxMeasureAxesYVal->value());
    }
    else {
        qDebug() << "Keine Messpunkte gefunden";
    }

    ui->statusbar->showMessage("Messpunkte insgesamt: " + QString::number(iPointsTotal) + "        Messpunkte in der Grafik: " + QString::number(iPointsOut));

    QChart *chart = new QChart();
    if(chart->children().count() > 0) {
        chart->removeAllSeries();
    }

    chart->addSeries(series);
    if(ui->cbxShowNullAxis->isChecked()) {
        chart->addSeries(nullSeries);
    }
    if(ui->cbxShowMeasureAxesX->isChecked()) {
        chart->addSeries(xAxesSeries);
    }
    if(ui->cbxShowMeasureAxesY->isChecked()) {
        chart->addSeries(yAxesSeries);
    }

    chart->createDefaultAxes();
    chart->setTitle("Messungen");



    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    if(setAnimations) {
        chart->setAnimationOptions(QChart::AllAnimations);
    }
    else {
        chart->setAnimationOptions(QChart::NoAnimation);
    }

    chartView->setChart(chart);

    if(ui->cbxAntialiasingOn->isChecked()) {
        chartView->setRenderHint(QPainter::Antialiasing);
    }
    ui->horizontalLayout->addWidget(chartView);
    UpdateUi();
}

void MainWindow::on_sbResolution_valueChanged(int value)
{
    //DrawLineChart(value);
}


void MainWindow::on_sbResolution_actionTriggered(int action)
{
    //DrawLineChart(action);
}


void MainWindow::on_sbResolution_sliderReleased()
{
    if(chartView->chart()->series().count() > 0) {
        DrawLineChart(ui->sbResolution->value(), bAnimationsOn);
    }
}


void MainWindow::on_sbResolution_sliderMoved(int position)
{
    ui->nbxResolution->setValue(position);
    iResolution = position;
}


void MainWindow::on_nbxResolution_valueChanged(int arg1)
{
    ui->sbResolution->setValue(arg1);
    iResolution = arg1;
    if(chartView->chart()->series().count() > 0) {
        //DrawLineChart(arg1, bAnimationsOn);
    }
}

void MainWindow::on_sbZoom_sliderReleased()
{
    int setWidth = ui->pageMain->width() + int(double(ui->pageMain->width()) * double(ui->sbZoom->value()) / 10);

    if(ui->sbZoom->value() <= 1) {
        setWidth = 0;
    }

    chartView->setMinimumWidth(setWidth);
}

void MainWindow::on_sbZoom_valueChanged(int value)
{
    ui->nbxZoom->setValue(value);
}

void MainWindow::on_cbxShowNullAxis_stateChanged(int arg1)
{
    if(chartView->chart()->series().count() > 0) {
        DrawLineChart(ui->sbResolution->value(), false);
    }
}

void MainWindow::on_nbxMeasureAxesXVal_valueChanged(double arg1)
{
    if(chartView->chart()->series().count() > 0) {
        DrawLineChart(ui->sbResolution->value(), false);
    }
}

void MainWindow::on_cbxShowMeasureAxesY_stateChanged(int arg1)
{
    if(chartView->chart()->series().count() > 0) {
        DrawLineChart(ui->sbResolution->value(), false);
    }
}

void MainWindow::on_nbxMeasureAxesYVal_valueChanged(double arg1)
{
    if(chartView->chart()->series().count() > 0) {
        DrawLineChart(ui->sbResolution->value(), false);
    }
}

void MainWindow::on_cbxShowMeasureAxesX_stateChanged(int arg1)
{
    if(chartView->chart()->series().count() > 0) {
        DrawLineChart(ui->sbResolution->value(), false);
    }
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_actionExportImage_triggered()
{
    QStringList imagePathSplit = sFilePath.split('.');
    QString imagePath = imagePathSplit[0] + ".png";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Image speichern"), imagePath, tr("Images (*.png *.xpm *.jpg)"));

    if(ui->cbxUseOriginalSize->isChecked() == false) {
        chartView->setMinimumWidth(ui->nbxResX->value());
        //chartView->setMaximumWidth(ui->nbxResX->value());
        chartView->setMinimumHeight(ui->nbxResY->value());
        //chartView->setMaximumHeight(ui->nbxResY->value());
    }

    if(fileName.right(3).toLower() == "png" || fileName.right(3).toLower() == "xpm" || fileName.right(3).toLower() == "jpg") {
        chartView->grab().save(fileName);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    sFilePath = QFileDialog::getOpenFileName(this, tr("Öffne CSV-Datei"), "", tr("csv Dateien (*.csv)"));

    iPointsTotal = 0;

    if(sFilePath.right(3).toLower() == "csv") {
        csv.LoadFileContent();
        ui->stackedWidget->setCurrentWidget(ui->pageMain);
        DrawLineChart(ui->sbResolution->value(), bAnimationsOn);
    }
}

void MainWindow::on_btnboxPreferences_accepted()
{
    UpdateUi();

    auto path = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path();

    path /= "Roaming\\ibmuhr\\Scope Datawrapper\\";
    std::filesystem::create_directories(path);

    // Create a new file
    QFile file(path += "config.cfg");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    QJsonObject setupObject;

    setupObject.insert("cbxAntialiasingOn", ui->cbxAntialiasingOn->isChecked());
    setupObject.insert("cbxAnimationsOn", ui->cbxAnimationsOn->isChecked());
    setupObject.insert("rbtnIncrementSizeX10", ui->rbtnSingleSteptSizeX10->isChecked());
    setupObject.insert("rbtnIncrementSizeX1", ui->rbtnSingleSteptSizeX1->isChecked());
    setupObject.insert("rbtnIncrementSizeX01", ui->rbtnSingleSteptSizeX01->isChecked());
    setupObject.insert("rbtnIncrementSizeX001", ui->rbtnSingleSteptSizeX001->isChecked());
    setupObject.insert("rbtnIncrementSizeX0001", ui->rbtnSingleSteptSizeX0001->isChecked());
    setupObject.insert("rbtnIncrementSizeY10", ui->rbtnSingleSteptSizeY10->isChecked());
    setupObject.insert("rbtnIncrementSizeY1", ui->rbtnSingleSteptSizeY1->isChecked());
    setupObject.insert("rbtnIncrementSizeY01", ui->rbtnSingleSteptSizeY01->isChecked());
    setupObject.insert("rbtnIncrementSizeY001", ui->rbtnSingleSteptSizeY001->isChecked());
    setupObject.insert("rbtnIncrementSizeY0001", ui->rbtnSingleSteptSizeY0001->isChecked());
    setupObject.insert("cmbLegend", ui->cmbLegend->currentIndex());
    setupObject.insert("cmbWindowStyle", ui->cmbWindowStyle->currentIndex());
    setupObject.insert("nbxResX", ui->nbxResX->value());
    setupObject.insert("nbxResY", ui->nbxResY->value());
    setupObject.insert("cbxUseOriginalSize", ui->cbxUseOriginalSize->isChecked());
    setupObject.insert("cmbDelimiter", ui->cmbDelimiter->currentIndex());

    QJsonDocument doc(setupObject);
    out << doc.toJson();

    file.close();

    SetSingleStep();
    SetDelimiter();

    bAnimationsOn = ui->cbxAnimationsOn->isChecked();
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
    DrawLineChart(ui->sbResolution->value(), bAnimationsOn);
}

void MainWindow::on_btnboxPreferences_rejected()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}

void MainWindow::PopulateThemeBox()
{
    // add items to theme combobox
    ui->cmbWindowStyle->addItem("Light", QChart::ChartThemeLight);
    ui->cmbWindowStyle->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    ui->cmbWindowStyle->addItem("Dark", QChart::ChartThemeDark);
    ui->cmbWindowStyle->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    ui->cmbWindowStyle->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    ui->cmbWindowStyle->addItem("High Contrast", QChart::ChartThemeHighContrast);
    ui->cmbWindowStyle->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
}

void MainWindow::PopulateLegendBox()
{
    // add items to legend combobox
    ui->cmbLegend->addItem("Aus", 0);
    ui->cmbLegend->addItem("Legende Oben", Qt::AlignTop);
    ui->cmbLegend->addItem("Legende Unten", Qt::AlignBottom);
    ui->cmbLegend->addItem("Legende Links", Qt::AlignLeft);
    ui->cmbLegend->addItem("Legende Rechts", Qt::AlignRight);
}

void MainWindow::on_actionGraphik_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMain);
}


void MainWindow::on_actionPreferences_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->page_Preferences);
}

void MainWindow::UpdateUi()
{
    //![6]
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                ui->cmbWindowStyle->itemData(ui->cmbWindowStyle->currentIndex()).toInt());
    //![6]
    //const auto charts = chartView;
    chartView->chart()->setTheme(theme);

        // Set palette colors based on selected theme
        //![8]
        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight) {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
            ui->menubar->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->toolBar->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->pageHome->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFile.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferences.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphik.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImage.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxClose.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAbout.png"));
        //![8]
        } else if (theme == QChart::ChartThemeDark) {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
            ui->menubar->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->toolBar->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->pageHome->setStyleSheet("background-color: rgb(18,18,24); color: rgb(214,214,214)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFileGrey.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferencesGrey.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphikGrey.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImageGrey.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxCloseGrey.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAboutGrey.png"));
        } else if (theme == QChart::ChartThemeBlueCerulean) {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
            ui->menubar->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->toolBar->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->pageHome->setStyleSheet("background-color: rgb(64,67,74); color: rgb(214,214,214)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFileGrey.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferencesGrey.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphikGrey.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImageGrey.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxCloseGrey.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAboutGrey.png"));
        } else if (theme == QChart::ChartThemeBrownSand) {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
            ui->menubar->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->toolBar->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->pageHome->setStyleSheet("background-color: rgb(158,137,101); color: rgb(24,24,24)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFile.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferences.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphik.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImage.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxClose.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAbout.png"));
        } else if (theme == QChart::ChartThemeBlueNcs) {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
            ui->menubar->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->toolBar->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->pageHome->setStyleSheet("background-color: rgb(1,139,186); color: rgb(24,24,24)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFile.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferences.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphik.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImage.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxClose.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAbout.png"));
        } else if (theme == QChart::ChartThemeHighContrast) {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
            ui->menubar->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->toolBar->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->pageHome->setStyleSheet("background-color: rgb(255,171,3); color: rgb(24,24,24)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFile.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferences.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphik.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImage.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxClose.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAbout.png"));
        } else if (theme == QChart::ChartThemeBlueIcy) {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
            ui->menubar->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->toolBar->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->pageHome->setStyleSheet("background-color: rgb(206,231,240); color: rgb(24,24,24)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFile.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferences.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphik.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImage.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxClose.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAbout.png"));
        } else {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
            ui->menubar->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->toolBar->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpChartConfigs->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpSingleStepX->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpSingleStepY->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpWindowThemes->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpImageExportSettings->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->grpCsvSettings->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->pageHome->setStyleSheet("background-color: rgb(240,240,240); color: rgb(24,24,24)");
            ui->actionOpen->setIcon(QIcon(":/icons/icons/toolboxOpenFile.png"));
            ui->actionPreferences->setIcon(QIcon(":/icons/icons/toolboxPreferences.png"));
            ui->actionGraphik->setIcon(QIcon(":/icons/icons/toolboxGraphik.png"));
            ui->actionExportImage->setIcon(QIcon(":/icons/icons/toolboxExportImage.png"));
            ui->actionClose->setIcon(QIcon(":/icons/icons/toolboxClose.png"));
            ui->actionAbout->setIcon(QIcon(":/icons/icons/toolboxAbout.png"));
        }
        window()->setPalette(pal);

//    // Update antialiasing
//    //![11]
//    bool checked = ui->cbxAntialiasingOn->isChecked();
//        chart->setRenderHint(QPainter::Antialiasing, checked);
//    //![11]

//    // Update animation options
//    //![9]
//    QChart::AnimationOptions options(
//                m_ui->animatedComboBox->itemData(m_ui->animatedComboBox->currentIndex()).toInt());
//    if (!m_charts.isEmpty() && m_charts.at(0)->chart()->animationOptions() != options) {
//        for (QChartView *chartView : charts)
//            chartView->chart()->setAnimationOptions(options);
//    }
//    //![9]

    // Update legend alignment
    //![10]
    Qt::Alignment alignment(
                ui->cmbLegend->itemData(ui->cmbLegend->currentIndex()).toInt());

    if (!alignment) {
        chartView->chart()->legend()->hide();
    }
    else {
        chartView->chart()->legend()->setAlignment(alignment);
        chartView->chart()->legend()->show();
    }
//![10]
}


void MainWindow::LoadConfigs() {
    auto path = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path();

    path /= "Roaming\\ibmuhr\\Scope Datawrapper\\";

    if (!std::filesystem::exists(path))
        std::filesystem::create_directories(path);

//    qDebug() << path;

    QString val;
    QFile file;
    file.setFileName(path += "config.cfg");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject obj = doc.object();

    // Main parameter
    QJsonValue mainVal = obj.value(QString("cbxAnimationsOn"));
    ui->cbxAnimationsOn->setChecked(mainVal.toBool());
    bAnimationsOn = ui->cbxAnimationsOn->isChecked();
    mainVal = obj.value(QString("cbxAntialiasingOn"));
    ui->cbxAntialiasingOn->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeX10"));
    ui->rbtnSingleSteptSizeX10->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeX1"));
    ui->rbtnSingleSteptSizeX1->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeX01"));
    ui->rbtnSingleSteptSizeX01->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeX001"));
    ui->rbtnSingleSteptSizeX001->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeX0001"));
    ui->rbtnSingleSteptSizeX0001->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeY10"));
    ui->rbtnSingleSteptSizeY10->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeY1"));
    ui->rbtnSingleSteptSizeY1->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeY01"));
    ui->rbtnSingleSteptSizeY01->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeY001"));
    ui->rbtnSingleSteptSizeY001->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("rbtnIncrementSizeY0001"));
    ui->rbtnSingleSteptSizeY0001->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("cmbLegend"));
    ui->cmbLegend->setCurrentIndex(mainVal.toInt());
    mainVal = obj.value(QString("cmbWindowStyle"));
    ui->cmbWindowStyle->setCurrentIndex(mainVal.toInt());
    mainVal = obj.value(QString("nbxResX"));
    ui->nbxResX->setValue(mainVal.toInt());
    mainVal = obj.value(QString("nbxResY"));
    ui->nbxResY->setValue(mainVal.toInt());
    mainVal = obj.value(QString("cbxUseOriginalSize"));
    ui->cbxUseOriginalSize->setChecked(mainVal.toBool());
    mainVal = obj.value(QString("nbxResY"));
    ui->cmbDelimiter->setCurrentIndex(mainVal.toInt());
    mainVal = obj.value(QString("cmbDelimiter"));
    ui->cmbDelimiter->setCurrentIndex(mainVal.toInt());

    SetSingleStep();
    SetDelimiter();
}

void MainWindow::on_nbxZoom_valueChanged(int arg1)
{
    ui->sbZoom->setValue(arg1);
    on_sbZoom_sliderReleased();
}


void MainWindow::on_actionAbout_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
}

void MainWindow::SetSingleStep() {
    if(ui->rbtnSingleSteptSizeX10->isChecked()) {
        ui->nbxMeasureAxesXVal->setSingleStep(10);
    }
    else if(ui->rbtnSingleSteptSizeX1->isChecked()) {
        ui->nbxMeasureAxesXVal->setSingleStep(1);
    }
    else if(ui->rbtnSingleSteptSizeX01->isChecked()) {
        ui->nbxMeasureAxesXVal->setSingleStep(0.1);
    }
    else if(ui->rbtnSingleSteptSizeX001->isChecked()) {
        ui->nbxMeasureAxesXVal->setSingleStep(0.01);
    }
    else {
        ui->nbxMeasureAxesXVal->setSingleStep(0.001);
    }
    if(ui->rbtnSingleSteptSizeY10->isChecked()) {
        ui->nbxMeasureAxesYVal->setSingleStep(10);
    }
    else if(ui->rbtnSingleSteptSizeY1->isChecked()) {
        ui->nbxMeasureAxesYVal->setSingleStep(1);
    }
    else if(ui->rbtnSingleSteptSizeY01->isChecked()) {
        ui->nbxMeasureAxesYVal->setSingleStep(0.1);
    }
    else if(ui->rbtnSingleSteptSizeY001->isChecked()) {
        ui->nbxMeasureAxesYVal->setSingleStep(0.01);
    }
    else {
        ui->nbxMeasureAxesYVal->setSingleStep(0.001);
    }
}

void MainWindow::SetDelimiter() {
    switch (ui->cmbDelimiter->currentIndex()) {
    case 0:
        cDelimiter = ';';
        break;
    case 1:
        cDelimiter = '\t';
        break;
    case 2:
        cDelimiter = ',';
        break;
    default:
        cDelimiter = ';';
        break;
    }
}
