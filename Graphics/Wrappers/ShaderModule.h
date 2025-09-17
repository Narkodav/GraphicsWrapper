#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class ShaderModuleRef : public BaseComponent<VkShaderModule, ShaderModuleRef>
    {
        using Base = BaseComponent<VkShaderModule, ShaderModuleRef>;
    public:
        using Base::Base;
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

            CreateInfo(const std::vector<char>& shaderCode) : Base()
            {
                this->flags = 0;
                this->codeSize = shaderCode.size();
                this->pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
			}

            CreateInfo& setShaderCode(const std::vector<char>& shaderCode) {
                this->codeSize = shaderCode.size();
                this->pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
                return *this;
			}
        };

        void create(const DeviceRef& device, const DeviceFunctionTable& functions, const CreateInfo& createInfo);
        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);

        static std::vector<char> parseShaderCodeSPIRV(const std::string& filename);

        static ShaderType inferShaderType(const std::vector<char>& spirvCode);
    };
}

