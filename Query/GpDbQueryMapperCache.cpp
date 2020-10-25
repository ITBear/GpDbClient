#include "GpDbQueryMapperCache.hpp"

namespace GPlatform {

GpDbQueryMapperCache::GpDbQueryMapperCache (void) noexcept
{
}

GpDbQueryMapperCache::~GpDbQueryMapperCache (void) noexcept
{
}

const GpDbQueryMapperCache::CacheValueT&    GpDbQueryMapperCache::Get (const GpTypeStructInfo&  aStructInfo,
                                                                       std::string_view         aPurpose,
                                                                       std::string_view         aTablePath,
                                                                       GenFnT                   aGenFn)
{
    GpDbQueryMapperCacheKey key(aStructInfo.UID(), aPurpose + aTablePath);
    const auto              val = iCache.Find(key);

    if (val.has_value())
    {
        return val.value().get();
    }

    auto res = iCache.TryRegister(std::move(key), aGenFn(aStructInfo));

    if (res.has_value())
    {
        return res.value().get();
    } else
    {
        return Get(aStructInfo, aPurpose, aTablePath, aGenFn);
    }
}

}//namespace GPlatform
