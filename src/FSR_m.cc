//
// Generated file, do not edit! Created by opp_msgtool 6.1 from inet/routing/fsr/FSR.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "FSR_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

Register_Enum(inet::FSRControlPacketType, (inet::FSRControlPacketType::UPDATE));

Register_Class(FSRControlPacket)

FSRControlPacket::FSRControlPacket() : ::inet::FieldsChunk()
{
}

FSRControlPacket::FSRControlPacket(const FSRControlPacket& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

FSRControlPacket::~FSRControlPacket()
{
}

FSRControlPacket& FSRControlPacket::operator=(const FSRControlPacket& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void FSRControlPacket::copy(const FSRControlPacket& other)
{
    this->packetType = other.packetType;
}

void FSRControlPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->packetType);
}

void FSRControlPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->packetType);
}

FSRControlPacketType FSRControlPacket::getPacketType() const
{
    return this->packetType;
}

void FSRControlPacket::setPacketType(FSRControlPacketType packetType)
{
    handleChange();
    this->packetType = packetType;
}

class FSRControlPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_packetType,
    };
  public:
    FSRControlPacketDescriptor();
    virtual ~FSRControlPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(FSRControlPacketDescriptor)

FSRControlPacketDescriptor::FSRControlPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::FSRControlPacket)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

FSRControlPacketDescriptor::~FSRControlPacketDescriptor()
{
    delete[] propertyNames;
}

bool FSRControlPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FSRControlPacket *>(obj)!=nullptr;
}

const char **FSRControlPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *FSRControlPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int FSRControlPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int FSRControlPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_packetType
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *FSRControlPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "packetType",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int FSRControlPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "packetType") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *FSRControlPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::FSRControlPacketType",    // FIELD_packetType
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **FSRControlPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_packetType: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *FSRControlPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_packetType:
            if (!strcmp(propertyName, "enum")) return "inet::FSRControlPacketType";
            return nullptr;
        default: return nullptr;
    }
}

int FSRControlPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void FSRControlPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'FSRControlPacket'", field);
    }
}

const char *FSRControlPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FSRControlPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        case FIELD_packetType: return enum2string(pp->getPacketType(), "inet::FSRControlPacketType");
        default: return "";
    }
}

void FSRControlPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        case FIELD_packetType: pp->setPacketType((inet::FSRControlPacketType)string2enum(value, "inet::FSRControlPacketType")); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRControlPacket'", field);
    }
}

omnetpp::cValue FSRControlPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        case FIELD_packetType: return static_cast<int>(pp->getPacketType());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'FSRControlPacket' as cValue -- field index out of range?", field);
    }
}

void FSRControlPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        case FIELD_packetType: pp->setPacketType(static_cast<inet::FSRControlPacketType>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRControlPacket'", field);
    }
}

const char *FSRControlPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr FSRControlPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void FSRControlPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRControlPacket *pp = omnetpp::fromAnyPtr<FSRControlPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRControlPacket'", field);
    }
}

FSRRoute::FSRRoute()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const FSRRoute& a)
{
    doParsimPacking(b,a.source);
    doParsimPacking(b,a.target);
}

void __doUnpacking(omnetpp::cCommBuffer *b, FSRRoute& a)
{
    doParsimUnpacking(b,a.source);
    doParsimUnpacking(b,a.target);
}

class FSRRouteDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_source,
        FIELD_target,
    };
  public:
    FSRRouteDescriptor();
    virtual ~FSRRouteDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(FSRRouteDescriptor)

FSRRouteDescriptor::FSRRouteDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::FSRRoute)), "")
{
    propertyNames = nullptr;
}

FSRRouteDescriptor::~FSRRouteDescriptor()
{
    delete[] propertyNames;
}

bool FSRRouteDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FSRRoute *>(obj)!=nullptr;
}

const char **FSRRouteDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *FSRRouteDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int FSRRouteDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int FSRRouteDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_source
        FD_ISEDITABLE,    // FIELD_target
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *FSRRouteDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "source",
        "target",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int FSRRouteDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "target") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *FSRRouteDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_source
        "int",    // FIELD_target
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **FSRRouteDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *FSRRouteDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int FSRRouteDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void FSRRouteDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'FSRRoute'", field);
    }
}

const char *FSRRouteDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FSRRouteDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        case FIELD_source: return long2string(pp->source);
        case FIELD_target: return long2string(pp->target);
        default: return "";
    }
}

void FSRRouteDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        case FIELD_source: pp->source = string2long(value); break;
        case FIELD_target: pp->target = string2long(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRRoute'", field);
    }
}

omnetpp::cValue FSRRouteDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        case FIELD_source: return pp->source;
        case FIELD_target: return pp->target;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'FSRRoute' as cValue -- field index out of range?", field);
    }
}

void FSRRouteDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        case FIELD_source: pp->source = omnetpp::checked_int_cast<int>(value.intValue()); break;
        case FIELD_target: pp->target = omnetpp::checked_int_cast<int>(value.intValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRRoute'", field);
    }
}

const char *FSRRouteDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr FSRRouteDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void FSRRouteDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRRoute *pp = omnetpp::fromAnyPtr<FSRRoute>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRRoute'", field);
    }
}

Register_Class(FSRUpdatePacket)

FSRUpdatePacket::FSRUpdatePacket() : ::inet::FSRControlPacket()
{
}

FSRUpdatePacket::FSRUpdatePacket(const FSRUpdatePacket& other) : ::inet::FSRControlPacket(other)
{
    copy(other);
}

FSRUpdatePacket::~FSRUpdatePacket()
{
    delete [] this->routes;
}

FSRUpdatePacket& FSRUpdatePacket::operator=(const FSRUpdatePacket& other)
{
    if (this == &other) return *this;
    ::inet::FSRControlPacket::operator=(other);
    copy(other);
    return *this;
}

void FSRUpdatePacket::copy(const FSRUpdatePacket& other)
{
    delete [] this->routes;
    this->routes = (other.routes_arraysize==0) ? nullptr : new FSRRoute[other.routes_arraysize];
    routes_arraysize = other.routes_arraysize;
    for (size_t i = 0; i < routes_arraysize; i++) {
        this->routes[i] = other.routes[i];
    }
}

void FSRUpdatePacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FSRControlPacket::parsimPack(b);
    b->pack(routes_arraysize);
    doParsimArrayPacking(b,this->routes,routes_arraysize);
}

void FSRUpdatePacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FSRControlPacket::parsimUnpack(b);
    delete [] this->routes;
    b->unpack(routes_arraysize);
    if (routes_arraysize == 0) {
        this->routes = nullptr;
    } else {
        this->routes = new FSRRoute[routes_arraysize];
        doParsimArrayUnpacking(b,this->routes,routes_arraysize);
    }
}

size_t FSRUpdatePacket::getRoutesArraySize() const
{
    return routes_arraysize;
}

const FSRRoute& FSRUpdatePacket::getRoutes(size_t k) const
{
    if (k >= routes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routes_arraysize, (unsigned long)k);
    return this->routes[k];
}

void FSRUpdatePacket::setRoutesArraySize(size_t newSize)
{
    handleChange();
    FSRRoute *routes2 = (newSize==0) ? nullptr : new FSRRoute[newSize];
    size_t minSize = routes_arraysize < newSize ? routes_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        routes2[i] = this->routes[i];
    delete [] this->routes;
    this->routes = routes2;
    routes_arraysize = newSize;
}

void FSRUpdatePacket::setRoutes(size_t k, const FSRRoute& routes)
{
    if (k >= routes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routes_arraysize, (unsigned long)k);
    handleChange();
    this->routes[k] = routes;
}

void FSRUpdatePacket::insertRoutes(size_t k, const FSRRoute& routes)
{
    if (k > routes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routes_arraysize, (unsigned long)k);
    handleChange();
    size_t newSize = routes_arraysize + 1;
    FSRRoute *routes2 = new FSRRoute[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        routes2[i] = this->routes[i];
    routes2[k] = routes;
    for (i = k + 1; i < newSize; i++)
        routes2[i] = this->routes[i-1];
    delete [] this->routes;
    this->routes = routes2;
    routes_arraysize = newSize;
}

void FSRUpdatePacket::appendRoutes(const FSRRoute& routes)
{
    insertRoutes(routes_arraysize, routes);
}

void FSRUpdatePacket::eraseRoutes(size_t k)
{
    if (k >= routes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)routes_arraysize, (unsigned long)k);
    handleChange();
    size_t newSize = routes_arraysize - 1;
    FSRRoute *routes2 = (newSize == 0) ? nullptr : new FSRRoute[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        routes2[i] = this->routes[i];
    for (i = k; i < newSize; i++)
        routes2[i] = this->routes[i+1];
    delete [] this->routes;
    this->routes = routes2;
    routes_arraysize = newSize;
}

class FSRUpdatePacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_routes,
    };
  public:
    FSRUpdatePacketDescriptor();
    virtual ~FSRUpdatePacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(FSRUpdatePacketDescriptor)

FSRUpdatePacketDescriptor::FSRUpdatePacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::FSRUpdatePacket)), "inet::FSRControlPacket")
{
    propertyNames = nullptr;
}

FSRUpdatePacketDescriptor::~FSRUpdatePacketDescriptor()
{
    delete[] propertyNames;
}

bool FSRUpdatePacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FSRUpdatePacket *>(obj)!=nullptr;
}

const char **FSRUpdatePacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *FSRUpdatePacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int FSRUpdatePacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int FSRUpdatePacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISRESIZABLE,    // FIELD_routes
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *FSRUpdatePacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "routes",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int FSRUpdatePacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "routes") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *FSRUpdatePacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::FSRRoute",    // FIELD_routes
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **FSRUpdatePacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *FSRUpdatePacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int FSRUpdatePacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        case FIELD_routes: return pp->getRoutesArraySize();
        default: return 0;
    }
}

void FSRUpdatePacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        case FIELD_routes: pp->setRoutesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'FSRUpdatePacket'", field);
    }
}

const char *FSRUpdatePacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FSRUpdatePacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        case FIELD_routes: return "";
        default: return "";
    }
}

void FSRUpdatePacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRUpdatePacket'", field);
    }
}

omnetpp::cValue FSRUpdatePacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        case FIELD_routes: return omnetpp::toAnyPtr(&pp->getRoutes(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'FSRUpdatePacket' as cValue -- field index out of range?", field);
    }
}

void FSRUpdatePacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRUpdatePacket'", field);
    }
}

const char *FSRUpdatePacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_routes: return omnetpp::opp_typename(typeid(FSRRoute));
        default: return nullptr;
    };
}

omnetpp::any_ptr FSRUpdatePacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        case FIELD_routes: return omnetpp::toAnyPtr(&pp->getRoutes(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void FSRUpdatePacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    FSRUpdatePacket *pp = omnetpp::fromAnyPtr<FSRUpdatePacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FSRUpdatePacket'", field);
    }
}

}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

