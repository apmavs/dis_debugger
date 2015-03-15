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
    virtual std::string getType() const = 0;
    virtual std::string getValue() const = 0;
    QByteArray getRawData() const;
    virtual DatumValue* createCopy() const = 0;

    std::string getStringRepresentation() const;
    static DatumValue* createFromStringRepresentation(std::string rep);
};

template<class DatumType>
class DatumValueTemplate : public DatumValue
{
private:
    DatumValueTemplate & operator=(const DatumValueTemplate& copyVal);
    DatumValueTemplate(const DatumValueTemplate& copyVal);

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

    virtual std::string getType() const;

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

template <> inline std::string DatumValueTemplate<uint8_t> ::getType() const { return "uint8";  }
template <> inline std::string DatumValueTemplate<uint16_t>::getType() const { return "uint16"; }
template <> inline std::string DatumValueTemplate<uint32_t>::getType() const { return "uint32"; }
template <> inline std::string DatumValueTemplate<uint64_t>::getType() const { return "uint64"; }
template <> inline std::string DatumValueTemplate<int8_t>  ::getType() const { return "int8";   }
template <> inline std::string DatumValueTemplate<int16_t> ::getType() const { return "int16";  }
template <> inline std::string DatumValueTemplate<int32_t> ::getType() const { return "int32";  }
template <> inline std::string DatumValueTemplate<int64_t> ::getType() const { return "int64";  }
template <> inline std::string DatumValueTemplate<float>   ::getType() const { return "float";  }
template <> inline std::string DatumValueTemplate<double>  ::getType() const { return "double"; }

typedef DatumValueTemplate<uint8_t>  Uint8Value;
typedef DatumValueTemplate<uint16_t> Uint16Value;
typedef DatumValueTemplate<uint32_t> Uint32Value;
typedef DatumValueTemplate<uint64_t> Uint64Value;
typedef DatumValueTemplate<int8_t>   Int8Value;
typedef DatumValueTemplate<int16_t>  Int16Value;
typedef DatumValueTemplate<int32_t>  Int32Value;
typedef DatumValueTemplate<int64_t>  Int64Value;
typedef DatumValueTemplate<float>    FloatValue;
typedef DatumValueTemplate<double>   DoubleValue;

class StringValue : public DatumValue
{
private:
    StringValue & operator=(const StringValue& copyVal);
    StringValue(const StringValue& copyVal);

public:
    StringValue();
    virtual ~StringValue();
    virtual std::string getValue() const;
    virtual std::string getType() const;
    virtual DatumValue* createCopy() const;
    virtual bool lessThan(DatumValue* rhs) const;
    virtual bool greaterThan(DatumValue* rhs) const;
};


#endif // DATUMVALUE_H
