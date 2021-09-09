#include "GpDbConnectionGuard.hpp"
#include "GpDbManager.hpp"
#include "GpDbConnection.hpp"
#include "GpDbManagerCatalog.hpp"
#include "GpDbException.hpp"

#include <iostream>

namespace GPlatform {

GpDbConnectionGuard::GpDbConnectionGuard (void) noexcept
{
}

GpDbConnectionGuard::GpDbConnectionGuard (GpDbManager& aManager) noexcept:
iManager(aManager)
{
}

GpDbConnectionGuard::~GpDbConnectionGuard (void) noexcept
{
    ConnectionRelease();
}

void    GpDbConnectionGuard::CommitTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    std::optional<std::exception_ptr> eptr;

    try
    {
        connection.CommitTransaction();     
    } catch (...)
    {
        eptr = std::current_exception();
    }

    ConnectionRelease();

    if (eptr.has_value())
    {
        std::rethrow_exception(eptr.value());
    }
}

void    GpDbConnectionGuard::RollbackTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    std::optional<std::exception_ptr> eptr;

    try
    {
        connection.RollbackTransaction();
    } catch (...)
    {
        eptr = std::current_exception();
    }

    ConnectionRelease();

    if (eptr.has_value())
    {
        std::rethrow_exception(eptr.value());
    }
}

GpDbQueryRes::SP    GpDbConnectionGuard::Execute
(
    GpDbQuery::CSP      aQuery,
    const count_t       aMinResultRowsCount
)
{
    GpDbQueryPrepared::CSP  queryPrepared   = Manager().Prepare(aQuery);
    GpDbConnection&         connection      = ConnectionAcquire();
    GpDbQueryRes::SP        res;

    std::optional<std::exception_ptr> eptr;

    try
    {
        res = connection.Execute(queryPrepared, aMinResultRowsCount);
    } catch (...)
    {
        eptr = std::current_exception();
    }

    if (   eptr.has_value()
        || (connection.IsTransactionOpen() == false))
    {
        ConnectionRelease();

        if (eptr.has_value())
        {
            std::rethrow_exception(eptr.value());
        }
    }

    return res;
}

GpDbQueryRes::SP    GpDbConnectionGuard::Execute
(
    std::string_view    aSQL,
    const count_t       aMinResultRowsCount
)
{
    GpDbQuery::SP query = MakeSP<GpDbQuery>(aSQL);
    return Execute(query, aMinResultRowsCount);
}

GpDbManager&    GpDbConnectionGuard::Manager (void)
{
    if (iManager.has_value() == false)
    {
        iManager = GpDbManagerCatalog::S().Manager("default"_sv);
    }

    return iManager.value().get();
}

GpDbConnection& GpDbConnectionGuard::ConnectionAcquire (void)
{
    if (iConnection.IsNULL())
    {
        auto res = Manager().Acquire();

        THROW_DBE_COND
        (
            res.has_value(),
            GpDbExceptionCode::CONNECTION_LIMIT_EXCEEDED,
            "DB connection limit exceeded"_sv
        );

        iConnection = res.value();
    }

    return iConnection.Vn();
}

void    GpDbConnectionGuard::ConnectionRelease (void) noexcept
{
    if (iConnection.IsNULL())
    {
        return;
    }

    if (iManager.has_value() == false)
    {
        return;
    }

    GpDbConnection& conn = iConnection.Vn();
    if (conn.IsTransactionOpen())
    {
        try
        {
            conn.RollbackTransaction();
            Manager().Release(iConnection);
        } catch (const std::exception& e)
        {
            GpExceptionsSink::SSink(e);
        } catch (...)
        {
            GpExceptionsSink::SSinkUnknown();
        }
    } else
    {
        Manager().Release(iConnection);
    }

    iConnection.Clear();
}

}//namespace GPlatform
