#ifndef DATUMVALUE_H
#define DATUMVALUE_H

#include <QMutex>
#include <QByteArray>

class DatumValue
{
private:
    QMutex *mutex;
    double timestamp;
    QByteArray value;

public:
    DatumValue();
    DatumValue(double t, QByteArray v);
    ~DatumValue();
    DatumValue & operator=(const DatumValue& copyVal);
    DatumValue(const DatumValue& copyVal);
    void setTimestamp(double t);
    void setValue(QByteArray v);
    double getTimestamp() const;
    QByteArray getValue() const;
};

#endif // DATUMVALUE_H
