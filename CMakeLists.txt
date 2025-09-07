cmake_minimum_required(VERSION 3.12)

project(ipfilter LANGUAGES CXX)

option(WITH_BOOST_TEST "Build Boost test" OFF)

# --- основной бинарник ---
add_executable(ipfilter_cli
    ip_filter.cpp
)

set_target_properties(ipfilter_cli PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(ipfilter_cli PRIVATE /W4)
else()
    target_compile_options(ipfilter_cli PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# --- опциональные тесты ---
if (WITH_BOOST_TEST)
    # подавим предупреждение про CMP0167 на новых cmake
    if (POLICY CMP0167)
        cmake_policy(SET CMP0167 NEW)
    endif()

    find_package(Boost 1.70 REQUIRED COMPONENTS unit_test_framework)

    add_executable(test_version
        test_version.cpp
    )
    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    target_link_libraries(test_version PRIVATE Boost::unit_test_framework)
    target_compile_definitions(test_version PRIVATE BOOST_TEST_DYN_LINK)

    enable_testing()
    add_test(NAME test_version COMMAND test_version)
endif()

# --- установка (опционально) ---
install(TARGETS ipfilter_cli RUNTIME DESTINATION bin)

# --- упаковка (можно вообще убрать, если не нужно) ---
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_CONTACT example@example.com)
set(CPACK_PACKAGE_FILE_NAME "ipfilter-1.0.0-Linux")
include(CPack)