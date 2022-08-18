#include "Memory.h"
#include <glad/glad.h>

// Nvidia GPU Info
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049


int gr::Memory::GetBufferSize(unsigned int buffer)
{
    int size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    return size;
}

int gr::Memory::VRAMtotalSize()
{
    GLint kb = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &kb);
    return kb;
}

int gr::Memory::VRAMcurrentUsage()
{
    GLint kb = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &kb);
    return kb;
}
