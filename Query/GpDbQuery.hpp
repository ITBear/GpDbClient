#pragma once

#include "GpDbQueryValType.hpp"
#include "GpDbQueryValue.hpp"
#include "GpDbQueryBuilder.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbQuery
{
public:
    CLASS_REMOVE_CTRS(GpDbQuery)
    CLASS_DECLARE_DEFAULTS(GpDbQuery)

    using ValuesTypesVecT   = GpVector<GpDbQueryValType::EnumT>;
    using ValuesVecT        = GpVector<GpDbQueryValue>;

public:
                                    GpDbQuery           (std::string_view aQueryStr);
                                    GpDbQuery           (std::string_view       aQueryStr,
                                                         const ValuesTypesVecT& aValuesTypes);
                                    GpDbQuery           (std::string&&      aQueryStr,
                                                         ValuesTypesVecT&&  aValuesTypes) noexcept;
                                    ~GpDbQuery          (void) noexcept;

    std::string_view                QueryStr            (void) const noexcept {return iQueryStr;}
    const ValuesTypesVecT&          ValuesTypes         (void) const noexcept {return iValuesTypes;}
    const ValuesVecT&               Values              (void) const noexcept {return iValues;}

    GpDbQuery&                      NextInt16           (const s_int_16                     aValue);
    GpDbQuery&                      NextInt32           (const s_int_32                     aValue);
    GpDbQuery&                      NextInt64           (const s_int_64                     aValue);
    GpDbQuery&                      NextDouble          (const double                       aValue);
    GpDbQuery&                      NextFloat           (const float                        aValue);
    GpDbQuery&                      NextStrValue        (std::string_view                   aValue);
    GpDbQuery&                      NextStrValue        (std::string&&                      aValue);
    GpDbQuery&                      NextStrValueArray   (const GpVector<std::string_view>&  aValue);
    GpDbQuery&                      NextStrValueArray   (const GpVector<std::string>&       aValue);
    GpDbQuery&                      NextStrValueArray   (GpVector<std::string>&&            aValue);
    GpDbQuery&                      NextStrValueArray   (const GpEnumFlags&                 aValue);
    GpDbQuery&                      NextStrName         (std::string_view                   aValue);
    GpDbQuery&                      NextStrName         (std::string&&                      aValue);
    GpDbQuery&                      NextStrJson         (std::string_view                   aValue);
    GpDbQuery&                      NextStrJson         (std::string&&                      aValue);
    GpDbQuery&                      NextUUID            (const GpUUID&                      aValue);
    GpDbQuery&                      NextBLOB            (const GpRawPtrByteR                aValue);
    GpDbQuery&                      NextBLOB            (GpBytesArray&&                     aValue);
    GpDbQuery&                      NextBoolean         (const bool                         aValue);
    GpDbQuery&                      NextNULL            (void);

    template<GpDbQueryValType::EnumT E,
             typename                T>
    GpDbQuery&                      Next            (T aValue);

    template<typename... Ts>
    GpDbQuery&                      Nexts           (Ts... aValues);

    template<typename T>
    GpDbQueryValType::EnumT         NextUnpack      (T aValue);

    s_int_16                        Int16           (const count_t aId) const;
    s_int_32                        Int32           (const count_t aId) const;
    s_int_64                        Int64           (const count_t aId) const;
    double                          Double          (const count_t aId) const;
    float                           Float           (const count_t aId) const;
    std::string_view                StrValue        (const count_t aId) const;
    const GpVector<std::string>&    StrValueArray   (const count_t aId) const;
    std::string_view                StrName         (const count_t aId) const;
    std::string_view                StrJson         (const count_t aId) const;
    const GpUUID&                   UUID            (const count_t aId) const;
    bool                            Boolean         (const count_t aId) const;
    const GpBytesArray&             BLOB            (const count_t aId) const;

private:
    template<typename                   T,
             GpDbQueryValType::EnumT    E>
    void                    _MoveNext       (T&& aValue)
    {
        const GpDbQueryValType::EnumT valType = iValuesTypes.at(iValues.size());
        if (valType == E)
        {
            iValues.emplace_back(std::move(aValue));
        } else
        {
            THROW_GPE("Wrong value type, must be "_sv + GpDbQueryValType::SToString(valType));
        }
    }

    template<typename                   T,
             GpDbQueryValType::EnumT    E>
    void                    _SetNext        (const T& aValue)
    {
        const GpDbQueryValType::EnumT valType = iValuesTypes.at(iValues.size());
        if (valType == E)
        {
            iValues.emplace_back(aValue);
        } else
        {
            THROW_GPE("Wrong value type, must be "_sv + GpDbQueryValType::SToString(valType));
        }
    }

private:
    const std::string       iQueryStr;
    const ValuesTypesVecT   iValuesTypes;
    ValuesVecT              iValues;
};

template<GpDbQueryValType::EnumT E,
         typename                T>
GpDbQuery&  GpDbQuery::Next (T aValue)
{
    if constexpr(E == GpDbQueryValType::INT_16)
    {
        return NextInt16(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::INT_32)
    {
        return NextInt32(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::INT_64)
    {
        return NextInt64(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::DOUBLE)
    {
        return NextDouble(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::FLOAT)
    {
        return NextFloat(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::STRING_VALUE)
    {
        return NextStrValue(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::STRING_VALUE_ARRAY)
    {
        return NextStrValueArray(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::STRING_NAME)
    {
        return NextStrName(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::STRING_JSON)
    {
        return NextStrJson(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::UUID)
    {
        return NextUUID(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::BLOB)
    {
        return NextBLOB(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::BOOLEAN)
    {
        return NextBoolean(std::forward<T>(aValue));
    } else if constexpr(E == GpDbQueryValType::NULL_VAL)
    {
        return NextNULL();
    } else
    {
        GpThrowCe<std::out_of_range>("Unknown type"_sv);
    }
}

template<typename... Ts>
GpDbQuery&  GpDbQuery::Nexts (Ts... aValues)
{
    [[maybe_unused]] GpArray<GpDbQueryValType::EnumT,sizeof...(Ts)> VAL_T = {NextUnpack<Ts>(aValues)...};
    return *this;
}

template<typename T>
GpDbQueryValType::EnumT GpDbQuery::NextUnpack (T aValue)
{
    constexpr GpDbQueryValType::EnumT valueType = GpDbQueryBuilder::SDetectValType<T>();
    Next<valueType>(std::forward<T>(aValue));
    return valueType;
}

}//GPlatform
