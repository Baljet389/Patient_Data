cmake_minimum_required(VERSION 3.16)

project(Patient_Data_Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Sql)

# Copy database to build dir
configure_file(PatientenDaten/PatientDataDatabase.db PatientDataDatabase.db COPYONLY)

# Collect all source/header/ui files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${INCLUDE_DIR}/mainwindow.h
    ${UI_DIR}/mainwindow.ui

    ${SRC_DIR}/logindialog.cpp
    ${INCLUDE_DIR}/logindialog.h
    ${UI_DIR}/logindialog.ui

    ${SRC_DIR}/datensatz_bearbeiten.cpp
    ${INCLUDE_DIR}/datensatz_bearbeiten.h
    ${UI_DIR}/datensatz_bearbeiten.ui

    ${SRC_DIR}/datensatz_anzeigen.cpp
    ${INCLUDE_DIR}/datensatz_anzeigen.h
    ${UI_DIR}/datensatz_anzeigen.ui

    ${SRC_DIR}/nutzer_anlegen.cpp
    ${INCLUDE_DIR}/nutzer_anlegen.h
    ${UI_DIR}/nutzer_anlegen.ui

    ${SRC_DIR}/io_data.cpp
    ${INCLUDE_DIR}/io_data.h

    ${SRC_DIR}/user.cpp
    ${INCLUDE_DIR}/user.h

    ${SRC_DIR}/database.cpp
    ${INCLUDE_DIR}/database.h

    ${RESOURCES_DIR}/pictures.qrc
)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt_add_executable(Patient_Data_Project
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(Patient_Data_Project
    PRIVATE
        ${SRC_DIR}
        ${UI_DIR}
        ${INCLUDE_DIR}
)

target_link_libraries(Patient_Data_Project
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Gui Qt6::Sql
)

set_target_properties(Patient_Data_Project PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Optional: For macOS bundle ID pre-Qt 6.1
if(QT_VERSION VERSION_LESS 6.1.0)
  set_target_properties(Patient_Data_Project PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Patient_Data_Project
  )
endif()

# Installation rules
include(GNUInstallDirs)
install(TARGETS Patient_Data_Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Patient_Data_Project)
endif()
