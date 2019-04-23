add_library( jobs STATIC
    transform_job.cpp
    )

target_include_directories( jobs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( jobs PRIVATE common Poco::JSON rpn-processor )
