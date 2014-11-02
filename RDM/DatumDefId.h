#ifndef DATUMDEFIID_H
#define DATUMDEFIID_H

#include <stdint.h>

// Definition ID to track what pdu a definition is tied to
// Also identifies what variable or fixed datum it ties to
class DatumDefId
{
protected:
    uint8_t pdu_type;
    uint32_t fixed_type;
    uint32_t variable_type;

public:
    DatumDefId();
    void setPduType(uint8_t t);
    void setFixedType(uint32_t t);
    void setVariableType(uint32_t t);
    uint8_t  getPduType();
    uint32_t getFixedType();
    uint32_t getVariableType();
};

#endif // DATUMDEFIID_H
