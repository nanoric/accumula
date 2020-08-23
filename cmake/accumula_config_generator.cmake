# generate accumula-config.cmake for other libraries

function(accumula_generate_config)
    get_filename_component(ACCUMULA_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
    get_filename_component(THIRD_PARTY_INCLUDES "${ACCUMULA_DIR}/3rd_party/include" ABSOLUTE)
    get_filename_component(OUT_DIR "${OUT_DIR}" ABSOLUTE)
    get_filename_component(LIB_DIR "${LIB_DIR}" ABSOLUTE)
    get_filename_component(SRC_DIR "${ACCUMULA_DIR}/src" ABSOLUTE)

    include(CMakePackageConfigHelpers)

    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/templates/accumula-config.template.cmake
        ${CMAKE_CURRENT_LIST_DIR}/accumula-config.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cmake
        PATH_VARS ACCUMULA_DIR THIRD_PARTY_INCLUDES OUT_DIR LIB_DIR SRC_DIR)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_LIST_DIR}/accumula-version.cmake
      VERSION 1.0.0
      COMPATIBILITY AnyNewerVersion)
endfunction()

accumula_generate_config()
