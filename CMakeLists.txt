cmake_minimum_required(VERSION 3.1)

project("Real time inter model agreement application")

find_package(FAST REQUIRED)

include(${FAST_USE_FILE})

add_executable(real_time_demo main.cpp DistanceNetProcessor.hpp DistanceNetProcessor.cpp
        AgreementCalculator.hpp AgreementCalculator.cpp CustomGUI.cpp CustomGUI.hpp
        CustomBar.hpp CustomBar.cpp)
add_dependencies(real_time_demo fast_copy)
target_link_libraries(real_time_demo ${FAST_LIBRARIES})
