#pragma once

#include "ComputeEngine.h"
#include "ProgramBuilder.h"

namespace FlexRigLib {
	namespace Compute {

		class ComputeController {
		public:
			enum class ComputeState {
				None,
				Inited,
				Built,
				BuildError
			};

			ComputeController(std::string inc_dir);

			ProgramBuilder* GetProgramBuilder() { return m_builder; }

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


			// known kernels

			// other kernels
			std::map<std::string, kernelEnt> m_kernel_entries;

		};


	}
}
