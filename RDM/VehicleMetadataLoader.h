#ifndef VEHICLEMETADATALOADER_H
#define VEHICLEMETADATALOADER_H

#include "DatumDef.h"
#include "PduDef.h"

#include <map>
#include <stdint.h>
#include <QtXml>

class VehicleMetadataLoader
{
private:
    std::string filename;
    std::map<uint8_t, PduDef*> datum_defs;
    QDomDocument xml_doc;
    bool file_error_detected;

    // Handle Datum definitions
    void createDatumDefinitions(QDomElement e);
    PduDef* createEntityStateDefinition(QDomElement e);
    PduDef* createDataDefinition(QDomElement e);
    PduDef* createSetDataDefinition(QDomElement e);
    void populateDatumInfo(QDomElement singleDatumInfo, PduDef* pduDef);
    void addDefToPduDef(PduDef* def, uint8_t pduType);

    // Handle Unit definitions
    // Fill in once UnitDef class is defined

public:
    VehicleMetadataLoader(std::string filename);
    bool errorDetected();
    std::map<uint8_t, PduDef*> getDefinitions();
};

#endif // VEHICLEMETADATALOADER_H
