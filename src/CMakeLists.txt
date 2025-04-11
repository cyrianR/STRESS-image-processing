add_executable(stress main.cpp stress.cpp stress.hpp)
target_link_libraries( stress ${OpenCV_LIBS} )