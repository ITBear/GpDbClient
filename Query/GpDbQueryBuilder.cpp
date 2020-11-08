#include "GpDbQueryBuilder.hpp"

namespace GPlatform {

GpArray<std::string, GpDbQueryValType::SCount().As<size_t>()>   GpDbQueryBuilder::sBindStrs
{
    "::bigint",     //INT_64,
    "",             //STRING_VALUE,::text
    "",             //STRING_NAME,
    "::jsonb",      //STRING_JSON,
    "::uuid",       //UUID,
    "::bytea",      //BLOB,
    "::boolean",    //BOOLEAN,
    ""              //NULL_VAL
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
    iQueryStr
        .append("INSERT INTO "_sv)
        .append(AddValueBind(GpDbQueryValType::STRING_NAME))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INSERT_INTO (std::string_view aTableName)
{
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
    iQueryStr
        .append("AND"_sv)
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

GpDbQueryBuilder&   GpDbQueryBuilder::DESC (void)
{
    iQueryStr
        .append("DESC"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NULLS (void)
{
    iQueryStr
        .append("NULLS"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LAST (void)
{
    iQueryStr
        .append("LAST"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ON (void)
{
    iQueryStr
        .append("ON"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN (void)
{
    iQueryStr
        .append("BETWEEN"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN (const GpDbQueryValType::EnumT aValueTypeA,
                                               const GpDbQueryValType::EnumT aValueTypeB)
{
    BETWEEN().VALUE(aValueTypeA).AND().VALUE(aValueTypeB);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_BEGIN (void)
{
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


GpDbQueryBuilder&   GpDbQueryBuilder::SELECT (void)
{
    iQueryStr
        .append("SELECT"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM (std::string_view aTableName)
{
    iQueryStr
        .append("FROM "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WHERE (void)
{
    iQueryStr
        .append("WHERE"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::UPDATE (std::string_view aTableName)
{
    iQueryStr
        .append("UPDATE "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SET (void)
{
    iQueryStr
        .append("SET"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ORDER_BY (void)
{
    iQueryStr
        .append("ORDER BY"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const count_t aValue)
{
    iQueryStr
        .append("LIMIT "_sv)
        .append(GpStringOps::SToString(aValue))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RETURNING (void)
{
    iQueryStr
        .append("RETURNING"_sv)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INNER_JOIN (std::string_view aTableName)
{
    iQueryStr
        .append("INNER JOIN "_sv)
        .append(aTableName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::PARAM_NAME (std::string_view aName)
{
    iQueryStr
        .append(aName)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::PARAM_NAMES (const GpVector<std::string_view>& aNames)
{
    iQueryStr
        .append(GpStringOps::SJoin<std::string_view>(aNames, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const GpDbQueryValType::EnumT aValueType)
{
    iQueryStr
        .append(AddValueBind(aValueType))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (std::string_view aValue)
{
    iQueryStr
        .append(aValue)
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const SInt64 aValue)
{
    iQueryStr
        .append(GpStringOps::SToString(aValue))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RAW (std::string_view aStr)
{
    iQueryStr
        .append(aStr);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::STRUCT_PARAM_NAMES (const GpTypeStructInfo& aTypeInfo)
{
    const auto info = SFromTypeInfo(aTypeInfo);

    auto getFn = [](auto& i) -> std::string_view
    {
        return i->name;
    };

    iQueryStr
        .append(GpStringOps::SJoin<std::string_view>(info, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::STRUCT_PARAM_BINDS (const GpTypeStructInfo& aTypeInfo)
{
    const auto info = SFromTypeInfo(aTypeInfo);

    auto getFn = [&](auto& i) -> std::string
    {
        iValuesTypes.emplace_back(i->type);

        std::string res;
        res.reserve(10);

        res
            .append("$"_sv)
            .append(GpStringOps::SToString<size_t>(iValuesTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(GpStringOps::SJoin<std::string>(info, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::STRUCT_PARAM_ASSIGN (const GpTypeStructInfo& aTypeInfo)
{
    const auto info = SFromTypeInfo(aTypeInfo);

    auto getFn = [&](auto& i) -> std::string
    {
        iValuesTypes.emplace_back(i->type);

        std::string res;
        res.reserve(20);

        res
            .append(i->name)
            .append("=$"_sv)
            .append(GpStringOps::SToString<size_t>(iValuesTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(GpStringOps::SJoin<std::string>(info, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INC_VERSION (void)
{
    PARAM_NAME("_version"_sv).ASSIGN().PARAM_NAME("_version"_sv).ADD().VALUE(1_s_int_64);

    return *this;
}

std::string GpDbQueryBuilder::AddValueBind (const GpDbQueryValType::EnumT aValueType)
{
    iValuesTypes.emplace_back(aValueType);

    std::string res;
    res.reserve(10);

    res
        .append("$"_sv)
        .append(GpStringOps::SToString<size_t>(iValuesTypes.size()))
        .append(sBindStrs.at(size_t(aValueType)));

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
        THROW_GPE_COND_CHECK_M(propInfo.Container() == GpTypeContainer::NO,
                               "Container of property '"_sv + aTypeInfo.Name() + "."_sv + propInfo.Name() + "' must be NO");

        std::string             valueBind;
        GpDbQueryValType::EnumT valueType;

        switch (propInfo.Type())
        {
            case GpType::U_INT_8:
            case GpType::S_INT_8:
            case GpType::U_INT_16:
            case GpType::S_INT_16:
            case GpType::U_INT_32:
            case GpType::S_INT_32:
            case GpType::U_INT_64:
            case GpType::S_INT_64:
            {
                valueType   = GpDbQueryValType::INT_64;
                valueBind   = "::bigint"_sv;
            } break;
            case GpType::DOUBLE:    THROW_GPE("Unsupported type DOUBLE"_sv); break;
            case GpType::FLOAT:     THROW_GPE("Unsupported type FLOAT"_sv); break;
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
            case GpType::STRUCT:    THROW_GPE("Unsupported type STRUCT"_sv); break;
            case GpType::STRUCT_SP: THROW_GPE("Unsupported type STRUCT_SP"_sv); break;
            case GpType::ENUM:
            {
                valueType   = GpDbQueryValType::STRING_VALUE;
                valueBind   = ""_sv;
            } break;
            case GpType::NOT_SET:
            default:
            {
                THROW_GPE("Unsupported type NOT_SET"_sv); break;
            }
        }

        res.emplace_back(propInfo.Name(), valueBind, valueType);
    }

    return res;
}

}//GPlatform
