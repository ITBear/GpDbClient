#include "GpDbQueryMapperCache.hpp"

namespace GPlatform {

GpDbQueryMapperCache::GpDbQueryMapperCache (void) noexcept
{
}

GpDbQueryMapperCache::~GpDbQueryMapperCache (void) noexcept
{
}

const GpDbQueryMapperCache::CacheValueT&    GpDbQueryMapperCache::Get (const CacheKeyT& aUID,
                                                                       GenFnT           aGenFn)
{
    return iCache.FindOrRegister(aUID.Value(), [&](){return aGenFn();});
}

}//namespace GPlatform
