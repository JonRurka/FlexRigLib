#include "ProgramBuilder.h"

using namespace FlexRigLib::Compute;


ProgramBuilder::ProgramBuilder(ComputeContext* c_context, std::string name)
{
	m_context = c_context;
	m_name = name;

	m_source = "";
	m_program = m_context->Add_Program(m_name);
}

void ProgramBuilder::AddConstant(std::string name, std::string value)
{
	m_program->AddConstant(name, value);
}

void ProgramBuilder::AddFunction(std::string name, std::string f_source)
{
	AppendSource(f_source);
}

void ProgramBuilder::AddKernel(std::string name, std::string f_source)
{
	AppendSource(f_source);
	m_kernels.push_back(name);
}

void ProgramBuilder::AppendSource(std::string f_source)
{
	m_source += f_source + "\n";
}

int ProgramBuilder::Build()
{
	char errorstr[ERROR_SIZE];
	m_program->Set(m_source.c_str());
	int status = m_program->Build(errorstr, ERROR_SIZE);
	m_error_msg = errorstr;

	for (int i = 0; i < m_kernels.size(); i++)
	{
		m_program->GetKernel(m_kernels[i]);
	}

	return status;
}

ComputeKernel* ProgramBuilder::GetKernel(std::string name)
{
	return m_program->GetKernel(name);
}
