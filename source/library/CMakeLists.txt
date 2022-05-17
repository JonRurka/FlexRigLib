project("FlexRigLib")

set(${PROJECT_NAME}_headers
    dll_main.h
    ForwardDeclarations.h
    C_Variables.h
    Logger.h
    stdafx.h
)

set(${PROJECT_NAME}_sources
    dll_main.cpp
    C_Variables.cpp
    Logger.cpp
)

add_subdirectory(utils)
add_subdirectory(resources)
# add_subdirectory(physics)
# add_subdirectory(terrain)
add_subdirectory(Compute)


add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})
target_link_libraries(${PROJECT_NAME} Utils Resources Compute)
target_link_libraries (FlexRigLib OpenCL)