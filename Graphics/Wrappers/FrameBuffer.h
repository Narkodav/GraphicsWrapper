#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "RenderPass.h"
#include "Image.h"

namespace Graphics
{
	class FrameBufferRef : public BaseComponent<VkFramebuffer, FrameBufferRef>
	{
		using Base = BaseComponent<VkFramebuffer, FrameBufferRef>;
	public:
		using Base::Base;
	};

    class FrameBuffer : public VerificatorComponent<VkFramebuffer, FrameBufferRef>
    {
        using Base = VerificatorComponent<VkFramebuffer, FrameBufferRef>;
    public:
		using Base::Base;

        class CreateInfo : public StructBase<VkFramebufferCreateInfo, CreateInfo>
        {
			using Base = StructBase<VkFramebufferCreateInfo, CreateInfo>;
		public:
			using Base::Base;
			CreateInfo(const RenderPassRef& renderPass,
				const std::vector<Image::View>& attachments,
				const Extent2D& extent, uint32_t layers = 1) : Base()
			{
				this->renderPass = renderPass.getHandle();
				this->attachmentCount = attachments.size();
				this->pAttachments = Image::View::underlyingCast(attachments.data());
				this->width = extent.getWidth();
				this->height = extent.getHeight();
				this->layers = layers;
			}
			CreateInfo& setRenderPass(const RenderPassRef& renderPass) {
				this->renderPass = renderPass.getHandle();
				return *this;
			}
			CreateInfo& setAttachments(const std::vector<Image::ViewRef>& attachments) {
				this->attachmentCount = attachments.size();
				this->pAttachments = Image::ViewRef::underlyingCast(attachments.data());
				return *this;
			}
			CreateInfo& setAttachments(const std::vector<Image::View>& attachments) {
				this->attachmentCount = attachments.size();
				this->pAttachments = Image::View::underlyingCast(attachments.data());
				return *this;
			}
			CreateInfo& setExtent(const Extent2D& extent) {
				this->width = extent.getWidth();
				this->height = extent.getHeight();
				return *this;
			}
			CreateInfo& setLayers(uint32_t layers) {
				this->layers = layers;
				return *this;
			}
		};

		void create(const DeviceRef& device, const DeviceFunctionTable& functions,
			const CreateInfo& createInfo);
		void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };

}

