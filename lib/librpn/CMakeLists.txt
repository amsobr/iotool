
add_library( rpn STATIC
    Argument.cpp
    rpn_lib.cpp
    builtin_instruction_handlers.cpp
    context_impl.cpp
    include/rpn/abstract_instruction.hpp
    include/rpn/Argument.h
    include/rpn/ArgumentList.h
    include/rpn/InstructionAdapter.h
    include/rpn/SubroutineAdapter.h
)

set_target_properties( rpn PROPERTIES
                        CMAKE_CXX_STANDARD 17
                        CMAKE_CXX_STANDARD_REQUIRED ON )

#target_include_directories( rpn-processor PRIVATE include/drivers ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( rpn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( rpn PUBLIC Poco::Foundation Poco::JSON )
