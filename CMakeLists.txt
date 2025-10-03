cmake_minimum_required(VERSION 3.12)

project("foss-fight" LANGUAGES C CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

file(GLOB foss-fight_SRC
     "src/*.hpp"
     "src/*.cpp"
)
   
add_executable(foss-fight ${foss-fight_SRC})

set_property(TARGET "foss-fight" PROPERTY CXX_STANDARD 20)

target_link_libraries("foss-fight" PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)


