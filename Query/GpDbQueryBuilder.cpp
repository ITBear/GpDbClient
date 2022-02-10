#include "GpDbQueryBuilder.hpp"

namespace GPlatform {

GpArray<std::string, GpDbQueryValType::SCount().As<size_t>()>   GpDbQueryBuilder::sBindStrs
{
    "int2",     //INT_16,
    "int4",     //INT_32,
    "int8",     //INT_64,
    "float8",   //DOUBLE,
    "float4",   //FLOAT,
    "",         //STRING_VALUE,::text
    "",         //STRING_VALUE_ARRAY,::...[]
    "",         //STRING_NAME,
    "jsonb",    //STRING_JSON,
    "uuid",     //UUID,
    "bytea",    //BLOB,
    "boolean",  //BOOLEAN,
    ""          //NULL_VAL
};

GpDbQueryBuilder::GpDbQueryBuilder (void)
{
    iQueryStr.reserve(512);
}

GpDbQueryBuilder::~GpDbQueryBuilder (void) noexcept
{
}

GpDbQueryBuilder&   GpDbQueryBuilder::INSERT_INTO (void)
{
    CheckForSpace();

    iQueryStr
        .append("INSERT INTO "_sv)
        .append(AddValueBind(GpDbQueryValType::STRING_NAME))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INSERT_INTO (std::string_view aTableName)
{
    CheckForSpace();

    iQueryStr
        .append("INSERT INTO "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BRACE_BEGIN (void)
{
    iQueryStr
        .append("("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BRACE_END (void)
{
    iQueryStr
        .append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COMMA (void)
{
    iQueryStr
        .append(","_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AND (void)
{
    CheckForSpace();

    iQueryStr
        .append("AND"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OR (void)
{
    CheckForSpace();

    iQueryStr
        .append("OR"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT (void)
{
    CheckForSpace();

    iQueryStr
        .append("NOT"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BITWISE_AND (void)
{
    CheckForSpace();

    iQueryStr
        .append("&"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BITWISE_OR (void)
{
    CheckForSpace();

    iQueryStr
        .append("|"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::EQUAL (void)
{
    iQueryStr
        .append("="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::GREATER (void)
{
    iQueryStr
        .append(">"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::GREATER_EQUAL (void)
{
    iQueryStr
        .append(">="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LESS (void)
{
    iQueryStr
        .append("<"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LESS_EQUAL (void)
{
    iQueryStr
        .append("<="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ASSIGN (void)
{
    iQueryStr
        .append("="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ADD (void)
{
    iQueryStr
        .append("+"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SUB (void)
{
    iQueryStr
        .append("-"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COALESCE_BEGIN (void)
{
    CheckForSpace();

    iQueryStr
        .append("COALESCE("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COALESCE_END (void)
{
    iQueryStr
        .append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ASC (void)
{
    CheckForSpace();

    iQueryStr
        .append("ASC"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DESC (void)
{
    CheckForSpace();

    iQueryStr
        .append("DESC"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NULLS (void)
{
    CheckForSpace();

    iQueryStr
        .append("NULLS"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LAST (void)
{
    CheckForSpace();

    iQueryStr
        .append("LAST"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ON (void)
{
    CheckForSpace();

    iQueryStr
        .append("ON"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AS (void)
{
    CheckForSpace();

    iQueryStr
        .append("AS"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AS (std::string_view aName)
{
    CheckForSpace();

    iQueryStr
        .append("AS"_sv)
        .append(" "_sv)
        .append(aName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IN (void)
{
    CheckForSpace();

    iQueryStr
        .append("IN"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ANY
(
    const GpDbQueryValType::EnumT   aValueType,
    std::string_view                aTypeCast
)
{
    CheckForSpace();

    iQueryStr.append("ANY("_sv);
    VALUE(aValueType);
    iQueryStr.append("::"_sv).append(aTypeCast).append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN (void)
{
    CheckForSpace();

    iQueryStr
        .append("BETWEEN "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN
(
    const GpDbQueryValType::EnumT aValueTypeA,
    const GpDbQueryValType::EnumT aValueTypeB
)
{
    BETWEEN().VALUE(aValueTypeA).AND().VALUE(aValueTypeB);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIKE (const GpDbQueryValType::EnumT aValueType)
{
    CheckForSpace();

    iQueryStr
        .append("LIKE "_sv)
        .append(AddValueBind(aValueType))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IS_NULL (void)
{
    CheckForSpace();

    iQueryStr
        .append("IS NULL"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_BEGIN (void)
{
    CheckForSpace();

    iQueryStr
        .append("VALUES("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_END (void)
{
    iQueryStr
        .append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES (void)
{
    CheckForSpace();

    iQueryStr
        .append("VALUES "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WITH (std::string_view aName)
{
    CheckForSpace();

    iQueryStr
        .append("WITH "_sv)
        .append(aName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SELECT (void)
{
    CheckForSpace();

    iQueryStr
        .append("SELECT"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM (std::string_view aTableName)
{
    CheckForSpace();

    iQueryStr
        .append("FROM "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM (void)
{
    CheckForSpace();

    iQueryStr
        .append("FROM"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WHERE (void)
{
    CheckForSpace();

    iQueryStr
        .append("WHERE"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::UPDATE (std::string_view aTableName)
{
    CheckForSpace();

    iQueryStr
        .append("UPDATE "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SET (void)
{
    CheckForSpace();

    iQueryStr
        .append("SET"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ORDER_BY (void)
{
    CheckForSpace();

    iQueryStr
        .append("ORDER BY"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ORDER_BY (std::string_view aParamName)
{
    ORDER_BY().PARAM(aParamName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const count_t aValue)
{
    CheckForSpace();

    iQueryStr
        .append("LIMIT "_sv)
        .append(StrOps::SToString(aValue))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const GpDbQueryValType::EnumT aValueType)
{
    CheckForSpace();

    iQueryStr
        .append("LIMIT "_sv);

    VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OFFSET (const count_t aValue)
{
    CheckForSpace();

    iQueryStr
        .append("OFFSET "_sv)
        .append(StrOps::SToString(aValue))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OFFSET (const GpDbQueryValType::EnumT aValueType)
{
    CheckForSpace();

    iQueryStr
        .append("OFFSET "_sv);

    VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RETURNING (void)
{
    CheckForSpace();

    iQueryStr
        .append("RETURNING"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FOR_UPDATE (void)
{
    CheckForSpace();

    iQueryStr
        .append("FOR UPDATE"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SKIP_LOCKED (void)
{
    CheckForSpace();

    iQueryStr
        .append("SKIP LOCKED"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DELETE_FROM (std::string_view aTableName)
{
    return RAW("DELETE FROM"_sv).RAW(aTableName);
}

GpDbQueryBuilder&   GpDbQueryBuilder::INNER_JOIN (std::string_view aTableName)
{
    CheckForSpace();

    iQueryStr
        .append("INNER JOIN "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LEFT_JOIN (std::string_view aTableName)
{
    CheckForSpace();

    iQueryStr
        .append("LEFT JOIN "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RIGHT_JOIN (std::string_view aTableName)
{
    CheckForSpace();

    iQueryStr
        .append("RIGHT JOIN "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::PARAM (std::string_view aName)
{
    CheckForSpace();

    iQueryStr
        .append(aName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::PARAMS (const GpVector<std::string_view>& aNames)
{
    CheckForSpace();

    iQueryStr
        .append(StrOps::SJoin<std::string_view>(aNames, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE_NULL (void)
{
    RAW("NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const GpDbQueryValType::EnumT aValueType)
{
    CheckForSpace();

    iQueryStr
        .append(AddValueBind(aValueType))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpDbQueryValType::EnumT   aValueType,
    std::string_view                aTypeCast
)
{
    CheckForSpace();

    iQueryStr
        .append(AddValueBind(aValueType, aTypeCast))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (std::string_view aValue)
{
    CheckForSpace();

    iQueryStr
        .append("'"_sv)
        .append(aValue)
        .append("'"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    std::string_view    aValue,
    std::string_view    aTypeCast
)
{
    CheckForSpace();

    VALUE(aValue);

    if (aTypeCast.length() > 0)
    {
        iQueryStr
            .append("::"_sv).append(aTypeCast);
    }

    iQueryStr
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const SInt64 aValue)
{
    CheckForSpace();

    iQueryStr
        .append(StrOps::SToString(aValue))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const GpBool aValue)
{
    CheckForSpace();

    iQueryStr
        .append(aValue.Value() ? "true"_sv : "false"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpEnum&       aValue,
    std::string_view    aTypeCast
)
{
    return VALUE(aValue.ToString(), aTypeCast);
}

GpDbQueryBuilder&   GpDbQueryBuilder::RAW (std::string_view aStr)
{
    CheckForSpace();

    iQueryStr
        .append(aStr);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::STRUCT_PARAM_NAMES
(
    std::string_view        aTableName,
    const GpTypeStructInfo& aTypeInfo
)
{
    CheckForSpace();

    const auto info = SFromTypeInfo(aTypeInfo);

    auto getFn = [&](auto& i) -> std::string
    {
        if (aTableName.length() > 0)
        {
            return aTableName + ".\""_sv + i->name + "\"";
        } else
        {
            return "\""_sv + i->name + "\"";
        }
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(info, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::STRUCT_PARAM_BINDS (const GpTypeStructInfo& aTypeInfo)
{
    CheckForSpace();

    const auto info = SFromTypeInfo(aTypeInfo);

    auto getFn = [&](auto& i) -> std::string
    {
        iValuesTypes.emplace_back(i->type);

        std::string res;
        res.reserve(10);

        res
            .append("$"_sv)
            .append(StrOps::SToString<size_t>(iValuesTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(info, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::STRUCT_PARAM_ASSIGN
(
    std::string_view        aTableName,
    const GpTypeStructInfo& aTypeInfo
)
{
    CheckForSpace();

    const auto info = SFromTypeInfo(aTypeInfo);

    auto getFn = [&](auto& i) -> std::string
    {
        iValuesTypes.emplace_back(i->type);

        std::string res;
        res.reserve(20);

        if (aTableName.length() > 0)
        {
            res
                .append(aTableName)
                .append("."_sv);
        }

        res
            .append(i->name)
            .append("=$"_sv)
            .append(StrOps::SToString<size_t>(iValuesTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(info, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INC_VERSION (void)
{
    PARAM("_version"_sv).ASSIGN().PARAM("_version"_sv).ADD().VALUE(1_s_int_64);

    return *this;
}

std::string GpDbQueryBuilder::AddValueBind (const GpDbQueryValType::EnumT aValueType)
{
    return AddValueBind(aValueType, sBindStrs.at(size_t(aValueType)));
}

std::string GpDbQueryBuilder::AddValueBind
(
    const GpDbQueryValType::EnumT   aValueType,
    std::string_view                aTypeCast
)
{
    iValuesTypes.emplace_back(aValueType);

    std::string res;

    res
        .append("$"_sv)
        .append(StrOps::SToString<size_t>(iValuesTypes.size()));

    if (aTypeCast.length() > 0)
    {
        res.append("::"_sv).append(aTypeCast);
    }

    return res;
}

GpVector<GpDbQueryBuilder::TypeInfo>    GpDbQueryBuilder::SFromTypeInfo (const GpTypeStructInfo& aTypeInfo)
{
    GpVector<TypeInfo> res;

    const auto&     props       = aTypeInfo.Props();
    const size_t    propsCout   = props.size();

    res.reserve(propsCout);

    for (const GpTypePropInfo& propInfo: aTypeInfo.Props())
    {
        THROW_GPE_COND
        (
            propInfo.Container() == GpTypeContainer::NO,
            [&](){return "Container of property '"_sv + aTypeInfo.Name() + "."_sv + propInfo.Name() + "' must be NO"_sv;}
        );

        std::string             valueBind;
        GpDbQueryValType::EnumT valueType;

        switch (propInfo.Type())
        {
            case GpType::U_INT_8:    [[fallthrough]];
            case GpType::S_INT_8:    [[fallthrough]];
            case GpType::U_INT_16:   [[fallthrough]];
            case GpType::S_INT_16:
            {
                valueType   = GpDbQueryValType::INT_16;
                valueBind   = "::int2"_sv;
            } break;
            case GpType::U_INT_32:   [[fallthrough]];
            case GpType::S_INT_32:
            {
                valueType   = GpDbQueryValType::INT_32;
                valueBind   = "::int4"_sv;
            } break;
            case GpType::U_INT_64:   [[fallthrough]];
            case GpType::S_INT_64:   [[fallthrough]];
            case GpType::UNIX_TS_S:  [[fallthrough]];
            case GpType::UNIX_TS_MS:
            {
                valueType   = GpDbQueryValType::INT_64;
                valueBind   = "::int8"_sv;
            } break;
            case GpType::DOUBLE:
            {
                valueType   = GpDbQueryValType::DOUBLE;
                valueBind   = "::float8"_sv;
            } break;
            case GpType::FLOAT:
            {
                valueType   = GpDbQueryValType::FLOAT;
                valueBind   = "::float4"_sv;
            } break;
            case GpType::BOOLEAN:
            {
                valueType   = GpDbQueryValType::BOOLEAN;
                valueBind   = "::boolean"_sv;
            } break;
            case GpType::UUID:
            {
                valueType   = GpDbQueryValType::UUID;
                valueBind   = "::uuid"_sv;
            } break;
            case GpType::STRING:
            {
                valueType   = GpDbQueryValType::STRING_VALUE;
                //valueBind = "::text"_sv;
                valueBind   = ""_sv;
            } break;
            case GpType::BLOB:
            {
                valueType   = GpDbQueryValType::BLOB;
                valueBind   = "::bytea"_sv;
            } break;
            case GpType::STRUCT:
            {
                valueType   = GpDbQueryValType::STRING_JSON;
                valueBind   = "::jsonb"_sv;
            } break;
            case GpType::STRUCT_SP:
            {
                valueType   = GpDbQueryValType::STRING_JSON;
                valueBind   = "::jsonb"_sv;
            } break;
            case GpType::ENUM:
            {
                valueType   = GpDbQueryValType::STRING_VALUE;
                valueBind   = ""_sv;
            } break;
            case GpType::ENUM_FLAGS:
            {
                valueType   = GpDbQueryValType::STRING_VALUE_ARRAY;
                valueBind   = ""_sv;//TODO ? can be any ENUM DB type (CREATE TYPE schema.table AS ENUM ('A', 'B', 'C');
                                    //then valueBind must be = ::schema.table[]
            } break;
            case GpType::NOT_SET: [[fallthrough]];
            default:
            {
                THROW_GPE("Unsupported type NOT_SET"_sv); break;
            }
        }

        res.emplace_back(propInfo.Name(), valueBind, valueType);
    }

    return res;
}

void    GpDbQueryBuilder::CheckForSpace (void)
{
    if (iQueryStr.length() > 0)
    {
        if (iQueryStr.at(iQueryStr.length() - 1) != ' ')
        {
            iQueryStr.append(" "_sv);
        }
    }
}

}//GPlatform
