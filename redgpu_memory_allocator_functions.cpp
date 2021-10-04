#include "redgpu_memory_allocator_functions.h"

#define VK_NO_PROTOTYPES
#ifdef _WIN32
#include "C:/VulkanSDK/1.2.135.0/Include/vulkan/vulkan.h"
#endif
#ifdef __linux__
#include "/opt/RedGpuSDK/sdk/1.2.135.0/x86_64/include/vulkan/vulkan.h"
#endif