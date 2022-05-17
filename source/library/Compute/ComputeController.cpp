#include "ComputeController.h"

using namespace FlexRigLib::Compute;

ComputeController::ComputeController(std::string inc_dir)
{
	ComputeEngine::Init(1, inc_dir);

	m_context = ComputeEngine::GetNewContext();

	m_builder = new ProgramBuilder(m_context, "program");

	m_cur_state = ComputeState::Inited;
}

void ComputeController::BuildProgram()
{
	if (m_cur_state == ComputeState::Inited)
	{
		int buildres = m_builder->Build();

		if (buildres == 0)
		{
			// get kernels
			std::vector<std::string> kernls = m_builder->GetKernels();

			for (int i = 0; i < kernls.size(); i++)
			{
				std::string k_name = kernls[i];

				kernelEnt k_ent;
				k_ent.name = k_name;
				k_ent.kernel = m_builder->GetKernel(k_name);
				k_ent.args = 0;

				m_kernel_entries[k_ent.name] = k_ent;

				printf("Added kernel: %s\n", k_ent.name.c_str());
			}

			// established kernels.
			// someKnownKernel = m_kernel_entries["coolKernel"];

			m_cur_state = ComputeState::Built;
		}
		else
		{
			m_cur_state = ComputeState::BuildError;
		}
	}
}

ComputeBuffer* ComputeController::NewReadBuffer(size_t length)
{
	return ComputeController::NewBuffer(ComputeBuffer::Buffer_Type::READ, length);
}

ComputeBuffer* ComputeController::NewWriteBuffer(size_t length)
{
	return ComputeController::NewBuffer(ComputeBuffer::Buffer_Type::Write, length);
}

ComputeBuffer* ComputeController::NewReadWriteBuffer(size_t length)
{
	return ComputeController::NewBuffer(ComputeBuffer::Buffer_Type::Read_Write, length);
}

ComputeBuffer* ComputeController::NewBuffer(ComputeBuffer::Buffer_Type type, size_t length)
{
	printf("Get New buffer: %i\n", (int)length);
	return m_context->GetBuffer(type, length);
}

int FlexRigLib::Compute::ComputeController::KernelAddBuffer(std::string k_name, ComputeBuffer* buffer)
{
	if (m_kernel_entries.count(k_name) <= 0)
		return -1;

	int res = BindKernel(k_name, buffer, m_kernel_entries[k_name].args);
	printf("Bound buffer to: %i\n", m_kernel_entries[k_name].args);
	m_kernel_entries[k_name].args++;
	return res;
}

int ComputeController::BindKernel(std::string k_name, ComputeBuffer* buffer, int arg)
{
	if (m_kernel_entries.count(k_name) <= 0)
		return -1;


	return BindKernel(buffer, m_kernel_entries[k_name].kernel, arg);
}

int FlexRigLib::Compute::ComputeController::RunKernel(std::string k_name, int size_x, int size_y, int size_z)
{
	if (m_kernel_entries.count(k_name) <= 0)
		return -1;

	size_t global[] = { std::max(size_x, 1) , std::max(size_y, 1) , std::max(size_z, 1) };
	return m_kernel_entries[k_name].kernel->Execute(0, 3, global);
}

int ComputeController::BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg)
{
	return kernel->SetBuffer(0, buffer, arg);
}
