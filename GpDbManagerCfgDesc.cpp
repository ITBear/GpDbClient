#include "GpDbManagerCfgDesc.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpDbManagerCfgDesc, GP_MODULE_UUID)

GpDbManagerCfgDesc::GpDbManagerCfgDesc (void) noexcept
{
}

GpDbManagerCfgDesc::GpDbManagerCfgDesc (const GpDbManagerCfgDesc& aDesc):
driver_name(aDesc.driver_name),
max_conn_pool_size(aDesc.max_conn_pool_size),
connection_str(aDesc.connection_str),
mode(aDesc.mode),
event_poller(aDesc.event_poller)
{
}

GpDbManagerCfgDesc::GpDbManagerCfgDesc (GpDbManagerCfgDesc&& aDesc) noexcept:
driver_name(std::move(aDesc.driver_name)),
max_conn_pool_size(std::move(aDesc.max_conn_pool_size)),
connection_str(std::move(aDesc.connection_str)),
mode(std::move(aDesc.mode)),
event_poller(std::move(aDesc.event_poller))
{
}

GpDbManagerCfgDesc::~GpDbManagerCfgDesc (void) noexcept
{
}

void    GpDbManagerCfgDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(driver_name);
    PROP(max_conn_pool_size);
    PROP(connection_str);
    PROP(mode);
    PROP(event_poller);
}

}//namespace GPlatform
