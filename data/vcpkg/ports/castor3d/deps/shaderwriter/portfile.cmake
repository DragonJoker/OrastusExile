vcpkg_minimum_required(VERSION 2022-10-12) # for ${VERSION}

vcpkg_from_github(OUT_SOURCE_PATH SOURCE_PATH
    REPO DragonJoker/ShaderWriter
    REF 073f111b92467c93eee884309ad52d871c18a33b
    HEAD_REF development
    SHA512 42908d0df6826aadf9edbf3a65ca686af677dfa455d6387e6cae59f7d437c5db9c46828e1607d4f10661c8e81740de0dc457bdbbbc16bad8d225481d5d8253a3
)

vcpkg_from_github(OUT_SOURCE_PATH CMAKE_SOURCE_PATH
    REPO DragonJoker/CMakeUtils
    REF d66b1c5e964368e036fbfd70a22e84077a7de236
    HEAD_REF master
    SHA512 040c6886675894b4bdb89e9ef5f9f24b669c5644aec88082d06429cb0d47bb041e72517921518b85e02f124058db6182f57e74568d08b1b2316b1992b01f1a3a
)

file(REMOVE_RECURSE "${SOURCE_PATH}/CMake")
file(COPY "${CMAKE_SOURCE_PATH}/" DESTINATION "${SOURCE_PATH}/CMake")

string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "static" BUILD_STATIC)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        spirv SDW_BUILD_EXPORTER_SPIRV
        glsl  SDW_BUILD_EXPORTER_HLSL
        hlsl  SDW_BUILD_EXPORTER_GLSL
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DPROJECTS_USE_PRECOMPILED_HEADERS=OFF
        -DSDW_GENERATE_SOURCE=OFF
        -DSDW_BUILD_VULKAN_LAYER=OFF
        -DSDW_BUILD_TESTS=OFF
        -DSDW_BUILD_STATIC_SDW=${BUILD_STATIC}
        -DSDW_BUILD_STATIC_SDAST=${BUILD_STATIC}
        -DSDW_UNITY_BUILD=ON
        ${FEATURE_OPTIONS}
)

vcpkg_copy_pdbs()
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME shaderwriter CONFIG_PATH lib/cmake/shaderwriter)

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
