#Add some things the binaries share, such as a SSL socket wrapper

set(LIBRARY_OUTPUT_PATH ${OUT}/lib)

file(GLOB_RECURSE SOURCE_FILES "*.[ch]pp")
include_directories(src)

set(LIB_NAME sslWrapper)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})

#Find and link OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(${LIB_NAME} ${OPENSSL_LIBRARIES})

target_link_libraries(${LIB_NAME} stdc++fs)

file(GLOB_RECURSE HEADERS "*.hpp")
file(COPY ${HEADERS} DESTINATION ${OUT}/lib/include/SSLWrap)

set(SSL_WRAP_LIB $<TARGET_FILE:${LIB_NAME}> PARENT_SCOPE)
set(SSL_WRAP_DEPS ${OPENSSL_LIBRARIES} stdc++fs PARENT_SCOPE)
set(SSL_WRAP_INCLUDE ${OUT}/lib/include PARENT_SCOPE)