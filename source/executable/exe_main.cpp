//#include "dll_main.h"
#include "Compute/ComputeController.h"
#include "Compute/ProgramBuilder.h"
#include "dll_main.h"

#include <time.h>
#include <chrono>

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


typedef struct mat_col { float c[3]; } mat_col;
typedef struct fmat3x3 { mat_col r[3]; } fmat3x3;

//int other_inputData[DATA_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//int other_results[DATA_SIZE] = { 1 };

typedef struct variables {
	float s40; //sum of x^4
	float s30; //sum of x^3
	float s20; //sum of x^2
	float s10;  //sum of x

	float s21; //sum of x^2*y
	float s11;  //sum of x*y
	float s01;   //sum of y
} Variables;

float getSx(float points[3][2]) // get sum of x
{
	float Sx = 0;
	for(int i = 0; i < 3; i++)
	{
		Sx += points[i][0];
	}
	return Sx;
}

float getSy(float points[3][2]) // get sum of y
{
	float Sy = 0;
	for(int i = 0; i < 3; i++)
	{
		Sy += points[i][1];
	}
	return Sy;
}

float getSx2(float points[3][2]) // get sum of x^2
{
	float Sx2 = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx2 += std::pow(points[i][0], 2); // sum of x^2
	}
	return Sx2;
}

float getSx3(float points[3][2]) // get sum of x^3
{
	float Sx3 = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx3 += std::pow(points[i][0], 3); // sum of x^3
	}
	return Sx3;
}

float getSx4(float points[3][2]) // get sum of x^4
{
	float Sx4 = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx4 += std::pow(points[i][0], 4); // sum of x^4
	}
	return Sx4;
}

float getSxy(float points[3][2]) // get sum of x*y
{
	float Sxy = 0;
	for (int i = 0; i < 3; i++)
	{
		Sxy += points[i][0] * points[i][1]; // sum of x*y
	}
	return Sxy;
}

float getSx2y(float points[3][2]) // get sum of x^2*y
{
	float Sx2y = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx2y += std::pow(points[i][0], 2) * points[i][1]; // sum of x^2*y
	}
	return Sx2y;
}

variables preCalculate(float points[3][2])
{
	Variables v;

	v.s40 = getSx4(points); //sum of x^4
	v.s30 = getSx3(points); //sum of x^3
	v.s20 = getSx2(points); //sum of x^2
	v.s10 = getSx(points);  //sum of x

	v.s21 = getSx2y(points); //sum of x^2*y
	v.s11 = getSxy(points);  //sum of x*y
	v.s01 = getSy(points);   //sum of y

	return v;
}

float aTerm(Variables v)
{
	//notation sjk to mean the sum of x_i^j*y_i^k. 
	float s40 = v.s40; //sum of x^4
	float s30 = v.s30; //sum of x^3
	float s20 = v.s20; //sum of x^2
	float s10 = v.s10;  //sum of x

	//sum of x^0 * y^0  ie 1 * number of entries

	float s21 = v.s21; //sum of x^2*y
	float s11 = v.s11;  //sum of x*y
	float s01 = v.s01;   //sum of y
	

	float s00 = 3;

	//a = Da/D
	return (s21 * (s20 * s00 - s10 * s10) -
		s11 * (s30 * s00 - s10 * s20) +
		s01 * (s30 * s10 - s20 * s20))
		/
		(s40 * (s20 * s00 - s10 * s10) -
			s30 * (s30 * s00 - s10 * s20) +
			s20 * (s30 * s10 - s20 * s20));
}

float bTerm(Variables v)
{
	//notation sjk to mean the sum of x_i^j*y_i^k. 
	float s40 = v.s40; //sum of x^4
	float s30 = v.s30; //sum of x^3
	float s20 = v.s20; //sum of x^2
	float s10 = v.s10;  //sum of x

	//sum of x^0 * y^0  ie 1 * number of entries

	float s21 = v.s21; //sum of x^2*y
	float s11 = v.s11;  //sum of x*y
	float s01 = v.s01;   //sum of y


	float s00 = 3;

	//b = Db/D
	return (s40 * (s11 * s00 - s01 * s10) -
		s30 * (s21 * s00 - s01 * s20) +
		s20 * (s21 * s10 - s11 * s20))
		/
		(s40 * (s20 * s00 - s10 * s10) -
			s30 * (s30 * s00 - s10 * s20) +
			s20 * (s30 * s10 - s20 * s20));
}

float cTerm(Variables v)
{
	//notation sjk to mean the sum of x_i^j*y_i^k. 
	float s40 = v.s40; //sum of x^4
	float s30 = v.s30; //sum of x^3
	float s20 = v.s20; //sum of x^2
	float s10 = v.s10;  //sum of x

	//sum of x^0 * y^0  ie 1 * number of entries

	float s21 = v.s21; //sum of x^2*y
	float s11 = v.s11;  //sum of x*y
	float s01 = v.s01;   //sum of y


	float s00 = 3;

	//c = Dc/D
	return (s40 * (s20 * s01 - s10 * s11) -
		s30 * (s30 * s01 - s10 * s21) +
		s20 * (s30 * s11 - s20 * s21))
		/
		(s40 * (s20 * s00 - s10 * s10) -
			s30 * (s30 * s00 - s10 * s20) +
			s20 * (s30 * s10 - s20 * s20));
}


int main()
{
	float a_term = 0;
	float b_term = 0;
	float c_term = 0;

	clock_t tStart = clock();
	auto ch_start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1; i++)
	{

		float points[3][2] = {
			{ 0 + (400000 - 1) / (float)10, 1 },
			{ 0.5 + (400000 - 1) / (float)10, 0 },
			{ 1 + (400000 - 1) / (float)10, 1 }
		};



		Variables var = preCalculate(points);



		//LstSquQuadRegr.Variables var = solvr.PreCalculate();


		/*for (int i = 0; i < 3; i++)
		{
			Console.Write("P{0} X: ", i + 1);
			float x = float.Parse(Console.ReadLine());

			Console.Write("P{0} Y: ", i + 1);
			float y = float.Parse(Console.ReadLine());

			solvr.AddPoints(x, y);
		}*/



		a_term += aTerm(var);
		b_term += bTerm(var);
		c_term += cTerm(var);

		//printf("Equation: %fx ^ 2 + %fx + %f\n", a_term, b_term, c_term);

		//Console.WriteLine("Equation: {0}x^2 + {1}x + {2}", a_term, b_term, c_term);
	}
	auto ch_finish = std::chrono::high_resolution_clock::now();
	clock_t tfinish = clock();
	printf("Clock Time taken: %.10fs\n", (double)(tfinish - (clock_t)tStart) / CLOCKS_PER_SEC);

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(ch_finish - ch_start);
	double dur = duration.count() / (double)1000000;
	printf("Chrono Time taken: %.10fs\n", dur);

	printf("Equation - CPU: %fx ^ 2 + %fx + %f\n", a_term, b_term, c_term);

	//return 0;

	std::vector<ComputeEngine::Platform> platforms = ComputeEngine::GetSupportedPlatforms();

	for (const auto p : platforms)
	{
		printf("Platform %s:\n", p.name);

		std::vector<ComputeEngine::Device> devices = ComputeEngine::GetSupportedDevices(p);
		for (auto d : devices)
		{
			printf("\t%s - %s: Frequency: %u, threads: %u, Memory: %lu, Work Size: %u \n", d.vendor, d.name, d.clock_frequency, d.num_compute_units * d.group_size, d.mem_size, d.max_work_size);
		}
	}

	ComputeEngine::Platform platf = platforms[0];
	ComputeEngine::Device dev = ComputeEngine::GetSupportedDevices(platf)[0];

	//return 0;

	/*const glm::vec3 u = glm::vec3(0);
	const glm::vec3 v = glm::vec3(0);
	const glm::vec3 n = glm::vec3(0);


	auto l_matrix = glm::mat3x3(u[0], u[1], u[2],
								v[0], v[1], v[2],
								n[0], n[1], n[2]);
	auto m_matrix = glm::inverse(l_matrix);

	auto m = m_matrix * l_matrix;*/

	//printf("size of Device: %u\n", sizeof(ComputeEngine::Device));

	ComputeController* controller = new ComputeController(platf, dev, "");

	controller->ConstructProgram();
	controller->BuildProgram();

	if (controller->GetState() == ComputeController::ComputeState::BuildError)
	{
		printf("%s\n\n -------------- \n\n", controller->GetProgramBuilder()->GetFullSource().c_str());
		printf("%s\n", controller->GetProgramBuilder()->GetError().c_str());
		return -1;
	}

	fmat3x3 matrix;
	
	const int IN_SIZE = 1;
	const int OUT_SIZE = 1;
	const int RUN_SIZE = 1;

	/*glm::vec3 inputData[IN_SIZE] = {
		glm::vec3(0, 0, 0),
		glm::vec3(0.5, 1, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0.5f, 0.5f, 1)
	};*/

	/*float inputData[IN_SIZE][3] = {
		{1.1, 1.2, 1.3},
		{2.1, 2.2, 2.3},
		{3.1, 3.2, 3.3},
		{4.1, 4.2, 4.3}
	};*/

	int inputData[IN_SIZE] =
	{
		400000
	};

	glm::vec3 resultData[OUT_SIZE] = {
		glm::vec3(0)
	};

	ComputeBuffer* c_buf_in = controller->NewReadBuffer(sizeof(int) * IN_SIZE);
	c_buf_in->SetData(inputData);

	ComputeBuffer* c_buf_out = controller->NewWriteBuffer(sizeof(glm::vec3) * OUT_SIZE);

	controller->KernelAddBuffer("testRegression", c_buf_in);
	controller->KernelAddBuffer("testRegression", c_buf_out);

	ch_start = std::chrono::high_resolution_clock::now();

	controller->RunKernel("testRegression", RUN_SIZE, 1, 1);

	


	c_buf_out->GetData(resultData);

	ch_finish = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(ch_finish - ch_start);
	dur = duration.count() / (double)1000000;
	printf("Chrono Time taken - GPU: %.10fs\n", dur);

	for (int i = 0; i < 1; i++)
	{
		printf("%f, %f, %f\n", resultData[i].x, resultData[i].y, resultData[i].z);
	}

	// printf("Init stats: %i\n", ComputeEngine::Init(1, ""));

	//char version[1000];
	//ComputeEngine_Get_CL_Version(version, 1000);

	//ComputeEngine_Init();

	//ComputeEngine_Get_CL_Version(NULL, 0);

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