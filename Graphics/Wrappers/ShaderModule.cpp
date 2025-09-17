#include "ShaderModule.h"

namespace Graphics
{
    void ShaderModule::create(const DeviceRef& device, const DeviceFunctionTable& functions, const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to create a valid shader module");

        auto result = functions.execute<DeviceFunction::CreateShaderModule>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create a shader module: " +
            s_resultMessages.at(result));
    }

    void ShaderModule::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid shader module");
        functions.execute<DeviceFunction::DestroyShaderModule>(
            device.getHandle(), getHandle(), nullptr);
    }

    std::vector<char> ShaderModule::parseShaderCodeSPIRV(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    ShaderType ShaderModule::inferShaderType(const std::vector<char>& spirvCode)
    {
        const uint32_t* words = reinterpret_cast<const uint32_t*>(spirvCode.data());
        size_t wordCount = spirvCode.size() / sizeof(uint32_t);

        // Verify SPIR-V magic number
        if (wordCount < 5 || words[0] != 0x07230203) {
            return ShaderType::Unknown;
        }

        // Iterate through the SPIR-V code
        for (size_t i = 5; i < wordCount;) {
            uint32_t instruction = words[i];
            uint32_t opCode = instruction & 0xFF;         // OpCode is in the lowest 8 bits
            uint32_t wordLength = instruction >> 16;      // Word count is in the upper 16 bits

            if (opCode == 15) { // OpEntryPoint
                if (i + 1 < wordCount) {
                    uint32_t executionModel = words[i + 1];

                    switch (executionModel) {
                    case 0:     return ShaderType::Vertex;
                    case 1:     return ShaderType::TessellationControl;
                    case 2:     return ShaderType::TessellationEvaluation;
                    case 3:     return ShaderType::Geometry;
                    case 4:     return ShaderType::Fragment;
                    case 5:     return ShaderType::Compute;
                    case 5267:  return ShaderType::TaskNV;
                    case 5364:  return ShaderType::Task;
                    case 5268:  return ShaderType::MeshNV;
                    case 5365:  return ShaderType::Mesh;
                    default:    return ShaderType::Unknown;
                    }
                }
                else return ShaderType::Unknown;
            }

            if (wordLength == 0) { // Prevent infinite loop
                return ShaderType::Unknown;
            }

            i += wordLength; // Move to next instruction
        }

        return ShaderType::Unknown;
    }

}