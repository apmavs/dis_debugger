#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <QFileDialog>
#include <QMetaType>
#include <QMessageBox>

static QString REQ_BROADCAST_IP     = "Broadcast IP";
static QString REQ_BROADCAST_PORT   = "Broadcast Port";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    entry_dialog(this),
    last_entry_request(""),
    layout_file_name("")
{
    ui->setupUi(this);
    showMaximized();

    // Set up default ratios of main sections
    ui->MainSplitter->setStretchFactor(0, 1);
    ui->MainSplitter->setStretchFactor(1, 2);
    ui->MainSplitter->setStretchFactor(2, 1);

    controller = DataModelController::getInstance();
    controller->registerObserver(this);

    qRegisterMetaType<std::string>();
    QObject::connect(this, SIGNAL(newDatumSignal(const DatumInfo*)), this,
                     SLOT(newDatumSlot(const DatumInfo*)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(removeEntitySignal(std::string)), this,
                     SLOT(removeEntitySlot(std::string)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(invalidateAllSignal()), this,
                     SLOT(entitiesInvalidSlot()), Qt::QueuedConnection);
    QObject::connect(&entry_dialog, SIGNAL(confirmedEdit(QString)), this,
                     SLOT(handleDialogEntry(QString)), Qt::QueuedConnection);

    ui->actionOpen_XML->setShortcut           (Qt::CTRL + Qt::Key_O);
    ui->actionSave_Layout->setShortcut        (Qt::CTRL + Qt::Key_S);
    ui->actionSave_Layout_As->setShortcut     (Qt::CTRL + Qt::Key_A);
    ui->actionLoad_Layout->setShortcut        (Qt::CTRL + Qt::Key_L);
    ui->actionExit->setShortcut               (Qt::CTRL + Qt::Key_Q);
    ui->actionChangeBroadcastIP->setShortcut  (Qt::CTRL + Qt::Key_I);
    ui->actionChangeBroadcastPort->setShortcut(Qt::CTRL + Qt::Key_P);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newDatumSlot(const DatumInfo* datum)
{
    QString newEntityName = QString(datum->getEntityName().c_str());
    mutex.lock();
    bool foundEntity = false;
    int disIdPos = newEntityName.lastIndexOf("(");
    QString newDisId = newEntityName.mid(disIdPos);
    for(int idx = 0; idx < ui->EntityView->count(); idx++)
    {
        QListWidgetItem* item = ui->EntityView->item(idx);
        QString name = item->text();

        // Check if name has changed or name is already in the list
        if(name.endsWith(newDisId))
        {
            item->setText(newEntityName);
            foundEntity = true;
            break;
        }
    }

    if(!foundEntity)
        ui->EntityView->addItem(newEntityName);
    mutex.unlock();
}

void MainWindow::removeEntitySlot(std::string)
{
    // Do not remove, still want to see old data from it
}

void MainWindow::entitiesInvalidSlot()
{
    ui->EntityView->clear();
    ui->WatchView->clear();
    ui->PlotsGroup->deleteAllPlots();
}

void MainWindow::notifyNewDatum(const DatumInfo* datum)
{
    emit newDatumSignal(datum);
}

void MainWindow::notifyNewValue(const DatumInfo* datum)
{
    std::cerr << "MainWindow should not be subscribed to datum values:" <<
                 datum->getEntityName() << std::endl;
}

void MainWindow::notifyEntityRemoved(std::string entity)
{
    emit removeEntitySignal(entity);
}

void MainWindow::notifyAllDatumsInvalid()
{
    emit invalidateAllSignal();
}

void MainWindow::on_EntityView_currentItemChanged(QListWidgetItem *current)
{
    if(current != NULL)
    {
        std::string entitySelected = current->text().toStdString();
        ui->AttributeView->setActiveEntity(entitySelected);
    }
}

void MainWindow::on_FilterInput_textChanged(const QString &arg1)
{
    ui->AttributeView->filterList(arg1);
}

void MainWindow::openXml()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open XML file"), "");
    if(xml_file != file.toStdString())
    {
        controller->removeAllDatums();
        xml_file = file.toStdString();
        controller->loadMetadataXml(xml_file);
    }
}

void MainWindow::changeBroadcastIp()
{
    last_entry_request = REQ_BROADCAST_IP;
    QString req(controller->getBroadcastIp().c_str());
    entry_dialog.popup(last_entry_request, req);
}

void MainWindow::changeBroadcastPort()
{
    last_entry_request = REQ_BROADCAST_PORT;
    QString req = QString::number(controller->getBroadcastPort());
    entry_dialog.popup(last_entry_request, req);
}

void MainWindow::handleDialogEntry(QString value)
{
    if(last_entry_request == REQ_BROADCAST_IP)
        controller->changeBroadcastIp(value.toStdString());
    else if(last_entry_request == REQ_BROADCAST_PORT)
        controller->changeBroadcastPort(value.toUInt());
    else
        std::cerr << "Received edit for unknown request:" << value.toStdString() << std::endl;
}

void MainWindow::saveLayout()
{
    if(layout_file_name != "")
    {
        if(!layout_file_name.endsWith(".lay"))
            layout_file_name += ".lay";

        QString layout = "<MainLayout>\n";
        layout += ui->AttributeView->getStringRepresentation();
        layout += ui->WatchView->getStringRepresentation();
        layout += ui->PlotsGroup->getStringRepresentation();
        layout += "</MainLayout>\n";

        QFile writeFile(layout_file_name);
        bool opened = writeFile.open(QIODevice::WriteOnly);
        if(opened)
        {
            QTextStream stream(&writeFile);
            stream << layout;
            writeFile.close();
        }
        else
        {
            std::string msg = "Error: Unable to open ";
            msg += layout_file_name.toStdString();
            msg += " for writing!";
            QMessageBox::information(this, tr("DIS Debugger"), tr(msg.c_str()));
        }
    }
    else
    {
        // User has not selected a layout to save under yet
        // Call saveLayoutAs instead to prompt user
        // (this will call back to this fxn once a layout has been selected
        saveLayoutAs();
    }
}

void MainWindow::saveLayoutAs()
{
    // Get layout filename from user, store into member variable
    // Then call saveLayout to save it.
    layout_file_name = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "./" + layout_file_name,
                                                    tr("Layout files (*.lay)"));
    if(layout_file_name != "") // User didn't click cancel
        saveLayout();
}

void MainWindow::loadLayout()
{
    layout_file_name = QFileDialog::getOpenFileName(this, tr("Load File"),
                                                    "./" + layout_file_name,
                                                    tr("Layout files (*.lay)"));
    if(layout_file_name != "")
    {
        QFile readFile(layout_file_name);
        bool opened = readFile.open(QIODevice::ReadOnly);
        if(opened)
        {
            QTextStream stream(&readFile);
            QString layout = stream.readAll();
            QString guts = QString(Configuration::getTagValue(layout.toStdString(),
                                                              "MainLayout").c_str());
            ui->AttributeView->setFromStringRepresentation(guts);
            ui->FilterInput->setText(ui->AttributeView->getActiveFilter());
            ui->WatchView->setFromStringRepresentation(guts);
            ui->PlotsGroup->setFromStringRepresentation(guts);
        }
        else
        {
            std::string msg = "Error: Unable to open ";
            msg += layout_file_name.toStdString();
            msg += " for reading!";
            QMessageBox::information(this, tr("DIS Debugger"), tr(msg.c_str()));
        }
    }
}



