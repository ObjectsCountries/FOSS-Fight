cmake_minimum_required(VERSION 4.0)

project("foss-fight" LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

file(GLOB foss-fight_SRC
     "src/*.hpp"
     "src/*.cpp"
)

file(GLOB foss-fight_ASSETS
    "data/characters/*.png"
)

foreach(spriteSheet ${foss-fight_ASSETS})
    add_custom_command(
            OUTPUT ${spriteSheet}.o
            COMMAND ld -r -b binary -o ${spriteSheet}.o ../${spriteSheet}
            DEPENDS "../${spriteSheet}")
endforeach()

file(GLOB foss-fight_DATA
        "data/characters/*.ff"
)

foreach(data ${foss-fight_DATA})
    add_custom_command(
            OUTPUT ${data}.o
            COMMAND ld -r -b binary -o ${data}.o ../${data}
            DEPENDS "../${data}")
endforeach()

add_custom_command(
        OUTPUT debuggy_data.o
        COMMAND ld -r -b binary -o debuggy_data.o ../data/characters/Debuggy.ff
        DEPENDS "../data/characters/Debuggy.ff")

add_executable(foss-fight ${foss-fight_SRC} debuggy_data.o)

set_property(TARGET "foss-fight" PROPERTY CXX_STANDARD 26)
set_property(TARGET "foss-fight" PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries("foss-fight" PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)