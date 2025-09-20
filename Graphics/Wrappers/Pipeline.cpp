#include "Pipeline.h"

namespace Graphics {
	void Pipeline::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const CreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid pipeline");
		auto result = functions.execute<DeviceFunction::CreateGraphicsPipelines>(
			device.getHandle(), VK_NULL_HANDLE, 1,
			createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY(result == VK_SUCCESS,
			"Failed to create graphics pipeline: " + s_resultMessages.at(result));
	}
	void Pipeline::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid pipeline");
		functions.execute<DeviceFunction::DestroyPipeline>(device.getHandle(), getHandle(), nullptr);
		reset();
	}

	std::vector<Pipeline> Pipeline::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, std::span<const CreateInfo> createInfos)
	{
		std::vector<Pipeline> pipelines(createInfos.size());
		auto result = functions.execute<DeviceFunction::CreateGraphicsPipelines>(
			device.getHandle(), VK_NULL_HANDLE, createInfos.size(),
			CreateInfo::underlyingCast(createInfos.data()), nullptr, Pipeline::underlyingCast(pipelines.data()));
		GRAPHICS_VERIFY(result == VK_SUCCESS,
			"Failed to create graphics pipeline: " + s_resultMessages.at(result));
		return pipelines;
	}

	void Pipeline::Layout::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const CreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid pipeline layout");
		auto result = functions.execute<DeviceFunction::CreatePipelineLayout>(
			device.getHandle(), createInfo.getUnderlyingPointer(),
			nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY(result == VK_SUCCESS,
			"Failed to create pipeline layout: " + s_resultMessages.at(result));
	}
	void Pipeline::Layout::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid pipeline layout");
		functions.execute<DeviceFunction::DestroyPipelineLayout>(device.getHandle(), getHandle(), nullptr);
		reset();
	}
}