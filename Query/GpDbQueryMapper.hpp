#pragma once

#include "GpDbQueryMapperCache.hpp"
#include "GpDbQueryBuilder.hpp"
#include "../GpDbConnection.hpp"
#include "../GpDbException.hpp"

namespace GPlatform {

class GpDbConnection;
class GpDbQueryRes;

class GPDBCLIENT_API GpDbQueryMapper
{
    CLASS_REMOVE_CTRS(GpDbQueryMapper)

public:
    static GpDbQueryMapperCache&    SMapperCache            (void) noexcept {return sMapperCache;}

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

    template<typename       T,
             typename       TThrowOnNotFound,
             typename...    KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsRowCTNR      (std::string_view                                   aTablePath,
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

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SUpdateByKeysAsRowCTNR      (const GpTypeStructBase&                            aStruct,
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

    template<typename       T,
             typename       TThrowOnNotFound,
             typename...    KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsJsonbCTNR    (std::string_view                                   aTablePath,
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

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SUpdateByKeysAsJsonbCTNR    (const GpTypeStructBase&                            aStruct,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpJsonMapperFlags                            aFlags,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    //*************************************** Utils **********************************************

    static void                 SWriteRowValues             (GpDbQuery&                 aDbQuery,
                                                             const GpTypeStructBase&    aStruct);
    static SInt64               SRowToVersion               (const GpDbQueryRes&    aDbQueryRes,
                                                             const count_t          aColOffset);
    static count_t              SRowToStruct                (const GpDbQueryRes&    aDbQueryRes,
                                                             GpTypeStructBase&      aStruct,
                                                             const count_t          aRowId,
                                                             const count_t          aColOffset);
    static void                 SJsonToStruct               (GpDbQueryRes&          aDbQueryRes,
                                                             GpTypeStructBase&      aStruct,
                                                             const count_t          aRowId,
                                                             const count_t          aColOffset);

    inline static void          SThrowOnNoResult            (std::function<void()> aSqlFn,
                                                             std::function<void()> aThrowFn);

    template<typename T>
    static T                    SThrowOnNoResult            (std::function<T()> aSqlFn,
                                                             std::function<void()> aThrowFn);

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

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeInfo(),
                                                                 "SSelectByKeysAsRow|"_sv + aTablePath + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 [&](const GpTypeStructInfo& aTypeInfo)
    {
        GpDbQueryBuilder builder;

        builder
            .SELECT()
                .PARAM_NAME({"_version"_sv})
                .COMMA().STRUCT_PARAM_NAMES(aTypeInfo)
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
    SRowToStruct(dbQueryRes.VCn(), aStruct, 0_cnt, 1_cnt);

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

template<typename       T,
         typename       TThrowOnNotFound,
         typename...    KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsRowCTNR (std::string_view                                   aTablePath,
                                         GpDbConnection&                                    aDbConn,
                                         const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                         KeysT...                                           aKeysValues)
{
    return SThrowOnNoResult<std::tuple<typename T::SP, SInt64>>
    (
        [&]()
        {
            return GpDbQueryMapper::SSelectByKeysAsRow<T, KeysT...>(aTablePath,
                                                                    aDbConn,
                                                                    aKeysNames,
                                                                    aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
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

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeInfo(),
                                                                 "SUpdateByKeysAsRow|"_sv + aTablePath + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 [&](const GpTypeStructInfo& aTypeInfo)
    {
        GpDbQueryBuilder builder;

        builder
            .UPDATE(aTablePath)
            .SET()
                .STRUCT_PARAM_ASSIGN(aTypeInfo)
                .COMMA().INC_VERSION()
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
    dbQuery.Nexts<KeysT...>(std::forward<KeysT...>(aKeysValues...));
    dbQuery.NextInt64(aVersion);
    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsRowCTNR (const GpTypeStructBase&                            aStruct,
                                                 const SInt64                                       aVersion,
                                                 std::string_view                                   aTablePath,
                                                 GpDbConnection&                                    aDbConn,
                                                 const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                 KeysT...                                           aKeysValues)
{
    SThrowOnNoResult<void>
    (
        [&]()
        {
            GpDbQueryMapper::SUpdateByKeysAsRow<KeysT...>(aStruct,
                                                          aVersion,
                                                          aTablePath,
                                                          aDbConn,
                                                          aKeysNames,
                                                          aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

template<typename... KeysT>
SInt64  GpDbQueryMapper::SSelectByKeysAsJsonb (GpTypeStructBase&                                    aStruct,
                                               std::string_view                                     aTablePath,
                                               GpDbConnection&                                      aDbConn,
                                               const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                               KeysT...                                             aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeInfo(),
                                                                 "SSelectByKeysAsJsonb|"_sv + aTablePath + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 [&](const GpTypeStructInfo&)
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
    SJsonToStruct(dbQueryRes.Vn(), aStruct, 0_cnt, 1_cnt);

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

template<typename       T,
         typename       TThrowOnNotFound,
         typename...    KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsJsonbCTNR (std::string_view                                     aTablePath,
                                           GpDbConnection&                                      aDbConn,
                                           const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                           KeysT...                                             aKeysValues)
{
    return SThrowOnNoResult<std::tuple<typename T::SP, SInt64>>
    (
        [&]()
        {
            return GpDbQueryMapper::SSelectByKeysAsJsonb<T, KeysT...>(aTablePath,
                                                                      aDbConn,
                                                                      aKeysNames,
                                                                      aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
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

    const GpDbQueryMapperCacheValue& cacheVal = sMapperCache.Get(aStruct.TypeInfo(),
                                                                 "SUpdateByKeysAsJsonb|"_sv + aTablePath + GpStringOps::SJoin<std::string_view>(aKeysNames, "|"_sv),
                                                                 [&](const GpTypeStructInfo&)
    {
        GpDbQueryBuilder builder;

        builder
            .UPDATE(aTablePath)
            .SET()
                .PARAM_NAME("data"_sv).ASSIGN().VALUE(GpDbQueryValType::STRING_JSON)
                .COMMA().INC_VERSION()
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
    dbQuery.Nexts<KeysT...>(aKeysValues...);
    dbQuery.NextInt64(aVersion);
    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsJsonbCTNR (const GpTypeStructBase&                              aStruct,
                                                   const SInt64                                         aVersion,
                                                   std::string_view                                     aTablePath,
                                                   GpDbConnection&                                      aDbConn,
                                                   const GpJsonMapperFlags                              aFlags,
                                                   const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                                   KeysT...                                             aKeysValues)
{
    SThrowOnNoResult<void>
    (
        [&]()
        {
            GpDbQueryMapper::SUpdateByKeysAsJsonb<KeysT...>(aStruct,
                                                            aVersion,
                                                            aTablePath,
                                                            aDbConn,
                                                            aFlags,
                                                            aKeysNames,
                                                            aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

void    GpDbQueryMapper::SThrowOnNoResult (std::function<void()> aSqlFn,
                                           std::function<void()> aThrowFn)
{
    try
    {
        aSqlFn();
    } catch (const GpDbException& e)
    {
        if (e.Code() == GpDbExceptionCode::EMPTY_QUERY_RES)
        {
            aThrowFn();
        } else
        {
            throw;
        }
    }
}

template<typename T>
T   GpDbQueryMapper::SThrowOnNoResult (std::function<T()> aSqlFn,
                                       std::function<void()> aThrowFn)
{
    try
    {
        return aSqlFn();
    } catch (const GpDbException& e)
    {
        if (e.Code() == GpDbExceptionCode::EMPTY_QUERY_RES)
        {
            aThrowFn();
        }

        throw;
    }
}

}//namespace GPlatform
