#include "DatumIdentifier.h"

DatumIdentifier::DatumIdentifier()
{
    site   = 0;
    app    = 0;
    entity = 0;
    datum_id = 0;
}

bool DatumIdentifier::operator==(DatumIdentifier rhs)
{
    bool retVal = false;

    if((site     == rhs.site)   &&
       (app      == rhs.app)    &&
       (entity   == rhs.entity) &&
       (datum_id == rhs.datum_id))
    {
        retVal = true;
    }

    return retVal;
}

void DatumIdentifier::setSite(uint16_t s)
{
    site = s;
}

void DatumIdentifier::setApp(uint16_t a)
{
    app = a;
}

void DatumIdentifier::setEntity(uint16_t e)
{
    entity = e;
}

void DatumIdentifier::setDatum(uint32_t d)
{
    datum_id = d;
}
