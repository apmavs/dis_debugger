#include "PduDeconstructor.h"
#include "VehicleMetadataLoader.h"

PduDeconstructor::PduDeconstructor()
{
}

bool PduDeconstructor::loadXml(std::string filename)
{
    bool success = true;

    VehicleMetadataLoader* loader = new VehicleMetadataLoader(filename);

    mutex.lock();
    definitions = loader->getDefinitions();
    mutex.unlock();

    if(loader->errorDetected())
        success = false;
    delete loader;

    return success;
}
