set(EXEC_NAME client)

set(EXECUTABLE_OUTPUT_PATH ${OUT}/bin/)

include_directories(src)
include_directories(${SHARED_PATH})
file(GLOB_RECURSE SOURCE_FILES "src/*.[ch]pp")

add_executable(${EXEC_NAME} ${SOURCE_FILES} ${SHARED_FILES})

find_package(PkgConfig REQUIRED)

#Link
    include_directories(${SSL_WRAP_INCLUDE})
    target_link_libraries(${EXEC_NAME} ${SSL_WRAP_LIB} ${SSL_WRAP_DEPS})

#Find and link Crypto++
    find_package(CryptoPP REQUIRED)
    target_link_libraries(${EXEC_NAME} ${CRYPTOPP_LIBRARIES})
    target_include_directories(${EXEC_NAME} PUBLIC ${CRYPTOPP_INCLUDE_DIRS})

#Find and link FMT
    find_package(fmt REQUIRED)
    target_link_libraries(${EXEC_NAME} fmt::fmt)

#Find and link PugiXML
    pkg_check_modules(PUGIXML REQUIRED pugixml)
	target_link_libraries(${EXEC_NAME} ${PUGIXML_LIBRARIES})
	target_include_directories(${EXEC_NAME} PUBLIC ${PUGIXML_INCLUDE_DIRS})

#Find and link OpenSSL
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${EXEC_NAME} ${OPENSSL_LIBRARIES})
    target_include_directories(${EXEC_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
