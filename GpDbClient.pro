TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=c192f513-2e74-42f6-b3f9-c86c92d72624
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER
DEFINES		   += GPDBCLIENT_LIBRARY
PACKET_NAME     = GpDbClient
DIR_LEVEL       = ./..

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
	GP_CORE_LIB_V		= 2
	GP_JSON_LIB_V		= 0
}

os_linux{
}

LIBS += -lGpCore2$$TARGET_POSTFIX$$GP_CORE_LIB_V
LIBS += -lGpJson$$TARGET_POSTFIX$$GP_JSON_LIB_V
#------------------------------ LIBS END ---------------------------------

HEADERS += \
	GpDbClient.hpp \
	GpDbClient_global.hpp \
	GpDbConnection.hpp \
	GpDbConnectionGuard.hpp \
	GpDbConnectionMode.hpp \
	GpDbConnectionStatus.hpp \
	GpDbDriver.hpp \
	GpDbDriverFactory.hpp \
	GpDbException.hpp \
	GpDbExceptionCode.hpp \
	GpDbManager.hpp \
	GpDbManagerCatalog.hpp \
	GpDbManagerCfgDesc.hpp \
	GpDbTransactionIsolation.hpp \
	Query/GpDbQuery.hpp \
	Query/GpDbQueryBuilder.hpp \
	Query/GpDbQueryMapper.hpp \
	Query/GpDbQueryMapperCache.hpp \
	Query/GpDbQueryRes.hpp \
	Query/GpDbQueryResState.hpp \
	Query/GpDbQueryValStrJson.hpp \
	Query/GpDbQueryValStrName.hpp \
	Query/GpDbQueryValStrValue.hpp \
	Query/GpDbQueryValType.hpp \
	Query/GpDbQueryValue.hpp

SOURCES += \
	GpDbConnection.cpp \
	GpDbConnectionGuard.cpp \
	GpDbConnectionMode.cpp \
	GpDbConnectionStatus.cpp \
	GpDbDriver.cpp \
	GpDbDriverFactory.cpp \
	GpDbException.cpp \
	GpDbExceptionCode.cpp \
	GpDbManager.cpp \
	GpDbManagerCatalog.cpp \
	GpDbManagerCfgDesc.cpp \
	GpDbTransactionIsolation.cpp \
	Query/GpDbQuery.cpp \
	Query/GpDbQueryBuilder.cpp \
	Query/GpDbQueryMapper.cpp \
	Query/GpDbQueryMapperCache.cpp \
	Query/GpDbQueryRes.cpp \
	Query/GpDbQueryResState.cpp \
	Query/GpDbQueryValType.cpp
