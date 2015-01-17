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

Q_DECLARE_METATYPE(std::string)
class MainWindow : public QMainWindow, public DatumObserver
{
    Q_OBJECT

private slots:
    // These are slots to ensure GUI updates are done in GUI thread
    void newDatumSlot(const DatumInfo* datum);
    void removeEntitySlot(std::string entity);
    void entitiesInvalidSlot();

    // GUI interaction slots
    void on_EntityView_currentItemChanged(QListWidgetItem *current);
    void on_FilterInput_textChanged(const QString &arg1);
    void openXml();

signals:
    void newDatumSignal(const DatumInfo* datum);
    void removeEntitySignal(std::string entity);
    void invalidateAllSignal();

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

private:
    QMutex mutex;
    std::string xml_file;
    Ui::MainWindow *ui;
    DataModelController* controller;
};

#endif // MAINWINDOW_H
