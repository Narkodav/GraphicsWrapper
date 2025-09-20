#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Fence.h"
#include "Semaphore.h"

namespace Graphics
{
    class Attachment : public StructBase<VkAttachmentDescription, Attachment>
    {
		using Base = StructBase<VkAttachmentDescription, Attachment>;
    public:
        using Base::Base;

		static inline const auto s_unusedAttachment = VK_ATTACHMENT_UNUSED;

        class Reference : public StructBase<VkAttachmentReference, Reference>
		{
			using Base = StructBase<VkAttachmentReference, Reference>;
		public:
			using Base::Base;
			Reference(uint32_t attachmentIndex = s_unusedAttachment,
				ImageLayout layout = ImageLayout::Undefined) : Base()
			{
				this->attachment = attachmentIndex;
				this->layout = convertCEnum(layout);
			}

			Reference& setAttachment(uint32_t attachmentIndex) {
				this->attachment = attachmentIndex;
				return *this;
			}

			Reference& setLayout(ImageLayout layout) {
				this->layout = convertCEnum(layout);
				return *this;
			}

			uint32_t getAttachment() const { return this->attachment; };
			ImageLayout getLayout() const { return convertCEnum(this->layout); };
		};

        Attachment(Format format,
            AttachmentLoadOp loadOp = AttachmentLoadOp::DontCare,
            AttachmentStoreOp storeOp = AttachmentStoreOp::DontCare,
            AttachmentLoadOp stencilLoadOp = AttachmentLoadOp::DontCare,
            AttachmentStoreOp stencilStoreOp = AttachmentStoreOp::DontCare,
            ImageLayout initialLayout = ImageLayout::Undefined,
            ImageLayout finalLayout = ImageLayout::Undefined,
            AttachmentCreate::Flags flags = AttachmentCreate::Bits::None,
            SampleCount::Bits samples = SampleCount::Bits::SC1) : Base()
        {
            this->format = convertCEnum(format);
            this->loadOp = convertCEnum(loadOp);
            this->storeOp = convertCEnum(storeOp);
            this->stencilLoadOp = convertCEnum(stencilLoadOp);
            this->stencilStoreOp = convertCEnum(stencilStoreOp);
            this->initialLayout = convertCEnum(initialLayout);
            this->finalLayout = convertCEnum(finalLayout);
            this->flags = flags;
            this->samples = static_cast<VkSampleCountFlagBits>(samples);
        }
        Attachment& setFormat(Format format) {
            this->format = convertCEnum(format);
            return *this;
        }
        Attachment& setLoadOp(AttachmentLoadOp loadOp) {
            this->loadOp = convertCEnum(loadOp);
            return *this;
        }
        Attachment& setStoreOp(AttachmentStoreOp storeOp) {
            this->storeOp = convertCEnum(storeOp);
            return *this;
        }
        Attachment& setStencilLoadOp(AttachmentLoadOp stencilLoadOp) {
            this->stencilLoadOp = convertCEnum(stencilLoadOp);
            return *this;
        }
        Attachment& setStencilStoreOp(AttachmentStoreOp stencilStoreOp) {
            this->stencilStoreOp = convertCEnum(stencilStoreOp);
            return *this;
        }
        Attachment& setInitialLayout(ImageLayout initialLayout) {
            this->initialLayout = convertCEnum(initialLayout);
            return *this;
        }
        Attachment& setFinalLayout(ImageLayout finalLayout) {
            this->finalLayout = convertCEnum(finalLayout);
            return *this;
        }
        Attachment& setFlags(AttachmentCreate::Flags flags) {
            this->flags = flags;
            return *this;
        }
        Attachment& setSamples(SampleCount::Bits samples) {
            this->samples = static_cast<VkSampleCountFlagBits>(samples);
            return *this;
        }
	};

    class Subpass : public StructBase<VkSubpassDescription, Subpass>
    {
		using Base = StructBase<VkSubpassDescription, Subpass>;
        public:
        using Base::Base;

		static inline const auto s_externalSubpass = VK_SUBPASS_EXTERNAL;        

        class Dependency : public StructBase<VkSubpassDependency, Dependency>
        {
			using Base = StructBase<VkSubpassDependency, Dependency>;
            public:
            using Base::Base;
            Dependency(uint32_t srcSubpass = s_externalSubpass,
                uint32_t dstSubpass = s_externalSubpass,
                PipelineStage::Flags srcStageMask = PipelineStage::Bits::AllCommands,
                PipelineStage::Flags dstStageMask = PipelineStage::Bits::AllCommands,
                Access::Flags srcAccessMask = Access::Bits::None,
                Access::Flags dstAccessMask = Access::Bits::None,
                Graphics::Dependency::Flags flags = Graphics::Dependency::Bits::None) : Base()
            {
                this->srcSubpass = srcSubpass;
                this->dstSubpass = dstSubpass;
                this->srcStageMask = srcStageMask;
                this->dstStageMask = dstStageMask;
                this->srcAccessMask = srcAccessMask;
                this->dstAccessMask = dstAccessMask;
                this->dependencyFlags = flags;
            }
            Dependency& setSrcSubpass(uint32_t srcSubpass) {
                this->srcSubpass = srcSubpass;
                return *this;
            }
            Dependency& setDstSubpass(uint32_t dstSubpass) {
                this->dstSubpass = dstSubpass;
                return *this;
            }
            Dependency& setSrcStageMask(PipelineStage::Flags srcStageMask) {
                this->srcStageMask = srcStageMask;
                return *this;
            }
            Dependency& setDstStageMask(PipelineStage::Flags dstStageMask) {
                this->dstStageMask = dstStageMask;
                return *this;
            }
            Dependency& setSrcAccessMask(Access::Flags srcAccessMask) {
                this->srcAccessMask = srcAccessMask;
                return *this;
            }
            Dependency& setDstAccessMask(Access::Flags dstAccessMask) {
                this->dstAccessMask = dstAccessMask;
                return *this;
            }
            Dependency& setDependencyFlags(Graphics::Dependency::Flags flags) {
                this->dependencyFlags = flags;
                return *this;
            }
		};

        Subpass(PipelineBindPoint bindPoint = PipelineBindPoint::Graphics,
            std::span<const Attachment::Reference> inputAttachments = {},
            std::span<const Attachment::Reference> colorAttachments = {},
            std::span<const Attachment::Reference> resolveAttachments = {},
            const Attachment::Reference* depthStencilAttachment = nullptr,
            std::span<const uint32_t> preserveAttachments = {}) : Base()
        {
            this->pipelineBindPoint = convertCEnum(bindPoint);
            this->inputAttachmentCount = inputAttachments.size();
            this->pInputAttachments = Attachment::Reference::underlyingCast(inputAttachments.data());
            this->colorAttachmentCount = colorAttachments.size();
            this->pColorAttachments = Attachment::Reference::underlyingCast(colorAttachments.data());
            this->pResolveAttachments = Attachment::Reference::underlyingCast(resolveAttachments.data());
            this->pDepthStencilAttachment = Attachment::Reference::underlyingCast(depthStencilAttachment);
            this->preserveAttachmentCount = preserveAttachments.size();
            this->pPreserveAttachments = preserveAttachments.data();
        }

        Subpass& setBindPoint(PipelineBindPoint bindPoint) {
            this->pipelineBindPoint = convertCEnum(bindPoint);
            return *this;
        }
        Subpass& setInputAttachments(std::span<const Attachment::Reference> inputAttachments) {
            this->inputAttachmentCount = inputAttachments.size();
            this->pInputAttachments = Attachment::Reference::underlyingCast(inputAttachments.data());
            return *this;
        }
        Subpass& setColorAttachments(std::span<const Attachment::Reference> colorAttachments) {
            this->colorAttachmentCount = colorAttachments.size();
            this->pColorAttachments = Attachment::Reference::underlyingCast(colorAttachments.data());
            return *this;
        }
        Subpass& setResolveAttachments(std::span<const Attachment::Reference> resolveAttachments) {
            this->pResolveAttachments = Attachment::Reference::underlyingCast(resolveAttachments.data());
            return *this;
        }
        Subpass& setDepthStencilAttachment(const Attachment::Reference* depthStencilAttachment) {
            this->pDepthStencilAttachment = Attachment::Reference::underlyingCast(depthStencilAttachment);
            return *this;
        }
        Subpass& setPreserveAttachments(std::span<const uint32_t> preserveAttachments) {
            this->preserveAttachmentCount = preserveAttachments.size();
            this->pPreserveAttachments = preserveAttachments.data();
			return *this;
		}
	};

    class RenderPassRef : public BaseComponent<VkRenderPass, RenderPassRef>
    {
        using Base = BaseComponent<VkRenderPass, RenderPassRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "RenderPass";
    };

    class RenderPass : public VerificatorComponent<VkRenderPass, RenderPassRef>
    {
        using Base = VerificatorComponent<VkRenderPass, RenderPassRef>;
    public:
        using Base::Base;

        class BeginInfo : public StructBase<VkRenderPassBeginInfo, BeginInfo>
        {
            using Base = StructBase<VkRenderPassBeginInfo, BeginInfo>;
        public:
            using Base::Base;

            BeginInfo& setRenderPass(const RenderPassRef& renderPass) {
                this->renderPass = renderPass;
                return *this;
            }

            BeginInfo& setFrameBuffer(const FrameBufferRef& frameBuffer) {
                this->framebuffer = frameBuffer;
                return *this;
            }

            BeginInfo& setClearValues(std::span<const ClearValue> clearValues) {
                this->clearValueCount = clearValues.size(); 
                this->pClearValues = ClearValue::underlyingCast(clearValues.data());
                return *this;
            }

            BeginInfo& setRenderArea(const Rect2D& renderArea) {
                this->renderArea = renderArea;
                return *this;
            }
        };

        class CreateInfo : public StructBase<VkRenderPassCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkRenderPassCreateInfo, CreateInfo>;
        public:
            using Base::Base;
            CreateInfo(std::span<const Attachment> attachments,
                std::span<const Subpass> subpasses,
                std::span<const Subpass::Dependency> dependencies = {}) : Base()
            {
                this->attachmentCount = attachments.size();
                this->pAttachments = Attachment::underlyingCast(attachments.data());
                this->subpassCount = subpasses.size();
                this->pSubpasses = Subpass::underlyingCast(subpasses.data());
                this->dependencyCount = dependencies.size();
                this->pDependencies = Subpass::Dependency
                    ::underlyingCast(dependencies.data());
            }
            CreateInfo& setAttachments(std::span<const Attachment> attachments) {
                this->attachmentCount = attachments.size();
                this->pAttachments = Attachment::underlyingCast(attachments.data());
                return *this;
            }
            CreateInfo& setSubpasses(std::span<const Subpass> subpasses) {
                this->subpassCount = subpasses.size();
                this->pSubpasses = Subpass::underlyingCast(subpasses.data());
                return *this;
            }
            CreateInfo& setDependencies(std::span<const Subpass::Dependency> dependencies) {
                this->dependencyCount = dependencies.size();
                this->pDependencies = Subpass::Dependency
                    ::underlyingCast(dependencies.data());
                return *this;
            }
        };

        void create(const DeviceRef& device, const DeviceFunctionTable& functions,
            const CreateInfo& createInfo);
        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };
}