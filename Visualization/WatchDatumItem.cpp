#include "WatchDatumItem.h"

WatchDatumItem::WatchDatumItem(EntityDatumItem* parent, QString categoryName)
    : EntityDatumItem(parent, categoryName)
{
}

WatchDatumItem::WatchDatumItem(EntityDatumItem* parent, const DatumInfo* d)
    : EntityDatumItem(parent, d)
{
}

WatchDatumItem* WatchDatumItem::createWatchItem(EntityDatumItem* fromThis)
{
    WatchDatumItem* newItem;
    const DatumInfo* datum = fromThis->getWatchedDatum();
    if(datum != NULL)
    {
        newItem = new WatchDatumItem(NULL, datum);
        newItem->setToolTip(0, QString(datum->getDescription().c_str()));
        newItem->setToolTip(1, QString(datum->getDescription().c_str()));
        newItem->setToolTip(2, QString(datum->getDescription().c_str()));
    }
    else
        newItem = new WatchDatumItem(NULL, fromThis->getCategoryName());

    // Create copies of all children
    for(int kidIdx = 0; kidIdx < fromThis->childCount(); kidIdx++)
    {
        EntityDatumItem* kid = (EntityDatumItem*)(fromThis->child(kidIdx));
        WatchDatumItem* newKid = createWatchItem(kid);
        newItem->addChild(newKid);
    }

    return newItem;
}


WatchDatumItem::~WatchDatumItem()
{
}

void WatchDatumItem::setDisplay()
{
    mutex.lock();
    if(watched_datum == NULL)
    {   // category item
        setText(0, category_name);
    }
    else
    {
        QString entityName(watched_datum->getEntityName().c_str());
        setText(0, entityName);
        setText(1, watched_datum->getName().c_str());
        std::string curVal = watched_datum->getValue() + " ";
        curVal += watched_datum->getUnit();
        setText(2, curVal.c_str());
    }
    setColor(2);
    mutex.unlock();
}

QString WatchDatumItem::getStringRepresentation() const
{
    QString rep("<WatchDatumItem>\n");
    if(watched_datum != NULL)
    {
        rep += "<WatchedDatum>" +
                QString(watched_datum->getStringRepresentation().c_str()) +
               "</WatchedDatum>\n";
    }
    else
        rep += "<Category>" + category_name + "</Category>\n";
    for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
    {
        WatchDatumItem* kid = (WatchDatumItem*)(child(kidIdx));
        rep += "<Child>" + QString(kid->getStringRepresentation()) +
                "</Child>\n";
    }

    rep += "</WatchDatumItem>\n";

    return rep;
}

WatchDatumItem* WatchDatumItem::createFromStringRepresentation(QString rep,
                                                     WatchDatumItem* parent)
{
    WatchDatumItem* ret;
    QString guts = QString(Configuration::getTagValue(rep.toStdString(),
                                             "WatchDatumItem").c_str());

    // Truncate children data to make sure we don't grab their stuff
    QString myData = guts;
    int child = myData.indexOf("<Child>");
    if(child >= 0)
        myData.truncate(child);

    DatumInfo* datum = DataModelController::getInstance()->
                                    getDatumInfoPtr(myData.toStdString());
    if(datum != NULL)
        ret = new WatchDatumItem(parent, datum);
    else
    {
        QString cat = QString(Configuration::getTagValue(myData.toStdString(),
                                                         "Category").c_str());
        ret = new WatchDatumItem(parent, cat);
    }

    // Parse children
    QString endTag = "</Child>";
    QString childData = QString(Configuration::getTagValue(guts.toStdString(),
                                                           "Child").c_str());
    while(childData != "")
    {
        WatchDatumItem* childItem = WatchDatumItem::
                createFromStringRepresentation(childData, ret);
        if(childItem == NULL)
            std::cerr << "this should never happen" << std::endl;

        int endTagPos = guts.indexOf(endTag);
        if(endTagPos >= 0)
        {
            guts = guts.remove(0, endTagPos + endTag.length());
            childData = QString(Configuration::getTagValue(guts.toStdString(),
                                                           "Child").c_str());
        }
        else
        {
            std::cerr << __FILE__ << ": Broken XML:" + rep.toStdString() << std::endl;
            break;
        }
    }

    return ret;
}
