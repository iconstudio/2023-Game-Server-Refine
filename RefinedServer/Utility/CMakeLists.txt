# CMakeList.txt: Utility에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
IF (POLICY CMP0141)
	CMAKE_POLICY(SET CMP0141 NEW)
	SET(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
ENDIF ()

PROJECT("Utility")

ADD_EXECUTABLE(Utility "src/Testbed.cpp"
               src/Array.ixx
               src/Atomic.ixx
               src/Constraints.ixx
               src/Error.ixx
               src/Expected.ixx
               src/Identity.ixx
               src/Locks.ixx
               src/LooseMonad.ixx
               src/Memory.ixx
               src/Meta.ixx
               src/Monad.ixx
               src/Option.ixx
               src/Print.ixx
               src/Singleton.ixx
               src/String.ixx
               src/
               src/Thread.ixx
               src/ThreadUnit.ixx
               src/Traits.ixx
               src/Union.ixx
               src/Utility.ixx)

SET_TARGET_PROPERTIES(Utility PROPERTIES LINKER_LANGUAGE CXX)
SET_PROPERTY(TARGET Utility PROPERTY CXX_STANDARD 23)

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
# add_test(NAME Utility COMMAND Utility)
