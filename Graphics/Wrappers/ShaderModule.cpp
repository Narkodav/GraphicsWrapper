#include "ShaderModule.h"

namespace Graphics
{
    void ShaderModule::create(const DeviceFunctionTable& functions, const DeviceRef& device, const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid shader module");

        auto result = functions.execute<DeviceFunction::CreateShaderModule>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create a shader module: " +
            s_resultMessages.at(result));
    }

    void ShaderModule::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid shader module");
        functions.execute<DeviceFunction::DestroyShaderModule>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }

    std::vector<char> ShaderModule::parseShaderCodeSPIRV(std::string_view filename)
    {
        std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

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

    ShaderStage::Bits ShaderModule::inferShaderStage(std::span<const char> spirvCode)
    {
        const uint32_t* words = reinterpret_cast<const uint32_t*>(spirvCode.data());
        size_t wordCount = spirvCode.size() / sizeof(uint32_t);

        // Verify SPIR-V magic number
        if (wordCount < 5 || words[0] != 0x07230203) {
            return ShaderStage::Bits::None;
        }

        ShaderStage::Bits foundStage = ShaderStage::Bits::None;
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
        int entryPointCount = 0;
#endif

        // Iterate through the SPIR-V code
        for (size_t i = 5; i < wordCount;) {
            uint32_t instruction = words[i];
            uint32_t opCode = instruction & 0xFF;         // OpCode is in the lowest 8 bits
            uint32_t wordLength = instruction >> 16;      // Word count is in the upper 16 bits

            if (opCode == 15) { // OpEntryPoint
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
                entryPointCount++;
                GRAPHICS_VERIFY(entryPointCount == 1, "Multiple entry points found in SPIR-V module - use separate files for each shader stage");
#endif
                if (i + 1 < wordCount) {
                    uint32_t executionModel = words[i + 1];

                    switch (executionModel) {
                    case 0:     foundStage = ShaderStage::Bits::Vertex; break;
                    case 1:     foundStage = ShaderStage::Bits::TessellationControl; break;
                    case 2:     foundStage = ShaderStage::Bits::TessellationEvaluation; break;
                    case 3:     foundStage = ShaderStage::Bits::Geometry; break;
                    case 4:     foundStage = ShaderStage::Bits::Fragment; break;
                    case 5:     foundStage = ShaderStage::Bits::Compute; break;
                    case 5267:  foundStage = ShaderStage::Bits::TaskNV; break;
                    case 5364:  foundStage = ShaderStage::Bits::TaskEXT; break;
                    case 5268:  foundStage = ShaderStage::Bits::MeshNV; break;
                    case 5365:  foundStage = ShaderStage::Bits::MeshEXT; break;
                    default:    foundStage = ShaderStage::Bits::None; break;
                    }
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
                    return foundStage; // Early return in release mode
#endif
                }
            }

            if (wordLength == 0) { // Prevent infinite loop
                return ShaderStage::Bits::None;
            }

            i += wordLength; // Move to next instruction
        }

        return foundStage;
    }

}