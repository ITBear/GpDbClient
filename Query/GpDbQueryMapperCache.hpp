#pragma once

#include "GpDbQuery.hpp"

namespace GPlatform {

class GpDbQueryMapperCacheValue
{
public:
                                    GpDbQueryMapperCacheValue   (void) noexcept {}
                                    GpDbQueryMapperCacheValue   (GpDbQuery::ValuesTypesVecT&&   aValuesTypes,
                                                                 std::string&&                  aQuery) noexcept:
                                                                iValuesTypes(std::move(aValuesTypes)), iQuery(std::move(aQuery)) {}
                                    GpDbQueryMapperCacheValue   (const GpDbQueryMapperCacheValue& aVal):
                                                                iValuesTypes(aVal.iValuesTypes), iQuery(aVal.iQuery) {}
                                    GpDbQueryMapperCacheValue   (GpDbQueryMapperCacheValue&& aVal) noexcept:
                                                                iValuesTypes(std::move(aVal.iValuesTypes)), iQuery(std::move(aVal.iQuery)) {}
                                    ~GpDbQueryMapperCacheValue  (void) noexcept {}

    auto&                           operator=                   (const GpDbQueryMapperCacheValue& aVal)
                                                                {iValuesTypes = aVal.iValuesTypes; iQuery = aVal.iQuery; return *this;}
    auto&                           operator=                   (GpDbQueryMapperCacheValue&& aVal) noexcept
                                                                {iValuesTypes = std::move(aVal.iValuesTypes); iQuery = std::move(aVal.iQuery); return *this;}

public:
    GpDbQuery::ValuesTypesVecT      iValuesTypes;
    std::string                     iQuery;
};

class GpDbQueryCacheUID_Shell;
using GpDbQueryCacheUID = GpTypeShell<GpUUID, GpDbQueryCacheUID_Shell>;

class GPDBCLIENT_API GpDbQueryMapperCache
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbQueryMapperCache)
    CLASS_DECLARE_DEFAULTS(GpDbQueryMapperCache)

public:
    using CacheKeyT     = GpDbQueryCacheUID;
    using CacheValueT   = GpDbQueryMapperCacheValue;
    using CacheT        = GpElementsCatalog<GpUUID, CacheValueT>;
    using GenFnT        = std::function<GpDbQueryMapperCacheValue()>;

public:
                                GpDbQueryMapperCache    (void) noexcept;
                                ~GpDbQueryMapperCache   (void) noexcept;

    const CacheValueT&          Get                     (const CacheKeyT&   aUID,
                                                         GenFnT             aGenFn);

private:
    CacheT                      iCache;
};

}//GPlatform
