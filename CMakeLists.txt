cmake_minimum_required(VERSION 3.8)
project(fire)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "--pipe -W -Wextra -Wall -fPIC -finline-functions -Werror -g")

set(SOURCE_FILES src/main.cpp)

include_directories(src)

file(GLOB SOURCES_FIRE_EVENT
        src/event/*.h
        src/event/*.cpp
        src/event/watcher/*.h
        src/event/watcher/*.cpp
        )

file(GLOB SOURCES_FIRE_CORE
        src/core/*.h
        src/core/*.cpp
        )

file(GLOB SOURCES_FIRE_UTILS
        src/utils/*.h
        src/utils/*.cpp
        )

##############################################################################
#
#           添加第三方依赖
#
##############################################################################
# import ExternalProject
include(ExternalProject)

set(DEPS_DIR ${PROJECT_SOURCE_DIR}/deps)


# import libev from deps/libev-4.24
set(DEPS_LIBEV libev)
set(DEPS_LIBEV_DIR ${DEPS_DIR}/libev-4.24)
ExternalProject_Add(${DEPS_LIBEV}
        SOURCE_DIR ${DEPS_LIBEV_DIR}
        CONFIGURE_COMMAND ${DEPS_LIBEV_DIR}/configure --prefix=<INSTALL_DIR> --disable-shared
        BUILD_COMMAND ${MAKE}
        )
ExternalProject_Get_Property(${DEPS_LIBEV} INSTALL_DIR)
MESSAGE( STATUS "${DEPS_LIBEV} INSTALL_DIR:        " ${INSTALL_DIR} )

include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/lib)

# import readerwriterqueue(thread safe) from github.com
ExternalProject_Add(readerwriterqueue
        GIT_REPOSITORY https://github.com/cameron314/readerwriterqueue.git
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        )

ExternalProject_Get_Property(readerwriterqueue SOURCE_DIR)
MESSAGE( STATUS "readerwriterqueue SOURCE_DIR:        " ${SOURCE_DIR} )

include_directories(${SOURCE_DIR})


# import atom_val(compile time string hash) from github.com
ExternalProject_Add(atom_val
        GIT_REPOSITORY https://github.com/iNevd/atom_val.git
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        )

ExternalProject_Get_Property(atom_val SOURCE_DIR)
MESSAGE( STATUS "atom_val SOURCE_DIR:        " ${SOURCE_DIR} )

include_directories(${SOURCE_DIR})


# import slice from leveldb in github.com
ExternalProject_Add(leveldb_slice
        GIT_REPOSITORY https://github.com/google/leveldb.git
        GIT_TAG master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND mkdir -p <INSTALL_DIR>/include/leveldb
        INSTALL_COMMAND cp <SOURCE_DIR>/include/leveldb/slice.h <SOURCE_DIR>/include/leveldb/export.h <INSTALL_DIR>/include/leveldb
        )

ExternalProject_Get_Property(leveldb_slice INSTALL_DIR)
MESSAGE( STATUS "slice INSTALL_DIR:        " ${INSTALL_DIR} )

include_directories(${INSTALL_DIR}/include)


# import zmalloc from github.com
ExternalProject_Add(zmalloc
        GIT_REPOSITORY https://github.com/iNevd/zmalloc.git
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        )

ExternalProject_Get_Property(zmalloc INSTALL_DIR)
MESSAGE( STATUS "zmalloc INSTALL_DIR:        " ${INSTALL_DIR} )

include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/libstatic)


# import sds from github.com
ExternalProject_Add(sds
        GIT_REPOSITORY https://github.com/iNevd/sds.git
        GIT_TAG cmake_c11
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        )

ExternalProject_Get_Property(sds INSTALL_DIR)
MESSAGE( STATUS "sds INSTALL_DIR:        " ${INSTALL_DIR} )

include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/libstatic)


# import spdlog from github.com
ExternalProject_Add(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v0.16.3
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DSPDLOG_BUILD_TESTING=OFF
)
ExternalProject_Get_Property(spdlog INSTALL_DIR)
MESSAGE( STATUS "spdlog INSTALL_DIR:        " ${INSTALL_DIR} )

include_directories(${INSTALL_DIR}/include)

##############################################################################
#
#           设置executable
#
##############################################################################
add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${SOURCES_FIRE_EVENT}
        ${SOURCES_FIRE_CORE}
        ${SOURCES_FIRE_UTILS}
        )



##############################################################################
#
#           添加第三方依赖相关链接配置
#
##############################################################################
add_dependencies(${PROJECT_NAME} ${DEPS_LIBEV})
add_dependencies(${PROJECT_NAME} readerwriterqueue)
add_dependencies(${PROJECT_NAME} atom_val)
add_dependencies(${PROJECT_NAME} leveldb_slice)
add_dependencies(${PROJECT_NAME} zmalloc)
add_dependencies(${PROJECT_NAME} sds)
add_dependencies(${PROJECT_NAME} spdlog)

# link third lib
target_link_libraries(${PROJECT_NAME} -lev)
target_link_libraries(${PROJECT_NAME} -lzmalloc)
target_link_libraries(${PROJECT_NAME} -lsds)
target_link_libraries(${PROJECT_NAME} -lpthread)
