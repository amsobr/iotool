add_executable( csv-benchmark
    csv_writer_benchmark.cpp
    )

target_link_libraries( csv-benchmark PRIVATE common jobs Poco::Foundation pthread )