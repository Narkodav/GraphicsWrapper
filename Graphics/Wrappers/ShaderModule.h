#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class ShaderModuleRef : public BaseComponent<VkShaderModule, ShaderModuleRef>
    {
        using Base = BaseComponent<VkShaderModule, ShaderModuleRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "ShaderModule";
    };

    class ShaderModule : public VerificatorComponent<VkShaderModule, ShaderModuleRef>
    {
        using Base = VerificatorComponent<VkShaderModule, ShaderModuleRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkShaderModuleCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkShaderModuleCreateInfo, CreateInfo>;
        public:
            using Base::Base;

            CreateInfo(std::span<const char> shaderCode) : Base()
            {
                this->flags = 0;
                this->codeSize = shaderCode.size();
                this->pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
			}

            CreateInfo& setShaderCode(std::span<const char> shaderCode) {
                this->codeSize = shaderCode.size();
                this->pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
                return *this;
			}
        };

        void create(const DeviceFunctionTable& functions, const DeviceRef& device, const CreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);

        static std::vector<char> parseShaderCodeSPIRV(std::string_view filename);
        static ShaderStage::Bits inferShaderStage(std::span<const char> spirvCode);
    };
}

