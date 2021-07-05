#pragma once

#include "GpDbQuery.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbQueryPrepared
{
public:
    CLASS_REMOVE_CTRS(GpDbQueryPrepared)
    CLASS_DECLARE_DEFAULTS(GpDbQueryPrepared)

protected:
                            GpDbQueryPrepared   (GpDbQuery::CSP aQuery) noexcept;

public:
    virtual                 ~GpDbQueryPrepared  (void) noexcept;

    virtual void            Prepare             (void) = 0;
    GpDbQuery::CSP          Query               (void) const noexcept {return iQuery;}

private:
    GpDbQuery::CSP          iQuery;
};

}//namespace GPlatform
