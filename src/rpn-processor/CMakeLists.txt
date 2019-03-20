
add_library( rpn-processor STATIC  
    rpn_lib.cpp
    subroutine.cpp
        basic_instruction.cpp
        builtin_instruction_handlers.cpp
    context_impl.cpp
)

#target_include_directories( rpn-processor PRIVATE include/drivers ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( rpn-processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( rpn-processor PUBLIC Poco::Foundation )
