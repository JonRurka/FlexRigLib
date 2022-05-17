set(module "Physics")

set(${module}_headers
	SimConstants.h
)

set(${module}_sources
)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})

