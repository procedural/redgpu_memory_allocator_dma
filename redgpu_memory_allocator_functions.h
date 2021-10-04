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

VKAPI_ATTR void     VKAPI_CALL rmaDmaVkGetPhysicalDeviceProperties2      (VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2 * pProperties);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkGetPhysicalDeviceMemoryProperties (VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties * pMemoryProperties);
VKAPI_ATTR VkResult VKAPI_CALL rmaDmaVkAllocateMemory                    (VkDevice device, const VkMemoryAllocateInfo * pAllocateInfo, const VkAllocationCallbacks * pAllocator, VkDeviceMemory * pMemory, const char * name);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkFreeMemory                        (VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks * pAllocator);
VKAPI_ATTR VkResult VKAPI_CALL rmaDmaVkMapMemory                         (VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void ** ppData);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkUnmapMemory                       (VkDevice device, VkDeviceMemory memory);
VKAPI_ATTR VkResult VKAPI_CALL rmaDmaVkBindBufferMemory2                 (VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo * pBindInfos);
VKAPI_ATTR VkResult VKAPI_CALL rmaDmaVkBindImageMemory2                  (VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo * pBindInfos);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkGetBufferMemoryRequirements2      (VkDevice device, const VkBufferMemoryRequirementsInfo2 * pInfo, VkMemoryRequirements2 * pMemoryRequirements);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkGetImageMemoryRequirements2       (VkDevice device, const VkImageMemoryRequirementsInfo2 * pInfo, VkMemoryRequirements2 * pMemoryRequirements);
VKAPI_ATTR VkResult VKAPI_CALL rmaDmaVkCreateBuffer                      (VkDevice device, const VkBufferCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkBuffer * pBuffer);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkDestroyBuffer                     (VkDevice device, VkBuffer buffer, const VkAllocationCallbacks * pAllocator);
VKAPI_ATTR VkResult VKAPI_CALL rmaDmaVkCreateImage                       (VkDevice device, const VkImageCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkImage * pImage);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkDestroyImage                      (VkDevice device, VkImage image, const VkAllocationCallbacks * pAllocator);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkCmdInsertDebugUtilsLabelEXT       (VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkCmdBeginDebugUtilsLabelEXT        (VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo);
VKAPI_ATTR void     VKAPI_CALL rmaDmaVkCmdEndDebugUtilsLabelEXT          (VkCommandBuffer commandBuffer);

#ifdef __cplusplus
}
#endif