#include "GpDbConnectionGuard.hpp"
#include "GpDbManager.hpp"
#include "GpDbConnection.hpp"
#include "GpDbManagerCatalog.hpp"

namespace GPlatform {

GpDbConnectionGuard::GpDbConnectionGuard (void):
GpDbConnectionGuard(GpDbManagerCatalog::S().Manager("default"_sv))
{
}

GpDbConnectionGuard::GpDbConnectionGuard (GpDbManager& aManager) noexcept:
iManager(aManager)
{
}

GpDbConnectionGuard::~GpDbConnectionGuard (void) noexcept
{
    if (iConnection.IsNULL())
    {
        return;
    }

    GpDbConnection& conn = iConnection.Vn();
    if (conn.IsTransactionOpen())
    {
        try
        {
            conn.RollbackTransaction();
            iManager.Release(iConnection);
        } catch (const std::exception& e)
        {
            GpExceptionsSink::SSink(e);
        } catch (...)
        {
            GpExceptionsSink::SSinkUnknown();
        }
    } else
    {
        iManager.Release(iConnection);
    }

    iConnection.Clear();
}

GpDbConnection& GpDbConnectionGuard::Connection (void)
{
    if (iConnection.IsNULL())
    {
        auto res = iManager.Acquire();
        THROW_GPE_COND(res.has_value(), "DB connection limit exceeded"_sv);
        iConnection = res.value();
    }

    return iConnection.Vn();
}

}//namespace GPlatform
