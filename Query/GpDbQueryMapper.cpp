#include "GpDbQueryMapper.hpp"

namespace GPlatform {

GpDbQueryMapperCache    GpDbQueryMapper::sMapperCache;

s_int_64    GpDbQueryMapper::SSelectPagingStartPoint
(
    const GpDbQueryCacheUID&    aCacheUID,
    std::string_view            aTablePath,
    GpDbConnection&             aDbConn,
    std::string_view            aPagingCounterName,
    std::string_view            aTimestampName,
    const unix_ts_s_t           aTimestamp
)
{
    //SELECT
    //   COALESCE(aPagingCounterName, 0)
    //FROM
    //  aTablePath
    //WHERE
    //  aTimestampName <= $aTimestampt
    //ORDER BY
    // aPagingCounterName DESC NULLS LAST LIMIT 1

    const auto& cacheVal = GpDbQueryMapper::SMapperCache().Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .SELECT()
                    .COALESCE_BEGIN()
                        .PARAM(aPagingCounterName)
                        .COMMA().VALUE(0_s_int_64)
                    .COALESCE_END()
                .FROM(aTablePath)
                .WHERE()
                    .PARAM(aTimestampName).LESS_EQUAL().VALUE(GpDbQueryValType::INT_64)
                .ORDER_BY()
                    .PARAM(aPagingCounterName).DESC().NULLS().LAST()
                .LIMIT(1_cnt);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);

    dbQuery.NextInt64(aTimestamp.As<s_int_64>());

    GpDbQueryRes::SP    dbQueryResSP    = aDbConn.Execute(dbQuery, 0_cnt);
    const GpDbQueryRes& dbQueryRes      = dbQueryResSP.VC();

    const count_t rowsCount = dbQueryRes.RowsCount();

    if (rowsCount == 0_cnt)
    {
        return 0;
    }

    return dbQueryRes.GetInt64(0_cnt, 0_cnt, std::nullopt);
}

s_int_64    GpDbQueryMapper::SSelectPagingStartPoint
(
    const GpDbQueryCacheUID&    aCacheUID,
    std::string_view            aTablePath,
    GpDbConnection&             aDbConn,
    std::string_view            aPagingCounterName,
    std::string_view            aUidParamName,
    const GpUUID&               aUid,
    std::string_view            aTimestampName,
    const unix_ts_s_t           aTimestamp
)
{
    //SELECT COALESCE(
    //   aStartPointIdValueName
    //  ,0)
    //FROM
    //  aTablePath
    //WHERE
    //  aGroupValueName = $aGroupUid
    //  AND aTimestampValueName <= $aTimestampt
    //ORDER BY
    // aStartPointIdValueName DESC NULLS LAST LIMIT 1

    const auto& cacheVal = GpDbQueryMapper::SMapperCache().Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .SELECT()
                    .COALESCE_BEGIN()
                        .PARAM(aPagingCounterName)
                        .COMMA().VALUE(0_s_int_64)
                    .COALESCE_END()
                .FROM(aTablePath)
                .WHERE()
                    .PARAM(aUidParamName).EQUAL().VALUE(GpDbQueryValType::UUID)
                    .AND().PARAM(aTimestampName).LESS_EQUAL().VALUE(GpDbQueryValType::INT_64)
                .ORDER_BY()
                    .PARAM(aPagingCounterName).DESC().NULLS().LAST()
                .LIMIT(1_cnt);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);

    dbQuery.NextUUID(aUid);
    dbQuery.NextInt64(aTimestamp.As<s_int_64>());

    GpDbQueryRes::SP    dbQueryResSP    = aDbConn.Execute(dbQuery, 0_cnt);
    const GpDbQueryRes& dbQueryRes      = dbQueryResSP.VC();

    const count_t rowsCount = dbQueryRes.RowsCount();

    if (rowsCount == 0_cnt)
    {
        return 0;
    }

    return dbQueryRes.GetInt64(0_cnt, 0_cnt, std::nullopt);
}

void    GpDbQueryMapper::SCreatePagingOrderCounter
(
    const GpUUID&       aUid,
    std::string_view    aName,
    GpDbConnection&     aDbConn
)
{
    const auto& cacheVal = sMapperCache.Get
    (
        GpDbQueryCacheUID(GpUUID::CE_FromString("81116fa6-0e0e-4cd9-9441-6b0f42aa91c5"_sv)),
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .INSERT_INTO("paging_order_counters"_sv)
                .BRACE_BEGIN()
                    .PARAM("uid"_sv)
                    .COMMA().PARAM("name"_sv)
                    .COMMA().PARAM("next_val"_sv)
                .BRACE_END()
                .VALUES_BEGIN()
                    .VALUE(GpDbQueryValType::UUID)
                    .COMMA().VALUE(GpDbQueryValType::STRING_VALUE)
                    .COMMA().VALUE(GpDbQueryValType::INT_64)
                .VALUES_END();

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);

    dbQuery.NextUUID(aUid);
    dbQuery.NextStrValue(aName);
    dbQuery.NextInt64(0);

    aDbConn.Execute(dbQuery, 0_cnt);
}

void    GpDbQueryMapper::SInsertAsRow
(
    const GpDbQueryCacheUID&    aCacheUID,
    const GpTypeStructBase&     aStruct,
    std::string_view            aTablePath,
    GpDbConnection&             aDbConn
)
{
    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .INSERT_INTO(aTablePath)
                .BRACE_BEGIN()
                    .STRUCT_PARAM_NAMES(""_sv, aStruct.TypeInfo())
                .BRACE_END()
                .VALUES_BEGIN()
                    .STRUCT_PARAM_BINDS(aStruct.TypeInfo())
                .VALUES_END();

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    SWriteRowValues(dbQuery, aStruct);
    aDbConn.Execute(dbQuery, 0_cnt);
}

void    GpDbQueryMapper::SInsertAsRowVec
(
    const GpDbQueryCacheUID&            /*aCacheUID*/,
    const GpTypeStructBase::C::Vec::SP& /*aStructVec*/,
    std::string_view                    /*aTablePath*/,
    GpDbConnection&                     /*aDbConn*/
)
{
    //TODO implement
    THROW_GPE_NOT_IMPLEMENTED();

    /*if (aStructVec.size() == 0)
    {
        return;
    }

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .INSERT_INTO(aTablePath)
                .BRACE_BEGIN()
                    .STRUCT_PARAM_NAMES(""_sv, aStruct.TypeInfo())
                .BRACE_END()
                .VALUES();


                    .STRUCT_PARAM_BINDS(aStruct.TypeInfo())
                .VALUES_END();

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    SWriteRowValues(dbQuery, aStruct);
    aDbConn.Execute(dbQuery, 0_cnt);*/
}

void    GpDbQueryMapper::SInsertAsJsonb
(
    const GpDbQueryCacheUID&    aCacheUID,
    const GpTypeStructBase&     aStruct,
    std::string_view            aTablePath,
    GpDbConnection&             aDbConn,
    const GpJsonMapperFlags     aFlags
)
{
    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .INSERT_INTO(aTablePath)
                .BRACE_BEGIN()
                    .PARAM("data"_sv)
                .BRACE_END()
                .VALUES_BEGIN()
                    .VALUE(GpDbQueryValType::STRING_JSON)
                .VALUES_END();

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.NextStrJson(GpJsonMapper::SToJson(aStruct, aFlags));
    aDbConn.Execute(dbQuery, 0_cnt);
}

void    GpDbQueryMapper::SWriteRowValues
(
    GpDbQuery&              aDbQuery,
    const GpTypeStructBase& aStruct
)
{
    const GpTypeStructInfo& typeInfo = aStruct.TypeInfo();

    for (const GpTypePropInfo& propInfo: typeInfo.Props())
    {
        THROW_GPE_COND
        (
            propInfo.Container() == GpTypeContainer::NO,
            [&](){return "Container of property '"_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "' must be NO"_sv;}
        );

        switch (propInfo.Type())
        {
            case GpType::U_INT_8:   aDbQuery.NextInt16(NumOps::SConvert<s_int_16>(propInfo.Value_UInt8(aStruct)));  break;
            case GpType::S_INT_8:   aDbQuery.NextInt16(NumOps::SConvert<s_int_16>(propInfo.Value_SInt8(aStruct)));  break;
            case GpType::U_INT_16:  aDbQuery.NextInt16(NumOps::SConvert<s_int_16>(propInfo.Value_UInt16(aStruct))); break;
            case GpType::S_INT_16:  aDbQuery.NextInt16(NumOps::SConvert<s_int_16>(propInfo.Value_SInt16(aStruct))); break;
            case GpType::U_INT_32:  aDbQuery.NextInt32(NumOps::SConvert<s_int_32>(propInfo.Value_UInt32(aStruct))); break;
            case GpType::S_INT_32:  aDbQuery.NextInt32(NumOps::SConvert<s_int_32>(propInfo.Value_SInt32(aStruct))); break;
            case GpType::U_INT_64:  aDbQuery.NextInt64(NumOps::SConvert<s_int_64>(propInfo.Value_UInt64(aStruct))); break;
            case GpType::S_INT_64:  aDbQuery.NextInt64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt64(aStruct))); break;
            case GpType::UNIX_TS_S: aDbQuery.NextInt64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt64(aStruct))); break;
            case GpType::UNIX_TS_MS:aDbQuery.NextInt64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt64(aStruct))); break;
            case GpType::DOUBLE:    THROW_GPE("Unsupported type DOUBLE"_sv); break;
            case GpType::FLOAT:     THROW_GPE("Unsupported type FLOAT"_sv); break;
            case GpType::BOOLEAN:   aDbQuery.NextBoolean(propInfo.Value_Bool(aStruct)); break;
            case GpType::UUID:      aDbQuery.NextUUID(propInfo.Value_UUID(aStruct)); break;
            case GpType::STRING:    aDbQuery.NextStrValue(propInfo.Value_String(aStruct)); break;
            case GpType::BLOB:      aDbQuery.NextBLOB(propInfo.Value_BLOB(aStruct)); break;
            case GpType::STRUCT:    aDbQuery.NextStrJson(GpJsonMapper::SToJson(propInfo.Value_Struct(aStruct), {GpJsonMapperFlag::WRITE_STRUCT_UID})); break;
            case GpType::STRUCT_SP:
            {
                const auto& structSP = propInfo.Value_StructSP(aStruct);
                if (structSP.IsNULL()) aDbQuery.NextNULL();
                else aDbQuery.NextStrJson(GpJsonMapper::SToJson(structSP.VCn(), {GpJsonMapperFlag::WRITE_STRUCT_UID}));
            } break;
            case GpType::ENUM:      aDbQuery.NextStrValue(propInfo.Value_Enum(aStruct).ToString()); break;
            case GpType::ENUM_FLAGS:aDbQuery.NextStrValueArray(propInfo.Value_EnumFlags(aStruct));  break;
            case GpType::NOT_SET:
            default:
            {
                THROW_GPE("Unsupported type NOT_SET"_sv); break;
            }
        }
    }
}

s_int_64    GpDbQueryMapper::SRowToVersion
(
    const GpDbQueryRes& aDbQueryRes,
    const count_t       aRowId,
    const count_t       aColOffset
)
{
    const count_t   colId   = aColOffset;
    const s_int_64  version = aDbQueryRes.GetInt64(aRowId, colId, std::nullopt);

    return version;
}

count_t GpDbQueryMapper::SRowToStruct
(
    const GpDbQueryRes& aDbQueryRes,
    GpTypeStructBase&   aStruct,
    const count_t       aRowId,
    const count_t       aColOffset
)
{
    const GpTypeStructInfo& typeInfo    = aStruct.TypeInfo();
    count_t                 colId       = aColOffset;

    for (const GpTypePropInfo& propInfo: typeInfo.Props())
    {
        THROW_GPE_COND
        (
            propInfo.Container() == GpTypeContainer::NO,
            [&](){return "Container of property '"_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "' must be NO"_sv;}
        );

        switch (propInfo.Type())
        {
            case GpType::U_INT_8:   propInfo.Value_UInt8(aStruct)  = NumOps::SConvert<u_int_8> (aDbQueryRes.GetInt16(aRowId, colId++, s_int_16(0))); break;
            case GpType::S_INT_8:   propInfo.Value_SInt8(aStruct)  = NumOps::SConvert<s_int_8> (aDbQueryRes.GetInt16(aRowId, colId++, s_int_16(0))); break;
            case GpType::U_INT_16:  propInfo.Value_UInt16(aStruct) = NumOps::SConvert<u_int_16>(aDbQueryRes.GetInt16(aRowId, colId++, s_int_16(0))); break;
            case GpType::S_INT_16:  propInfo.Value_SInt16(aStruct) = NumOps::SConvert<s_int_16>(aDbQueryRes.GetInt16(aRowId, colId++, s_int_16(0))); break;
            case GpType::U_INT_32:  propInfo.Value_UInt32(aStruct) = NumOps::SConvert<u_int_32>(aDbQueryRes.GetInt32(aRowId, colId++, s_int_32(0))); break;
            case GpType::S_INT_32:  propInfo.Value_SInt32(aStruct) = NumOps::SConvert<s_int_32>(aDbQueryRes.GetInt32(aRowId, colId++, s_int_32(0))); break;
            case GpType::U_INT_64:  propInfo.Value_UInt64(aStruct) = NumOps::SConvert<u_int_64>(aDbQueryRes.GetInt64(aRowId, colId++, s_int_64(0))); break;
            case GpType::S_INT_64:  propInfo.Value_SInt64(aStruct) = NumOps::SConvert<s_int_64>(aDbQueryRes.GetInt64(aRowId, colId++, s_int_64(0))); break;
            case GpType::UNIX_TS_S: propInfo.Value_SInt64(aStruct) = NumOps::SConvert<s_int_64>(aDbQueryRes.GetInt64(aRowId, colId++, s_int_64(0))); break;
            case GpType::UNIX_TS_MS:propInfo.Value_SInt64(aStruct) = NumOps::SConvert<s_int_64>(aDbQueryRes.GetInt64(aRowId, colId++, s_int_64(0))); break;
            case GpType::DOUBLE:    THROW_GPE("Unsupported type DOUBLE"_sv); break;
            case GpType::FLOAT:     THROW_GPE("Unsupported type FLOAT"_sv); break;
            case GpType::BOOLEAN:   propInfo.Value_Bool(aStruct) = aDbQueryRes.GetBoolean(aRowId, colId++, false);  break;
            case GpType::UUID:      propInfo.Value_UUID(aStruct) = aDbQueryRes.GetUUID(aRowId, colId++, GpUUID());  break;
            case GpType::STRING:    propInfo.Value_String(aStruct) = aDbQueryRes.GetStr(aRowId, colId++, ""_sv); break;
            case GpType::BLOB:      propInfo.Value_BLOB(aStruct) = GpBytesArrayUtils::SMake(aDbQueryRes.GetBLOB(aRowId, colId++, {})); break;
            case GpType::STRUCT:
            {
                std::string_view    jsonStr     = aDbQueryRes.GetJsonStr(aRowId, colId++, ""_sv);
                auto&               structVal   = propInfo.Value_Struct(aStruct);

                if (jsonStr.length() > 0)
                {
                    GpJsonMapper::SFromJson(jsonStr, structVal);
                }
            } break;
            case GpType::STRUCT_SP:
            {
                std::string_view    jsonStr     = aDbQueryRes.GetJsonStr(aRowId, colId++, ""_sv);
                auto&               structVal   = propInfo.Value_StructSP(aStruct);

                if (jsonStr.length() > 0)
                {
                    const GpTypeStructInfo& structTypeInfo = GpTypeManager::S().Find(propInfo.TypeUID()).value();
                    structVal = GpJsonMapper::SFromJson(jsonStr, structTypeInfo);
                } else
                {
                    structVal.Clear();
                }
            } break;
            case GpType::ENUM:      propInfo.Value_Enum(aStruct).FromString(aDbQueryRes.GetStr(aRowId, colId++, std::nullopt));break;
            case GpType::ENUM_FLAGS:propInfo.Value_EnumFlags(aStruct).FromStringArray(aDbQueryRes.GetStrArray(aRowId, colId++, std::nullopt));break;
            case GpType::NOT_SET:               
            default:
            {
                THROW_GPE("Unsupported type NOT_SET"_sv); break;
            }
        }
    }

    return colId;
}

void    GpDbQueryMapper::SJsonToStruct
(
    const GpDbQueryRes& aDbQueryRes,
    GpTypeStructBase&   aStruct,
    const count_t       aRowId,
    const count_t       aColOffset
)
{
    std::string_view jsonStr = aDbQueryRes.GetJsonStr(aRowId, aColOffset, std::nullopt);
    GpJsonMapper::SFromJson(jsonStr, aStruct);
}

}//namespace GPlatform
