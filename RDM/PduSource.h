#ifndef PDUSOURCE_H
#define PDUSOURCE_H

#include <QThread>
#include <QMutex>

#include "PduObserver.h"

class PduSource : public QThread
{
protected:
    QMutex observer_mutex;
    std::vector<PduObserver*> observers;
    void notifyObservers(KDIS::KOCTET *raw_data, KDIS::KUINT32 size);

public:
    PduSource();

    void registerPduObserver(PduObserver *obs);
    void removePduObserver(PduObserver *obs);
    virtual void run() = 0;
};

#endif // PDUSOURCE_H
