#include "dll_main.h"
#include "Compute/ComputeEngine.h"



#define PHYSICS_DT 0.0005f // fixed dt of 0.5 ms

//using namespace FlexRigLib::Compute;

EXPORTED void ComputeEngine_Init()
{
	FlexRigLib::Compute::ComputeEngine::Init(1, "");
}

EXPORTED void ComputeEngine_Get_CL_Version(char* str, int size)
{
	strcpy(str, FlexRigLib::Compute::ComputeEngine::Get_CL_Version().c_str());
}




