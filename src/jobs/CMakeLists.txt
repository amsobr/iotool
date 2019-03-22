add_library( jobs STATIC
    job.cpp
    csv_writer.cpp
    output_channel_manager.cpp
    )

target_include_directories( jobs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( jobs PRIVATE common Poco::JSON rpn-processor )
