#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "RenderPass.h"
#include "ShaderModule.h"
#include "DescriptorSet.h"

namespace Graphics
{
	class VertexInputAttributeDescription : public StructBase<VkVertexInputAttributeDescription, VertexInputAttributeDescription>
	{
		using Base = StructBase<VkVertexInputAttributeDescription, VertexInputAttributeDescription>;
	public:
		using Base::Base;

		VertexInputAttributeDescription(uint32_t location, uint32_t binding,
			Format format, uint32_t offset) : Base() {
			this->location = location;
			this->binding = binding;
			this->format = convertCEnum(format);
			this->offset = offset;
		}
		VertexInputAttributeDescription& setLocation(uint32_t location) {
			this->location = location;
			return *this;
		}
		VertexInputAttributeDescription& setBinding(uint32_t binding) {
			this->binding = binding;
			return *this;
		}
		VertexInputAttributeDescription& setFormat(Format format) {
			this->format = convertCEnum(format);
			return *this;
		}
		VertexInputAttributeDescription& setOffset(uint32_t offset) {
			this->offset = offset;
			return *this;
		}
	};

	class VertexInputBindingDescription : public StructBase<VkVertexInputBindingDescription, VertexInputBindingDescription>
	{
		using Base = StructBase<VkVertexInputBindingDescription, VertexInputBindingDescription>;
	public:
		using Base::Base;

		VertexInputBindingDescription(uint32_t binding, uint32_t stride, VertexInputRate inputRate) : Base() {
			this->binding = binding;
			this->stride = stride;
			this->inputRate = convertCEnum(inputRate);
		}
		VertexInputBindingDescription& setBinding(uint32_t binding) {
			this->binding = binding;
			return *this;
		}
		VertexInputBindingDescription& setStride(uint32_t stride) {
			this->stride = stride;
			return *this;
		}
		VertexInputBindingDescription& setInputRate(VertexInputRate inputRate) {
			this->inputRate = convertCEnum(inputRate);
			return *this;
		}
	};

	class StencilOpState : public StructBase<VkStencilOpState, StencilOpState>
	{
		using Base = StructBase<VkStencilOpState, StencilOpState>;
	public:
		using Base::Base;

		StencilOpState(StencilOp failOp, StencilOp passOp,
			StencilOp depthFailOp, CompareOp compareOp,
			uint32_t compareMask, uint32_t writeMask,
			uint32_t reference) : Base() {
			this->failOp = convertCEnum(failOp);
			this->passOp = convertCEnum(passOp);
			this->depthFailOp = convertCEnum(depthFailOp);
			this->compareOp = convertCEnum(compareOp);
			this->compareMask = compareMask;
			this->writeMask = writeMask;
			this->reference = reference;
		}
		StencilOpState& setFailOp(StencilOp failOp) {
			this->failOp = convertCEnum(failOp);
			return *this;
		}
		StencilOpState& setPassOp(StencilOp passOp) {
			this->passOp = convertCEnum(passOp);
			return *this;
		}
		StencilOpState& setDepthFailOp(StencilOp depthFailOp) {
			this->depthFailOp = convertCEnum(depthFailOp);
			return *this;
		}
		StencilOpState& setCompareOp(CompareOp compareOp) {
			this->compareOp = convertCEnum(compareOp);
			return *this;
		}
		StencilOpState& setCompareMask(uint32_t compareMask) {
			this->compareMask = compareMask;
			return *this;
		}
		StencilOpState& setWriteMask(uint32_t writeMask) {
			this->writeMask = writeMask;
			return *this;
		}
		StencilOpState& setReference(uint32_t reference) {
			this->reference = reference;
			return *this;
		}
	};

	class SpecializationMapEntry : public StructBase<VkSpecializationMapEntry, SpecializationMapEntry>
	{
		using Base = StructBase<VkSpecializationMapEntry, SpecializationMapEntry>;
	public:
		using Base::Base;
		SpecializationMapEntry& setConstantID(uint32_t constantID) {
			this->constantID = constantID;
			return *this;
		}
		SpecializationMapEntry& setOffset(uint32_t offset) {
			this->offset = offset;
			return *this;
		}
		SpecializationMapEntry& setSize(size_t size) {
			this->size = size;
			return *this;
		}
	};

	class SpecializationInfo : public StructBase<VkSpecializationInfo, SpecializationInfo>
	{
		using Base = StructBase<VkSpecializationInfo, SpecializationInfo>;
	public:
	using Base::Base;
		SpecializationInfo& setMapEntries(std::span<const SpecializationMapEntry> mapEntries) {
			this->mapEntryCount = mapEntries.size();
			this->pMapEntries = SpecializationMapEntry::underlyingCast(mapEntries.data());
			return *this;
		}
		SpecializationInfo& setData(const void* data, size_t size) {
			this->dataSize = size;
			this->pData = data;
			return *this;
		}
	};

	class PipelineRef : public BaseComponent<VkPipeline, PipelineRef>
	{
		using Base = BaseComponent<VkPipeline, PipelineRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Pipeline";
	};

	class Pipeline : public VerificatorComponent<VkPipeline, PipelineRef>
	{
		using Base = VerificatorComponent<VkPipeline, PipelineRef>;
	public:
		using Base::Base;

		class LayoutRef : public BaseComponent<VkPipelineLayout, LayoutRef>
		{
			using Base = BaseComponent<VkPipelineLayout, LayoutRef>;
		public:
			using Base::Base;
			static inline const std::string s_typeName = "Pipeline::Layout";
		};

		class Layout : public VerificatorComponent<VkPipelineLayout, LayoutRef>
		{
			using Base = VerificatorComponent<VkPipelineLayout, LayoutRef>;
		public:
			using Base::Base;

			class CreateInfo : public StructBase<VkPipelineLayoutCreateInfo, CreateInfo>
			{
				using Base = StructBase<VkPipelineLayoutCreateInfo, CreateInfo>;
			public:
				using Base::Base;

				CreateInfo(std::span<const DescriptorSet::Layout> setLayouts,
					std::span<const PushConstantRange> pushConstantRanges = {},
					PipelineLayoutCreate::Flags flags = PipelineLayoutCreate::Bits::None) : Base() {
					this->setLayoutCount = static_cast<uint32_t>(setLayouts.size());
					this->pSetLayouts = DescriptorSet::Layout::underlyingCast(setLayouts.data());
					this->pushConstantRangeCount = pushConstantRanges.size();
					this->pPushConstantRanges = PushConstantRange::underlyingCast(pushConstantRanges.data());
					this->flags = flags;
				}

				CreateInfo& setFlags(PipelineLayoutCreate::Flags flags) {
					this->flags = flags;
					return *this;
				}

				CreateInfo& setSetLayouts(std::span<const DescriptorSet::Layout> setLayouts) {
					this->setLayoutCount = static_cast<uint32_t>(setLayouts.size());
					this->pSetLayouts = DescriptorSet::Layout::underlyingCast(setLayouts.data());
					return *this;
				}

				CreateInfo& setPushConstantRanges(std::span<const PushConstantRange> pushConstantRanges) {
					this->pushConstantRangeCount = pushConstantRanges.size();
					this->pPushConstantRanges = PushConstantRange::underlyingCast(pushConstantRanges.data());
					return *this;
				}

				PipelineLayoutCreate::Flags getFlags() const { return this->flags; };
				std::span<const DescriptorSet::LayoutRef> getSetLayouts() const {
					return std::span<const DescriptorSet::LayoutRef>(DescriptorSet::Layout::underlyingCast(this->pSetLayouts), this->setLayoutCount);
				};
				std::span<const PushConstantRange> getPushConstantRanges() const {
					return std::span<const PushConstantRange>(PushConstantRange::underlyingCast(this->pPushConstantRanges), this->pushConstantRangeCount);
				};
			};

			void create(const DeviceFunctionTable& functions,
				const DeviceRef& device, const CreateInfo& createInfo);
			void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);
		};

		class ShaderStageCreateInfo : public StructBase<VkPipelineShaderStageCreateInfo, ShaderStageCreateInfo>
		{
			using Base = StructBase<VkPipelineShaderStageCreateInfo, ShaderStageCreateInfo>;
		public:
			using Base::Base;

			ShaderStageCreateInfo(ShaderStage::Bits stage, 
				const ShaderModuleRef& module, const char* name,
				const SpecializationInfo& specializationInfo = SpecializationInfo(),
				PipelineShaderStageCreate::Flags flags = PipelineShaderStageCreate::Bits::None) : Base() {
				this->stage = static_cast<VkShaderStageFlagBits>(stage);
				this->module = module.getHandle();
				this->pName = name;
				this->pSpecializationInfo = specializationInfo.getUnderlyingPointer();
				this->flags = flags;
			}
			ShaderStageCreateInfo& setStage(ShaderStage::Bits stage) {
				this->stage = static_cast<VkShaderStageFlagBits>(stage);
				return *this;
			}
			ShaderStageCreateInfo& setModule(const ShaderModuleRef& module) {
				this->module = module.getHandle();
				return *this;
			}
			ShaderStageCreateInfo& setName(const char* name) {
				this->pName = name;
				return *this;
			}
			ShaderStageCreateInfo& setSpecializationInfo(const SpecializationInfo& specializationInfo) {
				this->pSpecializationInfo = specializationInfo.getUnderlyingPointer();
				return *this;
			}
			ShaderStageCreateInfo& setFlags(PipelineShaderStageCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
		};
		
		class VertexInputStateCreateInfo : public StructBase<VkPipelineVertexInputStateCreateInfo, VertexInputStateCreateInfo>
		{
			using Base = StructBase<VkPipelineVertexInputStateCreateInfo, VertexInputStateCreateInfo>;
		public:
			using Base::Base;
			VertexInputStateCreateInfo(std::span<const VertexInputBindingDescription> bindingDescriptions,
				std::span<const VertexInputAttributeDescription> attributeDescriptions,
				PipelineVertexInputStateCreate::Flags flags = PipelineVertexInputStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->vertexBindingDescriptionCount = bindingDescriptions.size();
				this->pVertexBindingDescriptions = VertexInputBindingDescription::underlyingCast(bindingDescriptions.data());
				this->vertexAttributeDescriptionCount = attributeDescriptions.size();
				this->pVertexAttributeDescriptions = VertexInputAttributeDescription::underlyingCast(attributeDescriptions.data());
			}

			VertexInputStateCreateInfo& setFlags(PipelineVertexInputStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			VertexInputStateCreateInfo& setVertexBindingDescriptions(std::span<const VertexInputBindingDescription> bindingDescriptions) {
				this->vertexBindingDescriptionCount = bindingDescriptions.size();
				this->pVertexBindingDescriptions = VertexInputBindingDescription::underlyingCast(bindingDescriptions.data());
				return *this;
			}
			VertexInputStateCreateInfo& setVertexAttributeDescriptions(std::span<const VertexInputAttributeDescription> attributeDescriptions) {
				this->vertexAttributeDescriptionCount = attributeDescriptions.size();
				this->pVertexAttributeDescriptions = VertexInputAttributeDescription::underlyingCast(attributeDescriptions.data());
				return *this;
			}
		};

		class InputAssemblyStateCreateInfo : public StructBase<VkPipelineInputAssemblyStateCreateInfo, InputAssemblyStateCreateInfo>
		{
			using Base = StructBase<VkPipelineInputAssemblyStateCreateInfo, InputAssemblyStateCreateInfo>;
		public:
			using Base::Base;
			InputAssemblyStateCreateInfo(PrimitiveTopology topology, 
				bool primitiveRestartEnable, 
				PipelineInputAssemblyStateCreate::Flags flags = PipelineInputAssemblyStateCreate::Bits::None) : Base() {
				this->primitiveRestartEnable = primitiveRestartEnable;
				this->topology = convertCEnum(topology);
				this->flags = flags;
			};

			InputAssemblyStateCreateInfo& setFlags(PipelineInputAssemblyStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			InputAssemblyStateCreateInfo& setTopology(PrimitiveTopology topology) {
				this->topology = convertCEnum(topology);
				return *this;
			}
			InputAssemblyStateCreateInfo& setPrimitiveRestartEnable(bool primitiveRestartEnable) {
				this->primitiveRestartEnable = primitiveRestartEnable;
				return *this;
			}
		};

		class TessellationStateCreateInfo : public StructBase<VkPipelineTessellationStateCreateInfo, TessellationStateCreateInfo>
		{
			using Base = StructBase<VkPipelineTessellationStateCreateInfo, TessellationStateCreateInfo>;
		public:
			using Base::Base;
			TessellationStateCreateInfo& setFlags(PipelineTessellationStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			TessellationStateCreateInfo& setPatchControlPoints(uint32_t patchControlPoints) {
				this->patchControlPoints = patchControlPoints;
				return *this;
			}
		};

		class ViewportStateCreateInfo : public StructBase<VkPipelineViewportStateCreateInfo, ViewportStateCreateInfo>
		{
			using Base = StructBase<VkPipelineViewportStateCreateInfo, ViewportStateCreateInfo>;
		public:
			using Base::Base;

			ViewportStateCreateInfo(std::span<const Viewport> viewports,
				std::span<const Rect2D> scissors,
				PipelineViewportStateCreate::Flags flags = PipelineViewportStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->viewportCount = viewports.size();
				this->pViewports = Viewport::underlyingCast(viewports.data());
				this->scissorCount = scissors.size();
				this->pScissors = Rect2D::underlyingCast(scissors.data());
			}

			ViewportStateCreateInfo& setFlags(PipelineViewportStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			ViewportStateCreateInfo& setViewports(std::span<const Viewport> viewports) {
				this->viewportCount = viewports.size();
				this->pViewports = Viewport::underlyingCast(viewports.data());
				return *this;
			}
			ViewportStateCreateInfo& setScissors(std::span<const Rect2D> scissors) {
				this->scissorCount = scissors.size();
				this->pScissors = Rect2D::underlyingCast(scissors.data());
				return *this;
			}
		};

		class RasterizationStateCreateInfo : public StructBase<VkPipelineRasterizationStateCreateInfo, RasterizationStateCreateInfo>
		{
			using Base = StructBase<VkPipelineRasterizationStateCreateInfo, RasterizationStateCreateInfo>;
		public:
			using Base::Base;

			RasterizationStateCreateInfo(bool depthClampEnable,
				bool rasterizerDiscardEnable, PolygonMode polygonMode,
				CullMode::Flags cullMode, FrontFace frontFace,
				bool depthBiasEnable, float depthBiasConstantFactor,
				float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
				PipelineRasterizationStateCreate::Flags flags = PipelineRasterizationStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->depthClampEnable = depthClampEnable;
				this->rasterizerDiscardEnable = rasterizerDiscardEnable;
				this->polygonMode = convertCEnum(polygonMode);
				this->cullMode = cullMode;
				this->frontFace = convertCEnum(frontFace);
				this->depthBiasEnable = depthBiasEnable;
				this->depthBiasConstantFactor = depthBiasConstantFactor;
				this->depthBiasClamp = depthBiasClamp;
				this->depthBiasSlopeFactor = depthBiasSlopeFactor;
				this->lineWidth = lineWidth;
			};

			RasterizationStateCreateInfo& setFlags(PipelineRasterizationStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			RasterizationStateCreateInfo& setDepthClampEnable(bool depthClampEnable) {
				this->depthClampEnable = depthClampEnable;
				return *this;
			}
			RasterizationStateCreateInfo& setRasterizerDiscardEnable(bool rasterizerDiscardEnable) {
				this->rasterizerDiscardEnable = rasterizerDiscardEnable;
				return *this;
			}
			RasterizationStateCreateInfo& setPolygonMode(PolygonMode polygonMode) {
				this->polygonMode = convertCEnum(polygonMode);
				return *this;
			}
			RasterizationStateCreateInfo& setCullMode(CullMode::Flags cullMode) {
				this->cullMode = cullMode;
				return *this;
			}
			RasterizationStateCreateInfo& setFrontFace(FrontFace frontFace) {
				this->frontFace = convertCEnum(frontFace);
				return *this;
			}
			RasterizationStateCreateInfo& setDepthBiasEnable(bool depthBiasEnable) {
				this->depthBiasEnable = depthBiasEnable;
				return *this;
			}
			RasterizationStateCreateInfo& setDepthBiasConstantFactor(float depthBiasConstantFactor) {
				this->depthBiasConstantFactor = depthBiasConstantFactor;
				return *this;
			}
			RasterizationStateCreateInfo& setDepthBiasClamp(float depthBiasClamp) {
				this->depthBiasClamp = depthBiasClamp;
				return *this;
			}
			RasterizationStateCreateInfo& setDepthBiasSlopeFactor(float depthBiasSlopeFactor) {
				this->depthBiasSlopeFactor = depthBiasSlopeFactor;
				return *this;
			}
			RasterizationStateCreateInfo& setLineWidth(float lineWidth) {
				this->lineWidth = lineWidth;
				return *this;
			}
		};

		class MultisampleStateCreateInfo : public StructBase<VkPipelineMultisampleStateCreateInfo, MultisampleStateCreateInfo>
		{
			using Base = StructBase<VkPipelineMultisampleStateCreateInfo, MultisampleStateCreateInfo>;
		public:
		using Base::Base;

			MultisampleStateCreateInfo(bool sampleShadingEnable,
				SampleCount::Bits rasterizationSamples,
				float minSampleShading, std::span<const SampleMask> sampleMask,
				bool alphaToCoverageEnable, bool alphaToOneEnable,
				PipelineMultisampleStateCreate::Flags flags = PipelineMultisampleStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->sampleShadingEnable = sampleShadingEnable;
				this->rasterizationSamples = static_cast<VkSampleCountFlagBits>(rasterizationSamples);
				this->minSampleShading = minSampleShading;
				this->pSampleMask = sampleMask.data();
				this->alphaToCoverageEnable = alphaToCoverageEnable;
				this->alphaToOneEnable = alphaToOneEnable;
			};
			MultisampleStateCreateInfo& setFlags(PipelineMultisampleStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			MultisampleStateCreateInfo& setRasterizationSamples(SampleCount::Bits rasterizationSamples) {
				this->rasterizationSamples = static_cast<VkSampleCountFlagBits>(rasterizationSamples);
				return *this;
			}
			MultisampleStateCreateInfo& setSampleShadingEnable(bool sampleShadingEnable) {
				this->sampleShadingEnable = sampleShadingEnable;
				return *this;
			}
			MultisampleStateCreateInfo& setMinSampleShading(float minSampleShading) {
				this->minSampleShading = minSampleShading;
				return *this;
			}
			MultisampleStateCreateInfo& setSampleMask(std::span<const SampleMask> sampleMask) {
				this->pSampleMask = sampleMask.data();
				return *this;
			}
			MultisampleStateCreateInfo& setAlphaToCoverageEnable(bool alphaToCoverageEnable) {
				this->alphaToCoverageEnable = alphaToCoverageEnable;
				return *this;
			}
			MultisampleStateCreateInfo& setAlphaToOneEnable(bool alphaToOneEnable) {
				this->alphaToOneEnable = alphaToOneEnable;
				return *this;
			}
		};
		
		class DepthStencilStateCreateInfo : public StructBase<VkPipelineDepthStencilStateCreateInfo, DepthStencilStateCreateInfo>
		{
			using Base = StructBase<VkPipelineDepthStencilStateCreateInfo, DepthStencilStateCreateInfo>;
		public:
		using Base::Base;
			DepthStencilStateCreateInfo(const StencilOpState& front, const StencilOpState& back,
				CompareOp depthCompareOp, bool depthBoundsTestEnable,
				bool stencilTestEnable, bool depthWriteEnable,
				float minDepthBounds, float maxDepthBounds,
				bool depthTestEnable, PipelineDepthStencilStateCreate::Flags flags = PipelineDepthStencilStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->depthTestEnable = depthTestEnable;
				this->depthWriteEnable = depthWriteEnable;
				this->depthCompareOp = convertCEnum(depthCompareOp);
				this->depthBoundsTestEnable = depthBoundsTestEnable;
				this->stencilTestEnable = stencilTestEnable;
				this->front = front.getStruct();
				this->back = back.getStruct();
				this->minDepthBounds = minDepthBounds;
				this->maxDepthBounds = maxDepthBounds;
			};

			DepthStencilStateCreateInfo& setFlags(PipelineDepthStencilStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			DepthStencilStateCreateInfo& setDepthTestEnable(bool depthTestEnable) {
				this->depthTestEnable = depthTestEnable;
				return *this;
			}
			DepthStencilStateCreateInfo& setDepthWriteEnable(bool depthWriteEnable) {
				this->depthWriteEnable = depthWriteEnable;
				return *this;
			}
			DepthStencilStateCreateInfo& setDepthCompareOp(CompareOp depthCompareOp) {
				this->depthCompareOp = convertCEnum(depthCompareOp);
				return *this;
			}
			DepthStencilStateCreateInfo& setDepthBoundsTestEnable(bool depthBoundsTestEnable) {
				this->depthBoundsTestEnable = depthBoundsTestEnable;
				return *this;
			}
			DepthStencilStateCreateInfo& setStencilTestEnable(bool stencilTestEnable) {
				this->stencilTestEnable = stencilTestEnable;
				return *this;
			}
			DepthStencilStateCreateInfo& setFront(const StencilOpState& front) {
				this->front = front.getStruct();
				return *this;
			}
			DepthStencilStateCreateInfo& setBack(const StencilOpState& back) {
				this->back = back.getStruct();
				return *this;
			}
			DepthStencilStateCreateInfo& setMinDepthBounds(float minDepthBounds) {
				this->minDepthBounds = minDepthBounds;
				return *this;
			}
			DepthStencilStateCreateInfo& setMaxDepthBounds(float maxDepthBounds) {
				this->maxDepthBounds = maxDepthBounds;
				return *this;
			}
		};

		class ColorBlendAttachmentState : public StructBase<VkPipelineColorBlendAttachmentState, ColorBlendAttachmentState>
		{
			using Base = StructBase<VkPipelineColorBlendAttachmentState, ColorBlendAttachmentState>;
		public:
			using Base::Base;

			ColorBlendAttachmentState(bool blendEnable,
				BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor,
				BlendOp colorBlendOp, BlendFactor srcAlphaBlendFactor,
				BlendFactor dstAlphaBlendFactor, BlendOp alphaBlendOp,
				ColorComponent::Flags colorWriteMask) : Base() {
				this->blendEnable = blendEnable;
				this->srcColorBlendFactor = convertCEnum(srcColorBlendFactor);
				this->dstColorBlendFactor = convertCEnum(dstColorBlendFactor);
				this->colorBlendOp = convertCEnum(colorBlendOp);
				this->srcAlphaBlendFactor = convertCEnum(srcAlphaBlendFactor);
				this->dstAlphaBlendFactor = convertCEnum(dstAlphaBlendFactor);
				this->alphaBlendOp = convertCEnum(alphaBlendOp);
				this->colorWriteMask = colorWriteMask;
			};

			ColorBlendAttachmentState& setBlendEnable(bool blendEnable) {
				this->blendEnable = blendEnable;
				return *this;
			}
			ColorBlendAttachmentState& setSrcColorBlendFactor(BlendFactor srcColorBlendFactor) {
				this->srcColorBlendFactor = convertCEnum(srcColorBlendFactor);
				return *this;
			}
			ColorBlendAttachmentState& setDstColorBlendFactor(BlendFactor dstColorBlendFactor) {
				this->dstColorBlendFactor = convertCEnum(dstColorBlendFactor);
				return *this;
			}
			ColorBlendAttachmentState& setColorBlendOp(BlendOp colorBlendOp) {
				this->colorBlendOp = convertCEnum(colorBlendOp);
				return *this;
			}
			ColorBlendAttachmentState& setSrcAlphaBlendFactor(BlendFactor srcAlphaBlendFactor) {
				this->srcAlphaBlendFactor = convertCEnum(srcAlphaBlendFactor);
				return *this;
			}
			ColorBlendAttachmentState& setDstAlphaBlendFactor(BlendFactor dstAlphaBlendFactor) {
				this->dstAlphaBlendFactor = convertCEnum(dstAlphaBlendFactor);
				return *this;
			}
			ColorBlendAttachmentState& setAlphaBlendOp(BlendOp alphaBlendOp) {
				this->alphaBlendOp = convertCEnum(alphaBlendOp);
				return *this;
			}
			ColorBlendAttachmentState& setColorWriteMask(ColorComponent::Flags colorWriteMask) {
				this->colorWriteMask = colorWriteMask;
				return *this;
			}
		};

		class ColorBlendStateCreateInfo : public StructBase<VkPipelineColorBlendStateCreateInfo, ColorBlendStateCreateInfo>
		{
			using Base = StructBase<VkPipelineColorBlendStateCreateInfo, ColorBlendStateCreateInfo>;
		public:
			using Base::Base;

			ColorBlendStateCreateInfo(std::span<const ColorBlendAttachmentState> attachments,
				bool logicOpEnable, LogicOp logicOp, const Color& blendConstants,
				PipelineColorBlendStateCreate::Flags flags = PipelineColorBlendStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->logicOpEnable = logicOpEnable;
				this->logicOp = convertCEnum(logicOp);
				this->attachmentCount = attachments.size();
				this->pAttachments = ColorBlendAttachmentState::underlyingCast(attachments.data());
				std::memcpy(this->blendConstants, blendConstants.getStruct().data(), sizeof(float) * 4);
			}
			ColorBlendStateCreateInfo& setFlags(PipelineColorBlendStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			ColorBlendStateCreateInfo& setLogicOpEnable(bool logicOpEnable) {
				this->logicOpEnable = logicOpEnable;
				return *this;
			}
			ColorBlendStateCreateInfo& setLogicOp(LogicOp logicOp) {
				this->logicOp = convertCEnum(logicOp);
				return *this;
			}
			ColorBlendStateCreateInfo& setAttachments(std::span<const ColorBlendAttachmentState> attachments) {
				this->attachmentCount = attachments.size();
				this->pAttachments = ColorBlendAttachmentState::underlyingCast(attachments.data());
				return *this;
			}
			ColorBlendStateCreateInfo& setBlendConstants(const Color& blendConstants) {
				std::memcpy(this->blendConstants, blendConstants.getStruct().data(), sizeof(float) * 4);
				return *this;
			}
		};

		class DynamicStateCreateInfo : public StructBase<VkPipelineDynamicStateCreateInfo, DynamicStateCreateInfo>
		{
			using Base = StructBase<VkPipelineDynamicStateCreateInfo, DynamicStateCreateInfo>;
		public:
			using Base::Base;

			DynamicStateCreateInfo(std::span<const DynamicState> dynamicStates,
				PipelineDynamicStateCreate::Flags flags = PipelineDynamicStateCreate::Bits::None) : Base() {
				this->flags = flags;
				this->dynamicStateCount = dynamicStates.size();
				this->pDynamicStates = convertCEnum(dynamicStates.data());
			}

			DynamicStateCreateInfo& setFlags(PipelineDynamicStateCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			DynamicStateCreateInfo& setDynamicStates(std::span<const DynamicState> dynamicStates) {
				this->dynamicStateCount = dynamicStates.size();
				this->pDynamicStates = convertCEnum(dynamicStates.data());
				return *this;
			}
		};

		class CreateInfo : public StructBase<VkGraphicsPipelineCreateInfo, CreateInfo>
		{
			using Base = StructBase<VkGraphicsPipelineCreateInfo, CreateInfo>;
		public:
			using Base::Base;
			CreateInfo& setFlags(PipelineCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			CreateInfo& setStages(std::span<const ShaderStageCreateInfo> stages) {
				this->stageCount = stages.size();
				this->pStages = ShaderStageCreateInfo::underlyingCast(stages.data());
				return *this;
			}
			CreateInfo& setVertexInputState(const VertexInputStateCreateInfo& vertexInputState) {
				this->pVertexInputState = vertexInputState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setInputAssemblyState(const InputAssemblyStateCreateInfo& inputAssemblyState) {
				this->pInputAssemblyState = inputAssemblyState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setTessellationState(const TessellationStateCreateInfo& tessellationState) {
				this->pTessellationState = tessellationState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setViewportState(const ViewportStateCreateInfo& viewportState) {
				this->pViewportState = viewportState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setRasterizationState(const RasterizationStateCreateInfo& rasterizationState) {
				this->pRasterizationState = rasterizationState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setMultisampleState(const MultisampleStateCreateInfo& multisampleState) {
				this->pMultisampleState = multisampleState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setDepthStencilState(const DepthStencilStateCreateInfo& depthStencilState) {
				this->pDepthStencilState = depthStencilState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setColorBlendState(const ColorBlendStateCreateInfo& colorBlendState) {
				this->pColorBlendState = colorBlendState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setDynamicState(const DynamicStateCreateInfo& dynamicState) {
				this->pDynamicState = dynamicState.getUnderlyingPointer();
				return *this;
			}
			CreateInfo& setLayout(const Pipeline::LayoutRef& layout) {
				this->layout = layout.getHandle();
				return *this;
			}
			CreateInfo& setRenderPass(const RenderPassRef& renderPass) {
				this->renderPass = renderPass.getHandle();
				return *this;
			}
			CreateInfo& setSubpass(uint32_t subpass) {
				this->subpass = subpass;
				return *this;
			}
			CreateInfo& setBasePipelineHandle(const PipelineRef& basePipeline) {
				this->basePipelineHandle = basePipeline.getHandle();
				return *this;
			}
			CreateInfo& setBasePipelineIndex(int32_t basePipelineIndex) {
				this->basePipelineIndex = basePipelineIndex;
				return *this;
			}
		};

		void create(const DeviceFunctionTable& functions,
			const DeviceRef& device, const CreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);

		static std::vector<Pipeline> create(const DeviceFunctionTable& functions,
			const DeviceRef& device, std::span<const CreateInfo> createInfos);
	};
}
