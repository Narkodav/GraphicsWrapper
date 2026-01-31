#include "Graphics/Graphics.h"

namespace Graphics
{
    void ShaderModule::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const ShaderModuleCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid shader module");

        auto result = functions.execute<DeviceFunction::CreateShaderModule>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create a shader module");
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

    Flags::ShaderStage::Bits ShaderModule::inferShaderStage(std::span<const char> spirvCode)
    {
        const uint32_t* words = reinterpret_cast<const uint32_t*>(spirvCode.data());
        size_t wordCount = spirvCode.size() / sizeof(uint32_t);

        // Verify SPIR-V magic number
        if (wordCount < 5 || words[0] != 0x07230203) {
            return Flags::ShaderStage::Bits::None;
        }

        Flags::ShaderStage::Bits foundStage = Flags::ShaderStage::Bits::None;
#if defined(_DEBUG) && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
        size_t entryPointCount = 0;
#endif

        // Iterate through the SPIR-V code
        for (size_t i = 5; i < wordCount;) {
            uint32_t instruction = words[i];
            uint32_t opCode = instruction & 0xFF;         // OpCode is in the lowest 8 bits
            uint32_t wordLength = instruction >> 16;      // Word count is in the upper 16 bits

            if (opCode == 15) { // OpEntryPoint
#if defined(_DEBUG) && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
                entryPointCount++;
                GRAPHICS_VERIFY(entryPointCount == 1, "Multiple entry points found in SPIR-V module - use separate files for each shader stage");
#endif
                if (i + 1 < wordCount) {
                    uint32_t executionModel = words[i + 1];

                    switch (executionModel) {
                    case 0:     foundStage = Flags::ShaderStage::Bits::Vertex; break;
                    case 1:     foundStage = Flags::ShaderStage::Bits::TessellationControl; break;
                    case 2:     foundStage = Flags::ShaderStage::Bits::TessellationEvaluation; break;
                    case 3:     foundStage = Flags::ShaderStage::Bits::Geometry; break;
                    case 4:     foundStage = Flags::ShaderStage::Bits::Fragment; break;
                    case 5:     foundStage = Flags::ShaderStage::Bits::Compute; break;
                    case 5267:  foundStage = Flags::ShaderStage::Bits::TaskNV; break;
                    case 5364:  foundStage = Flags::ShaderStage::Bits::TaskEXT; break;
                    case 5268:  foundStage = Flags::ShaderStage::Bits::MeshNV; break;
                    case 5365:  foundStage = Flags::ShaderStage::Bits::MeshEXT; break;
                    default:    foundStage = Flags::ShaderStage::Bits::None; break;
                    }
#if !defined(_DEBUG) && !defined(GRAPHICS_ALWAYS_VERIFY) || defined(GRAPHICS_NO_VERIFY) 
                    return foundStage; // Early return in release mode
#endif
                }
            }

            if (wordLength == 0) { // Prevent infinite loop
                return Flags::ShaderStage::Bits::None;
            }

            i += wordLength; // Move to next instruction
        }

        return foundStage;
    }

}