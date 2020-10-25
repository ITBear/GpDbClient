#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

class GpDbManager;
class GpDbConnection;

class GPDBCLIENT_API GpDbConnectionGuard
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbConnectionGuard)
    CLASS_DECLARE_DEFAULTS(GpDbConnectionGuard)

public:
                            GpDbConnectionGuard     (void);
                            GpDbConnectionGuard     (GpDbManager& aManager) noexcept;
                            ~GpDbConnectionGuard    (void) noexcept;

    GpDbConnection&         Connection              (void);

private:
    GpDbManager&            iManager;
    GpSP<GpDbConnection>    iConnection;
};

}//GPlatform
