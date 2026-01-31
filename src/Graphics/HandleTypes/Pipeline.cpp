#include "Graphics/Graphics.h"

namespace Graphics {
	void Pipeline::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const PipelineCreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid pipeline");
		auto result = functions.execute<DeviceFunction::CreateGraphicsPipelines>(
			device.getHandle(), VK_NULL_HANDLE, 1,
			createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY_RESULT(result, "Failed to create a graphics pipeline");
	}
	void Pipeline::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid pipeline");
		functions.execute<DeviceFunction::DestroyPipeline>(device.getHandle(), getHandle(), nullptr);
		reset();
	}

	std::vector<Pipeline> Pipeline::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, std::span<const PipelineCreateInfo> createInfos)
	{
		std::vector<Pipeline> pipelines(createInfos.size());
		auto result = functions.execute<DeviceFunction::CreateGraphicsPipelines>(
			device.getHandle(), nullptr, createInfos.size(),
			PipelineCreateInfo::underlyingCast(createInfos.data()), nullptr, Pipeline::underlyingCast(pipelines.data()));
		GRAPHICS_VERIFY_RESULT(result, "Failed to create graphics pipelines");
		return pipelines;
	}

	void PipelineLayout::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const PipelineLayoutCreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid pipeline layout");
		auto result = functions.execute<DeviceFunction::CreatePipelineLayout>(
			device.getHandle(), createInfo.getUnderlyingPointer(),
			nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY_RESULT(result, "Failed to create a pipeline layout");
	}
	void PipelineLayout::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid pipeline layout");
		functions.execute<DeviceFunction::DestroyPipelineLayout>(device.getHandle(), getHandle(), nullptr);
		reset();
	}
}