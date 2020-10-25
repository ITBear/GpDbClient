#pragma once

#include "GpDbQuery.hpp"

namespace GPlatform {

class GpDbQueryMapperCacheKey
{
public:
                    GpDbQueryMapperCacheKey (void) noexcept {}
                    GpDbQueryMapperCacheKey (const GpUUID& aTypeStructUID, std::string_view aPurpose):
                                            iTypeStructUID(aTypeStructUID),iPurpose(aPurpose) {}
                    GpDbQueryMapperCacheKey (const GpDbQueryMapperCacheKey& aKey):
                                            iTypeStructUID(aKey.iTypeStructUID),iPurpose(aKey.iPurpose) {}
                    GpDbQueryMapperCacheKey (GpDbQueryMapperCacheKey&& aKey) noexcept:
                                            iTypeStructUID(std::move(aKey.iTypeStructUID)),iPurpose(std::move(aKey.iPurpose)) {}
                    ~GpDbQueryMapperCacheKey(void) noexcept {}

    auto&           operator=               (const GpDbQueryMapperCacheKey& aKey)
                                            {iTypeStructUID = aKey.iTypeStructUID; iPurpose = aKey.iPurpose; return *this;}
    auto&           operator=               (GpDbQueryMapperCacheKey&& aKey) noexcept
                                            {iTypeStructUID = std::move(aKey.iTypeStructUID); iPurpose = std::move(aKey.iPurpose); return *this;}
    bool            operator>               (const GpDbQueryMapperCacheKey& aKey) const noexcept
                                            {return (iTypeStructUID == aKey.iTypeStructUID) ? (iPurpose > aKey.iPurpose) : (iTypeStructUID > aKey.iTypeStructUID);}
    bool            operator<               (const GpDbQueryMapperCacheKey& aKey) const noexcept
                                            {return (iTypeStructUID == aKey.iTypeStructUID) ? (iPurpose < aKey.iPurpose) : (iTypeStructUID < aKey.iTypeStructUID);}

public:
    GpUUID          iTypeStructUID;
    std::string     iPurpose;
};

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

class GPDBCLIENT_API GpDbQueryMapperCache
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbQueryMapperCache)
    CLASS_DECLARE_DEFAULTS(GpDbQueryMapperCache)

public:
    using CacheKeyT     = GpDbQueryMapperCacheKey;
    using CacheValueT   = GpDbQueryMapperCacheValue;

    using CacheT        = GpElementsCatalog<CacheKeyT, CacheValueT, GpMap>;
    using GenFnT        = std::function<GpDbQueryMapperCacheValue(const GpTypeStructInfo&)>;

public:
                                GpDbQueryMapperCache    (void) noexcept;
                                ~GpDbQueryMapperCache   (void) noexcept;

    const CacheValueT&          Get                     (const GpTypeStructInfo&    aStructInfo,
                                                         std::string_view           aPurpose,
                                                         std::string_view           aTablePath,
                                                         GenFnT                     aGenFn);

private:
    CacheT                      iCache;
};

}//GPlatform
