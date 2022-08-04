
#include "ShaderDepository.h"

using namespace FlexRigLib::Compute;

void ShaderDepository::Load()
{
    AddKernel("testRegression", "\n\n__kernel void testRegression(__global int* input, __global float3 *output)\n{\n	size_t id = get_global_id(0);\n	\n	float a_term = 0;\n	float b_term = 0;\n	float c_term = 0;\n	int i = 0;\n	for (i = 0; i < input[id]; i++)\n	{		\n		float points[3][2] = {\n			{ 0 + i / (float)10, 1 },\n			{ 0.5 + i / (float)10, 0 },\n			{ 1 + i / (float)10, 1 }\n		};\n		\n		struct Variables var = regress3_preCalculate(points);\n		\n		a_term = regress3_aTerm(var);\n		b_term = regress3_bTerm(var);\n		c_term = regress3_cTerm(var);\n	}\n	\n\n	output[id] = (float3)((float)i, b_term, c_term);\n}");
    AddSource("regression3", "struct Variables {\n	float s40; //sum of x^4\n	float s30; //sum of x^3\n	float s20; //sum of x^2\n	float s10;  //sum of x\n\n	float s21; //sum of x^2*y\n	float s11;  //sum of x*y\n	float s01;   //sum of y\n} Variables;\n\nfloat getSx(float points[3][2]) // get sum of x\n{\n	float Sx = 0;\n	for(int i = 0; i < 3; i++)\n	{\n		Sx += points[i][0];\n	}\n	return Sx;\n}\n\nfloat getSy(float points[3][2]) // get sum of y\n{\n	float Sy = 0;\n	for(int i = 0; i < 3; i++)\n	{\n		Sy += points[i][1];\n	}\n	return Sy;\n}\n\nfloat getSx2(float points[3][2]) // get sum of x^2\n{\n	float Sx2 = 0;\n	for (int i = 0; i < 3; i++)\n	{\n		Sx2 += pow(points[i][0], 2); // sum of x^2\n	}\n	return Sx2;\n}\n\nfloat getSx3(float points[3][2]) // get sum of x^3\n{\n	float Sx3 = 0;\n	for (int i = 0; i < 3; i++)\n	{\n		Sx3 += pow(points[i][0], 3); // sum of x^3\n	}\n	return Sx3;\n}\n\nfloat getSx4(float points[3][2]) // get sum of x^4\n{\n	float Sx4 = 0;\n	for (int i = 0; i < 3; i++)\n	{\n		Sx4 += pow(points[i][0], 4); // sum of x^4\n	}\n	return Sx4;\n}\n\nfloat getSxy(float points[3][2]) // get sum of x*y\n{\n	float Sxy = 0;\n	for (int i = 0; i < 3; i++)\n	{\n		Sxy += points[i][0] * points[i][1]; // sum of x*y\n	}\n	return Sxy;\n}\n\nfloat getSx2y(float points[3][2]) // get sum of x^2*y\n{\n	float Sx2y = 0;\n	for (int i = 0; i < 3; i++)\n	{\n		Sx2y += pow(points[i][0], 2) * points[i][1]; // sum of x^2*y\n	}\n	return Sx2y;\n}\n\nstruct Variables regress3_preCalculate(float points[3][2])\n{\n	struct Variables v;\n\n	v.s40 = getSx4(points); //sum of x^4\n	v.s30 = getSx3(points); //sum of x^3\n	v.s20 = getSx2(points); //sum of x^2\n	v.s10 = getSx(points);  //sum of x\n\n	v.s21 = getSx2y(points); //sum of x^2*y\n	v.s11 = getSxy(points);  //sum of x*y\n	v.s01 = getSy(points);   //sum of y\n\n	return v;\n}\n\nfloat regress3_aTerm(struct Variables v)\n{\n	//notation sjk to mean the sum of x_i^j*y_i^k. \n	float s40 = v.s40; //sum of x^4\n	float s30 = v.s30; //sum of x^3\n	float s20 = v.s20; //sum of x^2\n	float s10 = v.s10;  //sum of x\n\n	//sum of x^0 * y^0  ie 1 * number of entries\n\n	float s21 = v.s21; //sum of x^2*y\n	float s11 = v.s11;  //sum of x*y\n	float s01 = v.s01;   //sum of y\n	\n\n	float s00 = 3;\n\n	//a = Da/D\n	return (s21 * (s20 * s00 - s10 * s10) -\n		s11 * (s30 * s00 - s10 * s20) +\n		s01 * (s30 * s10 - s20 * s20))\n		/\n		(s40 * (s20 * s00 - s10 * s10) -\n			s30 * (s30 * s00 - s10 * s20) +\n			s20 * (s30 * s10 - s20 * s20));\n}\n\nfloat regress3_bTerm(struct Variables v)\n{\n	//notation sjk to mean the sum of x_i^j*y_i^k. \n	float s40 = v.s40; //sum of x^4\n	float s30 = v.s30; //sum of x^3\n	float s20 = v.s20; //sum of x^2\n	float s10 = v.s10;  //sum of x\n\n	//sum of x^0 * y^0  ie 1 * number of entries\n\n	float s21 = v.s21; //sum of x^2*y\n	float s11 = v.s11;  //sum of x*y\n	float s01 = v.s01;   //sum of y\n\n\n	float s00 = 3;\n\n	//b = Db/D\n	return (s40 * (s11 * s00 - s01 * s10) -\n		s30 * (s21 * s00 - s01 * s20) +\n		s20 * (s21 * s10 - s11 * s20))\n		/\n		(s40 * (s20 * s00 - s10 * s10) -\n			s30 * (s30 * s00 - s10 * s20) +\n			s20 * (s30 * s10 - s20 * s20));\n}\n\nfloat regress3_cTerm(struct Variables v)\n{\n	//notation sjk to mean the sum of x_i^j*y_i^k. \n	float s40 = v.s40; //sum of x^4\n	float s30 = v.s30; //sum of x^3\n	float s20 = v.s20; //sum of x^2\n	float s10 = v.s10;  //sum of x\n\n	//sum of x^0 * y^0  ie 1 * number of entries\n\n	float s21 = v.s21; //sum of x^2*y\n	float s11 = v.s11;  //sum of x*y\n	float s01 = v.s01;   //sum of y\n\n\n	float s00 = 3;\n\n	//c = Dc/D\n	return (s40 * (s20 * s01 - s10 * s11) -\n		s30 * (s30 * s01 - s10 * s21) +\n		s20 * (s30 * s11 - s20 * s21))\n		/\n		(s40 * (s20 * s00 - s10 * s10) -\n			s30 * (s30 * s00 - s10 * s20) +\n			s20 * (s30 * s10 - s20 * s20));\n}");

}


