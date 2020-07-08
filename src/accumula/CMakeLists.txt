set(project_name aaqt_storage)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.hpp *.h)

add_library(${project_name} STATIC
    ${SOURCES}
    ${HEADERS}
)
target_link_libraries(${project_name}
    fmt::fmt-header-only
    scn::scn-header-only
)
