#ifndef DATUMVALUE_H
#define DATUMVALUE_H

#include <QMutex>
#include <QByteArray>
#include <QString>
#include <string>
#include <stdint.h>

class DatumValue
{
protected:
    QMutex *mutex;
    double timestamp;
    QByteArray value;

    DatumValue & operator=(const DatumValue& copyVal);
    DatumValue(const DatumValue& copyVal);

public:
    DatumValue();
    virtual ~DatumValue();
    DatumValue(double t, QByteArray v);
    virtual bool lessThan(DatumValue* rhs) const = 0;
    virtual bool greaterThan(DatumValue* rhs) const = 0;
    static DatumValue* create(std::string v, std::string type);
    static DatumValue* create(QByteArray v, std::string type);
    void setTimestamp(double t);
    void setValue(QByteArray v);
    double getTimestamp() const;
    virtual std::string getValue() const = 0;
    QByteArray getRawData() const;
    virtual DatumValue* createCopy() const = 0;
};

template<class DatumType>
class DatumValueTemplate : public DatumValue
{
private:
    DatumType getVal() const
    {
        DatumType val = 0;
        mutex->lock();
        if(value.size() >= 1)
            val = *((DatumType*)value.constData());
        mutex->unlock();
        return val;
    }

public:
    DatumValueTemplate() : DatumValue() {}
    virtual ~DatumValueTemplate() {}

    virtual std::string getValue() const
    {
        DatumType val = getVal();
        return QString("%1").arg(val).toStdString();
    }

    virtual DatumValue* createCopy() const
    {
        DatumValueTemplate<DatumType>* copy = new DatumValueTemplate<DatumType>();
        mutex->lock();
        copy->value     = value;
        copy->timestamp = timestamp;
        mutex->unlock();
        return copy;
    }

    virtual bool lessThan(DatumValue* rhs) const
    {
        DatumValueTemplate* rhsTemplate = (DatumValueTemplate*)rhs;
        DatumType val1 = getVal();
        DatumType val2 = rhsTemplate->getVal();
        return (val1 < val2);
    }

    virtual bool greaterThan(DatumValue* rhs) const
    {
        DatumValueTemplate* rhsTemplate = (DatumValueTemplate*)rhs;
        DatumType val1 = getVal();
        DatumType val2 = rhsTemplate->getVal();
        return (val1 > val2);
    }
};

class Uint8Value : public DatumValueTemplate<uint8_t>
{
private:
    Uint8Value & operator=(const Uint8Value& copyVal);
    Uint8Value(const Uint8Value& copyVal);

public:
    Uint8Value(){}
    virtual ~Uint8Value(){}
};

class Uint16Value : public DatumValueTemplate<uint16_t>
{
private:
    Uint16Value & operator=(const Uint16Value& copyVal);
    Uint16Value(const Uint16Value& copyVal);

public:
    Uint16Value(){}
    virtual ~Uint16Value(){}
};

class Uint32Value : public DatumValueTemplate<uint32_t>
{
private:
    Uint32Value & operator=(const Uint32Value& copyVal);
    Uint32Value(const Uint32Value& copyVal);

public:
    Uint32Value(){}
    virtual ~Uint32Value(){}
};

class Uint64Value : public DatumValueTemplate<uint64_t>
{
private:
    Uint64Value & operator=(const Uint64Value& copyVal);
    Uint64Value(const Uint64Value& copyVal);

public:
    Uint64Value(){}
    virtual ~Uint64Value(){}
};

class Int8Value : public DatumValueTemplate<int8_t>
{
private:
    Int8Value & operator=(const Int8Value& copyVal);
    Int8Value(const Int8Value& copyVal);

public:
    Int8Value(){}
    virtual ~Int8Value(){}
};

class Int16Value : public DatumValueTemplate<int16_t>
{
private:
    Int16Value & operator=(const Int16Value& copyVal);
    Int16Value(const Int16Value& copyVal);

public:
    Int16Value(){}
    virtual ~Int16Value(){}
};

class Int32Value : public DatumValueTemplate<int32_t>
{
private:
    Int32Value & operator=(const Int32Value& copyVal);
    Int32Value(const Int32Value& copyVal);

public:
    Int32Value(){}
    virtual ~Int32Value(){}
};

class Int64Value : public DatumValueTemplate<int64_t>
{
private:
    Int64Value & operator=(const Int64Value& copyVal);
    Int64Value(const Int64Value& copyVal);

public:
    Int64Value(){}
    virtual ~Int64Value(){}
};

class FloatValue : public DatumValueTemplate<float>
{
private:
    FloatValue & operator=(const FloatValue& copyVal);
    FloatValue(const FloatValue& copyVal);

public:
    FloatValue(){}
    virtual ~FloatValue(){}
};

class DoubleValue : public DatumValueTemplate<double>
{
private:
    DoubleValue & operator=(const DoubleValue& copyVal);
    DoubleValue(const DoubleValue& copyVal);

public:
    DoubleValue(){}
    virtual ~DoubleValue(){}
};

class StringValue : public DatumValue
{
private:
    StringValue & operator=(const StringValue& copyVal);
    StringValue(const StringValue& copyVal);

public:
    StringValue();
    virtual ~StringValue();
    virtual std::string getValue() const;
    virtual DatumValue* createCopy() const;
    virtual bool lessThan(DatumValue* rhs) const;
    virtual bool greaterThan(DatumValue* rhs) const;
};


#endif // DATUMVALUE_H
