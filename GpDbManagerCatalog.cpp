#include "GpDbManagerCatalog.hpp"
#include "GpDbManager.hpp"
#include "GpDbDriverFactory.hpp"

namespace GPlatform {

GpDbManagerCatalog::GpDbManagerCatalog (void) noexcept
{
}

GpDbManagerCatalog::~GpDbManagerCatalog (void) noexcept
{
    iManagers.Clear();
}

GpDbManagerCatalog& GpDbManagerCatalog::S (void) noexcept
{
    static GpDbManagerCatalog sDbGlobal;
    return sDbGlobal;
}

void    GpDbManagerCatalog::InitFromConfig (const GpDbManagerCfgDesc::C::MapStr::SP&    aConnections,
                                            const GpDbConnectionMode::EnumT             aMode)
{
    for (const auto& iter: aConnections)
    {
        std::string_view            name        = iter.first;
        const GpDbManagerCfgDesc&   cfg         = iter.second.VC();
        GpDbManager::SP             dbManager   = MakeSP<GpDbManager>(name,
                                                                      DriverFactory(cfg.DriverName()).NewInstance(),
                                                                      cfg.ConnectionStr(),
                                                                      aMode);

        dbManager.V().Init(0_cnt, cfg.MaxConnPoolSize());

        AddManager(dbManager);
    }
}

void    GpDbManagerCatalog::Clear (void)
{
    iManagers.Clear();
}

void    GpDbManagerCatalog::AddManager (GpDbManager::SP aManager)
{
    const GpDbManager& manager = aManager.VC();
    iManagers.Register(manager.Name(), aManager);
}

GpDbManager&    GpDbManagerCatalog::Manager (std::string_view aName)
{
    auto res = iManagers.Find(aName);

    THROW_GPE_COND_CHECK_M(res.has_value(), "DB manager not found by name '"_sv + aName + "'"_sv);

    return res.value().get().V();
}

void    GpDbManagerCatalog::AddDriverFactory (GpSP<GpDbDriverFactory> aDriverFactory)
{
    const GpDbDriverFactory& driverFactory = aDriverFactory.VC();
    iDrivers.Register(driverFactory.Name(), aDriverFactory);
}

GpDbDriverFactory&  GpDbManagerCatalog::DriverFactory (std::string_view aName)
{
    auto res = iDrivers.Find(aName);

    THROW_GPE_COND_CHECK_M(res.has_value(), "DB driver factory not found by name '"_sv + aName + "'"_sv);

    return res.value().get().V();
}

}//namespace GPlatform
