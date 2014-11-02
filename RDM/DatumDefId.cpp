#include "DatumDefId.h"

DatumDefId::DatumDefId()
{
    pdu_type = 0;
    fixed_type = 0;
    variable_type = 0;
}

void DatumDefId::setPduType(uint8_t t)
{
    pdu_type = t;
}

void DatumDefId::setFixedType(uint32_t t)
{
    fixed_type = t;
}

void DatumDefId::setVariableType(uint32_t t)
{
    variable_type = t;
}

uint8_t  DatumDefId::getPduType()
{
    return pdu_type;
}

uint32_t DatumDefId::getFixedType()
{
    return fixed_type;
}

uint32_t DatumDefId::getVariableType()
{
    return variable_type;
}
