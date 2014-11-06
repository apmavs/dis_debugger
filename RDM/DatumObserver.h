#ifndef DATUMOBSERVER_H
#define DATUMOBSERVER_H

#include "DatumInfo.h"

class DatumObserver
{
public:
    virtual ~DatumObserver(){}
    virtual void notifyNewDatum(DatumInfo* datum) = 0;
    virtual void notifyNewValue(DatumInfo* datum) = 0;
    virtual void notifyEntityRemoved(std::string entity) = 0;
};

#endif // DATUMOBSERVER_H
