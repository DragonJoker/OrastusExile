vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO DragonJoker/Ashes
    REF 8511d0d68b6f16a45de17222899e24b7fc1cd8b0
    HEAD_REF master
    SHA512 cc0ddbc0969d4dc35f5ebca37ab2b1a8d50ecf99e78196c47f5c9badd86e3292280dbf946da8b1b38cddecfd62952d6a730b89ee9c304633e71b9a969e295f48
)

vcpkg_from_github(
    OUT_SOURCE_PATH CMAKE_SOURCE_PATH
    REPO DragonJoker/CMakeUtils
    REF 4e0292ed50d76dab5fc8c81840ae0e021dc60c2a
    HEAD_REF master
    SHA512 c79c6a5ef2e059b56d4de20cc73e74386bf8b6acea2f6b76fd9949a6a2760f82302c90419e4a753f50c30d01cc4f3a039e04b585f5c0d4461cce3464d9fb9c95
)

file(REMOVE_RECURSE "${SOURCE_PATH}/CMake")
file(COPY "${CMAKE_SOURCE_PATH}/" DESTINATION "${SOURCE_PATH}/CMake")

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        direct3d11 ASHES_BUILD_RENDERER_D3D11
        opengl     ASHES_BUILD_RENDERER_OGL
        vulkan     ASHES_BUILD_RENDERER_VKN
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DVCPKG_PACKAGE_BUILD=ON
        -DASHES_BUILD_TEMPLATES=OFF
        -DASHES_BUILD_TESTS=OFF
        -DASHES_BUILD_INFO=OFF
        -DASHES_BUILD_SAMPLES=OFF
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/ashes)

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)