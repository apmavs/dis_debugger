#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataModelController.h"
#include "DatumObserver.h"
#include "TextEntryPopup.h"

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
private:
    QMutex mutex;
    std::string xml_file;
    Ui::MainWindow *ui;
    DataModelController* controller;
    TextEntryPopup entry_dialog;
    QString last_entry_request;
    QString layout_file_name;

private slots:
    // These are slots to ensure GUI updates are done in GUI thread
    void newDatumSlot(const DatumInfo* datum);
    void removeEntitySlot(std::string);
    void entitiesInvalidSlot();

    // GUI interaction slots
    void on_EntityView_currentItemChanged(QListWidgetItem *current);
    void on_FilterInput_textChanged(const QString &arg1);
    void openXml();
    void changeBroadcastIp();
    void changeBroadcastPort();
    void handleDialogEntry(QString value);
    void saveLayout();
    void saveLayoutAs();
    void loadLayout();

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
};

#endif // MAINWINDOW_H
