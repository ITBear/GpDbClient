#include "GpDbQuery.hpp"

namespace GPlatform {

GpDbQuery::GpDbQuery (std::string_view aQueryStr):
iQueryStr(aQueryStr)
{
}

GpDbQuery::GpDbQuery (std::string_view          aQueryStr,
                      const ValuesTypesVecT&    aValuesTypes):
iQueryStr(aQueryStr),
iValuesTypes(aValuesTypes)
{
}

GpDbQuery::GpDbQuery (std::string&&     aQueryStr,
                      ValuesTypesVecT&& aValuesTypes) noexcept:
iQueryStr(std::move(aQueryStr)),
iValuesTypes(std::move(aValuesTypes))
{
}

GpDbQuery::~GpDbQuery (void) noexcept
{
}

GpDbQuery&  GpDbQuery::NextInt64 (const SInt64 aValue)
{
    _SetNext<SInt64, GpDbQueryValType::INT_64>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValue (std::string_view aValue)
{
    _MoveNext<GpDbQueryValStrValue, GpDbQueryValType::STRING_VALUE>(std::string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValue (std::string&& aValue)
{
    _MoveNext<GpDbQueryValStrValue, GpDbQueryValType::STRING_VALUE>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrName (std::string_view aValue)
{
    _MoveNext<GpDbQueryValStrName, GpDbQueryValType::STRING_NAME>(std::string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrName (std::string&& aValue)
{
    _MoveNext<GpDbQueryValStrName, GpDbQueryValType::STRING_NAME>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrJson (std::string_view aValue)
{
    _MoveNext<GpDbQueryValStrJson, GpDbQueryValType::STRING_JSON>(std::string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrJson (std::string&& aValue)
{
    _MoveNext<GpDbQueryValStrJson, GpDbQueryValType::STRING_JSON>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextUUID (const GpUUID& aValue)
{
    _SetNext<GpUUID, GpDbQueryValType::UUID>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextBLOB (const GpRawPtrByteR aValue)
{
    _MoveNext<GpBytesArray, GpDbQueryValType::BLOB>(GpBytesArrayUtils::SMake(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBLOB (GpBytesArray&& aValue)
{
    _MoveNext<GpBytesArray, GpDbQueryValType::BLOB>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBoolean (const bool aValue)
{
    _SetNext<bool, GpDbQueryValType::BOOLEAN>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextNULL (void)
{
    _SetNext<std::nullopt_t, GpDbQueryValType::NULL_VAL>(std::nullopt);
    return *this;
}

SInt64  GpDbQuery::Int64 (const count_t aId) const
{
    return std::get<SInt64>(iValues.at(aId.As<size_t>()));
}

std::string_view    GpDbQuery::StrValue (const count_t aId) const
{
    return std::get<GpDbQueryValStrValue>(iValues.at(aId.As<size_t>())).Value();
}

std::string_view    GpDbQuery::StrName (const count_t aId) const
{
    return std::get<GpDbQueryValStrName>(iValues.at(aId.As<size_t>())).Value();
}

std::string_view    GpDbQuery::StrJson (const count_t aId) const
{
    return std::get<GpDbQueryValStrJson>(iValues.at(aId.As<size_t>())).Value();
}

const GpUUID&   GpDbQuery::UUID (const count_t aId) const
{
    return std::get<GpUUID>(iValues.at(aId.As<size_t>()));
}

bool    GpDbQuery::Boolean (const count_t aId) const
{
    return std::get<bool>(iValues.at(aId.As<size_t>()));
}

const GpBytesArray& GpDbQuery::BLOB (const count_t aId) const
{
    return std::get<GpBytesArray>(iValues.at(aId.As<size_t>()));
}

}//namespace GPlatform