#pragma once

#include "GpDbQueryValType.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbQueryBuilder
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbQueryBuilder)
    CLASS_DECLARE_DEFAULTS(GpDbQueryBuilder)

    using ValuesTypesVecT = GpVector<GpDbQueryValType::EnumT>;

    struct TypeInfo
    {
                                TypeInfo (std::string_view              aName,
                                          std::string_view              aBindType,
                                          const GpDbQueryValType::EnumT aType):name(aName), bindType(aBindType), type(aType) {}

        std::string             name;       //*values names*/
        std::string             bindType;   //::type
        GpDbQueryValType::EnumT type;
    };

public:
                                GpDbQueryBuilder    (void);
                                ~GpDbQueryBuilder   (void) noexcept;

    std::string_view            QueryStr            (void) const noexcept {return iQueryStr;}
    const ValuesTypesVecT&      ValuesTypes         (void) const noexcept {return iValuesTypes;}

    std::string&&               QueryStrMove        (void) noexcept {return std::move(iQueryStr);}
    ValuesTypesVecT&&           ValuesTypesMove     (void) noexcept {return std::move(iValuesTypes);}

    GpDbQueryBuilder&           INSERT_INTO         (void);
    GpDbQueryBuilder&           INSERT_INTO         (std::string_view aTableName);

    GpDbQueryBuilder&           BRACE_BEGIN         (void);
    GpDbQueryBuilder&           BRACE_END           (void);
    GpDbQueryBuilder&           COMMA               (void);
    GpDbQueryBuilder&           AND                 (void);
    GpDbQueryBuilder&           EQUAL               (void);
    GpDbQueryBuilder&           GREATER             (void);
    GpDbQueryBuilder&           GREATER_EQUAL       (void);
    GpDbQueryBuilder&           LESS                (void);
    GpDbQueryBuilder&           LESS_EQUAL          (void);
    GpDbQueryBuilder&           ASSIGN              (void);
    GpDbQueryBuilder&           ADD                 (void);
    GpDbQueryBuilder&           SUB                 (void);
    GpDbQueryBuilder&           COALESCE_BEGIN      (void);
    GpDbQueryBuilder&           COALESCE_END        (void);
    GpDbQueryBuilder&           DESC                (void);
    GpDbQueryBuilder&           NULLS               (void);
    GpDbQueryBuilder&           LAST                (void);
    GpDbQueryBuilder&           ON                  (void);
    GpDbQueryBuilder&           BETWEEN             (void);
    GpDbQueryBuilder&           BETWEEN             (const GpDbQueryValType::EnumT aValueTypeA,
                                                     const GpDbQueryValType::EnumT aValueTypeB);

    GpDbQueryBuilder&           VALUES_BEGIN        (void);
    GpDbQueryBuilder&           VALUES_END          (void);

    GpDbQueryBuilder&           SELECT              (void);
    GpDbQueryBuilder&           FROM                (std::string_view aTableName);  
    GpDbQueryBuilder&           WHERE               (void);
    GpDbQueryBuilder&           UPDATE              (std::string_view aTableName);
    GpDbQueryBuilder&           SET                 (void);
    GpDbQueryBuilder&           ORDER_BY            (void);
    GpDbQueryBuilder&           LIMIT               (const count_t aValue);
    GpDbQueryBuilder&           RETURNING           (void);

    GpDbQueryBuilder&           INNER_JOIN          (std::string_view aTableName);

    GpDbQueryBuilder&           PARAM_NAME          (std::string_view aName);
    GpDbQueryBuilder&           PARAM_NAMES         (const GpVector<std::string_view>& aNames);
    GpDbQueryBuilder&           VALUE               (const GpDbQueryValType::EnumT aValueType); 
    GpDbQueryBuilder&           VALUE               (std::string_view aValue);
    GpDbQueryBuilder&           VALUE               (const SInt64 aValue);
    GpDbQueryBuilder&           RAW                 (std::string_view aStr);

    GpDbQueryBuilder&           STRUCT_PARAM_NAMES  (const GpTypeStructInfo& aTypeInfo);
    GpDbQueryBuilder&           STRUCT_PARAM_BINDS  (const GpTypeStructInfo& aTypeInfo);
    GpDbQueryBuilder&           STRUCT_PARAM_ASSIGN (const GpTypeStructInfo& aTypeInfo);

    GpDbQueryBuilder&           INC_VERSION         (void);

    std::string                 AddValueBind        (const GpDbQueryValType::EnumT aValueType);

    static GpVector<TypeInfo>   SFromTypeInfo       (const GpTypeStructInfo& aTypeInfo);

    template<typename T> static
    consteval GpDbQueryValType::EnumT
                                SDetectValType      (void);

private:
    std::string                 iQueryStr;
    ValuesTypesVecT             iValuesTypes;

    static GpArray<std::string, GpDbQueryValType::SCount().As<size_t>()>    sBindStrs;
};

template<typename T>
consteval GpDbQueryValType::EnumT GpDbQueryBuilder::SDetectValType (void)
{
    constexpr GpType::EnumT t = GpTypeUtils::SDetectType<T>();

         if constexpr (t == GpType::NOT_SET)    GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpType::U_INT_8)    return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::S_INT_8)    return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::U_INT_16)   return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::S_INT_16)   return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::U_INT_32)   return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::S_INT_32)   return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::U_INT_64)   return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::S_INT_64)   return GpDbQueryValType::INT_64;
    else if constexpr (t == GpType::DOUBLE)     GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpType::FLOAT)      GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpType::BOOLEAN)    return GpDbQueryValType::BOOLEAN;
    else if constexpr (t == GpType::UUID)       return GpDbQueryValType::UUID;
    else if constexpr (t == GpType::STRING)     return GpDbQueryValType::STRING_VALUE;
    else if constexpr (t == GpType::BLOB)       return GpDbQueryValType::BLOB;
    else if constexpr (t == GpType::STRUCT)     GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpType::STRUCT_SP)  GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpType::ENUM)       return GpDbQueryValType::STRING_VALUE;
    else GpThrowCe<T>("Unsupported type");
}

}//GPlatform