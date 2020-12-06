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
    iIsTransactionOpen  = true;
    iTransactionLevel   = aIsolationLevel;
}

void    GpDbConnection::CommitTransaction (void)
{
    OnCommitTransaction();
    iIsTransactionOpen  = false;
    iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
}

void    GpDbConnection::RollbackTransaction (void)
{
    OnRollbackTransaction();
    iIsTransactionOpen  = false;
    iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
}

}//namespace GPlatform
