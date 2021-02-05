#include "GpDbManager.hpp"
#include "GpDbDriver.hpp"
#include "GpDbConnection.hpp"
#include "GpDbManagerCfgDesc.hpp"

namespace GPlatform {

GpDbManager::GpDbManager
(
    std::string_view                aName,
    GpSP<GpDbDriver>                aDriver,
    std::string_view                aConnectionStr,
    const GpDbConnectionMode::EnumT aMode
):
iName(aName),
iDriver(std::move(aDriver)),
iConnStr(aConnectionStr),
iMode(aMode)
{
}

GpDbManager::~GpDbManager (void) noexcept
{
}

void    GpDbManager::PreInit (const count_t /*aCount*/)
{
}

GpDbConnection::SP  GpDbManager::NewElement (void)
{
    GpDbConnection::SP conn = iDriver.VCn().NewConnection(iMode, iConnStr);
    return conn;
}

void    GpDbManager::OnClear (void) noexcept
{
    //NOP
}

bool    GpDbManager::Validate (GpSP<GpDbConnection> /*aConnection*/) noexcept
{
    return true;
}

}//namespace GPlatform
