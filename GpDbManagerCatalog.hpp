#pragma once

#include "GpDbManagerCfgDesc.hpp"
#include "GpDbConnectionMode.hpp"

namespace GPlatform {

class GpDbManager;
class GpDbDriverFactory;

class GPDBCLIENT_API GpDbManagerCatalog
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbManagerCatalog)
    CLASS_DECLARE_DEFAULTS(GpDbManagerCatalog)
    CLASS_TAG(THREAD_SAFE)

    using ManagersT = GpElementsCatalog<std::string_view, GpSP<GpDbManager>>;
    using DriversT  = GpElementsCatalog<std::string_view, GpSP<GpDbDriverFactory>>;

private:
                                GpDbManagerCatalog      (void) noexcept;
                                ~GpDbManagerCatalog     (void) noexcept;

public:
    static GpDbManagerCatalog&  S                       (void) noexcept;

    void                        Clear                   (void);
    void                        InitFromConfig          (const GpDbManagerCfgDesc::C::MapStr::SP&   aConnections,
                                                         const GpDbConnectionMode::EnumT            aMode);

    void                        AddManager              (GpSP<GpDbManager> aManager);
    GpDbManager&                Manager                 (std::string_view aName);

    void                        AddDriverFactory        (GpSP<GpDbDriverFactory> aDriverFactory);
    GpDbDriverFactory&          DriverFactory           (std::string_view aName);

private:
    ManagersT                   iManagers;
    DriversT                    iDrivers;
};

}//GPlatform
