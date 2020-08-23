set(accumula_VERSION 3.0.0)

if (NOT accumula_FIND_COMPONENTS)
    set(accumula_FIND_COMPONENTS "accumula-header-only")
    foreach (_accumula_component ${accumula_FIND_COMPONENTS})
        set(accumula_FIND_REQUIRED_${_accumula_component} 1)
    endforeach()
endif()

#set(_accumula_include_dir "@PACKAGE_THIRD_PARTY_INCLUDES@;@PACKAGE_SRC_DIR@")
set(_accumula_include_dir "@PACKAGE_SRC_DIR@")

add_library(accumula::accumula-header-only INTERFACE IMPORTED)

set_target_properties(accumula::accumula-header-only PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_accumula_include_dir}"
)

unset(_library_ext)
unset(_accumula_include_dir)
