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

    try
    {
        connection.CommitTransaction();
        ConnectionRelease();
    } catch (...)
    {
        ConnectionRelease();
        throw;
    }
}

void    GpDbConnectionGuard::RollbackTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    try
    {
        connection.RollbackTransaction();
        ConnectionRelease();
    } catch (...)
    {
        ConnectionRelease();
        throw;
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

    try
    {
        res = connection.Execute(queryPrepared, aMinResultRowsCount);
    } catch (...)
    {
        ConnectionRelease();
        throw;
    }

    if (connection.IsTransactionOpen() == false)
    {
        ConnectionRelease();
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
