set(EXEC_NAME server)

set(EXECUTABLE_OUTPUT_PATH ${OUT}/bin/)

include_directories(src)
include_directories(${SHARED_PATH})
file(GLOB_RECURSE SOURCE_FILES "src/*.[ch]pp")

add_executable(${EXEC_NAME} ${SOURCE_FILES} ${SHARED_FILES})

find_package(PkgConfig REQUIRED)

#Link
	find_package(SSLWrap REQUIRED)
	include_directories(${SSLWRAP_INCLUDE})
	target_link_libraries(${EXEC_NAME} ${SSLWRAP_LIBRARY} ${SSLWRAP_DEPS})

#Find and link Crypto++
    find_package(CryptoPP REQUIRED)
    target_link_libraries(${EXEC_NAME} ${CRYPTOPP_LIBRARIES})
    target_include_directories(${EXEC_NAME} PUBLIC ${CRYPTOPP_INCLUDE_DIRS})

#Find and link FMT
    find_package(fmt REQUIRED)
    target_link_libraries(${EXEC_NAME} fmt::fmt)