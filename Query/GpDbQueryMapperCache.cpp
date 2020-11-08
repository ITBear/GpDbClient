#include "GpDbQueryMapperCache.hpp"

namespace GPlatform {

GpDbQueryMapperCache::GpDbQueryMapperCache (void) noexcept
{
}

GpDbQueryMapperCache::~GpDbQueryMapperCache (void) noexcept
{
}

const GpDbQueryMapperCache::CacheValueT&    GpDbQueryMapperCache::Get (const GpTypeStructInfo&  aTypeInfo,
                                                                       std::string_view         aPurpose,
                                                                       GenFnT                   aGenFn)
{
    GpDbQueryMapperCacheKey key(aTypeInfo.UID(), aPurpose);
    const auto              val = iCache.Find(key);

    if (val.has_value())
    {
        return val.value().get();
    }

    auto res = iCache.TryRegister(std::move(key), aGenFn(aTypeInfo));

    if (res.has_value())
    {
        return res.value().get();
    } else
    {
        return Get(aTypeInfo, aPurpose, aGenFn);
    }
}

}//namespace GPlatform
