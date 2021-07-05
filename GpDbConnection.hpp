#pragma once

#include "GpDbConnectionStatus.hpp"
#include "GpDbConnectionMode.hpp"
#include "GpDbTransactionIsolation.hpp"
#include "Query/GpDbQuery.hpp"
#include "Query/GpDbQueryRes.hpp"
#include "Query/GpDbQueryPrepared.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbConnection
{
public:
    CLASS_REMOVE_CTRS(GpDbConnection)
    CLASS_DECLARE_DEFAULTS(GpDbConnection)

    using StatusTE              = GpDbConnectionStatus::EnumT;
    using ModeTE                = GpDbConnectionMode::EnumT;
    using TransactionLevelTE    = GpDbTransactionIsolation::EnumT;

protected:
                                GpDbConnection          (const StatusTE         aStatus,
                                                         const ModeTE           aMode,
                                                         GpIOEventPoller::WP    aEventPoller) noexcept;

public:
    virtual                     ~GpDbConnection         (void) noexcept;

    GpIOEventPoller::WP         EventPoller             (void) const noexcept {return iEventPoller;}
    GpDbConnection::WP          SelfWP                  (void) const noexcept {return iSelfWP;}
    void                        SetSelfWP               (GpDbConnection::WP aSelfWP) {iSelfWP = aSelfWP;}

    StatusTE                    Status                  (void) const noexcept {return iStatus;}
    ModeTE                      Mode                    (void) const noexcept {return iMode;}

    bool                        IsTransactionOpen       (void) const noexcept {return iIsTransactionOpen;}
    void                        BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);
    TransactionLevelTE          TransactionLevel        (void) const noexcept {return iTransactionLevel;}

    virtual void                Close                   (void) = 0;
    virtual GpDbQueryRes::SP    Execute                 (GpDbQueryPrepared::CSP aQueryPrepared,
                                                         const count_t          aMinResultRowsCount) = 0;

    virtual bool                Validate                (void) const noexcept = 0;

protected:
    void                        SetStatus               (StatusTE aStatus) noexcept {iStatus = aStatus;}

    virtual void                OnBeginTransaction      (GpDbTransactionIsolation::EnumT aIsolationLevel) = 0;
    virtual void                OnCommitTransaction     (void) = 0;
    virtual void                OnRollbackTransaction   (void) = 0;

private:
    StatusTE                    iStatus             = StatusTE::CLOSED;
    const ModeTE                iMode;
    bool                        iIsTransactionOpen  = false;
    TransactionLevelTE          iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
    GpIOEventPoller::WP         iEventPoller;
    GpDbConnection::WP          iSelfWP;
};

}//namespace GPlatform
