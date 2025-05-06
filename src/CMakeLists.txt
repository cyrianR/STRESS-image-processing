add_executable(stress main.cpp stress.cpp stress.hpp utils.cpp utils.hpp contrast_enhancement.cpp contrast_enhancement.hpp)
target_link_libraries( stress PRIVATE ${OpenCV_LIBS} )

# Add OpenMP flags
target_compile_options(stress PRIVATE ${OpenMP_FLAGS} -Wunknown-pragmas)
target_link_libraries(stress PRIVATE OpenMP::OpenMP_CXX)