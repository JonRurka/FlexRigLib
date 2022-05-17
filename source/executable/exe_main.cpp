//#include "dll_main.h"
#include "Compute/ComputeController.h"
#include "Compute/ProgramBuilder.h"
#include "dll_main.h"

#define DATA_SIZE 10

using namespace FlexRigLib::Compute;

const char* KernelSource =
"__kernel void hello(__global float *input, __global float *output)\n"\
"{\n"\
"  size_t id = get_global_id(0);\n"
"  output[id] = input[id] * input[id];\n"
"}\n"\
"\n";

char error_str[ERROR_SIZE];

float other_inputData[DATA_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
float other_results[DATA_SIZE] = { 1 };

int main()
{
	printf("Init stats: %i\n", ComputeEngine::Init(1, ""));

	//char version[1000];
	//ComputeEngine_Get_CL_Version(version, 1000);

	ComputeEngine_Init();

	ComputeEngine_Get_CL_Version(NULL, 0);

	//printf("OpenCL Version: %s\n", ComputeEngine::Get_CL_Version().c_str());



	//printf("Version: %s\n", version);
	//return 0;

	/*size_t global = DATA_SIZE;

	char pBuf[256];
	size_t len = sizeof(pBuf);
	int bytes = GetModuleFileName(NULL, pBuf, len);

	std::string app_dir = pBuf;
	app_dir = app_dir.substr(0, app_dir.find_last_of('\\'));

	printf("App dir: %s\n", app_dir.c_str());

	ComputeEngine::Init(1, app_dir);


	char platformInfo[1000];

	clGetPlatformInfo(ComputeEngine::GetPlatform(), CL_PLATFORM_VERSION, 1000, platformInfo, NULL);
	printf("OpenCL Version: %s\n", platformInfo);

	ComputeContext* c_context = ComputeEngine::GetNewContext();

	c_context->Add_Program("p", KernelSource);


	int res = c_context->Programs("p")->Build(error_str, ERROR_SIZE);
	printf("Build res: %i\n", res);
	printf("Build Status: %s\n", error_str);

	ComputeKernel* c_kernel = c_context->GetKernel("p", "hello");

	ComputeBuffer* c_input = c_context->GetBuffer(ComputeBuffer::Buffer_Type::READ, sizeof(float) * DATA_SIZE);
	c_input->SetData(0, other_inputData);

	ComputeBuffer* c_output = c_context->GetBuffer(ComputeBuffer::Buffer_Type::Write, sizeof(float) * DATA_SIZE);

	c_kernel->SetBuffer(0, c_input, 0);
	c_kernel->SetBuffer(0, c_output, 1);

	int got = c_kernel->Execute(0, 1, &global);

	int out_res = c_output->GetData(0, other_results);
	printf("Got: %i\n", got);
	printf("Result: %i\n", out_res);
	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("%f \n", other_results[i]);
	}

	c_context->Dispose();

	return 0;*/



	/*char pBuf[256];
	size_t len = sizeof(pBuf);
	int bytes = GetModuleFileName(NULL, pBuf, len);

	std::string app_dir = pBuf;
	app_dir = app_dir.substr(0, app_dir.find_last_of('\\'));

	ComputeController* comp = new ComputeController("");

	char platformInfo[1000];

	printf("OpenCL Version: %s\n", ComputeEngine::Get_CL_Version().c_str());

	comp->GetProgramBuilder()->AddKernel("hello", KernelSource);

	comp->BuildProgram();

	printf("Build State: %i\n", comp->GetState());
	if (comp->GetState() == ComputeController::ComputeState::BuildError)
	{
		printf("Error building program: %s\n", comp->GetProgramBuilder()->GetError().c_str());
		return 1;
	}

	ComputeBuffer* in_buff = comp->NewReadBuffer(sizeof(float) * DATA_SIZE);
	in_buff->SetData(other_inputData);

	ComputeBuffer* out_buff = comp->NewWriteBuffer(sizeof(float) * DATA_SIZE);

	printf("bind in: %i\n", comp->KernelAddBuffer("hello", in_buff));
	printf("bind out: %i\n", comp->KernelAddBuffer("hello", out_buff));


	int runRes = comp->RunKernel("hello", DATA_SIZE, 0, 0);
	if (runRes != 0)
		printf("Kernel Run error: %i\n", runRes);

	out_buff->GetData(0, other_results);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("%f \n", other_results[i]);
	}*/
	
	//comp->Dispose();
}