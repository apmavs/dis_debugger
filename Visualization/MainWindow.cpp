#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up default ratios of main sections
    ui->MainSplitter->setStretchFactor(0, 1);
    ui->MainSplitter->setStretchFactor(1, 2);
    ui->MainSplitter->setStretchFactor(2, 3);

    // Set up default ratios of Entity group
    ui->EntitySplitter->setStretchFactor(0, 1);
    ui->EntitySplitter->setStretchFactor(1, 4);

    controller = DataModelController::getInstance();
    controller->registerObserver(this);
    controller->loadMetadataXml("C:\\Comp\\school_work\\CSE\\dis_debugger\\dis_definitions.xml");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::notifyNewDatum(const DatumInfo* datum)
{
    QString newEntityName = QString(datum->getEntityName().c_str());
    mutex.lock();
    QList<QListWidgetItem*> knownEntities = ui->EntityView->findItems(newEntityName, 0);
    if(knownEntities.isEmpty())
        ui->EntityView->addItem(newEntityName);
    mutex.unlock();
}

void MainWindow::notifyNewValue(const DatumInfo* datum)
{
    std::cerr << "MainWindow should not be subscribed to datum values:" <<
                 datum->getEntityName() << std::endl;
}

static const std::string REMOVED_STR = " <REMOVED>";
void MainWindow::notifyEntityRemoved(std::string entity)
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

void MainWindow::on_EntityView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    std::string entitySelected = current->text().toStdString();
    entitySelected = getStrippedName(entitySelected);

    ui->AttributeView->setActiveEntity(entitySelected);
}
