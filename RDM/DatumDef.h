#ifndef DATUMDEF_H
#define DATUMDEF_H

#include "DatumInfo.h"
#include "DatumIdentifier.h"
#include "DatumDefId.h"

#include "KDIS/PDU/Header.h"

#include <QByteArray>
#include <vector>
#include <stdint.h>


class DatumDef
{
protected:
    uint32_t length; // bytes
    uint32_t offset; // bytes
    std::string byte_order;
    std::string type;
    std::string unit;
    std::string unit_class;
    std::string name;
    std::string category;
    std::string description;
    QByteArray minimum;
    QByteArray maximum;

    void initializeMembers();
    void setEntity(KDIS::PDU::Header* pdu, DatumInfo* datum);

public:
    DatumDefId definition_id;
    void setDefinitionId(DatumDefId id);
    void setLength(uint32_t l); // in bytes
    void setOffset(uint32_t o); // in bytes
    void setByteOrder(std::string o);
    void setType(std::string t);
    void setUnit(std::string u);
    void setUnitClass(std::string u);
    void setName(std::string n);
    void setCategory(std::string c);
    void setDescription(std::string d);
    void setMin(QByteArray m);
    void setMax(QByteArray m);

    DatumDefId getDefinitionId();

    virtual std::vector<DatumInfo> getDatums(KDIS::PDU::Header* pdu) = 0;
};

#endif // DATUMDEF_H
