add_executable(stress main.cpp stress.cpp stress.hpp contrast_enhancement.cpp contrast_enhancement.hpp)
target_link_libraries( stress ${OpenCV_LIBS} )