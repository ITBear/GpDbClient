#include "GpDbQueryMapper.hpp"

namespace GPlatform {

GpDbQueryMapperCache    GpDbQueryMapper::sMapperCache;

void    GpDbQueryMapper::SInsertAsRow (const GpTypeStructBase&  aStruct,
                                       std::string_view         aTablePath,
                                       GpDbConnection&          aDbConn)
{
    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeInfo(),
                                                                 "GpDbQueryMapper::SInsertAsRow|"_sv + aTablePath,
                                                                 [&](const GpTypeStructInfo& aTypeInfo)
    {
        GpDbQueryBuilder builder;

        builder
            .INSERT_INTO(aTablePath)
            .BRACE_BEGIN()
                .STRUCT_PARAM_NAMES(aTypeInfo)
            .BRACE_END()
            .VALUES_BEGIN()
                .STRUCT_PARAM_BINDS(aTypeInfo)
            .VALUES_END();

        return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
    });

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    SWriteRowValues(dbQuery, aStruct);
    aDbConn.Execute(dbQuery, 0_cnt);
}

void    GpDbQueryMapper::SInsertAsJsonb (const GpTypeStructBase&    aStruct,
                                         std::string_view           aTablePath,
                                         GpDbConnection&            aDbConn,
                                         const GpJsonMapperFlags    aFlags)
{
    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeInfo(),
                                                                 "GpDbQueryMapper::SInsertAsJsonb|"_sv + aTablePath,
                                                                 [&](const GpTypeStructInfo&)
    {
        GpDbQueryBuilder builder;

        builder
            .INSERT_INTO(aTablePath)
            .BRACE_BEGIN()
                .PARAM_NAME("data"_sv)
            .BRACE_END()
            .VALUES_BEGIN()
                .VALUE(GpDbQueryValType::STRING_JSON)
            .VALUES_END();

        return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
    });

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.NextStrJson(GpJsonMapper::SToJson(aStruct, aFlags));
    aDbConn.Execute(dbQuery, 0_cnt);
}

void    GpDbQueryMapper::SWriteRowValues (GpDbQuery&                aDbQuery,
                                          const GpTypeStructBase&   aStruct)
{
    const GpTypeStructInfo& typeInfo = aStruct.TypeInfo();

    for (const GpTypePropInfo& propInfo: typeInfo.Props())
    {
        THROW_GPE_COND_CHECK_M(propInfo.Container() == GpTypeContainer::NO,
                               "Container of property '"_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "' must be NO");

        switch (propInfo.Type())
        {
            case GpType::U_INT_8:   aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_UInt8(aStruct)));   break;
            case GpType::S_INT_8:   aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_SInt8(aStruct)));   break;
            case GpType::U_INT_16:  aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_UInt16(aStruct)));  break;
            case GpType::S_INT_16:  aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_SInt16(aStruct)));  break;
            case GpType::U_INT_32:  aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_UInt32(aStruct)));  break;
            case GpType::S_INT_32:  aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_SInt32(aStruct)));  break;
            case GpType::U_INT_64:  aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_UInt64(aStruct)));  break;
            case GpType::S_INT_64:  aDbQuery.NextInt64(SInt64::SMake(propInfo.Value_SInt64(aStruct)));  break;
            case GpType::DOUBLE:    THROW_GPE("Unsupported type DOUBLE"_sv); break;
            case GpType::FLOAT:     THROW_GPE("Unsupported type FLOAT"_sv); break;
            case GpType::BOOLEAN:   aDbQuery.NextBoolean(propInfo.Value_Bool(aStruct)); break;
            case GpType::UUID:      aDbQuery.NextUUID(propInfo.Value_UUID(aStruct)); break;
            case GpType::STRING:    aDbQuery.NextStrValue(propInfo.Value_String(aStruct)); break;
            case GpType::BLOB:      aDbQuery.NextBLOB(propInfo.Value_BLOB(aStruct)); break;
            case GpType::STRUCT:    THROW_GPE("Unsupported type STRUCT"_sv); break;
            case GpType::STRUCT_SP: THROW_GPE("Unsupported type STRUCT_SP"_sv); break;
            case GpType::ENUM:      aDbQuery.NextStrValue(propInfo.Value_Enum(aStruct).ToString()); break;
            case GpType::NOT_SET:
            default:
            {
                THROW_GPE("Unsupported type NOT_SET"_sv); break;
            }
        }
    }
}

SInt64  GpDbQueryMapper::SRowToVersion (const GpDbQueryRes& aDbQueryRes,
                                        const count_t       aColOffset)
{
    const count_t   colId   = aColOffset;
    const SInt64    version = aDbQueryRes.GetInt64(0_cnt, colId, std::nullopt);

    return version;
}

count_t GpDbQueryMapper::SRowToStruct (const GpDbQueryRes&  aDbQueryRes,
                                       GpTypeStructBase&    aStruct,
                                       const count_t        aRowId,
                                       const count_t        aColOffset)
{
    const GpTypeStructInfo& typeInfo    = aStruct.TypeInfo();
    count_t                 colId       = aColOffset;

    for (const GpTypePropInfo& propInfo: typeInfo.Props())
    {
        THROW_GPE_COND_CHECK_M(propInfo.Container() == GpTypeContainer::NO,
                               "Container of property '"_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "' must be NO");

        switch (propInfo.Type())
        {
            case GpType::U_INT_8:   propInfo.Value_UInt8(aStruct)  = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<u_int_8>();   break;
            case GpType::S_INT_8:   propInfo.Value_SInt8(aStruct)  = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<s_int_8>();   break;
            case GpType::U_INT_16:  propInfo.Value_UInt16(aStruct) = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<u_int_16>();  break;
            case GpType::S_INT_16:  propInfo.Value_SInt16(aStruct) = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<s_int_16>();  break;
            case GpType::U_INT_32:  propInfo.Value_UInt32(aStruct) = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<u_int_32>();  break;
            case GpType::S_INT_32:  propInfo.Value_SInt32(aStruct) = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<s_int_32>();  break;
            case GpType::U_INT_64:  propInfo.Value_UInt64(aStruct) = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<u_int_64>();  break;
            case GpType::S_INT_64:  propInfo.Value_SInt64(aStruct) = aDbQueryRes.GetInt64(aRowId, colId++, 0_s_int_64).As<s_int_64>();  break;
            case GpType::DOUBLE:    THROW_GPE("Unsupported type DOUBLE"_sv); break;
            case GpType::FLOAT:     THROW_GPE("Unsupported type FLOAT"_sv); break;
            case GpType::BOOLEAN:   propInfo.Value_Bool(aStruct) = aDbQueryRes.GetBoolean(aRowId, colId++, false);  break;
            case GpType::UUID:      propInfo.Value_UUID(aStruct) = aDbQueryRes.GetUUID(aRowId, colId++, GpUUID());  break;
            case GpType::STRING:    propInfo.Value_String(aStruct) = aDbQueryRes.GetStr(aRowId, colId++, ""_sv); break;
            case GpType::BLOB:      propInfo.Value_BLOB(aStruct) = GpBytesArrayUtils::SMake(aDbQueryRes.GetBLOB(aRowId, colId++, {})); break;
            case GpType::STRUCT:    THROW_GPE("Unsupported type STRUCT"_sv); break;
            case GpType::STRUCT_SP: THROW_GPE("Unsupported type STRUCT_SP"_sv); break;
            case GpType::ENUM:      propInfo.Value_Enum(aStruct).FromString(aDbQueryRes.GetStr(aRowId, colId++, std::nullopt));break;
            case GpType::NOT_SET:
            default:
            {
                THROW_GPE("Unsupported type NOT_SET"_sv); break;
            }
        }
    }

    return colId;
}

void    GpDbQueryMapper::SJsonToStruct (GpDbQueryRes&       aDbQueryRes,
                                        GpTypeStructBase&   aStruct,
                                        const count_t       aRowId,
                                        const count_t       aColOffset)
{
    GpRawPtrCharRW jsonStrRW = aDbQueryRes.GetJsonStrRW(aRowId, aColOffset, std::nullopt);
    GpJsonMapper::SFromJsonInsitu(jsonStrRW, aStruct);
}

}//namespace GPlatform
