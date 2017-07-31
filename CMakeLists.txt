cmake_minimum_required(VERSION 2.8.2)
project (gamepad)

file(GLOB_RECURSE GAMEPAD_SOURCES "src/*.cxx" "src/*.cpp")

SET(CMAKE_CXX_FLAGS "-std=c++11 -DARCH=x64Linux3gcc4.8.2 -DRTI_UNIX -DRTI_LINUX -DRTI_64BIT -m64 ${CMAKE_CXX_FLAGS}")

find_path(
    NDDS_INCLUDE_DIRS 
    ndds/ndds_c.h
    /opt/rti/include
)

find_library(
    NDDS_LIBRARIES
    NAMES nddscore libnddscore
    HINTS "/opt/rti/lib/x64Linux3gcc4.8.2"
)

execute_process(COMMAND mkdir -pv ${CMAKE_SOURCE_DIR}/src/generated)
execute_process(COMMAND /opt/rti/bin/rtiddsgen -d ${CMAKE_SOURCE_DIR}/src/generated ${CMAKE_SOURCE_DIR}/src/GAMEPADWorld.idl -replace -language C++)

include_directories(${NDDS_INCLUDE_DIRS} ${NDDS_INCLUDE_DIRS}/ndds "src" "src/generated")
set(LIBS ${LIBS} ${NDDS_LIBRARIES})

add_executable(GAMEPAD ${GAMEPAD_SOURCES})
target_link_libraries(GAMEPAD
    ${NDDS_LIBRARIES} -lnddscore -lnddscppd -lnddscd -lnddscored -ldl -lnsl -lm -lpthread -lrt
)
