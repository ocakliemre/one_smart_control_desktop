#include "flowchartwidget.h"

OneSmartControl::FlowChartWidget::FlowChartWidget(EasyProtocol &easyProtocol, OneSmartControl::SmartControlData &smartControlData, QWidget *parent):
    QWidget(parent),
    robot(easyProtocol),
    controlData(smartControlData)
{
    timeWidget = new OneSmartControl::TimeWidget(controlData);

    basicLayout = new QVBoxLayout(this);
    buttonRowLayout = new QHBoxLayout;
    buttonRow = new QWidget;

    controlChartView= new QListView;
    controlDataList = new QStringList;
    controlDataListModel = new QStringListModel;

    loadButton = new QPushButton;
    saveButton = new QPushButton;
    addButton = new QPushButton;
    deleteButton = new QPushButton;
    playButton = new QPushButton;
    upButton = new QPushButton;
    downButton = new QPushButton;

    addDialogLayout = new QVBoxLayout;
    addDialog = new QDialog(this);

    createLayout();
}

void OneSmartControl::FlowChartWidget::createLayout()
{  
    QFont font("Arial Rounded MT Bold", 14);

    controlChartView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    controlChartView->setSelectionMode(QAbstractItemView::SingleSelection);
    controlChartView->setFont(font);
    controlChartView->setStyleSheet("QListView {background-color: rgb(255, 255, 255); border-radius: 10px; border: 2px solid rgb(200, 200, 200); padding: 20px 20px;}"
                                    "QListView::item:selected{background-color: rgb(30, 144, 255);border-radius: 5px; border: 2px solid rgb(200, 200, 200);}"
                                    "QScrollBar:horizontal{height: 10px; background-color: rgb(200, 200, 200); border: 5px transparent; border-radius: 5px;}"
                                    "QScrollBar::handle:horizontal{height: 10px; background: rgb(30, 144, 255); border: 5px rgb(30, 144, 255); border-radius: 5px;}"
                                    "QScrollBar::add-line:horizontal{border: none;background: none;}"
                                    "QScrollBar::sub-line:horizontal{border: none;background: none;}"
                                    "QScrollBar:vertical{width: 10px; background-color: rgb(200, 200, 200); border: 5px transparent; border-radius: 5px;}"
                                    "QScrollBar::handle:vertical{width: 10px; background: rgb(30, 144, 255); border: 5px rgb(30, 144, 255); border-radius: 5px;}"
                                    "QScrollBar::add-line:vertical{border: none;background: none;}"
                                    "QScrollBar::sub-line:vertical{border: none;background: none;}");

    loadButton->setIcon(QIcon(":/images/icons/load.png"));
    loadButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    loadButton->setIconSize(QSize(60, 60));
    connect(loadButton, SIGNAL(released()),this,SLOT(loadButtonHandle()));

    saveButton->setIcon(QIcon(":/images/icons/save.png"));
    saveButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    saveButton->setIconSize(QSize(60, 60));
    connect(saveButton, SIGNAL(released()),this,SLOT(saveButtonHandle()));

    addButton->setIcon(QIcon(":/images/icons/add.png"));
    addButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    addButton->setIconSize(QSize(60, 60));
    connect(addButton, SIGNAL(released()),this,SLOT(addButtonHandle()));

    deleteButton->setIcon(QIcon(":/images/icons/delete.png"));
    deleteButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    deleteButton->setIconSize(QSize(60, 60));
    connect(deleteButton, SIGNAL(released()),this,SLOT(deleteButtonHandle()));

    playButton->setIcon(QIcon(":/images/icons/play.png"));
    playButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    playButton->setIconSize(QSize(60, 60));
    connect(playButton, SIGNAL(released()),this,SLOT(playButtonHandle()));

    upButton->setIcon(QIcon(":/images/icons/up.png"));
    upButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    upButton->setIconSize(QSize(60, 60));
    connect(upButton, SIGNAL(released()),this,SLOT(upButtonHandle()));

    downButton->setIcon(QIcon(":/images/icons/down.png"));
    downButton->setStyleSheet("QPushButton{ background-color: rgb(255, 255, 255); border-radius: 10px; border: 1px solid rgb(255, 255, 255);}");
    downButton->setIconSize(QSize(60, 60));
    connect(downButton, SIGNAL(released()),this,SLOT(downButtonHandle()));

    connect(&robot.connection,SIGNAL(freeChannel()),this,SLOT(freeChannel()));
    connect(&robot.connection,SIGNAL(dataTransmitted()),this,SLOT(dataTransmitted()));

    connect(timeWidget,SIGNAL(toChart()),this,SLOT(addData()));

    buttonRowLayout->addWidget(addButton);
    buttonRowLayout->addWidget(upButton);
    buttonRowLayout->addWidget(downButton);
    buttonRowLayout->addWidget(deleteButton);
    buttonRowLayout->addWidget(playButton);
    buttonRowLayout->addWidget(loadButton);
    buttonRowLayout->addWidget(saveButton);

    buttonRow->setLayout(buttonRowLayout);

    basicLayout->addWidget(controlChartView);
    basicLayout->addWidget(buttonRow);
    basicLayout->setSpacing(28);
    setLayout(basicLayout);
}

void OneSmartControl::FlowChartWidget::addData()
{
    if(controlData.dataid == ID::move && controlData.workingSpaceStatus)
    {
        //Workingspace-Limit
    }
    else
    {
        controlDataStore.append(controlData);
        controlDataList->append(controlData.toString());
        controlDataListModel->setStringList(*controlDataList);
        controlChartView->setModel(controlDataListModel);
    }
}

void OneSmartControl::FlowChartWidget::addButtonHandle()
{
    addDialog->setWindowTitle("Time");
    addDialog->setFixedSize(600,120);
    addDialogLayout->addWidget(timeWidget);
    addDialog->setLayout(addDialogLayout);
    addDialog->setWindowFlags(addDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    addDialog->show();
}

void OneSmartControl::FlowChartWidget::upButtonHandle()
{
    int i = controlChartView->currentIndex().row();
    if(i>0)
    {
        SmartControlData data1 = controlDataStore.at(i);
        QString item1 = controlDataList->at(i);

        SmartControlData data2 = controlDataStore.at(i-1);
        QString item2 = controlDataList->at(i-1);

        controlDataList->replace(i-1,item1);
        controlDataStore.replace(i-1,data1);

        controlDataList->replace(i,item2);
        controlDataStore.replace(i,data2);

        controlDataListModel->setStringList(*controlDataList);
        controlChartView->setModel(controlDataListModel);

        QModelIndex index = controlDataListModel->index(i-1);
        controlChartView->setCurrentIndex(index);
    }
}

void OneSmartControl::FlowChartWidget::downButtonHandle()
{
    int i = controlChartView->currentIndex().row();

    if(i<controlDataStore.size()-1&&i<controlDataList->size()-1)
    {
        SmartControlData data1 = controlDataStore.at(i);
        QString item1 = controlDataList->at(i);

        SmartControlData data2 = controlDataStore.at(i+1);
        QString item2 = controlDataList->at(i+1);

        controlDataList->replace(i+1,item1);
        controlDataStore.replace(i+1,data1);

        controlDataList->replace(i,item2);
        controlDataStore.replace(i,data2);

        controlDataListModel->setStringList(*controlDataList);
        controlChartView->setModel(controlDataListModel);

        QModelIndex index = controlDataListModel->index(i+1);
        controlChartView->setCurrentIndex(index);
    }
}

void OneSmartControl::FlowChartWidget::deleteButtonHandle()
{
    int i = controlChartView->currentIndex().row();
    controlDataList->removeAt(i);
    controlDataStore.removeAt(i);

    controlDataListModel->setStringList(*controlDataList);
    controlChartView->setModel(controlDataListModel);

    if(i>0)
    {
        QModelIndex index = controlDataListModel->index(i-1);
        controlChartView->setCurrentIndex(index);
    }
    else controlChartView->clearSelection();
}

void OneSmartControl::FlowChartWidget::playButtonHandle()
{
    if(robot.connection.isConnected()&&!running&&!controlDataList->empty())
    {
        loadButton->setEnabled(false);
        saveButton->setEnabled(false);
        deleteButton->setEnabled(false);
        upButton->setEnabled(false);
        downButton->setEnabled(false);
        addButton->setEnabled(false);

        emit playlistOn();

        running = true;
        playButton->setIcon(QIcon(":/images/icons/stop.png"));
        activatePlayList();
    }
    else
    {
        loadButton->setEnabled(true);
        saveButton->setEnabled(true);
        deleteButton->setEnabled(true);
        upButton->setEnabled(true);
        downButton->setEnabled(true);

        controlChartView->setSelectionMode(QAbstractItemView::SingleSelection);
        emit playlistOff();

        running = false;
        playButton->setIcon(QIcon(":/images/icons/play.png"));
        robot.connection.stopTransmission();
    }
}

void OneSmartControl::FlowChartWidget::loadButtonHandle()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Save Flowchart"),"",tr("Flowchart (*.fc)"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    controlDataStore.clear();

    QTextStream stream(&file);

    int header = 3;

    while (!stream.atEnd())
    {
        if(header == 0)
        {
            QString line = stream.readLine();
            SmartControlData data;
            data.fromDataString(line);
            controlDataStore.append(data);
        }
        else
        {
            QString line = stream.readLine();
            header--;
        }
    }

    controlDataList->clear();

    for (int i=0; i<controlDataStore.size();i++)
    {
        SmartControlData data = controlDataStore.at(i);
        controlDataList->append(data.toString());
    }

    controlDataListModel->setStringList(*controlDataList);
    controlChartView->setModel(controlDataListModel);
}

void OneSmartControl::FlowChartWidget::saveButtonHandle()
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Flowchart"),"Flowchart_"+timestamp,tr("Flowchart (*.fc)"));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    stream << "Delta-Robot One Smart Control Flowchart" << "\n";

    stream << "Version: " << OneSmartControl::version.toString() << "\n";

    timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    stream << "Timestamp: " << timestamp << "\n";

    for (int i=0; i<controlDataStore.size();i++)
    {
        SmartControlData data = controlDataStore.at(i);
        stream << data.toDataString() << "\n";
    }
}

void OneSmartControl::FlowChartWidget::activatePlayList()
{
    QFont font("Arial Rounded MT Bold", 14);

    selectedRow=0;
    QModelIndex index = controlDataListModel->index(selectedRow);
    controlChartView->setCurrentIndex(index);
    controlChartView->repaint();

    for (int i = 0;i<controlDataStore.size();i++)
    {
        controlData = controlDataStore.at(i);
        if(controlData.dataid == ID::move)
        {
            robot.move.ptp(controlData.xPosition,controlData.yPosition,controlData.zPosition,controlData.velocity);
        }
        if(controlData.dataid == ID::gripper)
        {
            if(controlData.gripperStatus == true) robot.gripper.close();
            else robot.gripper.open();
        }
        if(controlData.dataid == ID::light)
        {
            if(controlData.colour!=Protocol::lightoff)robot.light.setColour(controlData.colour,controlData.intensity);
            else robot.light.off();
        }
        if(controlData.dataid == ID::waitFor)
        {
            robot.functions.waitFor(controlData.waitFortime);
        }
    }
}

void OneSmartControl::FlowChartWidget::freeChannel()
{
    running = false;
    playButton->setIcon(QIcon(":/images/icons/play.png"));

    loadButton->setEnabled(true);
    saveButton->setEnabled(true);
    deleteButton->setEnabled(true);
    upButton->setEnabled(true);
    downButton->setEnabled(true);
    addButton->setEnabled(true);

    controlChartView->setSelectionMode(QAbstractItemView::SingleSelection);

    emit playlistOff();
}

void OneSmartControl::FlowChartWidget::dataTransmitted()
{
    if(running)
    {
        selectedRow++;
        QModelIndex index = controlDataListModel->index(selectedRow);
        controlChartView->setCurrentIndex(index);
        controlChartView->repaint();
    }
}
