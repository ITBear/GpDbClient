#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

class GpDbDriver;

class GPDBCLIENT_API GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS(GpDbDriverFactory)
    CLASS_DECLARE_DEFAULTS(GpDbDriverFactory)

protected:
                                GpDbDriverFactory   (std::string_view aName);

public:
    virtual                     ~GpDbDriverFactory  (void) noexcept;

    std::string_view            Name                (void) const noexcept {return  iName;}

    virtual GpSP<GpDbDriver>    NewInstance         (void) const = 0;

private:
    const std::string           iName;
};

}//namespace GPlatform
