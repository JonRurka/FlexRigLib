set(module "Resources")

set(${module}_headers
    resources.h
	FileBuilder.h
)

set(${module}_sources
	FileBuilder.cpp
)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})
