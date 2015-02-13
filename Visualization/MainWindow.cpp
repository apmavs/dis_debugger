#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <QFileDialog>
#include <QMetaType>

static QString REQ_BROADCAST_IP     = "Broadcast IP";
static QString REQ_BROADCAST_PORT   = "Broadcast Port";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    entry_dialog(this),
    last_entry_request("")
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newDatumSlot(const DatumInfo* datum)
{
    QString newEntityName = QString(datum->getEntityName().c_str());
    mutex.lock();
    QList<QListWidgetItem*> knownEntities = ui->EntityView->findItems(newEntityName, 0);
    if(knownEntities.isEmpty())
        ui->EntityView->addItem(newEntityName);
    mutex.unlock();
}

static const std::string REMOVED_STR = " <REMOVED>";
void MainWindow::removeEntitySlot(std::string entity)
{
    mutex.lock();
    for(int row = 0; row < ui->EntityView->count(); row++)
    {
        QListWidgetItem* item = ui->EntityView->item(row);
        if(item->text().toStdString() == entity)
        {
            QString remName(item->text());
            remName.append(REMOVED_STR.c_str());
            item->setText(remName);
            item->setForeground(Qt::red);
            break;
        }
    }
    mutex.unlock();
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

std::string MainWindow::getStrippedName(std::string entityName)
{
    // If removed was appended to name, remove appended string
    if(entityName.length() >= REMOVED_STR.length())
    {
        if(0 == entityName.compare(
                    entityName.length() - REMOVED_STR.length(),
                    REMOVED_STR.length(),
                    REMOVED_STR))
        {
            entityName = entityName.substr(0,
                        entityName.length() - REMOVED_STR.length());
        }
    }

    return entityName;
}

void MainWindow::on_EntityView_currentItemChanged(QListWidgetItem *current)
{
    if(current != NULL)
    {
        std::string entitySelected = current->text().toStdString();
        entitySelected = getStrippedName(entitySelected);

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
