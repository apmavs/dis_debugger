#include "gtest/gtest.h"
#include "DatumDefTest.cpp"
#include "NetworkPduSourceTest.cpp"
#include "DataModelControllerTest.cpp"
#include "DatumIdentifierTest.cpp"
#include "DatumInfoTest.cpp"
#include "DatumValueTest.cpp"
#include "EntityDatumItemTest.cpp"
#include "WatchDatumItemTest.cpp"
#include "PlotCurveItemTest.cpp"
#include "EntityDataListTest.cpp"
#include "PlotWidgetTest.cpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
