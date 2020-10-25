#pragma once

#include "GpDbQueryMapperCache.hpp"
#include "GpDbQueryBuilder.hpp"
#include "../GpDbConnection.hpp"

namespace GPlatform {

class GpDbConnection;
class GpDbQueryRes;

class GPDBCLIENT_API GpDbQueryMapper
{
    CLASS_REMOVE_CTRS(GpDbQueryMapper)

public:
    //*************************************** ROW **********************************************

    static void                 SInsertAsRow                (const GpTypeStructBase&    aStruct,
                                                             std::string_view           aTablePath,
                                                             GpDbConnection&            aDbConn);

    template<typename... KeysT> static
    SInt64/*version*/           SSelectByKeysAsRow          (GpTypeStructBase&                                  aStruct,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename T, typename... KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsRow          (std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename... KeysT> static
    void                        SUpdateByKeysAsRow          (const GpTypeStructBase&                            aStruct,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    //*************************************** JSON **********************************************

    static void                 SInsertAsJsonb              (const GpTypeStructBase&    aStruct,
                                                             std::string_view           aTablePath,
                                                             GpDbConnection&            aDbConn,
                                                             const GpJsonMapperFlags    aFlags);

    template<typename... KeysT> static
    SInt64/*version*/           SSelectByKeysAsJsonb        (GpTypeStructBase&                                  aStruct,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename T, typename... KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsJsonb        (std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename... KeysT> static
    void                        SUpdateByKeysAsJsonb        (const GpTypeStructBase&                            aStruct,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpJsonMapperFlags                            aFlags,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

private:
    static void                 SWriteRowValues             (GpDbQuery&                 aDbQuery,
                                                             const GpTypeStructBase&    aStruct);
    static SInt64               SRowToVersion               (const GpDbQueryRes&    aDbQueryRes,
                                                             const count_t          aColOffset);
    static void                 SRowToStruct                (const GpDbQueryRes&    aDbQueryRes,
                                                             GpTypeStructBase&      aStruct,
                                                             const count_t          aColOffset);
    static void                 SJsonToStruct               (GpDbQueryRes&          aDbQueryRes,
                                                             GpTypeStructBase&      aStruct,
                                                             const count_t          aColOffset);

private:
    static GpDbQueryMapperCache sMapperCache;
};

template<typename... KeysT>
SInt64  GpDbQueryMapper::SSelectByKeysAsRow (GpTypeStructBase&                                  aStruct,
                                             std::string_view                                   aTablePath,
                                             GpDbConnection&                                    aDbConn,
                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                             KeysT...                                           aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeStructInfo(),
                                                                 "SSelectByKeysAsRow:"_sv + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 aTablePath,
                                                                 [&](const GpTypeStructInfo& aStructInfo)
    {
        GpDbQueryBuilder builder;

        builder
            .SELECT()
                .PARAM_NAME({"_version"_sv})
                .COMMA().STRUCT_PARAM_NAMES(aStructInfo)
            .FROM(aTablePath)
            .WHERE();

            for (size_t id = 0; id < sizeof...(KeysT); ++id)
            {
                if (id > 0) builder.AND();
                builder.PARAM_NAME(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
            }

        return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
    });

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.Nexts<KeysT...>(aKeysValues...);
    GpDbQueryRes::SP dbQueryRes = aDbConn.Execute(dbQuery, 1_cnt);

    //Read
    const SInt64 version = SRowToVersion(dbQueryRes.VCn(), 0_cnt);
    SRowToStruct(dbQueryRes.VCn(), aStruct, 1_cnt);

    return version;
}

template<typename T, typename... KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsRow (std::string_view                                   aTablePath,
                                     GpDbConnection&                                    aDbConn,
                                     const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                     KeysT...                                           aKeysValues)
{
    typename T::SP res = MakeSP<T>();
    const SInt64 version = SSelectByKeysAsRow<KeysT...>(res.Vn(), aTablePath, aDbConn, aKeysNames, aKeysValues...);
    return {res, version};
}

template<typename... KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsRow (const GpTypeStructBase&                            aStruct,
                                             const SInt64                                       aVersion,
                                             std::string_view                                   aTablePath,
                                             GpDbConnection&                                    aDbConn,
                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                             KeysT...                                           aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeStructInfo(),
                                                                 "SUpdateByKeysAsRow:"_sv + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 aTablePath,
                                                                 [&](const GpTypeStructInfo& aStructInfo)
    {
        GpDbQueryBuilder builder;

        builder
            .UPDATE(aTablePath)
            .SET()
                .STRUCT_PARAM_ASSIGN(aStructInfo)
                .COMMA().PARAM_NAME("_version"_sv).ASSIGN().VALUE(GpDbQueryValType::INT_64)
            .WHERE();
                for (size_t id = 0; id < sizeof...(KeysT); ++id)
                {
                    if (id > 0) builder.AND();
                    builder.PARAM_NAME(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
                }
        builder
                .AND().PARAM_NAME("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
            .RETURNING().PARAM_NAME("_version"_sv);

        return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
    });

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    SWriteRowValues(dbQuery, aStruct);
    dbQuery.NextInt64(aVersion + 1_s_int_64);
    dbQuery.Nexts<KeysT...>(std::forward<KeysT...>(aKeysValues...));
    dbQuery.NextInt64(aVersion);
    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename... KeysT>
SInt64  GpDbQueryMapper::SSelectByKeysAsJsonb (GpTypeStructBase&                                    aStruct,
                                               std::string_view                                     aTablePath,
                                               GpDbConnection&                                      aDbConn,
                                               const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                               KeysT...                                             aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeStructInfo(),
                                                                 "SSelectByKeysAsJsonb:"_sv + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 aTablePath,
                                                                 [&](const GpTypeStructInfo& /*aStructInfo*/)
    {
        GpDbQueryBuilder builder;

        builder
            .SELECT()
                .PARAM_NAME({"_version"_sv})
                .COMMA().PARAM_NAME({"data"_sv})
            .FROM(aTablePath)
            .WHERE();

            for (size_t id = 0; id < sizeof...(KeysT); ++id)
            {
                if (id > 0) builder.AND();
                builder.PARAM_NAME(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
            }

        return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
    });

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.Nexts<KeysT...>(std::forward<KeysT...>(aKeysValues...));
    GpDbQueryRes::SP dbQueryRes = aDbConn.Execute(dbQuery, 1_cnt);

    //Read
    const SInt64 version = SRowToVersion(dbQueryRes.VCn(), 0_cnt);
    SJsonToStruct(dbQueryRes.Vn(), aStruct, 1_cnt);

    return version;
}

template<typename T, typename... KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsJsonb (std::string_view                                     aTablePath,
                                       GpDbConnection&                                      aDbConn,
                                       const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                       KeysT...                                             aKeysValues)
{
    typename T::SP res = MakeSP<T>();
    const SInt64 version = SSelectByKeysAsJsonb<KeysT...>(res.Vn(), aTablePath, aDbConn, aKeysNames, std::forward<KeysT...>(aKeysValues...));
    return {res, version};
}

template<typename... KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsJsonb (const GpTypeStructBase&                              aStruct,
                                               const SInt64                                         aVersion,
                                               std::string_view                                     aTablePath,
                                               GpDbConnection&                                      aDbConn,
                                               const GpJsonMapperFlags                              aFlags,
                                               const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                               KeysT...                                             aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeStructInfo(),
                                                                 "SUpdateByKeysAsJsonb:"_sv + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 aTablePath,
                                                                 [&](const GpTypeStructInfo& /*aStructInfo*/)
    {
        GpDbQueryBuilder builder;

        builder
            .UPDATE(aTablePath)
            .SET()
                .PARAM_NAME("data"_sv).ASSIGN().VALUE(GpDbQueryValType::STRING_JSON)
                .COMMA().PARAM_NAME("_version"_sv).ASSIGN().VALUE(GpDbQueryValType::INT_64)
            .WHERE();
                for (size_t id = 0; id < sizeof...(KeysT); ++id)
                {
                    if (id > 0) builder.AND();
                    builder.PARAM_NAME(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
                }
        builder
                .AND().PARAM_NAME("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
            .RETURNING().PARAM_NAME("_version"_sv);

        return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
    });

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.NextStrJson(GpJsonMapper::SToJson(aStruct, aFlags));
    dbQuery.NextInt64(aVersion + 1_s_int_64);
    dbQuery.Nexts<KeysT...>(aKeysValues...);
    dbQuery.NextInt64(aVersion);
    aDbConn.Execute(dbQuery, 1_cnt);
}

}//namespace GPlatform
