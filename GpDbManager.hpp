#pragma once

#include "GpDbConnectionMode.hpp"
#include "Query/GpDbQueryPrepared.hpp"
#include "Query/GpDbQuery.hpp"

namespace GPlatform {

class GpDbDriver;
class GpDbConnection;
class GpDbManagerCfgDesc;

class GPDBCLIENT_API GpDbManager final: public GpElementsPool<GpSP<GpDbConnection>>
{
public:
    CLASS_REMOVE_CTRS(GpDbManager)
    CLASS_DECLARE_DEFAULTS(GpDbManager)
    CLASS_TAG(THREAD_SAFE)

public:
                                        GpDbManager             (std::string_view                   aName,
                                                                 GpSP<GpDbDriver>                   aDriver,
                                                                 std::string_view                   aConnectionStr,
                                                                 const GpDbConnectionMode::EnumT    aMode);
    virtual                             ~GpDbManager            (void) noexcept override final;

    std::string_view                    Name                    (void) const noexcept {return iName;}
    GpDbQueryPrepared::CSP              Prepare                 (GpDbQuery::CSP aQuery) const;

protected:
    virtual void                        PreInit                 (const count_t aCount) override final;
    virtual GpSP<GpDbConnection>        NewElement              (GpSpinlock& aLocked) override final;
    virtual void                        OnClear                 (void) noexcept override final;
    virtual bool                        Validate                (GpSP<GpDbConnection> aConnection) noexcept override final;

    virtual void                        OnAcquire               (value_type& aValue,
                                                                 GpSpinlock& aLocked) override final;
    virtual ReleaseAct                  OnRelease               (value_type& aValue,
                                                                 GpSpinlock& aLocked) override final;
    virtual std::optional<value_type>   OnAcquireNoElementsLeft (GpSpinlock& aLocked) override final;

private:
    const std::string                   iName;
    GpSP<GpDbDriver>                    iDriver;
    const std::string                   iConnStr;
    const GpDbConnectionMode::EnumT     iMode;
    GpTaskFiberBarrier::C::Queue::SP    iConnWaitTaskBarriers;
};

}//GPlatform
