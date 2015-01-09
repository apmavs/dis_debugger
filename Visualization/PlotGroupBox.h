#ifndef PLOTGROUPBOX_H
#define PLOTGROUPBOX_H

#include "DatumInfo.h"

#include <QGroupBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <vector>

class PlotGroupBox : public QGroupBox
{
private:
    QGroupBox* plot_group;
    QLabel* drop_box_label;

    void addCurveToPlots(const DatumInfo* datum);

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);

public:
    PlotGroupBox(QWidget* parent = 0);
    virtual ~PlotGroupBox();

    void setPlotGroup(QGroupBox* g);
    void setDropBoxLabel(QLabel* l);
};

#endif // PLOTGROUPBOX_H
