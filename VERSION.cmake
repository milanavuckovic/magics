

set (x 
EXECUTE_PROCESS(
        COMMAND sh -c "cd ${CMAKE_SOURCE_DIR}; git rev-parse --abbrev-ref HEAD")
        )
debug_var (x)
set( ${PROJECT_NAME}_VERSION_STR  "2.21.0-rc1" )

debug_var(${PROJECT_NAME}_VERSION_STR)
