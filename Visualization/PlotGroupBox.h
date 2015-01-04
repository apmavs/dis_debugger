#ifndef PLOTGROUPBOX_H
#define PLOTGROUPBOX_H

#include "DatumInfo.h"

#include <QGroupBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <vector>

class PlotGroupBox : public QGroupBox
{
private:
    void addCurveToPlots(const DatumInfo* datum);

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);

public:
    PlotGroupBox(QWidget* parent = 0);
    virtual ~PlotGroupBox();
};

#endif // PLOTGROUPBOX_H
