#ifndef VEHICLEMETADATALOADER_H
#define VEHICLEMETADATALOADER_H

#include "DatumDef.h"
#include "PduDef.h"
#include "BaseDef.h"

#include <map>
#include <stdint.h>
#include <QtXml>

class VehicleMetadataLoader
{
private:
    std::string filename;
    QDomDocument xml_doc;
    bool file_error_detected;

    // Handle Datum definitions
    void createDatumDefinitions(QDomElement e,
                        std::map<uint8_t, PduDef*>* defs);
    void populateDatumInfo(QDomElement singleDatumInfo, DatumDef* def);
    void addDefToPduDef(BaseDef* def, uint8_t pduType,
                        std::map<uint8_t, PduDef*>* defs);

    // Handle Unit definitions
    // Fill in once UnitDef class is defined

public:
    VehicleMetadataLoader(std::string filename);
    ~VehicleMetadataLoader();
    bool errorDetected();
    std::map<uint8_t, PduDef*> getDefinitions();
};

#endif // VEHICLEMETADATALOADER_H
