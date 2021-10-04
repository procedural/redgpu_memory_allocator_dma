#pragma once

#define VK_NO_PROTOTYPES
#ifdef _WIN32
#include "C:/VulkanSDK/1.2.135.0/Include/vulkan/vulkan/vulkan_core.h"
#endif
#ifdef __linux__
#include "/opt/RedGpuSDK/sdk/1.2.135.0/x86_64/include/vulkan/vulkan_core.h"
#endif
#undef  VK_NV_ray_tracing
#define VK_NV_ray_tracing 0

#ifdef _WIN32
#include "C:/RedGpuSDK/redgpu.h"
#endif
#ifdef __linux__
#include "/opt/RedGpuSDK/redgpu.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkGetPhysicalDeviceProperties2      (VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2 * pProperties);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkGetPhysicalDeviceMemoryProperties (VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties * pMemoryProperties);
REDGPU_DECLSPEC VkResult REDGPU_API rmaDmaVkAllocateMemory                    (VkDevice device, const VkMemoryAllocateInfo * pAllocateInfo, const VkAllocationCallbacks * pAllocator, VkDeviceMemory * pMemory, const char * name);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkFreeMemory                        (VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks * pAllocator);
REDGPU_DECLSPEC VkResult REDGPU_API rmaDmaVkMapMemory                         (VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void ** ppData);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkUnmapMemory                       (VkDevice device, VkDeviceMemory memory);
REDGPU_DECLSPEC VkResult REDGPU_API rmaDmaVkBindBufferMemory2                 (VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo * pBindInfos);
REDGPU_DECLSPEC VkResult REDGPU_API rmaDmaVkBindImageMemory2                  (VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo * pBindInfos);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkGetBufferMemoryRequirements2      (VkDevice device, const VkBufferMemoryRequirementsInfo2 * pInfo, VkMemoryRequirements2 * pMemoryRequirements);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkGetImageMemoryRequirements2       (VkDevice device, const VkImageMemoryRequirementsInfo2 * pInfo, VkMemoryRequirements2 * pMemoryRequirements);
REDGPU_DECLSPEC VkResult REDGPU_API rmaDmaVkCreateBuffer                      (VkDevice device, const VkBufferCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkBuffer * pBuffer);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkDestroyBuffer                     (VkDevice device, VkBuffer buffer, const VkAllocationCallbacks * pAllocator);
REDGPU_DECLSPEC VkResult REDGPU_API rmaDmaVkCreateImage                       (VkDevice device, const VkImageCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkImage * pImage);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkDestroyImage                      (VkDevice device, VkImage image, const VkAllocationCallbacks * pAllocator);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkCmdInsertDebugUtilsLabelEXT       (VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkCmdBeginDebugUtilsLabelEXT        (VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo);
REDGPU_DECLSPEC void     REDGPU_API rmaDmaVkCmdEndDebugUtilsLabelEXT          (VkCommandBuffer commandBuffer);

#ifdef __cplusplus
}
#endif