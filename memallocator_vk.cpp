/*
 * Copyright (c) 2019-2021, NVIDIA CORPORATION.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2019-2021 NVIDIA CORPORATION
 * SPDX-License-Identifier: Apache-2.0
 */

#include "memallocator_vk.hpp"

#include <cassert>

namespace nvvk {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemAllocateInfo::MemAllocateInfo(RedContext context, unsigned gpuIndex, const VkMemoryRequirements& memReqs, VkMemoryPropertyFlags memProps, bool isTilingOptimal)
    : m_context(context)
    , m_gpuIndex(gpuIndex)
    , m_memReqs(memReqs)
    , m_memProps(memProps)
    , m_isTilingOptimal(isTilingOptimal)
{
}

MemAllocateInfo::MemAllocateInfo(RedContext context, unsigned gpuIndex, VkDevice device, const RedVkBuffer * buffer, VkMemoryPropertyFlags memProps)
{
  m_context  = context;
  m_gpuIndex = gpuIndex;

  VkMemoryDedicatedRequirements dedicatedRegs = {VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS};
  VkMemoryRequirements2         memReqs       = {VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2, &dedicatedRegs};

  rmaDmaVkGetBufferMemoryRequirements2(m_context, m_gpuIndex, device, buffer, &memReqs);

  m_memReqs  = memReqs.memoryRequirements;
  m_memProps = memProps;

  if(dedicatedRegs.requiresDedicatedAllocation)
  {
    setDedicatedBuffer(buffer->handle);
  }

  setTilingOptimal(false);
}

MemAllocateInfo::MemAllocateInfo(RedContext context, unsigned gpuIndex, VkDevice device, const RedVkImage * image, VkMemoryPropertyFlags memProps)
{
  m_context  = context;
  m_gpuIndex = gpuIndex;

  VkMemoryDedicatedRequirements dedicatedRegs = {VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS};
  VkMemoryRequirements2         memReqs       = {VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2, &dedicatedRegs};

  rmaDmaVkGetImageMemoryRequirements2(m_context, m_gpuIndex, device, image, &memReqs);

  m_memReqs  = memReqs.memoryRequirements;
  m_memProps = memProps;

  if(dedicatedRegs.requiresDedicatedAllocation)
  {
    setDedicatedImage(image->handle);
  }

  setTilingOptimal(true);
}

MemAllocateInfo& MemAllocateInfo::setDedicatedImage(VkImage image)
{
  assert(!m_dedicatedBuffer);
  m_dedicatedImage = image;

  return *this;
}
MemAllocateInfo& MemAllocateInfo::setDedicatedBuffer(VkBuffer buffer)
{
  assert(!m_dedicatedImage);
  m_dedicatedBuffer = buffer;

  return *this;
}
MemAllocateInfo& MemAllocateInfo::setAllocationFlags(VkMemoryAllocateFlags flags)
{
  m_allocateFlags |= flags;
  return *this;
}

MemAllocateInfo& MemAllocateInfo::setDeviceMask(uint32_t mask)
{
  m_deviceMask = mask;
  return *this;
}


MemAllocateInfo& MemAllocateInfo::setDebugName(const std::string& name)
{
  m_debugName = name;
  return *this;
}

MemAllocateInfo& MemAllocateInfo::setExportable(bool exportable)
{
  m_isExportable = exportable;
  return *this;
}

MemAllocateInfo& MemAllocateInfo::setTilingOptimal(bool isTilingOptimal)
{
  m_isTilingOptimal = isTilingOptimal;
  return *this;
}

MemAllocateInfo& MemAllocateInfo::setPriority(const float priority /*= 0.5f*/)
{
  m_priority = priority;
  return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t getMemoryType(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t typeBits, const VkMemoryPropertyFlags& properties)
{
  for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
  {
    if(((typeBits & (1 << i)) > 0) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
    {
      return i;
    }
  }
  assert(0);
  return ~0u;
}

bool fillBakedAllocateInfo(const VkPhysicalDeviceMemoryProperties& physMemProps, const MemAllocateInfo& info, BakedAllocateInfo& baked)
{
  baked.memAllocInfo.allocationSize = info.getMemoryRequirements().size;
  baked.memAllocInfo.memoryTypeIndex =
      getMemoryType(physMemProps, info.getMemoryRequirements().memoryTypeBits, info.getMemoryProperties());

  // Put it last in the chain, so we can directly pass it into the DeviceMemoryAllocator::alloc function
  if(info.getDedicatedBuffer() || info.getDedicatedImage())
  {
    baked.dedicatedInfo.pNext = baked.memAllocInfo.pNext;
    baked.memAllocInfo.pNext  = &baked.dedicatedInfo;

    baked.dedicatedInfo.buffer = info.getDedicatedBuffer();
    baked.dedicatedInfo.image  = info.getDedicatedImage();
  }

  if(info.getExportable())
  {
    baked.exportInfo.pNext   = baked.memAllocInfo.pNext;
    baked.memAllocInfo.pNext = &baked.exportInfo;
#ifdef WIN32
    baked.exportInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;
#else
    baked.exportInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;
#endif
  }

  if(info.getDeviceMask() || info.getAllocationFlags())
  {
    baked.flagsInfo.pNext    = baked.memAllocInfo.pNext;
    baked.memAllocInfo.pNext = &baked.flagsInfo;

    baked.flagsInfo.flags      = info.getAllocationFlags();
    baked.flagsInfo.deviceMask = info.getDeviceMask();

    if(baked.flagsInfo.deviceMask)
    {
      baked.flagsInfo.flags |= VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT;
    }
  }

  return true;
}

}  // namespace nvvk