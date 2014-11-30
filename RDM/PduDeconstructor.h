#ifndef PDUDECONSTRUCTOR_H
#define PDUDECONSTRUCTOR_H

#include "PduDef.h"

#include <map>
#include <stdint.h>
#include <string>
#include <QMutex>
#include <KDIS/DataTypes/EntityIdentifier.h>

class PduDeconstructor
{
private:
    QMutex mutex;
    std::map<uint8_t, PduDef*> definitions;
    static std::map<KDIS::DATA_TYPE::EntityIdentifier, std::string> entity_markings;

public:
    PduDeconstructor();
    bool loadXml(std::string filename);
    std::vector<DatumInfo*> deconstruct(KDIS::PDU::Header* pdu);
    static std::string getEntityMarking(uint16_t site, uint16_t app, uint16_t entity);
    static std::string getEntityIdentifier(uint16_t site, uint16_t app, uint16_t entity);
};

#endif // PDUDECONSTRUCTOR_H
