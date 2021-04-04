#include "GpDbQueryMapperCache.hpp"

namespace GPlatform {

GpDbQueryMapperCache::GpDbQueryMapperCache (void) noexcept
{
}

GpDbQueryMapperCache::~GpDbQueryMapperCache (void) noexcept
{
}

const GpDbQueryMapperCache::CacheValueT&    GpDbQueryMapperCache::Get
(
    const CacheKeyT&    aUID,
    GenFnT              aGenFn
)
{
    /*static std::map<GpUUID, GpDbQueryMapperCacheValue, std::less<>> s;

    {
        static GpSpinlock sL;

        std::scoped_lock lock(sL);
        s.try_emplace(aUID.Value(), aGenFn());
    }

    return s.find(aUID.Value())->second;*/

    return iCache.FindOrRegister(aUID.Value(), [&](){return aGenFn();});
}

}//namespace GPlatform
