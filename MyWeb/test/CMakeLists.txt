cmake_minimum_required(VERSION 3.2)   
project (test)
#设置C++标准
set (CMAKE_CXX_STANDARD 14)
add_definitions("-g")

set(src ../src)
set(tools ../tools)
set(json_include ../tools/nlohmann)
set(mysql_include /www/server/mysql/include)
set(mysql_lib /www/server/mysql/lib)

# t_login
add_executable(t_login t_login.cpp ${src}/login.cpp ${tools}/_dbpool.cpp ${tools}/_log.cpp ${tools}/_mysql.cpp)
target_include_directories(t_login PUBLIC ${tools} ${src} ${mysql_include})
target_link_directories(t_login PUBLIC ${mysql_lib})
target_link_libraries(t_login pthread mysqlclient)

# t_dir
add_executable(t_dir t_dir.cpp ${tools}/_directory.cpp)
target_include_directories(t_dir PUBLIC ${tools} ${src})

# t_upload
add_executable(t_upload t_upload.cpp ${src}/upload.cpp ${tools}/_directory.cpp)
target_include_directories(t_upload PUBLIC ${tools} ${src})

# t_showlist
add_executable(t_showlist t_showlist.cpp ${src}/show.cpp ${tools}/_directory.cpp)
target_include_directories(t_showlist PUBLIC ${tools} ${src} ${json_include})

# t_get
#add_executable(t_get t_get.cpp ${src}/response.cpp ${tools}/_directory.cpp ${src}/login.cpp ${src}/show.cpp ${src}/upload.cpp)
#target_include_directories(t_get PUBLIC ${tools} ${src})

# t_post
add_executable(t_post t_post.cpp ${src}/response.cpp ${tools}/_directory.cpp  ${src}/show.cpp ${src}/upload.cpp
            ${src}/login.cpp ${tools}/_dbpool.cpp ${tools}/_log.cpp ${tools}/_mysql.cpp)
target_include_directories(t_post PUBLIC ${tools} ${src} ${mysql_include})
target_link_directories(t_post PUBLIC ${mysql_lib})
target_link_libraries(t_post pthread mysqlclient)

# t_login
add_executable(t_message t_message.cpp ${src}/message.cpp ${tools}/_dbpool.cpp ${tools}/_log.cpp ${tools}/_mysql.cpp)
target_include_directories(t_message PUBLIC ${tools} ${src} ${mysql_include})
target_link_directories(t_message PUBLIC ${mysql_lib})
target_link_libraries(t_message pthread mysqlclient)