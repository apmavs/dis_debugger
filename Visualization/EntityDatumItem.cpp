#include "EntityDatumItem.h"

EntityDatumItem::EntityDatumItem(EntityDatumItem* parent, QString categoryName)
    : QTreeWidgetItem(parent, QStringList(categoryName)),
      DatumItem(categoryName)
{
    clearDisplay();
}

EntityDatumItem::EntityDatumItem(EntityDatumItem* parent, const DatumInfo* d)
    : QTreeWidgetItem(parent), DatumItem(d)
{
    clearDisplay();
    setToolTip(0, QString(d->getDescription().c_str()));
    setToolTip(1, QString(d->getDescription().c_str()));
}

EntityDatumItem::~EntityDatumItem()
{
}

QString EntityDatumItem::getName()
{
    QString retVal = category_name;
    if(watched_datum != NULL)
        retVal = QString(watched_datum->getName().c_str());
    return retVal;
}

void EntityDatumItem::setDisplay()
{
    mutex.lock();
    if(watched_datum == NULL)
    {   // category item
        setText(0, category_name);
        setTextColor(1, Qt::black);
    }
    else
    {
        setText(0, watched_datum->getName().c_str());
        std::string curVal = watched_datum->getValue() + " ";
        curVal += watched_datum->getUnit();
        setText(1, curVal.c_str());
    }
    setColor(1);
    mutex.unlock();
}

void EntityDatumItem::clearDisplay()
{
    mutex.lock();
    if(watched_datum != NULL)
        setText(0, "COLLAPSED");
    mutex.unlock();
}

void EntityDatumItem::setColor(int column)
{
    if(watched_datum != NULL)
    {
        if(watched_datum->isGreaterThanMax())
            setTextColor(column, Qt::red);
        else if(watched_datum->isLessThanMin())
            setTextColor(column, Qt::blue);
        else
            setTextColor(column, Qt::black);
    }
    else
        setTextColor(column, Qt::black);
}

void EntityDatumItem::activate(const void *widgetPtr)
{
    mutex.lock();
    if(interested_widgets.count(widgetPtr) == 0)
    {
        mutex.unlock(); // Next line re-locks mutex
        DatumItem::activate(widgetPtr);
        mutex.lock();

        // If we have childeren, activate them too
        for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
        {
            EntityDatumItem* kid = (EntityDatumItem*)(child(kidIdx));
            kid->activate(widgetPtr);
        }
    }
    mutex.unlock();
}

void EntityDatumItem::deactivate(const void* widgetPtr)
{
    mutex.lock();
    if(interested_widgets.count(widgetPtr) != 0)
    {
        mutex.unlock(); // Next line re-locks mutex
        DatumItem::deactivate(widgetPtr);
        mutex.lock();

        // If we have childeren, deactivate them too
        for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
        {
            EntityDatumItem* kid = (EntityDatumItem*)(child(kidIdx));
            kid->deactivate(widgetPtr);
        }
    }
    mutex.unlock();
}

bool EntityDatumItem::equivalentTo(const EntityDatumItem* rhs) const
{
    bool ret = DatumItem::equivalentTo(rhs);

    int myNumChildren = childCount();
    int rhNumChildren = rhs->childCount();
    if(myNumChildren == rhNumChildren)
    {
        for(int i = 0; i < myNumChildren; i++)
        {
            EntityDatumItem* myChild = (EntityDatumItem*)(child(i));
            EntityDatumItem* rhChild = (EntityDatumItem*)(rhs->child(i));
            if(!myChild->equivalentTo(rhChild))
                ret = false;
        }
    }
    else
        ret = false;

    return ret;
}

QString EntityDatumItem::getStringRepresentation() const
{
    QString rep("<EntityDatumItem>\n");
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
        EntityDatumItem* kid = (EntityDatumItem*)(child(kidIdx));
        rep += "<Child>" + QString(kid->getStringRepresentation()) +
                "</Child>\n";
    }

    rep += "</EntityDatumItem>\n";

    return rep;
}

EntityDatumItem* EntityDatumItem::createFromStringRepresentation(QString rep,
                                                     EntityDatumItem* parent)
{
    EntityDatumItem* ret;
    QString guts = QString(Configuration::getTagValue(rep.toStdString(),
                                             "EntityDatumItem").c_str());

    // Truncate children data to make sure we don't grab their stuff
    QString myData = guts;
    int child = myData.indexOf("<Child>");
    if(child >= 0)
        myData.truncate(child);

    DatumInfo* datum = DataModelController::getInstance()->
                                    getDatumInfoPtr(myData.toStdString());
    if(datum != NULL)
        ret = new EntityDatumItem(parent, datum);
    else
    {
        QString cat = QString(Configuration::getTagValue(myData.toStdString(),
                                                         "Category").c_str());
        ret = new EntityDatumItem(parent, cat);
    }

    // Parse children
    QString endTag = "</Child>";
    QString childData = QString(Configuration::getTagValue(guts.toStdString(),
                                                           "Child").c_str());
    while(childData != "")
    {
        EntityDatumItem* childItem = EntityDatumItem::
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
