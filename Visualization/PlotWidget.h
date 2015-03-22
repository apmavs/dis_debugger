#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "PlotCurveItem.h"
#include "EntityDatumItem.h"

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <QRectF>
#include <QList>
#include <QTreeWidgetItem>
#include <QColor>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::PlotWidget* ui;
    QwtPlotZoomer* zoomer;
    QList<PlotCurveItem*> curves;
    unsigned int color_index;
    static const QColor COLOR_WHEEL[];
    QColor getNextColor();
    bool delete_always_hidden;
    bool hidden;
    QSize min_plot_size;
    int max_height;
    QSize normal_size;
    QSize min_size;
    QString focused_style;
    QString unfocused_style;

protected:
    void paintEvent(QPaintEvent*);

private slots:
    void zoomChanged(const QRectF&);
    void focusChange(QWidget* oldWidget, QWidget* newWidget);
    void on_ClearBtn_clicked();
    void on_DeleteBtn_clicked();
    void on_HideBtn_clicked();
    void on_FullHistoryBtn_clicked();
    void on_ResetBtn_clicked();

signals:
    void deletePlot(PlotWidget* plot);

public:
    explicit PlotWidget(QWidget* parent = 0);
    ~PlotWidget();

    void hideDelete();
    void addCurve(EntityDatumItem* item);
    void addCurves(QList<QTreeWidgetItem*> items);
    void clearCurves();

    bool equivalentTo(PlotWidget* rhs);
    virtual QString getStringRepresentation() const;
    static PlotWidget* createFromStringRepresentation(QString rep,
                                           QWidget* parent = NULL);
};

#endif // PLOTWIDGET_H
