#pragma once

#include "GpDbConnectionMode.hpp"
#include "Query/GpDbQueryPrepared.hpp"

namespace GPlatform {

class GpDbConnection;

class GPDBCLIENT_API GpDbDriver
{
public:
    CLASS_REMOVE_CTRS(GpDbDriver)
    CLASS_DECLARE_DEFAULTS(GpDbDriver)
    CLASS_TAG(THREAD_SAFE)

protected:
                                    GpDbDriver      (std::string_view                   aName,
                                                     const GpDbConnectionMode::EnumT    aMode,
                                                     GpIOEventPoller::WP                aEventPoller);

public:
    virtual                         ~GpDbDriver     (void) noexcept;

    std::string_view                Name            (void) const noexcept {return iName;}
    GpDbConnectionMode::EnumT       Mode            (void) const noexcept {return iMode;}
    GpIOEventPoller::WP             EventPoller     (void) const noexcept {return iEventPoller;}

    virtual GpSP<GpDbConnection>    NewConnection   (std::string_view aConnStr) const = 0;
    virtual GpDbQueryPrepared::CSP  Prepare         (GpDbQuery::CSP aQuery) const = 0;

private:
    const std::string               iName;
    const GpDbConnectionMode::EnumT iMode;
    GpIOEventPoller::WP             iEventPoller;
};

}//GPlatform
