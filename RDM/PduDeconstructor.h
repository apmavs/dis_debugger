#ifndef PDUDECONSTRUCTOR_H
#define PDUDECONSTRUCTOR_H

#include "PduDef.h"

#include <map>
#include <stdint.h>
#include <string>
#include <QMutex>

class PduDeconstructor
{
private:
    QMutex mutex;
    std::map<uint8_t, PduDef*> definitions;

public:
    PduDeconstructor();
    bool loadXml(std::string filename);
};

#endif // PDUDECONSTRUCTOR_H
