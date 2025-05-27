cmake_minimum_required(VERSION 3.16)

# Ime projekta
project(Flowchart LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Poišči potrebne Qt6 komponente
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Viri (.cpp datoteke)
set(SOURCES
    connection.cpp
    diagram.cpp
    drawarea.cpp
    main.cpp
    scrollarea.cpp
    shapes/abstractshape.cpp
    shapes/shapedecision.cpp
    shapes/shapeend.cpp
    shapes/shapefactory.cpp
    shapes/shapeio.cpp
    shapes/shapeprocess.cpp
    shapes/shapestart.cpp
    windowmain.cpp
    json/diagram.cpp
)

# Headerji (.h datoteke)
set(HEADERS
    connection.h
    diagram.h
    dialogshape.h
    drawarea.h
    scrollarea.h
    shapes/abstractshape.h
    shapes/shapedecision.h
    shapes/shapeend.h
    shapes/shapefactory.h
    shapes/shapeio.h
    shapes/shapeprocess.h
    shapes/shapestart.h
    shapes/shapetype.h
    windowmain.h
    json/diagram.h
)

# Resource datoteke
set(RESOURCES res.qrc)

# Ustvari Qt6 resource fajle
qt6_add_resources(GENERATED_RESOURCES ${RESOURCES})

# Ustvari izvršljivo aplikacijo
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${GENERATED_RESOURCES}
    json/abstractshape.h
    json/abstractshape.cpp
    json/shapeprocess.h
    json/shapeprocess.cpp
    json/shapefactory.h
    json/shapefactory.cpp
    json/connection.h
    json/connection.cpp
    dialogshape.cpp
    undo/addshape.h
    undo/addshape.cpp
    erroremitter.h

)

# Poveži Qt6 knjižnice
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Nastavi poti za auto-generirane datoteke (MOC, UIC, RCC)
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Namestitvena pot (enakovredno "target.path" v .pro)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
