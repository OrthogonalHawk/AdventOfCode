cmake_minimum_required(VERSION 3.10)
project(AdventOfCode)

# Function to create an executable for a specific day
function(add_day_executable DAY)
    # Define the sources for the executable
    set(SOURCES
        src/${DAY}.cpp
        src/conversion.cpp
        src/utils.cpp
    )

    # Create the executable
    add_executable(${DAY} ${SOURCES})
endfunction()

include_directories(hdr)

# Create executables for different days
add_day_executable(day00)
add_day_executable(day01)
add_day_executable(day02)
add_day_executable(day03)