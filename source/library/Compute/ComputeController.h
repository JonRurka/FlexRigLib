#pragma once

#include "ComputeEngine.h"
#include "ProgramBuilder.h"
#include "ShaderDepository.h"

namespace FlexRigLib {
	namespace Compute {

		class ComputeController {
		public:
			enum class ComputeState {
				None,
				Inited,
				Constructed,
				Built,
				BuildError
			};

			ComputeController(ComputeEngine::Platform pltform, ComputeEngine::Device device, std::string inc_dir);

			ProgramBuilder* GetProgramBuilder() { return m_builder; }

			void ConstructProgram();

			void BuildProgram();

			ComputeBuffer* NewReadBuffer(size_t length);

			ComputeBuffer* NewWriteBuffer(size_t length);

			ComputeBuffer* NewReadWriteBuffer(size_t length);

			ComputeBuffer* NewBuffer(ComputeBuffer::Buffer_Type type, size_t length);

			int KernelAddBuffer(std::string k_name, ComputeBuffer* buffer);

			int BindKernel(std::string k_name, ComputeBuffer* buffer, int arg);

			int RunKernel(std::string k_name, int size_x, int size_y, int size_z);

			ComputeState GetState() { return m_cur_state; }

		private:
			
			struct kernelEnt {
				std::string name;
				ComputeKernel* kernel;
				int args;
			};

			int BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg);


			ComputeContext* m_context{ nullptr };
			ProgramBuilder* m_builder{ nullptr };
			ComputeState m_cur_state{ ComputeState::None };
			ShaderDepository default_shaders;


			// known kernels

			// other kernels
			std::map<std::string, kernelEnt> m_kernel_entries;

		};


	}
}
