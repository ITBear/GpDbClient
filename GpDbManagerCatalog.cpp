#include "GpDbManagerCatalog.hpp"
#include "GpDbManager.hpp"
#include "GpDbDriverFactory.hpp"

#include <iostream>

namespace GPlatform {

static int _GpDbManagerCatalog_counter = 0;

GpDbManagerCatalog::GpDbManagerCatalog (void) noexcept
{
    _GpDbManagerCatalog_counter++;
    std::cout << "[GpDbManagerCatalog::GpDbManagerCatalog]: counter = " << _GpDbManagerCatalog_counter << std::endl;
}

GpDbManagerCatalog::~GpDbManagerCatalog (void) noexcept
{
    iManagers.Clear();

    _GpDbManagerCatalog_counter--;
    std::cout << "[GpDbManagerCatalog::~GpDbManagerCatalog]: counter = " << _GpDbManagerCatalog_counter << std::endl;
}

GpDbManagerCatalog& GpDbManagerCatalog::S (void) noexcept
{
    static GpDbManagerCatalog sDbGlobal;
    return sDbGlobal;
}

void    GpDbManagerCatalog::InitFromConfig (const GpDbManagerCfgDesc::C::MapStr::SP& aConnections)
{
    for (const auto& iter: aConnections)
    {
        std::string_view            name    = iter.first;
        const GpDbManagerCfgDesc&   cfg     = iter.second.VC();

        GpDbManager::SP dbManager = MakeSP<GpDbManager>
        (
            name,
            DriverFactory(cfg.DriverName()).NewInstance(cfg.Mode(), GpIOEventPollerCatalog::S().Find(cfg.EventPoller())),
            cfg.ConnectionStr(),
            cfg.Mode()
        );

        dbManager->Init(0_cnt, cfg.MaxConnPoolSize());

        AddManager(dbManager);
    }
}

void    GpDbManagerCatalog::Clear (void)
{
    iManagers.Clear();
    iDrivers.Clear();
}

void    GpDbManagerCatalog::AddManager (GpDbManager::SP aManager)
{
    const GpDbManager& manager = aManager.VC();
    iManagers.Register(manager.Name(), aManager);
}

GpDbManager&    GpDbManagerCatalog::Manager (std::string_view aName)
{
    auto res = iManagers.TryFind(aName);

    THROW_GPE_COND
    (
        res.has_value(),
        [&](){return "DB manager not found by name '"_sv + aName + "'"_sv;}
    );

    return res.value().get().V();
}

void    GpDbManagerCatalog::AddDriverFactory (GpSP<GpDbDriverFactory> aDriverFactory)
{
    const GpDbDriverFactory& driverFactory = aDriverFactory.VC();
    iDrivers.Register(driverFactory.Name(), aDriverFactory);
}

GpDbDriverFactory&  GpDbManagerCatalog::DriverFactory (std::string_view aName)
{
    auto res = iDrivers.TryFind(aName);

    THROW_GPE_COND
    (
        res.has_value(),
        [&](){return "DB driver factory not found by name '"_sv + aName + "'"_sv;}
    );

    return res.value().get().V();
}

}//namespace GPlatform
