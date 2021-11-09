#include "GpDbQuery.hpp"

namespace GPlatform {

GpDbQuery::GpDbQuery (std::string_view aQueryStr):
iQueryStr(aQueryStr)
{
}

GpDbQuery::GpDbQuery
(
    std::string_view        aQueryStr,
    const ValuesTypesVecT&  aValuesTypes
):
iQueryStr(aQueryStr),
iValuesTypes(aValuesTypes)
{
}

GpDbQuery::GpDbQuery
(
    std::string&&       aQueryStr,
    ValuesTypesVecT&&   aValuesTypes
) noexcept:
iQueryStr(std::move(aQueryStr)),
iValuesTypes(std::move(aValuesTypes))
{
}

GpDbQuery::~GpDbQuery (void) noexcept
{
}

GpDbQuery&  GpDbQuery::NextInt16 (const s_int_16 aValue)
{
    _SetNext<s_int_16, GpDbQueryValType::INT_16>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt32 (const s_int_32 aValue)
{
    _SetNext<s_int_32, GpDbQueryValType::INT_32>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt64 (const s_int_64 aValue)
{
    _SetNext<s_int_64, GpDbQueryValType::INT_64>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextDouble (const double aValue)
{
    _SetNext<double, GpDbQueryValType::DOUBLE>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextFloat (const float aValue)
{
    _SetNext<float, GpDbQueryValType::FLOAT>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValue (std::string_view aValue)
{
    _MoveNext<GpDbQueryValStrValue, GpDbQueryValType::STRING_VALUE>(GpDbQueryValStrValue(std::string(aValue)));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValue (std::string&& aValue)
{
    _MoveNext<GpDbQueryValStrValue, GpDbQueryValType::STRING_VALUE>(GpDbQueryValStrValue(std::move(aValue)));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValueArray (const GpVector<std::string_view>& aValue)
{
    GpVector<std::string> v;
    v.reserve(aValue.size());

    for (std::string_view e: aValue)
    {
        v.emplace_back(std::string(e));
    }

    _MoveNext<GpVector<std::string>, GpDbQueryValType::STRING_VALUE_ARRAY>(std::move(v));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValueArray (const GpVector<std::string>& aValue)
{
    _SetNext<GpVector<std::string>, GpDbQueryValType::STRING_VALUE_ARRAY>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValueArray (GpVector<std::string>&& aValue)
{
    _MoveNext<GpVector<std::string>, GpDbQueryValType::STRING_VALUE_ARRAY>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrValueArray (const GpEnumFlags& aValue)
{
    GpVector<std::string> v = aValue.ToStringArray();
    _MoveNext<GpVector<std::string>, GpDbQueryValType::STRING_VALUE_ARRAY>(std::move(v));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrName (std::string_view aValue)
{
    _MoveNext<GpDbQueryValStrName, GpDbQueryValType::STRING_NAME>(GpDbQueryValStrName(std::string(aValue)));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrName (std::string&& aValue)
{
    _MoveNext<GpDbQueryValStrName, GpDbQueryValType::STRING_NAME>(GpDbQueryValStrName(std::move(aValue)));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrJson (std::string_view aValue)
{
    _MoveNext<GpDbQueryValStrJson, GpDbQueryValType::STRING_JSON>(GpDbQueryValStrJson(std::string(aValue)));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrJson (std::string&& aValue)
{
    _MoveNext<GpDbQueryValStrJson, GpDbQueryValType::STRING_JSON>(GpDbQueryValStrJson(std::move(aValue)));
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

s_int_16    GpDbQuery::Int16 (const count_t aId) const
{
    return std::get<s_int_16>(iValues.at(aId.As<size_t>()));
}

s_int_32    GpDbQuery::Int32 (const count_t aId) const
{
    return std::get<s_int_32>(iValues.at(aId.As<size_t>()));
}

s_int_64    GpDbQuery::Int64 (const count_t aId) const
{
    return std::get<s_int_64>(iValues.at(aId.As<size_t>()));
}

double  GpDbQuery::Double (const count_t aId) const
{
    return std::get<double>(iValues.at(aId.As<size_t>()));
}

float   GpDbQuery::Float (const count_t aId) const
{
    return std::get<float>(iValues.at(aId.As<size_t>()));
}

std::string_view    GpDbQuery::StrValue (const count_t aId) const
{
    return std::get<GpDbQueryValStrValue>(iValues.at(aId.As<size_t>())).Value();
}

const GpVector<std::string>&    GpDbQuery::StrValueArray (const count_t aId) const
{
    return std::get<GpVector<std::string>>(iValues.at(aId.As<size_t>()));
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
