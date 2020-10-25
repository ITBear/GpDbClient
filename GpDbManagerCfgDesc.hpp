#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbManagerCfgDesc final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpDbManagerCfgDesc)
    TYPE_STRUCT_DECLARE()

public:
                                GpDbManagerCfgDesc  (void) noexcept;
                                GpDbManagerCfgDesc  (const GpDbManagerCfgDesc& aDesc);
                                GpDbManagerCfgDesc  (GpDbManagerCfgDesc&& aDesc) noexcept;
    virtual                     ~GpDbManagerCfgDesc (void) noexcept override final;

    std::string_view            DriverName          (void) const noexcept {return driver_name;}
    count_t                     MaxConnPoolSize     (void) const noexcept {return max_conn_pool_size;}
    std::string_view            ConnectionStr       (void) const noexcept {return connection_str;}

private:
    std::string                 driver_name;
    count_t                     max_conn_pool_size;
    std::string                 connection_str;
};

}//namespace GPlatform
