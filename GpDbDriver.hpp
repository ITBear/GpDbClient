#pragma once

#include "GpDbConnectionMode.hpp"

namespace GPlatform {

class GpDbConnection;

class GPDBCLIENT_API GpDbDriver
{
public:
    CLASS_REMOVE_CTRS(GpDbDriver)
    CLASS_DECLARE_DEFAULTS(GpDbDriver)
    CLASS_TAG(THREAD_SAFE)

public:
                                    GpDbDriver      (std::string_view aName);
    virtual                         ~GpDbDriver     (void) noexcept;

    std::string_view                Name            (void) const noexcept {return iName;}
    virtual GpSP<GpDbConnection>    NewConnection   (const GpDbConnectionMode::EnumT    aMode,
                                                     std::string_view                   aConnStr) const = 0;

private:
    const std::string               iName;
};

}//GPlatform
