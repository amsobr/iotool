add_executable( csv-benchmark
    csv_writer_benchmark.cpp
    )

target_link_libraries( csv-benchmark PRIVATE common jobs Poco::Foundation pthread )

add_executable( rpn-benchmark
    rpn_benchmark.cpp
    )

target_link_libraries( rpn-benchmark PRIVATE common rpn-processor jobs Poco::Foundation pthread )