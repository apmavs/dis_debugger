#ifndef PLOTGROUPBOX_H
#define PLOTGROUPBOX_H

#include "DatumInfo.h"
#include "PlotWidget.h"

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <vector>

namespace Ui {
class PlotGroupBox;
}

class PlotGroupBox : public QWidget
{
    Q_OBJECT

private:
    Ui::PlotGroupBox* ui;
    QList<PlotWidget*> created_plots;

private slots:
    void on_DeleteAllBtn_clicked();
    void deletePlot(PlotWidget* plot);

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);

public:
    explicit PlotGroupBox(QWidget* parent = 0);
    virtual ~PlotGroupBox();
    void addPlot(PlotWidget* plot);
    void deleteAllPlots();
    void addToFirstPlot(QList<QTreeWidgetItem*> items);

    bool equivalentTo(PlotGroupBox* rhs);
    virtual QString getStringRepresentation() const;
    static PlotGroupBox* createFromStringRepresentation(QString rep,
                                             QWidget* parent = NULL);
};

#endif // PLOTGROUPBOX_H
