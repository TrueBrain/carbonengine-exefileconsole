add_executable(ExeFileConsole
    ExeFileConsole.cpp
)
target_compile_definitions(ExeFileConsole PRIVATE NDEBUG _CONSOLE _UNICODE UNICODE)
