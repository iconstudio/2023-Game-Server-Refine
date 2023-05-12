# CMakeList.txt: Utility에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project("Utility")

add_executable(Utility "src/Testbed.cpp" src/Array.ixx src/Atomic.ixx src/Constraints.ixx src/Expected.ixx src/Identity.ixx src/Locks.ixx src/LooseMonad.ixx src/Memory.ixx src/Meta.ixx src/Monad.ixx src/Print.ixx src/Singleton.ixx src/ src/Thread.ixx src/Traits.ixx src/Union.ixx src/Utility.ixx)

set_target_properties(Utility PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET Utility PROPERTY CXX_STANDARD 23)

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
# add_test(NAME Utility COMMAND Utility)
