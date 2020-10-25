#include "GpDbManagerCfgDesc.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpDbManagerCfgDesc, "e200f131-8953-43f6-9ecb-c321b7b00e97"_sv, GP_MODULE_UUID)

GpDbManagerCfgDesc::GpDbManagerCfgDesc (void) noexcept
{
}

GpDbManagerCfgDesc::GpDbManagerCfgDesc (const GpDbManagerCfgDesc& aDesc):
driver_name(aDesc.driver_name),
max_conn_pool_size(aDesc.max_conn_pool_size),
connection_str(aDesc.connection_str)
{
}

GpDbManagerCfgDesc::GpDbManagerCfgDesc (GpDbManagerCfgDesc&& aDesc) noexcept:
driver_name(std::move(aDesc.driver_name)),
max_conn_pool_size(std::move(aDesc.max_conn_pool_size)),
connection_str(std::move(aDesc.connection_str))
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
}

}//namespace GPlatform
