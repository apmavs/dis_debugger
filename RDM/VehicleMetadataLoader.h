#ifndef VEHICLEMETADATALOADER_H
#define VEHICLEMETADATALOADER_H

#include "DatumDef.h"
#include "PduDef.h"
#include "BaseDef.h"
#include "UnitClassDef.h"

#include <map>
#include <stdint.h>
#include <QtXml>
#include <QString>

typedef std::map<uint8_t, PduDef *> PduDefMap;

class VehicleMetadataLoader
{
private:
    std::string filename;
    QDomDocument xml_doc;
    bool file_error_detected;

    static int lookupPduType(const QString & typeStr);

    // Handle Datum definitions
    void loadUnitClassDefs(QDomNode unitDefsNode, UnitClassDefMap & classMap);
    void createDatumDefinitions(QDomElement e, PduDefMap & defs);
    void populateDatumInfo(QDomElement singleDatumInfo, DatumDef* def);
    void addDefToPduDef(BaseDef* def, uint8_t pduType, PduDefMap & defs);

    // Handle Unit definitions
    // Fill in once UnitDef class is defined

public:
    VehicleMetadataLoader(std::string filename);
    ~VehicleMetadataLoader();
    bool errorDetected();
    void load(PduDefMap & pduDefs, UnitClassDefMap & classDefs);
};

#endif // VEHICLEMETADATALOADER_H
