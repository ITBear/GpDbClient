#pragma once

#include "GpDbConnectionMode.hpp"

namespace GPlatform {

class GpDbDriver;

class GPDBCLIENT_API GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriverFactory)
    CLASS_DECLARE_DEFAULTS(GpDbDriverFactory)

protected:
                                    GpDbDriverFactory   (std::string_view aName);

public:
    virtual                         ~GpDbDriverFactory  (void) noexcept;

    std::string_view                Name                (void) const noexcept {return  iName;}

    virtual GpSP<GpDbDriver>        NewInstance         (const GpDbConnectionMode::EnumT    aMode,
                                                         GpIOEventPoller::WP                aEventPoller) const = 0;

private:
    const std::string               iName;
};

}//namespace GPlatform
