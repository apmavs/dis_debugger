#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataModelController.h"
#include "DatumObserver.h"

#include <QMainWindow>
#include <QMutex>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public DatumObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // DatumObserver inheritance
    virtual void notifyNewDatum(const DatumInfo* datum);
    virtual void notifyNewValue(const DatumInfo* datum);
    virtual void notifyEntityRemoved(std::string entity);
    virtual void notifyAllDatumsInvalid();

protected:
    std::string getStrippedName(std::string entityName);

private slots:
    void on_EntityView_currentItemChanged(QListWidgetItem *current);
    void openXml();

private:
    QMutex mutex;
    std::string xml_file;
    Ui::MainWindow *ui;
    DataModelController* controller;
};

#endif // MAINWINDOW_H
