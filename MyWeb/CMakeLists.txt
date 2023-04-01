cmake_minimum_required(VERSION 3.2)   
project (test)
#设置C++标准
set (CMAKE_CXX_STANDARD 14)
add_definitions("-g")

set(src ./src)
set(tools ./tools)
set(json_include ./tools/nlohmann)
set(mysql_include /www/server/mysql/include)
set(mysql_lib /www/server/mysql/lib)


add_executable(main main.cpp ${tools}/_reactor.cpp ${tools}/_log.cpp  
                    ${tools}/_network.cpp ${tools}/_threadpool.cpp ${tools}/_channel.cpp
                    ${tools}/_directory.cpp ${tools}/_mysql.cpp ${tools}/_dbpool.cpp
                    ${tools}/_http.cpp ${tools}/http_parser.cpp
                    ${src}/get.cpp ${src}/login.cpp ${src}/upload.cpp)

target_include_directories(main PUBLIC ${tools} ${src} ${json_include} ${mysql_include} ${mysql_lib})
target_link_libraries(main pthread mysqlclient)
target_link_directories(main PUBLIC ${mysql_lib})