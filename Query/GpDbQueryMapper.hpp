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
public:
    CLASS_REMOVE_CTRS(GpDbQueryMapper)

    using PagingCounterID   = std::tuple<std::variant<std::string_view, GpUUID>/*uid param name/value*/,
                                         std::string_view/*name*/>;
    using PagingCounterIDS  = GpVector<PagingCounterID>;

    template<typename ROW_T>
    using ReadFnSPT         = std::function<typename ROW_T::SP(const count_t aRowId, const GpDbQueryRes& aDbQueryRes)>;

    template<typename ROW_T>
    using ReadFnRefT        = std::function<ROW_T(const count_t aRowId, const GpDbQueryRes& aDbQueryRes)>;

public:
    static GpDbQueryMapperCache&    SMapperCache            (void) noexcept {return sMapperCache;}

    static SInt64               SSelectPagingStartPoint     (const GpDbQueryCacheUID&   aCacheUID,
                                                             std::string_view           aTablePath,
                                                             GpDbConnection&            aDbConn,
                                                             std::string_view           aPagingCounterName,
                                                             std::string_view           aTimestampName,
                                                             const unix_ts_s_t          aTimestamp);

    static SInt64               SSelectPagingStartPoint     (const GpDbQueryCacheUID&   aCacheUID,
                                                             std::string_view           aTablePath,
                                                             GpDbConnection&            aDbConn,
                                                             std::string_view           aPagingCounterName,
                                                             std::string_view           aUidParamName,
                                                             const GpUUID&              aUid,
                                                             std::string_view           aTimestampName,
                                                             const unix_ts_s_t          aTimestamp);


    static void                 SCreatePagingOrderCounter   (const GpUUID&      aUid,
                                                             std::string_view   aName,
                                                             GpDbConnection&    aDbConn);

    template<typename... KeysT> static
    void                        SSetPagingOrder             (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SSetPagingOrderCTNR         (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    //*************************************** ROW **********************************************
    template<typename... KeysT> static
    void                        SSetPagingOrderRow          (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SSetPagingOrderRowCTNR      (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    static void                 SInsertAsRow                (const GpDbQueryCacheUID&   aCacheUID,
                                                             const GpTypeStructBase&    aStruct,
                                                             std::string_view           aTablePath,
                                                             GpDbConnection&            aDbConn);

    static void                 SInsertAsRowVec             (const GpDbQueryCacheUID&               aCacheUID,
                                                             const GpTypeStructBase::C::Vec::SP&    aStructVec,
                                                             std::string_view                       aTablePath,
                                                             GpDbConnection&                        aDbConn);

    template<typename... KeysT> static
    SInt64/*version*/           SSelectByKeysAsRow          (const GpDbQueryCacheUID&                           aCacheUID,
                                                             GpTypeStructBase&                                  aStruct,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename T, typename... KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsRow          (const GpDbQueryCacheUID&                           aCacheUID,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       T,
             typename       TThrowOnNotFound,
             typename...    KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsRowCTNR      (const GpDbQueryCacheUID&                           aCacheUID,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename... KeysT> static
    void                        SUpdateByKeysAsRow          (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SUpdateByKeysAsRowCTNR      (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    //*************************************** JSON **********************************************
    template<typename... KeysT> static
    void                        SSetPagingOrderJsonb        (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const GpJsonMapperFlags                            aFlags,
                                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SSetPagingOrderJsonbCTNR    (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const GpJsonMapperFlags                            aFlags,
                                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    static void                 SInsertAsJsonb              (const GpDbQueryCacheUID&   aCacheUID,
                                                             const GpTypeStructBase&    aStruct,
                                                             std::string_view           aTablePath,
                                                             GpDbConnection&            aDbConn,
                                                             const GpJsonMapperFlags    aFlags);

    template<typename... KeysT> static
    SInt64/*version*/           SSelectByKeysAsJsonb        (const GpDbQueryCacheUID&                           aCacheUID,
                                                             GpTypeStructBase&                                  aStruct,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename T, typename... KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsJsonb        (const GpDbQueryCacheUID&                           aCacheUID,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       T,
             typename       TThrowOnNotFound,
             typename...    KeysT> static
    std::tuple<typename T::SP, SInt64/*version*/>
                                SSelectByKeysAsJsonbCTNR    (const GpDbQueryCacheUID&                           aCacheUID,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename... KeysT> static
    void                        SUpdateByKeysAsJsonb        (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const GpTypeStructBase&                            aStruct,
                                                             const SInt64                                       aVersion,
                                                             std::string_view                                   aTablePath,
                                                             GpDbConnection&                                    aDbConn,
                                                             const GpJsonMapperFlags                            aFlags,
                                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                             KeysT...                                           aKeysValues);

    template<typename       TThrowOnNotFound,
             typename...    KeysT> static
    void                        SUpdateByKeysAsJsonbCTNR    (const GpDbQueryCacheUID&                           aCacheUID,
                                                             const GpTypeStructBase&                            aStruct,
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
                                                             const count_t          aRowId,
                                                             const count_t          aColOffset);
    static count_t              SRowToStruct                (const GpDbQueryRes&    aDbQueryRes,
                                                             GpTypeStructBase&      aStruct,
                                                             const count_t          aRowId,
                                                             const count_t          aColOffset);
    static void                 SJsonToStruct               (const GpDbQueryRes&    aDbQueryRes,
                                                             GpTypeStructBase&      aStruct,
                                                             const count_t          aRowId,
                                                             const count_t          aColOffset);

    inline static void          SThrowOnNoResult            (std::function<void()> aSqlFn,
                                                             std::function<void()> aThrowFn);

    template<typename T >static
    T                           SThrowOnNoResult            (std::function<T()> aSqlFn,
                                                             std::function<void()> aThrowFn);

    template<typename T> static
    typename T::C::Vec::SP      SReadQueryResVecSP          (const GpDbQueryRes&    aDbQueryRes,
                                                             ReadFnSPT<T>           aParserFn);

    template<typename T> static
    T                           SReadQueryResOne            (const GpDbQueryRes&    aDbQueryRes,
                                                             ReadFnRefT<T>          aParserFn);

private:
    static GpDbQueryMapperCache sMapperCache;
};

template<typename... KeysT>
void    GpDbQueryMapper::SSetPagingOrder (const GpDbQueryCacheUID&                              aCacheUID,
                                          const SInt64                                          aVersion,
                                          std::string_view                                      aTablePath,
                                          GpDbConnection&                                       aDbConn,
                                          const PagingCounterIDS&                               aPagingCounterIDS,
                                          const GpArray<std::string_view, sizeof...(KeysT)>&    aKeysNames,
                                          KeysT...                                              aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            //UPDATE
            //  aTablePath
            //SET
            //  _version=_version+1
            //  ,aCounterNames... = next_paging_order_val($aCounterIds, aTablePath+"."+aCounterNames)...
            //WHERE
            //      aKeysNames... = $aKeysValues...
            //  AND _version      = $aVersion
            //RETURNING
            //  _version

            GpDbQueryBuilder builder;

            builder
                .UPDATE(aTablePath)
                .SET()
                    .INC_VERSION();

            for (auto[uidParam, counterName]: aPagingCounterIDS)
            {
                builder
                    .COMMA().PARAM(counterName).ASSIGN()
                        .RAW("next_paging_order_val("_sv);

                if (std::holds_alternative<std::string_view>(uidParam))
                {
                    builder.PARAM(std::get<std::string_view>(uidParam));
                } else
                {
                    builder.VALUE(GpDbQueryValType::UUID);
                }

                builder
                    .COMMA()
                        .RAW("'"_sv + aTablePath + "."_sv + counterName + "'"_sv).RAW(")"_sv);
            }

            builder
                .WHERE();

            for (size_t id = 0; id < sizeof...(KeysT); ++id)
            {
                if (id > 0) builder.AND();
                builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
            }

            builder
                .AND().PARAM("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
                .RETURNING().PARAM("_version"_sv);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);

    for (auto[uidParam, counterName]: aPagingCounterIDS)
    {
        if (std::holds_alternative<GpUUID>(uidParam))
        {
            dbQuery.NextUUID(std::get<GpUUID>(uidParam));
        }
    }

    dbQuery.Nexts<KeysT...>(aKeysValues...);
    dbQuery.NextInt64(aVersion);

    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SSetPagingOrderCTNR (const GpDbQueryCacheUID&                              aCacheUID,
                                              const SInt64                                          aVersion,
                                              std::string_view                                      aTablePath,
                                              GpDbConnection&                                       aDbConn,
                                              const PagingCounterIDS&                               aPagingCounterIDS,
                                              const GpArray<std::string_view, sizeof...(KeysT)>&    aKeysNames,
                                              KeysT...                                              aKeysValues)
{
    return SThrowOnNoResult
    (
        [&]()
        {
            GpDbQueryMapper::SSetPagingOrder<KeysT...>(aCacheUID,
                                                       aVersion,
                                                       aTablePath,
                                                       aDbConn,
                                                       aPagingCounterIDS,
                                                       aKeysNames,
                                                       aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

template<typename... KeysT>
void    GpDbQueryMapper::SSetPagingOrderRow (const GpDbQueryCacheUID&                           aCacheUID,
                                             const SInt64                                       aVersion,
                                             std::string_view                                   aTablePath,
                                             GpDbConnection&                                    aDbConn,
                                             const GpTypeStructBase&                            aStruct,
                                             const PagingCounterIDS&                            aPagingCounterIDS,
                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                             KeysT...                                           aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            //UPDATE
            //  aTablePath
            //SET
            //   _version=_version+1
            //  ,data=$1::jsonb
            //  ,aCounterNames... = next_paging_order_val($aCounterIds, aTablePath+"."+aCounterNames)...
            //
            //WHERE
            //      aKeysNames... = $aKeysValues...
            //  AND _version      = $aVersion
            //RETURNING
            //  _version

            GpDbQueryBuilder builder;

            builder
                .UPDATE(aTablePath)
                .SET()
                    .INC_VERSION()
                    .COMMA().STRUCT_PARAM_ASSIGN(""_sv, aStruct.TypeInfo());

            for (auto[uidParam, counterName]: aPagingCounterIDS)
            {
                builder
                    .COMMA().PARAM(counterName).ASSIGN()
                        .RAW("next_paging_order_val("_sv);

                if (std::holds_alternative<std::string_view>(uidParam))
                {
                    builder.PARAM(std::get<std::string_view>(uidParam));
                } else
                {
                    builder.VALUE(GpDbQueryValType::UUID);
                }

                builder
                    .COMMA()
                        .RAW("'"_sv + aTablePath + "."_sv + counterName + "'"_sv).RAW(")"_sv);
            }

            builder
                .WHERE();

            for (size_t id = 0; id < sizeof...(KeysT); ++id)
            {
                if (id > 0) builder.AND();
                builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
            }

            builder
                .AND().PARAM("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
                .RETURNING().PARAM("_version"_sv);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);

    GpDbQueryMapper::SWriteRowValues(dbQuery, aStruct);

    for (auto[uidParam, counterName]: aPagingCounterIDS)
    {
        if (std::holds_alternative<GpUUID>(uidParam))
        {
            dbQuery.NextUUID(std::get<GpUUID>(uidParam));
        }
    }

    dbQuery.Nexts<KeysT...>(aKeysValues...);
    dbQuery.NextInt64(aVersion);

    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SSetPagingOrderRowCTNR (const GpDbQueryCacheUID&                           aCacheUID,
                                                 const SInt64                                       aVersion,
                                                 std::string_view                                   aTablePath,
                                                 GpDbConnection&                                    aDbConn,
                                                 const GpTypeStructBase&                            aStruct,
                                                 const PagingCounterIDS&                            aPagingCounterIDS,
                                                 const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                                 KeysT...                                           aKeysValues)
{
    return SThrowOnNoResult
    (
        [&]()
        {
            GpDbQueryMapper::SSetPagingOrderRow<KeysT...>(aCacheUID,
                                                          aVersion,
                                                          aTablePath,
                                                          aDbConn,
                                                          aStruct,
                                                          aPagingCounterIDS,
                                                          aKeysNames,
                                                          aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

template<typename... KeysT>
SInt64  GpDbQueryMapper::SSelectByKeysAsRow (const GpDbQueryCacheUID&                           aCacheUID,
                                             GpTypeStructBase&                                  aStruct,
                                             std::string_view                                   aTablePath,
                                             GpDbConnection&                                    aDbConn,
                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                             KeysT...                                           aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .SELECT()
                    .PARAM("_version"_sv)
                    .COMMA().STRUCT_PARAM_NAMES(""_sv, aStruct.TypeInfo())
                .FROM(aTablePath)
                .WHERE();

                for (size_t id = 0; id < sizeof...(KeysT); ++id)
                {
                    if (id > 0) builder.AND();
                    builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
                }

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.Nexts<KeysT...>(aKeysValues...);
    GpDbQueryRes::SP dbQueryRes = aDbConn.Execute(dbQuery, 1_cnt);

    //Read
    const SInt64 version = SRowToVersion(dbQueryRes.VCn(), 0_cnt, 0_cnt);
    SRowToStruct(dbQueryRes.VCn(), aStruct, 0_cnt, 1_cnt);

    return version;
}

template<typename T, typename... KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsRow (const GpDbQueryCacheUID&                           aCacheUID,
                                     std::string_view                                   aTablePath,
                                     GpDbConnection&                                    aDbConn,
                                     const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                     KeysT...                                           aKeysValues)
{
    typename T::SP res = MakeSP<T>();
    const SInt64 version = SSelectByKeysAsRow<KeysT...>(aCacheUID, res.Vn(), aTablePath, aDbConn, aKeysNames, aKeysValues...);
    return {res, version};
}

template<typename       T,
         typename       TThrowOnNotFound,
         typename...    KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsRowCTNR (const GpDbQueryCacheUID&                           aCacheUID,
                                         std::string_view                                   aTablePath,
                                         GpDbConnection&                                    aDbConn,
                                         const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                         KeysT...                                           aKeysValues)
{
    return SThrowOnNoResult<std::tuple<typename T::SP, SInt64>>
    (
        [&]()
        {
            return GpDbQueryMapper::SSelectByKeysAsRow<T, KeysT...>(aCacheUID,
                                                                    aTablePath,
                                                                    aDbConn,
                                                                    aKeysNames,
                                                                    aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

template<typename... KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsRow (const GpDbQueryCacheUID&                           aCacheUID,
                                             const GpTypeStructBase&                            aStruct,
                                             const SInt64                                       aVersion,
                                             std::string_view                                   aTablePath,
                                             GpDbConnection&                                    aDbConn,
                                             const GpArray<std::string_view, sizeof...(KeysT)>& aKeysNames,
                                             KeysT...                                           aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .UPDATE(aTablePath)
                .SET()
                    .STRUCT_PARAM_ASSIGN(""_sv, aStruct.TypeInfo())
                    .COMMA().INC_VERSION()
                .WHERE();
                    for (size_t id = 0; id < sizeof...(KeysT); ++id)
                    {
                        if (id > 0) builder.AND();
                        builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
                    }
            builder
                    .AND().PARAM("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
                .RETURNING().PARAM("_version"_sv);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    SWriteRowValues(dbQuery, aStruct);
    dbQuery.Nexts<KeysT...>(std::forward<KeysT...>(aKeysValues...));
    dbQuery.NextInt64(aVersion);
    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsRowCTNR (const GpDbQueryCacheUID&                           aCacheUID,
                                                 const GpTypeStructBase&                            aStruct,
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
            GpDbQueryMapper::SUpdateByKeysAsRow<KeysT...>(aCacheUID,
                                                          aStruct,
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
void    GpDbQueryMapper::SSetPagingOrderJsonb (const GpDbQueryCacheUID&                             aCacheUID,
                                               const SInt64                                         aVersion,
                                               std::string_view                                     aTablePath,
                                               GpDbConnection&                                      aDbConn,
                                               const GpTypeStructBase&                              aStruct,
                                               const GpJsonMapperFlags                              aFlags,
                                               const PagingCounterIDS&                              aPagingCounterIDS,
                                               const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                               KeysT...                                             aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            //UPDATE
            //  aTablePath
            //SET
            //   _version=_version+1
            //  ,data=$1::jsonb
            //  ,aCounterNames... = next_paging_order_val($aCounterIds, aTablePath+"."+aCounterNames)...
            //
            //WHERE
            //      aKeysNames... = $aKeysValues...
            //  AND _version      = $aVersion
            //RETURNING
            //  _version

            GpDbQueryBuilder builder;

            builder
                .UPDATE(aTablePath)
                .SET()
                    .INC_VERSION()
                    .COMMA().PARAM("data"_sv).ASSIGN().VALUE(GpDbQueryValType::STRING_JSON);

            for (auto[uidParam, counterName]: aPagingCounterIDS)
            {
                builder
                    .COMMA().PARAM(counterName).ASSIGN()
                        .RAW("next_paging_order_val("_sv);

                if (std::holds_alternative<std::string_view>(uidParam))
                {
                    builder.PARAM(std::get<std::string_view>(uidParam));
                } else
                {
                    builder.VALUE(GpDbQueryValType::UUID);
                }

                builder
                    .COMMA()
                        .RAW("'"_sv + aTablePath + "."_sv + counterName + "'"_sv).RAW(")"_sv);
            }

            builder
                .WHERE();

            for (size_t id = 0; id < sizeof...(KeysT); ++id)
            {
                if (id > 0) builder.AND();
                builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
            }

            builder
                .AND().PARAM("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
                .RETURNING().PARAM("_version"_sv);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);

    dbQuery.NextStrJson(GpJsonMapper::SToJson(aStruct, aFlags));

    for (auto[uidParam, counterName]: aPagingCounterIDS)
    {
        if (std::holds_alternative<GpUUID>(uidParam))
        {
            dbQuery.NextUUID(std::get<GpUUID>(uidParam));
        }
    }

    dbQuery.Nexts<KeysT...>(aKeysValues...);
    dbQuery.NextInt64(aVersion);

    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SSetPagingOrderJsonbCTNR (const GpDbQueryCacheUID&                             aCacheUID,
                                                   const SInt64                                         aVersion,
                                                   std::string_view                                     aTablePath,
                                                   GpDbConnection&                                      aDbConn,
                                                   const GpTypeStructBase&                              aStruct,
                                                   const GpJsonMapperFlags                              aFlags,
                                                   const PagingCounterIDS&                              aPagingCounterIDS,
                                                   const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                                   KeysT...                                             aKeysValues)
{
    SThrowOnNoResult<void>
    (
        [&]()
        {
            GpDbQueryMapper::SSetPagingOrderJsonb<KeysT...>(aCacheUID,
                                                            aVersion,
                                                            aTablePath,
                                                            aDbConn,
                                                            aStruct,
                                                            aFlags,
                                                            aPagingCounterIDS,
                                                            aKeysNames,
                                                            aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

template<typename... KeysT>
SInt64  GpDbQueryMapper::SSelectByKeysAsJsonb (const GpDbQueryCacheUID&                             aCacheUID,
                                               GpTypeStructBase&                                    aStruct,
                                               std::string_view                                     aTablePath,
                                               GpDbConnection&                                      aDbConn,
                                               const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                               KeysT...                                             aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .SELECT()
                    .PARAM({"_version"_sv})
                    .COMMA().PARAM({"data"_sv})
                .FROM(aTablePath)
                .WHERE();

                for (size_t id = 0; id < sizeof...(KeysT); ++id)
                {
                    if (id > 0) builder.AND();
                    builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
                }

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    //Do query
    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.Nexts<KeysT...>(std::forward<KeysT...>(aKeysValues...));
    GpDbQueryRes::SP dbQueryRes = aDbConn.Execute(dbQuery, 1_cnt);

    //Read
    const SInt64 version = SRowToVersion(dbQueryRes.VCn(), 0_cnt, 0_cnt);
    SJsonToStruct(dbQueryRes.Vn(), aStruct, 0_cnt, 1_cnt);

    return version;
}

template<typename T, typename... KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsJsonb (const GpDbQueryCacheUID&                             aCacheUID,
                                       std::string_view                                     aTablePath,
                                       GpDbConnection&                                      aDbConn,
                                       const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                       KeysT...                                             aKeysValues)
{
    typename T::SP res = MakeSP<T>();
    const SInt64 version = SSelectByKeysAsJsonb<KeysT...>(aCacheUID,
                                                          res.Vn(),
                                                          aTablePath,
                                                          aDbConn,
                                                          aKeysNames,
                                                          std::forward<KeysT...>(aKeysValues...));
    return {res, version};
}

template<typename       T,
         typename       TThrowOnNotFound,
         typename...    KeysT>
std::tuple<typename T::SP, SInt64>
GpDbQueryMapper::SSelectByKeysAsJsonbCTNR (const GpDbQueryCacheUID&                             aCacheUID,
                                           std::string_view                                     aTablePath,
                                           GpDbConnection&                                      aDbConn,
                                           const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                           KeysT...                                             aKeysValues)
{
    return SThrowOnNoResult<std::tuple<typename T::SP, SInt64>>
    (
        [&]()
        {
            return GpDbQueryMapper::SSelectByKeysAsJsonb<T, KeysT...>(aCacheUID,
                                                                      aTablePath,
                                                                      aDbConn,
                                                                      aKeysNames,
                                                                      aKeysValues...);
        },
        TThrowOnNotFound::SThrowOnNoResult
    );
}

template<typename... KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsJsonb (const GpDbQueryCacheUID&                             aCacheUID,
                                               const GpTypeStructBase&                              aStruct,
                                               const SInt64                                         aVersion,
                                               std::string_view                                     aTablePath,
                                               GpDbConnection&                                      aDbConn,
                                               const GpJsonMapperFlags                              aFlags,
                                               const GpArray<std::string_view, sizeof...(KeysT)>&   aKeysNames,
                                               KeysT...                                             aKeysValues)
{
    constexpr GpArray<GpDbQueryValType::EnumT,sizeof...(KeysT)> VAL_T = {GpDbQueryBuilder::SDetectValType<KeysT>()...};

    const auto& cacheVal = sMapperCache.Get
    (
        aCacheUID,
        [&]()
        {
            GpDbQueryBuilder builder;

            builder
                .UPDATE(aTablePath)
                .SET()
                    .PARAM("data"_sv).ASSIGN().VALUE(GpDbQueryValType::STRING_JSON)
                    .COMMA().INC_VERSION()
                .WHERE();
                    for (size_t id = 0; id < sizeof...(KeysT); ++id)
                    {
                        if (id > 0) builder.AND();
                        builder.PARAM(aKeysNames.at(id)).EQUAL().VALUE(VAL_T.at(id));
                    }
            builder
                    .AND().PARAM("_version"_sv).EQUAL().VALUE(GpDbQueryValType::INT_64)
                .RETURNING().PARAM("_version"_sv);

            return GpDbQueryMapperCacheValue(builder.ValuesTypesMove(), builder.QueryStrMove());
        }
    );

    GpDbQuery dbQuery(cacheVal.iQuery, cacheVal.iValuesTypes);
    dbQuery.NextStrJson(GpJsonMapper::SToJson(aStruct, aFlags));
    dbQuery.Nexts<KeysT...>(aKeysValues...);
    dbQuery.NextInt64(aVersion);
    aDbConn.Execute(dbQuery, 1_cnt);
}

template<typename       TThrowOnNotFound,
         typename...    KeysT>
void    GpDbQueryMapper::SUpdateByKeysAsJsonbCTNR (const GpDbQueryCacheUID&                             aCacheUID,
                                                   const GpTypeStructBase&                              aStruct,
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
            GpDbQueryMapper::SUpdateByKeysAsJsonb<KeysT...>(aCacheUID,
                                                            aStruct,
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

template<typename T>
typename T::C::Vec::SP  GpDbQueryMapper::SReadQueryResVecSP (const GpDbQueryRes&    aDbQueryRes,
                                                             ReadFnSPT<T>           aParserFn)
{
    typename T::C::Vec::SP  res;
    const count_t           rowsCount = aDbQueryRes.RowsCount();

    if (rowsCount == 0_cnt)
    {
        return res;
    }

    res.reserve(rowsCount.As<size_t>());

    for (count_t rowId = 0_cnt; rowId < rowsCount; ++rowId)
    {
        res.emplace_back(aParserFn(rowId, aDbQueryRes));
    }

    return res;
}

template<typename T>
T   GpDbQueryMapper::SReadQueryResOne (const GpDbQueryRes&  aDbQueryRes,
                                       ReadFnRefT<T>            aParserFn)
{
    const count_t rowsCount = aDbQueryRes.RowsCount();

    if (rowsCount == 0_cnt)
    {
        THROW_DBE(GpDbExceptionCode::EMPTY_QUERY_RES);
    }

    return aParserFn(0_cnt, aDbQueryRes);
}

}//namespace GPlatform
