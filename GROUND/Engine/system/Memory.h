#pragma once

#define GR_MEM_MB 1024.0
#define GR_MEM_GB 1048576.0

namespace gr
{
	class Memory
	{
	public:
		/*
		 * @brief Return the size of the buffer in bytes
		 * @param buffer: is the buffer that we want to take the size
		*/
		static int GetBufferSize(unsigned int buffer);

		/*
		* @brief Return the total amount of VRAM on the GPU
		* @return size in kb
		*/
		static int VRAMtotalSize();
		/*
		* @brief Return the current amount of VRAM used on the GPU
		* @return size in kb
		*/
		static int VRAMcurrentUsage();
	};
}