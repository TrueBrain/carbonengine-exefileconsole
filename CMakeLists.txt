cmake_minimum_required(VERSION 3.16)

project(ExeFileConsole VERSION 1.0.0)

include(cmake/CcpGlobalSettings.cmake)
include(cmake/CcpTargetConfigurations.cmake)
include(cmake/CcpPackageConfigHelpers.cmake)

ccp_add_executable(ExeFileConsole
    ExeFileConsole.cpp
)
target_compile_definitions(ExeFileConsole PRIVATE NDEBUG _CONSOLE _UNICODE UNICODE)

# Provide an install target if this is the top level project only
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(INSTALL_TO_MONOLITH "Install this package to a perforce branch vendor folder - provided for backwards compatability" OFF)
    if(INSTALL_TO_MONOLITH)
        # Install rule to ensure that our runtime and linker files are in the expected, platform-specific folders
        install(
                TARGETS ExeFileConsole
                EXPORT ExeFileConsoleTargets
                CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
                RUNTIME DESTINATION bin/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
        )
    else ()
        # Install rule to ensure that our runtime and linker files are in the vcpkg structure
        install(
                TARGETS ExeFileConsole
                EXPORT ExeFileConsoleTargets
                CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
                RUNTIME DESTINATION bin/
        )
    endif ()
endif()
