#include "redgpu_memory_allocator_functions.h"

#include <mutex> // For mutex
#include <map>   // For map

std::mutex                         __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArraysMutex;
std::map<RedHandleArray, RedArray> __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArrays;
std::mutex                         __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImagesMutex;
std::map<RedHandleImage, RedImage> __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImages;

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkGetPhysicalDeviceProperties2(RedContext context, unsigned gpuIndex, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2 * pProperties) {
  // NOTE(Constantine): Hardcoded for memorymanagement_vk.cpp:292
  VkPhysicalDeviceMaintenance3Properties * maintenance3 = (VkPhysicalDeviceMaintenance3Properties *)pProperties->pNext;
  maintenance3->maxMemoryAllocationSize = context->gpus[gpuIndex].maxMemoryAllocateBytesCount;
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkGetPhysicalDeviceMemoryProperties(RedContext context, unsigned gpuIndex, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties * pMemoryProperties) {
  VkPhysicalDeviceMemoryProperties properties = {};
  properties.memoryTypeCount = context->gpus[gpuIndex].memoryTypesCount;
  properties.memoryHeapCount = context->gpus[gpuIndex].memoryHeapsCount;
  for (unsigned i = 0; i < context->gpus[gpuIndex].memoryTypesCount; i += 1) {
    RedMemoryType memoryType = context->gpus[gpuIndex].memoryTypes[i];
    properties.memoryTypes[i].heapIndex = memoryType.memoryHeapIndex;
    if (memoryType.isGpuVram == 1) {
      properties.memoryTypes[i].propertyFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    if (memoryType.isCpuMappable == 1) {
      properties.memoryTypes[i].propertyFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }
    if (memoryType.isCpuCoherent == 1) {
      properties.memoryTypes[i].propertyFlags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    if (memoryType.isCpuCached == 1) {
      properties.memoryTypes[i].propertyFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    }
  }
  for (unsigned i = 0; i < context->gpus[gpuIndex].memoryHeapsCount; i += 1) {
    RedMemoryHeap memoryHeap = context->gpus[gpuIndex].memoryHeaps[i];
    properties.memoryHeaps[i].size = memoryHeap.memoryBytesCount;
    if (memoryHeap.isGpuVram == 1) {
      properties.memoryHeaps[i].flags |= VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;
    }
  }
  pMemoryProperties[0] = properties;
}

REDGPU_DECLSPEC RedStatus REDGPU_API rmaDmaVkAllocateMemory(RedContext context, unsigned gpuIndex, VkDevice device, const VkMemoryAllocateInfo * pAllocateInfo, const VkAllocationCallbacks * pAllocator, VkDeviceMemory * pMemory, const char * name) {
  RedStatuses statuses = {};
  if (context->gpus[gpuIndex].memoryTypes[pAllocateInfo->memoryTypeIndex].isCpuMappable == 1) {
    // NOTE(Constantine): Pass array from DMA to RMA in future.
    RedHandleArray array = 0;
    redMemoryAllocateMappable(context, (RedHandleGpu)device, name, 0, array, pAllocateInfo->allocationSize, pAllocateInfo->memoryTypeIndex, 0, (RedHandleMemory *)pMemory, &statuses, 0, 0, 0);
  } else {
    redMemoryAllocate(context, (RedHandleGpu)device, name, pAllocateInfo->allocationSize, pAllocateInfo->memoryTypeIndex, 0, 0, 0, (RedHandleMemory *)pMemory, &statuses, 0, 0, 0);
  }
  return statuses.statusError;
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkFreeMemory(RedContext context, unsigned gpuIndex, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks * pAllocator) {
  redMemoryFree(context, (RedHandleGpu)device, (RedHandleMemory)memory, 0, 0, 0);
}

REDGPU_DECLSPEC RedStatus REDGPU_API rmaDmaVkMapMemory(RedContext context, unsigned gpuIndex, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void ** ppData) {
  RedStatuses statuses = {};
  redMemoryMap(context, (RedHandleGpu)device, (RedHandleMemory)memory, offset, size, ppData, &statuses, 0, 0, 0);
  return statuses.statusError;
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkUnmapMemory(RedContext context, unsigned gpuIndex, VkDevice device, VkDeviceMemory memory) {
  redMemoryUnmap(context, (RedHandleGpu)device, (RedHandleMemory)memory, 0, 0, 0);
}

REDGPU_DECLSPEC RedStatus REDGPU_API rmaDmaVkBindBufferMemory2(RedContext context, unsigned gpuIndex, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo * pBindInfos) {
  RedStatuses statuses = {};
  redMemorySet(context, (RedHandleGpu)device, bindInfoCount, (const RedMemoryArray *)pBindInfos, 0, 0, &statuses, 0, 0, 0);
  return statuses.statusError;
}

REDGPU_DECLSPEC RedStatus REDGPU_API rmaDmaVkBindImageMemory2(RedContext context, unsigned gpuIndex, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo * pBindInfos) {
  RedStatuses statuses = {};
  redMemorySet(context, (RedHandleGpu)device, 0, 0, bindInfoCount, (const RedMemoryImage *)pBindInfos, &statuses, 0, 0, 0);
  return statuses.statusError;
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkGetBufferMemoryRequirements2(RedContext context, unsigned gpuIndex, VkDevice device, const VkBufferMemoryRequirementsInfo2 * pInfo, VkMemoryRequirements2 * pMemoryRequirements) {
  RedArray arrayData;
  {
    std::lock_guard<std::mutex> __mapArraysMutexScope(__REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArraysMutex);
    arrayData = __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArrays[(RedHandleArray)pInfo->buffer];
  }
  pMemoryRequirements->memoryRequirements.size           = arrayData.memoryBytesCount;
  pMemoryRequirements->memoryRequirements.alignment      = arrayData.memoryBytesAlignment;
  pMemoryRequirements->memoryRequirements.memoryTypeBits = arrayData.memoryTypesSupported;
  // NOTE(Constantine): Explicitly ignoring VkMemoryDedicatedRequirements in pMemoryRequirements->pNext.
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkGetImageMemoryRequirements2(RedContext context, unsigned gpuIndex, VkDevice device, const VkImageMemoryRequirementsInfo2 * pInfo, VkMemoryRequirements2 * pMemoryRequirements) {
  RedImage imageData;
  {
    std::lock_guard<std::mutex> __mapImagesMutexScope(__REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImagesMutex);
    imageData = __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImages[(RedHandleImage)pInfo->image];
  }
  pMemoryRequirements->memoryRequirements.size           = imageData.memoryBytesCount;
  pMemoryRequirements->memoryRequirements.alignment      = imageData.memoryBytesAlignment;
  pMemoryRequirements->memoryRequirements.memoryTypeBits = imageData.memoryTypesSupported;
  // NOTE(Constantine): Explicitly ignoring VkMemoryDedicatedRequirements in pMemoryRequirements->pNext.
}

REDGPU_DECLSPEC RedStatus REDGPU_API rmaDmaVkCreateBuffer(RedContext context, unsigned gpuIndex, VkDevice device, const VkBufferCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkBuffer * pBuffer) {
  RedStatuses statuses = {};
  // NOTE(Constantine): Pass structuredBufferElementBytesCount from DMA to RMA in future.
  uint64_t structuredBufferElementBytesCount = 0;
  // NOTE(Constantine): Pass initialAccess from DMA to RMA in future.
  RedAccessBitflags initialAccess = 0;
  RedArray array = {};
  redCreateArray(context, (RedHandleGpu)device, "REDGPU Memory Allocator DMA", (RedArrayType)pCreateInfo->usage, pCreateInfo->size, structuredBufferElementBytesCount, initialAccess, (pCreateInfo->sharingMode == VK_SHARING_MODE_CONCURRENT || pCreateInfo->queueFamilyIndexCount == 0) ? -1 : pCreateInfo->pQueueFamilyIndices[0], 0, &array, &statuses, 0, 0, 0);
  pBuffer[0] = (VkBuffer)array.handle;
  {
    std::lock_guard<std::mutex> __mapArraysMutexScope(__REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArraysMutex);
    __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArrays[array.handle] = array;
  }
  return statuses.statusError;
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkDestroyBuffer(RedContext context, unsigned gpuIndex, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks * pAllocator) {
  redDestroyArray(context, (RedHandleGpu)device, (RedHandleArray)buffer, 0, 0, 0);
  {
    std::lock_guard<std::mutex> __mapArraysMutexScope(__REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArraysMutex);
    __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapArrays.erase((RedHandleArray)buffer);
  }
}

REDGPU_DECLSPEC RedStatus REDGPU_API rmaDmaVkCreateImage(RedContext context, unsigned gpuIndex, VkDevice device, const VkImageCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkImage * pImage) {
  RedStatuses statuses = {};
  RedImageDimensions imageDimensions = RED_IMAGE_DIMENSIONS_2D;
  if (pCreateInfo->imageType == VK_IMAGE_TYPE_1D) {
    imageDimensions = RED_IMAGE_DIMENSIONS_1D;
  } else if (pCreateInfo->imageType == VK_IMAGE_TYPE_2D && ((pCreateInfo->flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) == VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)) {
    imageDimensions = RED_IMAGE_DIMENSIONS_2D_WITH_TEXTURE_DIMENSIONS_CUBE_AND_CUBE_LAYERED;
  } else if (pCreateInfo->imageType == VK_IMAGE_TYPE_2D) {
    imageDimensions = RED_IMAGE_DIMENSIONS_2D;
  } else if (pCreateInfo->imageType == VK_IMAGE_TYPE_3D && ((pCreateInfo->flags & VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT) == VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT)) {
    imageDimensions = RED_IMAGE_DIMENSIONS_3D_WITH_TEXTURE_DIMENSIONS_2D_AND_2D_LAYERED;
  } else if (pCreateInfo->imageType == VK_IMAGE_TYPE_3D) {
    imageDimensions = RED_IMAGE_DIMENSIONS_3D;
  }
  RedAccessBitflags restrictToAccess = 0;
  if ((pCreateInfo->usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) == VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
    restrictToAccess |= RED_ACCESS_BITFLAG_COPY_R;
  }
  if ((pCreateInfo->usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT) == VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
    restrictToAccess |= RED_ACCESS_BITFLAG_COPY_W;
  }
  if ((pCreateInfo->usage & VK_IMAGE_USAGE_SAMPLED_BIT) == VK_IMAGE_USAGE_SAMPLED_BIT) {
    restrictToAccess |= RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R | RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R;
  }
  if ((pCreateInfo->usage & VK_IMAGE_USAGE_STORAGE_BIT) == VK_IMAGE_USAGE_STORAGE_BIT) {
    restrictToAccess |= RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W;
  }
  if ((pCreateInfo->usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
    restrictToAccess |= RED_ACCESS_BITFLAG_OUTPUT_COLOR_W;
  }
  if ((pCreateInfo->usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
    restrictToAccess |= RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW;
    if ((RedFormat)pCreateInfo->format == RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT ||
        (RedFormat)pCreateInfo->format == RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT)
    {
      restrictToAccess |= RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW;
    }
  }
  // NOTE(Constantine): Pass initialAccess from DMA to RMA in future.
  RedAccessBitflags initialAccess = 0;
  RedImage image = {};
  redCreateImage(context, (RedHandleGpu)device, "REDGPU Memory Allocator DMA", imageDimensions, (RedFormat)pCreateInfo->format, pCreateInfo->extent.width, pCreateInfo->extent.height, pCreateInfo->extent.depth, pCreateInfo->mipLevels, pCreateInfo->arrayLayers, (RedMultisampleCountBitflag)pCreateInfo->samples, restrictToAccess, initialAccess, (pCreateInfo->sharingMode == VK_SHARING_MODE_CONCURRENT || pCreateInfo->queueFamilyIndexCount == 0) ? -1 : pCreateInfo->pQueueFamilyIndices[0], 0, &image, &statuses, 0, 0, 0);
  pImage[0] = (VkImage)image.handle;
  {
    std::lock_guard<std::mutex> __mapImagesMutexScope(__REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImagesMutex);
    __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImages[image.handle] = image;
  }
  return statuses.statusError;
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkDestroyImage(RedContext context, unsigned gpuIndex, VkDevice device, VkImage image, const VkAllocationCallbacks * pAllocator) {
  redDestroyImage(context, (RedHandleGpu)device, (RedHandleImage)image, 0, 0, 0);
  {
    std::lock_guard<std::mutex> __mapImagesMutexScope(__REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImagesMutex);
    __REDGPU_MEMORY_ALLOCATOR_DMA_FUNCTIONS_GLOBAL_711d87b049aded9364468c79f432a0d3e1b6f3de_mapImages.erase((RedHandleImage)image);
  }
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkCmdInsertDebugUtilsLabelEXT(RedContext context, unsigned gpuIndex, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo) {
  RedCallProceduresAndAddresses callPAs;
  redGetCallProceduresAndAddresses(context, context->gpus[gpuIndex].gpu, &callPAs, 0, 0, 0, 0);
  redCallMark(callPAs.redCallMark, (RedHandleCalls)commandBuffer, pLabelInfo->pLabelName);
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkCmdBeginDebugUtilsLabelEXT(RedContext context, unsigned gpuIndex, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT * pLabelInfo) {
  RedCallProceduresAndAddresses callPAs;
  redGetCallProceduresAndAddresses(context, context->gpus[gpuIndex].gpu, &callPAs, 0, 0, 0, 0);
  redCallMarkSet(callPAs.redCallMarkSet, (RedHandleCalls)commandBuffer, pLabelInfo->pLabelName);
}

REDGPU_DECLSPEC void REDGPU_API rmaDmaVkCmdEndDebugUtilsLabelEXT(RedContext context, unsigned gpuIndex, VkCommandBuffer commandBuffer) {
  RedCallProceduresAndAddresses callPAs;
  redGetCallProceduresAndAddresses(context, context->gpus[gpuIndex].gpu, &callPAs, 0, 0, 0, 0);
  redCallMarkEnd(callPAs.redCallMarkEnd, (RedHandleCalls)commandBuffer);
}
