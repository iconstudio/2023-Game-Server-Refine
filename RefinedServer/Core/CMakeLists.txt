CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
IF (POLICY CMP0141)
	CMAKE_POLICY(SET CMP0141 NEW)
	SET(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
ENDIF ()

PROJECT("Core")

ADD_EXECUTABLE(Core src/Asynchron.ixx
               src/BasicUser.ixx
               src/ISession.ixx
               src/PacketIdentifier.ixx
               src/User.ixx
               src/UserIdentifier.ixx
               src/UserManager.cpp
               src/UserManager.ixx
               src/WorkerManager.ixx)

SET_TARGET_PROPERTIES(Core PROPERTIES LINKER_LANGUAGE CXX)
SET_PROPERTY(TARGET Core PROPERTY CXX_STANDARD 23)
