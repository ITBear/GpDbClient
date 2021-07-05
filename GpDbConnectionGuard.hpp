#pragma once

#include "GpDbConnection.hpp"

namespace GPlatform {

class GpDbManager;

class GPDBCLIENT_API GpDbConnectionGuard
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbConnectionGuard)
    CLASS_DECLARE_DEFAULTS(GpDbConnectionGuard)

    using ManagerRefT = std::optional<std::reference_wrapper<GpDbManager>>;

public:
                                GpDbConnectionGuard     (void) noexcept;
                                GpDbConnectionGuard     (GpDbManager& aManager) noexcept;
                                ~GpDbConnectionGuard    (void) noexcept;

    void                        BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel) {ConnectionAcquire().BeginTransaction(aIsolationLevel);}
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);

    virtual GpDbQueryRes::SP    Execute                 (GpDbQuery::CSP     aQuery,
                                                         const count_t      aMinResultRowsCount);
    virtual GpDbQueryRes::SP    Execute                 (std::string_view   aSQL,
                                                         const count_t      aMinResultRowsCount);

private:
    GpDbManager&                Manager                 (void);
    GpDbConnection&             ConnectionAcquire       (void);
    void                        ConnectionRelease       (void) noexcept;

private:
    ManagerRefT                 iManager;
    GpDbConnection::SP          iConnection;
};

}//GPlatform
