set(TARGET assimpSceneImportEx)

file(GLOB assimpex_file "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(${TARGET} SHARED ${assimpex_file})

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

target_include_directories(${TARGET} PRIVATE 
       ${PROJECT_SOURCE_DIR}/3rdparty/assimp/include
       ${PROJECT_SOURCE_DIR}/build/3rdparty/assimp/include
       ${PROJECT_SOURCE_DIR}/3rdparty/stb
)

target_link_libraries(${TARGET} PUBLIC
       ${PROJECT_SOURCE_DIR}/build/3rdparty/assimp/lib/${CMAKE_BUILD_TYPE}/*.lib
       Qt${QT_VERSION_MAJOR}::Core
       Qt${QT_VERSION_MAJOR}::CorePrivate
       Qt${QT_VERSION_MAJOR}::Gui
       Qt${QT_VERSION_MAJOR}::3DCore
       Qt${QT_VERSION_MAJOR}::3DCorePrivate
       Qt${QT_VERSION_MAJOR}::3DRender
       Qt${QT_VERSION_MAJOR}::3DRenderPrivate
       Qt${QT_VERSION_MAJOR}::3DExtras
       Qt${QT_VERSION_MAJOR}::3DAnimation
)

if (WIN32)
    INSTALL(TARGETS ${TARGET} RUNTIME DESTINATION ${QT_SDK_DIR}/plugins/sceneparsers)

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
       set(ASSIMP_SCENE_IMPORT_EX_PDB ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/assimpSceneImportExd.pdb)
       INSTALL(FILES ${ASSIMP_SCENE_IMPORT_EX_PDB} DESTINATION ${QT_SDK_DIR}/plugins/sceneparsers)
    endif ()
elseif (UNIX)
    INSTALL(TARGETS ${TARGET} DESTINATION ${QT_SDK_DIR}/plugins/sceneparsers)
endif ()

