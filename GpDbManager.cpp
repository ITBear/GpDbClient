#include "GpDbManager.hpp"
#include "GpDbDriver.hpp"
#include "GpDbConnection.hpp"
#include "GpDbManagerCfgDesc.hpp"

#include <iostream>

namespace GPlatform {

static int _GpDbManager_counter = 0;

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
    _GpDbManager_counter++;
    std::cout << "[GpDbManager::GpDbManager]: counter = " << _GpDbManager_counter << std::endl;
}

GpDbManager::~GpDbManager (void) noexcept
{
    _GpDbManager_counter--;
    std::cout << "[GpDbManager::~GpDbManager]: counter = " << _GpDbManager_counter << std::endl;
}

GpDbQueryPrepared::CSP  GpDbManager::Prepare (GpDbQuery::CSP aQuery) const
{
    return iDriver->Prepare(aQuery);
}

void    GpDbManager::PreInit (const count_t /*aCount*/)
{
}

GpDbConnection::SP  GpDbManager::NewElement (GpSpinlock& aLocked)
{
    GpUnlockGuard unlock(aLocked);

    GpDbConnection::SP conn = iDriver.VCn().NewConnection(iConnStr);
    return conn;
}

void    GpDbManager::OnClear (void) noexcept
{
    //NOP
}

bool    GpDbManager::Validate (GpSP<GpDbConnection> aConnection) noexcept
{
    return aConnection->Validate();
}

void    GpDbManager::OnAcquire
(
    value_type& /*aValue*/,
    GpSpinlock& /*aLocked*/
)
{
    //NOP
}

GpDbManager::ReleaseAct GpDbManager::OnRelease
(
    value_type& aValue,
    GpSpinlock& /*aLocked*/
)
{
    if (iMode == GpDbConnectionMode::SYNC)
    {
        return ReleaseAct::PUSH_TO_ELEMENTS;
    }

    //Mode is ASYNC, check if there are tasks waiting for connections
    if (iConnWaitTaskBarriers.size() == 0)
    {
        return ReleaseAct::PUSH_TO_ELEMENTS;
    }

    GpTaskFiberBarrier::SP taskBarrier = iConnWaitTaskBarriers.front();
    iConnWaitTaskBarriers.pop();

    taskBarrier->Release(value_type(aValue));

    return ReleaseAct::ACQUIRED;
}

std::optional<GpDbManager::value_type>  GpDbManager::OnAcquireNoElementsLeft (GpSpinlock& aLocked)
{
    if (iMode == GpDbConnectionMode::SYNC)
    {
        return std::nullopt;
    }

    //Mode is ASYNC, wait for Release next connection
    GpTaskFiberBarrier::SP taskBarrier = MakeSP<GpTaskFiberBarrier>(1_cnt);
    iConnWaitTaskBarriers.push(taskBarrier);

    GpUnlockGuard unlock(aLocked);
    return std::any_cast<value_type>(taskBarrier->Wait());
}

}//namespace GPlatform
