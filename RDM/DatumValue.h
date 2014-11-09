#ifndef DATUMVALUE_H
#define DATUMVALUE_H

#include <QMutex>
#include <QByteArray>
#include <string>

class DatumValue
{
protected:
    QMutex *mutex;
    double timestamp;
    QByteArray value;

public:
    DatumValue();
    virtual ~DatumValue();
    DatumValue(double t, QByteArray v);
    DatumValue & operator=(const DatumValue& copyVal);
    DatumValue(const DatumValue& copyVal);
    void setTimestamp(double t);
    void setValue(QByteArray v);
    double getTimestamp() const;
    virtual std::string getValue() const = 0;
    QByteArray getRawData() const;
};

class Uint8Value : public DatumValue
{
public:
    Uint8Value();
    virtual ~Uint8Value();
    virtual std::string getValue() const;
};

class Uint16Value : public DatumValue
{
public:
    Uint16Value();
    virtual ~Uint16Value();
    virtual std::string getValue() const;
};

class Uint32Value : public DatumValue
{
public:
    Uint32Value();
    virtual ~Uint32Value();
    virtual std::string getValue() const;
};

class Uint64Value : public DatumValue
{
public:
    Uint64Value();
    virtual ~Uint64Value();
    virtual std::string getValue() const;
};

class Int8Value : public DatumValue
{
public:
    Int8Value();
    virtual ~Int8Value();
    virtual std::string getValue() const;
};

class Int16Value : public DatumValue
{
public:
    Int16Value();
    virtual ~Int16Value();
    virtual std::string getValue() const;
};

class Int32Value : public DatumValue
{
public:
    Int32Value();
    virtual ~Int32Value();
    virtual std::string getValue() const;
};

class Int64Value : public DatumValue
{
public:
    Int64Value();
    virtual ~Int64Value();
    virtual std::string getValue() const;
};

class FloatValue : public DatumValue
{
public:
    FloatValue();
    virtual ~FloatValue();
    virtual std::string getValue() const;
};

class DoubleValue : public DatumValue
{
public:
    DoubleValue();
    virtual ~DoubleValue();
    virtual std::string getValue() const;
};

class StringValue : public DatumValue
{
public:
    StringValue();
    virtual ~StringValue();
    virtual std::string getValue() const;
};


#endif // DATUMVALUE_H
