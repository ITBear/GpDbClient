#include "GpDbConnection.hpp"

namespace GPlatform {

GpDbConnection::GpDbConnection (const StatusTE  aStatus,
                                const ModeTE    aMode) noexcept:
iStatus(aStatus),
iMode(aMode)
{
}

GpDbConnection::~GpDbConnection (void) noexcept
{
}

void    GpDbConnection::BeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    OnBeginTransaction(aIsolationLevel);
    iIsTransactionOpen = true;
}

void    GpDbConnection::CommitTransaction (void)
{
    OnCommitTransaction();
    iIsTransactionOpen = false;
}

void    GpDbConnection::RollbackTransaction (void)
{
    OnRollbackTransaction();
    iIsTransactionOpen = false;
}

}//namespace GPlatform
