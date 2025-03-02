cmake_minimum_required(VERSION 3.10)
project(CppGame)

# Ustawienia dla Debug
set(CMAKE_BUILD_TYPE Debug)

# Tworzenie katalogu bin dla pliku wykonywalnego
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dodajemy katalog nagłówków
include_directories(include)

# Pobieramy wszystkie pliki źródłowe
file(GLOB SOURCES "src/*.cpp")

# Tworzymy plik wykonywalny
add_executable(CppGame ${SOURCES})

# Dodajemy katalogi nagłówkowe dla targetu CppGame
target_include_directories(CppGame PRIVATE include)
