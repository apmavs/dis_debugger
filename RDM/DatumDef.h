#ifndef DATUMDEF_H
#define DATUMDEF_H

#include "DatumInfo.h"
#include "DatumIdentifier.h"

#include "KDIS/PDU/Header.h"

#include <QByteArray>
#include <vector>

class DatumDef
{
protected:
    uint32_t length;
    uint32_t offset;
    std::string type;
    std::string unit;
    std::string name;
    std::string category;
    std::string description;
    QByteArray minimum;
    QByteArray maximum;

    void setEntity(KDIS::PDU::Header pdu, DatumIdentifier id);

public:
    void setLength(uint32_t l);
    void setOffset(uint32_t o);
    void setType(std::string t);
    void setUnit(std::string u);
    void setName(std::string n);
    void setCategory(std::string c);
    void setDescription(std::string d);
    void setMin(QByteArray m);
    void setMax(QByteArray m);

    virtual std::vector<DatumInfo> getDatums(KDIS::PDU::Header pdu) = 0;
};

#endif // DATUMDEF_H
