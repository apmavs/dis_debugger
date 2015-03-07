#include "DatumIdentifier.h"
#include "Configuration.h"

#include <stdlib.h>

DatumIdentifier::DatumIdentifier()
{
    site   = 0;
    app    = 0;
    entity = 0;
}

bool DatumIdentifier::operator==(DatumIdentifier rhs)
{
    bool retVal = false;

    if((site     == rhs.site)   &&
       (app      == rhs.app)    &&
       (entity   == rhs.entity))
    {
        retVal = true;
    }

    return retVal;
}

bool DatumIdentifier::operator!=(DatumIdentifier rhs)
{
    return !(operator==(rhs));
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

uint16_t DatumIdentifier::getSite()
{
    return site;
}

uint16_t DatumIdentifier::getApp()
{
    return app;
}

uint16_t DatumIdentifier::getEntity()
{
    return entity;
}

std::string DatumIdentifier::getStringRepresentation()
{
    std::string rep("<DatumIdentifier>\n");

    char siteStr[100];
    char appStr[100];
    char entityStr[100];
    sprintf(siteStr,   "%d", site);
    sprintf(appStr,    "%d", app);
    sprintf(entityStr, "%d", entity);
    rep += "<SiteID>"        + std::string(siteStr)   + "</SiteID>\n";
    rep += "<ApplicationID>" + std::string(appStr)    + "</ApplicationID>\n";
    rep += "<EntityID>"      + std::string(entityStr) + "</EntityID>\n";
    rep += "</DatumIdentifier>\n";

    return rep;
}

DatumIdentifier DatumIdentifier::fromStringRepresentation(std::string rep)
{
    DatumIdentifier id;

    std::string siteStr   = Configuration::getTagValue(rep, "SiteID");
    std::string appStr    = Configuration::getTagValue(rep, "ApplicationID");
    std::string entityStr = Configuration::getTagValue(rep, "EntityID");

    if(siteStr == "")
        std::cerr << __FILE__ << ": site not found:"   << rep << std::endl;
    if(appStr == "")
        std::cerr << __FILE__ << ": app not found:"    << rep << std::endl;
    if(entityStr == "")
        std::cerr << __FILE__ << ": entity not found:" << rep << std::endl;

    id.site   = atoi(siteStr.c_str());
    id.app    = atoi(appStr.c_str());
    id.entity = atoi(entityStr.c_str());

    return id;
}





