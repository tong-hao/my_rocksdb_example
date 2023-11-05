add_executable(get_started get_started.cpp)
target_link_libraries(get_started rocksdb zstd pthread bz2 lz4 z snappy)
