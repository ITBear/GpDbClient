#include "GpDbConnection.hpp"

#include <iostream>

namespace GPlatform {

static int _GpDbConnection_count = 0;

GpDbConnection::GpDbConnection
(
    const StatusTE      aStatus,
    const ModeTE        aMode,
    GpIOEventPoller::WP aEventPoller) noexcept:
iStatus(aStatus),
iMode(aMode),
iEventPoller(std::move(aEventPoller))
{
    _GpDbConnection_count++;
    std::cout << "[GpDbConnection::GpDbConnection]: count = " << _GpDbConnection_count << std::endl;
}

GpDbConnection::~GpDbConnection (void) noexcept
{
    _GpDbConnection_count--;
    std::cout << "[GpDbConnection::~GpDbConnection]: count = " << _GpDbConnection_count << std::endl;
}

void    GpDbConnection::BeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    THROW_GPE_COND
    (
        iIsTransactionOpen == false,
        "Transaction already open"_sv
    );

    OnBeginTransaction(aIsolationLevel);
    iIsTransactionOpen  = true;
    iTransactionLevel   = aIsolationLevel;
}

void    GpDbConnection::CommitTransaction (void)
{
    THROW_GPE_COND
    (
        iIsTransactionOpen == true,
        "Transaction not open"_sv
    );

    try
    {
        OnCommitTransaction();
    } catch (...)
    {
        iIsTransactionOpen  = false;
        iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;

        throw;
    }

    iIsTransactionOpen  = false;
    iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
}

void    GpDbConnection::RollbackTransaction (void)
{
    THROW_GPE_COND
    (
        iIsTransactionOpen == true,
        "Transaction not open"_sv
    );

    try
    {
        OnRollbackTransaction();
    } catch (...)
    {
        iIsTransactionOpen  = false;
        iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;

        throw;
    }

    iIsTransactionOpen  = false;
    iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
}

}//namespace GPlatform
