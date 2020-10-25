#pragma once

#include "GpDbConnectionMode.hpp"

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
                                    GpDbManager         (std::string_view                   aName,
                                                         GpSP<GpDbDriver>                   aDriver,
                                                         std::string_view                   aConnectionStr,
                                                         const GpDbConnectionMode::EnumT    aMode);
    virtual                         ~GpDbManager        (void) noexcept override final;

    std::string_view                Name                (void) const noexcept {return iName;}

protected:
    virtual void                    PreInit             (const count_t aCount) override final;
    virtual GpSP<GpDbConnection>    NewElement          (void) override final;
    virtual void                    OnClear             (void) noexcept override final;

private:
    const std::string               iName;
    GpSP<GpDbDriver>                iDriver;
    const std::string               iConnStr;
    const GpDbConnectionMode::EnumT iMode;
};

}//GPlatform
