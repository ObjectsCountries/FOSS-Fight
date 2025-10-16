cmake_minimum_required(VERSION 4.0)

project("foss-fight" LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

file(GLOB foss-fight_SRC
     "src/*.hpp"
     "src/*.cpp"
)

add_executable("foss-fight" "${foss-fight_SRC}")

set(foss-fight_ROSTER "Debuggy")

foreach(character ${foss-fight_ROSTER})
    add_custom_command(
        TARGET foss-fight
        PRE_LINK
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ld -r -b binary -o "${CMAKE_CURRENT_BINARY_DIR}/${character}_sprite_sheet.o" "data/characters/${character}.png"
    )
    add_custom_command(
        TARGET foss-fight
        PRE_LINK
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ld -r -b binary -o "${CMAKE_CURRENT_BINARY_DIR}/${character}_data.o" "data/characters/${character}.ff"
    )
endforeach()

file(GLOB foss-fight_ASSETS_OBJ
    "${CMAKE_CURRENT_BINARY_DIR}/*_sprite_sheet.o"
)

file(GLOB foss-fight_DATA_OBJ
    "${CMAKE_CURRENT_BINARY_DIR}/*_data.o"
)

target_sources(foss-fight PRIVATE "${foss-fight_ASSETS_OBJ}" "${foss-fight_DATA_OBJ}")

set_property(TARGET "foss-fight" PROPERTY CXX_STANDARD 26)
set_property(TARGET "foss-fight" PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries("foss-fight" PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)