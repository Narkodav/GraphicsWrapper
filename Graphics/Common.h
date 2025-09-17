#pragma once
#pragma warning(error: 4715)

//glfw
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//vulkan
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
//#include <vma/vk_mem_alloc.h>

//glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RIGHT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <stb_image.h>

#include <optional>
#include <functional>
#include <any>
#include <ranges>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <atomic>
#include <set>
#include <fstream>
#include <array>
#include <stack>
#include <chrono>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Enums.h"
#include "Flags.h"
#include "QueuePropertyEnum.h"
#include "PropertyEnum.h"
#include "FeatureEnum.h"
#include "StructTraits.h"

#define GRAPHICS_API_ATTR VKAPI_ATTR
#define GRAPHICS_API_CALL VKAPI_CALL
#define GRAPHICS_API_PTR  VKAPI_PTR
#define GRAPHICS_VERIFY(condition, message) Graphics::verify(condition, message, __FILE__, __LINE__, __FUNCTION__)

namespace Graphics {


    template<typename T>
    concept StructHasNext = requires(T t) { t.pNext; };

    template<typename T>
    concept StructHasSType = requires(T t) { t.sType; };

    template<typename T, typename Derived>
    class StructBase : protected T {
    public:
        using T::T;

        StructBase() : T() {
            std::memset(static_cast<T*>(this), 0, sizeof(T));
            if constexpr (StructHasSType<T>) {
                T::sType = static_cast<typename std::remove_const<decltype(T::sType)>::type>(StructToEnumTraits_v<T>);
            }
            if constexpr (StructHasNext<T>) {
                T::pNext = nullptr;
            }
        };

        ~StructBase() = default;
        StructBase(const T& other) : T(other) {}
        StructBase(T&& other) : T(std::move(other)) {}
        StructBase(const StructBase&) = default;
        StructBase(StructBase&&) = default;

        StructBase& operator=(const StructBase&) = default;
        StructBase& operator=(StructBase&&) = default;

        StructBase& operator=(const T& other) { T::operator=(other); return *this; };
        StructBase& operator=(T&& other) { T::operator=(std::move(other)); return *this; };

        operator T& () { return static_cast<T&>(*this); }
        operator const T& () const { return static_cast<const T&>(*this); }
        const T* getUnderlyingPointer() const { return reinterpret_cast<const T*>(this); };
        T* getUnderlyingPointer() { return reinterpret_cast<T*>(this); };
        static const T* underlyingCast(const StructBase* ptr) { return reinterpret_cast<const T*>(ptr); };
        static T* underlyingCast(StructBase* ptr) { return reinterpret_cast<T*>(ptr); };

        const T& getStruct() const { return *this; };
        T& getStruct() { return *this; };

        static const Derived* underlyingCast(const T* ptr) { return reinterpret_cast<const Derived*>(ptr); };
        static Derived* underlyingCast(T* ptr) { return reinterpret_cast<Derived*>(ptr); };

        const void* getNext() const requires StructHasNext<T> { return this->pNext; };
        Derived& setNext(void* pNext) requires StructHasNext<T> {
            this->pNext = pNext;
            return *reinterpret_cast<Derived*>(this);
        };

        StructureType getType() const requires StructHasSType<T> { return convertEnum(this->sType); }
    };

    template<typename T, typename Derived>
    class BaseComponent {
    protected:
        T m_handle = nullptr;

        template<typename U>
        void setHandle(U&& newHandle) { m_handle = std::forward<U>(newHandle); };
        T& getHandle() { return m_handle; };
        void reset() { m_handle = nullptr; };

        T* getUnderlyingPointer() { return reinterpret_cast<T*>(this); };
        operator T& () { return this->getHandle(); };        

    public:
        BaseComponent() = default;

        ~BaseComponent() = default;

        BaseComponent(BaseComponent&&) = default;
        BaseComponent& operator=(BaseComponent&&) = default;

        BaseComponent(const BaseComponent&) = default;
        BaseComponent& operator=(const BaseComponent&) = default;

        BaseComponent(T&& handle) : m_handle(std::move(handle)) {}

        BaseComponent& operator=(T&& handle) {
            m_handle = std::move(handle);
            return *this;
        };

        BaseComponent(const T& handle) : m_handle(handle) {}

        BaseComponent& operator=(const T& handle) {
            m_handle = handle;
            return *this;
        };

        const T& getHandle() const { return m_handle; };

        bool operator==(const BaseComponent& other) const { return this->getHandle() == other.getHandle(); };
        bool operator!=(const BaseComponent& other) const { return this->getHandle() != other.getHandle(); };
        bool operator==(const T& handle) const { return this->getHandle() == handle; };
        bool operator!=(const T& handle) const { return this->getHandle() != handle; };

        operator const T& () const { return this->getHandle(); };
        const T* getUnderlyingPointer() const { return reinterpret_cast<const T*>(this); };

        static const T* underlyingCast(const BaseComponent* ptr) { return reinterpret_cast<const T*>(ptr); };
        static T* underlyingCast(BaseComponent* ptr) { return reinterpret_cast<T*>(ptr); };
        static const Derived* underlyingCast(const T* ptr) { return reinterpret_cast<const Derived*>(ptr); };
        static Derived* underlyingCast(T* ptr) { return reinterpret_cast<Derived*>(ptr); };

        bool isSet() const { return m_handle != nullptr; };
    };

    template<typename T, typename RefType>
    class VerificatorComponent : public RefType {
    public:
        VerificatorComponent() { this->setHandle(nullptr); };

        VerificatorComponent(VerificatorComponent&& other) {
            this->setHandle(other.getHandle());
            other.reset();
        }

        VerificatorComponent& operator=(VerificatorComponent&& other) {
            if (this == &other)
                return *this;
            GRAPHICS_VERIFY(!isValid(), "Trying to move to valid object");
            this->setHandle(other.getHandle());
            other.reset();
            return *this;
        }

        ~VerificatorComponent() {
            GRAPHICS_VERIFY(!isValid(), "Valid object was not destroyed");
        };

        VerificatorComponent(T&& other) {
            this->setHandle(std::move(other));
        };

        VerificatorComponent& operator=(T&& other) {
            if (this == &other)
                return *this;
            GRAPHICS_VERIFY(!isValid(), "Trying to move to valid object");
            this->setHandle(std::move(other));
            return *this;
        }

        VerificatorComponent(const VerificatorComponent&) = delete;
        VerificatorComponent& operator=(const VerificatorComponent&) = delete;
        VerificatorComponent(const T&) = delete;
        VerificatorComponent& operator=(const T&) = delete;

        bool isValid() const { return this->isSet(); };

        RefType getReference() const { return RefType(this->getHandle()); };
    };

    using bool32_t = VkBool32;

    struct Version
    {
        uint32_t version;

        operator uint32_t() const
        {
            return version;
        }

        operator const uint32_t&() const
        {
            return version;
        }

        operator uint32_t& ()
        {
            return version;
        }

        Version(uint32_t maj, uint32_t min, uint32_t pat)
            : version(VK_MAKE_VERSION(maj, min, pat)) {
        }

        Version(uint32_t version)
            : version(version) {
        };

        uint32_t getVersion() const
        {
            return version;
        }

        uint32_t major() const
        {
            return VK_VERSION_MAJOR(version);
        }

        uint32_t minor() const
        {
            return VK_VERSION_MINOR(version);
        }

        uint32_t patch() const
        {
            return VK_VERSION_PATCH(version);
        }
    };

    struct Extent2D : public StructBase<VkExtent2D, Extent2D> {
		using Base = StructBase<VkExtent2D, Extent2D>;
    public:
		using Base::Base;

        Extent2D(uint32_t w = 800, uint32_t h = 600) : Base() {
            this->width = w;
            this->height = h;
        }

        Extent2D& setWidth(uint32_t w) { this->width = w; return *this; }
        Extent2D& setHeight(uint32_t h) { this->height = h; return *this; }

		uint32_t getWidth() const { return this->width; };
		uint32_t getHeight() const { return this->height; };

        // Helper to get current window size from GLFW window
        static Extent2D getWindowExtent(GLFWwindow* window) {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            return Extent2D{
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };
        }

        // Helper to get current frame buffer size from GLFW window
        static Extent2D getFrameBufferExtent(GLFWwindow* window) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            return Extent2D{
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };
        }
    };

    struct CopyRegion : public StructBase<VkBufferCopy, CopyRegion> {
        using Base = StructBase<VkBufferCopy, CopyRegion>;
    public:
        using Base::Base;

        CopyRegion(size_t srcOffset, size_t dstOffset, size_t size) : Base() {
			this->dstOffset = dstOffset;
			this->srcOffset = srcOffset;
			this->size = size;
        };
    };

	struct Color : public StructBase<std::array<float, 4>, Color>
    {
        using Base = StructBase<std::array<float, 4>, Color>;
    public:
        using Base::Base;

        Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) : Base({ r, g, b, a }) {};

        Color(uint8_t r, uint8_t g, uint8_t b,
            uint8_t a = std::numeric_limits<uint8_t>::max()) :
            Base({ (static_cast<float>(r) / 255.f),
            (static_cast<float>(g) / 255.f),
            (static_cast<float>(b) / 255.f),
            (static_cast<float>(a) / 255.f) }) {
        };

        operator const glm::vec4& () const { return *reinterpret_cast<const glm::vec4*>(this); };
        operator glm::vec4& () { return *reinterpret_cast<glm::vec4*>(this); };

        operator const VkClearValue& () const { return *reinterpret_cast<const VkClearValue*>(this); };
        operator VkClearValue& () { return *reinterpret_cast<VkClearValue*>(this); };

        static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
        static Color White() { return Color(1.0f, 1.0f, 1.0f); }
        static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
        static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
        static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }

        Color& setR(float r) {
            Base::operator[](0) = r;
            return *this;
        }

        Color& setG(float g) {
            Base::operator[](1) = g;
            return *this;
        }

        Color& setB(float b) {
            Base::operator[](2) = b;
            return *this;
        }

        Color& setA(float a) {
            Base::operator[](3) = a;
            return *this;
        }

        float getR() const { return Base::operator[](0); };
        float getG() const { return Base::operator[](1); };
        float getB() const { return Base::operator[](2); };
        float getA() const { return Base::operator[](3); };
    };

    ////only works as an interface to existing memory, doesn't actually own it
    //template <typename T>
    //class ArrayInterface
    //{
    //private:
    //    const T* const m_array = nullptr;
    //    const size_t m_size = 0;

    //public:
    //    ArrayInterface() : m_array(nullptr), m_size(0) {};

    //    ArrayInterface(const T* const array, const size_t size) :
    //        m_array(array), m_size(size) {
    //    };

    //    const T& operator[](const size_t index) const {
    //        return m_array[index];
    //    }

    //    const T* const data() const { return m_array; };
    //    size_t size() const { return m_size; };
    //};

    //template<typename StateNew, typename StatePrior, typename Converter, size_t N>
    //static std::array<StateNew, N> convert(
    //    std::array<StatePrior, N> const& prior,
    //    Converter&& converter)
    //{
    //    std::array<StateNew, N> current;
    //    for (int i = 0; i < N; i++) {
    //        current[i] = converter(prior[i]);
    //    }
    //    return current;
    //}

    //template<typename StateNew, typename StatePrior, typename Converter>
    //static std::vector<StateNew> convert(
    //    const std::vector<StatePrior>& prior,
    //    Converter&& converter)
    //{
    //    std::vector<StateNew> current(prior.size());
    //    for (int i = 0; i < prior.size(); i++) {
    //        current[i] = converter(prior[i]);
    //    }
    //    return current;
    //}

    struct alignas(4) DrawCommand
    {
        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t firstInstance;
    };

    struct Extent3D : public StructBase<VkExtent3D, Extent3D>
    {
        using Base = StructBase<VkExtent3D, Extent3D>;
    public:
        using Base::Base;

        using Base::operator VkExtent3D&;
        using Base::operator const VkExtent3D&;

        Extent3D(uint32_t w = 800, uint32_t h = 600, uint32_t d = 1) : Base() {
            this->width = w;
            this->height = h;
			this->depth = d;
        }

        Extent3D(const Extent2D& extent, uint32_t d = 1) : Base() {
            this->width = extent.getWidth();
            this->height = extent.getHeight();
            this->depth = d;
        }


        operator const glm::uvec3& () const { return *reinterpret_cast<const glm::uvec3*>(this); };
        operator glm::uvec3& () { return *reinterpret_cast<glm::uvec3*>(this); };

        Extent3D& setWidth(uint32_t w) {
            this->width = w;
            return *this;
        }

        Extent3D& setHeight(uint32_t h) {
            this->height = h;
            return *this;
		}

        Extent3D& setDepth(uint32_t d) {
            this->depth = d;
            return *this;
		}

		uint32_t getWidth() const { return this->width; };
		uint32_t getHeight() const { return this->height; };
		uint32_t getDepth() const { return this->depth; };
    };

    struct Offset3D : public StructBase<VkOffset3D, Offset3D>
    {
        using Base = StructBase<VkOffset3D, Offset3D>;
    public:
        using Base::Base;

        Offset3D(int32_t x = 0, int32_t y = 0, int32_t z = 0) : Base() {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        operator const glm::ivec3& () const { return *reinterpret_cast<const glm::ivec3*>(this); };
        operator glm::ivec3& () { return *reinterpret_cast<glm::ivec3*>(this); };

        Offset3D& setX(int32_t x) {
            this->x = x;
            return *this;
        }

        Offset3D& setY(int32_t y) {
            this->y = y;
            return *this;
        }

        Offset3D& setZ(int32_t z) {
            this->z = z;
            return *this;
        }

        int32_t getX() const { return this->x; };
        int32_t getY() const { return this->y; };
        int32_t getZ() const { return this->z; };
    };

    struct Offset2D : public StructBase<VkOffset2D, Offset2D>
    {
        using Base = StructBase<VkOffset2D, Offset2D>;
    public:
        using Base::Base;

        Offset2D(int32_t x = 0, int32_t y = 0) : Base() {
            this->x = x;
            this->y = y;
        }

        operator const glm::ivec2& () const { return *reinterpret_cast<const glm::ivec2*>(this); };
        operator glm::ivec2& () { return *reinterpret_cast<glm::ivec2*>(this); };

        Offset2D& setX(int32_t x) {
            this->x = x;
            return *this;
        }

        Offset2D& setY(int32_t y) {
            this->y = y;
            return *this;
        }

        int32_t getX() const { return this->x; };
        int32_t getY() const { return this->y; };
    };

    struct Viewport : public StructBase<VkViewport, Viewport>
    {
        using Base = StructBase<VkViewport, Viewport>;
    public:
        using Base::Base;
       
        Viewport(float x, float y, float width, float height,
            float minDepth, float maxDepth) : Base() {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->minDepth = minDepth;
            this->maxDepth = maxDepth;
        }

        int32_t getX() const { return this->x; };
        int32_t getY() const { return this->y; };
        int32_t getWidth() const { return this->width; };
        int32_t getHeight() const { return this->height; };
        int32_t getMinDepth() const { return this->minDepth; };
        int32_t getMaxDepth() const { return this->maxDepth; };

        Viewport& setX(float x) { this->x = x; return *this; };
        Viewport& setY(float y) { this->y = y; return *this; };
        Viewport& setWidth(float width) { this->width = width; return *this; };
        Viewport& setHeight(float height) { this->height = height; return *this; };
        Viewport& setMinDepth(float minDepth) { this->minDepth = minDepth; return *this; };
        Viewport& setMaxDepth(float maxDepth) { this->maxDepth = maxDepth; return *this; };
    };

    struct Rect2D : public StructBase<VkRect2D, Rect2D>
    {
        using Base = StructBase<VkRect2D, Rect2D>;
    public:
        using Base::Base;

        Rect2D(const Offset2D& offset, const Extent2D& extent) : Base() {
            this->offset.x = offset.getX();
            this->offset.y = offset.getY();
            this->extent.width = extent.getWidth();
            this->extent.height = extent.getHeight();
        };

    };

    struct RenderRegion {
    private:
        Viewport viewport;
        Rect2D scissor;

    public:
        RenderRegion() = default;
        ~RenderRegion() = default;

        RenderRegion(const RenderRegion&) noexcept = default;
        RenderRegion(RenderRegion&&) noexcept = default;

        RenderRegion& operator=(const RenderRegion&) = default;
        RenderRegion& operator=(RenderRegion&&) noexcept = default;

        // Create a full-window render region
        static RenderRegion createFullWindow(const Extent2D& extent) {
            RenderRegion region{};
            region.viewport = Viewport(
                0.0f,                                       // x
                0.0f,                                       // y
                static_cast<float>(extent.getWidth()),      // width
                static_cast<float>(extent.getHeight()),     // height
                0.0f,                                       // minDepth
                1.0f                                        // maxDepth
            );

            region.scissor = Rect2D(
                Offset2D(0, 0), extent);

            return region;
        }

        // Create a custom region within the window
        static RenderRegion createCustom(
            float x, float y,           //region left top corner
            float width, float height   //region dimensions
        )
        {
            RenderRegion region{};
            region.viewport = Viewport(x, y, width, height,
                0.0f, 1.0f);
            region.scissor = Rect2D(
                Offset2D(static_cast<int32_t>(x), static_cast<int32_t>(y)),
                Extent2D(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
            return region;
        }

        bool isWithinBounds(const Extent2D& extent) const {
            return (viewport.getX() >= 0.0f &&
                viewport.getY() >= 0.0f &&
                viewport.getX() + viewport.getWidth() <= static_cast<float>(extent.getWidth()) &&
                viewport.getY() + viewport.getHeight() <= static_cast<float>(extent.getHeight()));
        }
    };

	struct PushConstantRange : public StructBase<VkPushConstantRange, PushConstantRange>
    {
        using Base = StructBase<VkPushConstantRange, PushConstantRange>;
    public:
        using Base::Base;

        PushConstantRange(ShaderStage::Flags stage, uint32_t offset, uint32_t size) : Base() {
            this->stageFlags = stage;
            this->offset = offset;
            this->size = size;
        }

        PushConstantRange& setStageFlags(ShaderStage::Flags stage) {
            this->stageFlags = stage;
            return *this;
		}

        PushConstantRange& setOffset(uint32_t offset) {
            this->offset = offset;
            return *this;
        }

        PushConstantRange& setSize(uint32_t size) {
            this->size = size;
            return *this;
        }

		ShaderStage::Flags getStageFlags() const { return this->stageFlags; };
		uint32_t getOffset() const { return this->offset; };
		uint32_t getSize() const { return this->size; };
    };

    static inline void verify(bool condition,
        const std::string& message, const char* file, int line, const char* func) {
#ifdef _DEBUG
        if (!condition) {
            std::string formatted = "[VERIFY FAILED] " + message +
                "\n  File: " + file +
                "\n  Line: " + std::to_string(line) +
                "\n  Function: " + func;
            std::cerr << formatted << std::endl;
            throw std::runtime_error(formatted);
        }
#else
        (void)condition; (void)message; (void)file; (void)line; (void)func;
#endif
    }

    class MemoryRequirements : public StructBase<VkMemoryRequirements, MemoryRequirements>
    {
        using Base = StructBase<VkMemoryRequirements, MemoryRequirements>;
    public:
        using Base::Base;
        size_t getSize() const { return static_cast<size_t>(this->size); }
        size_t getAlignment() const { return static_cast<size_t>(this->alignment); }
        uint32_t getMemoryTypeBits() const { return this->memoryTypeBits; }
    };

    static inline const std::unordered_map<VkResult, const std::string> s_resultMessages = {
    {VK_SUCCESS, "Success"},
    {VK_NOT_READY, "A fence or query has not yet completed"},
    {VK_TIMEOUT, "A wait operation has not completed in the specified time"},
    {VK_EVENT_SET, "An event is signaled"},
    {VK_EVENT_RESET, "An event is unsignaled"},
    {VK_INCOMPLETE, "A return array was too small for the result"},

    {VK_ERROR_OUT_OF_HOST_MEMORY, "Out of host memory"},
    {VK_ERROR_OUT_OF_DEVICE_MEMORY, "Out of device memory"},
    {VK_ERROR_INITIALIZATION_FAILED, "Initialization failed"},
    {VK_ERROR_DEVICE_LOST, "The logical or physical device has been lost"},
    {VK_ERROR_MEMORY_MAP_FAILED, "Mapping of a memory object has failed"},
    {VK_ERROR_LAYER_NOT_PRESENT, "A requested layer is not present or could not be loaded"},
    {VK_ERROR_EXTENSION_NOT_PRESENT, "A requested extension is not supported"},
    {VK_ERROR_FEATURE_NOT_PRESENT, "A requested feature is not supported"},
    {VK_ERROR_INCOMPATIBLE_DRIVER, "The requested version of Vulkan is not supported by the driver"},
    {VK_ERROR_TOO_MANY_OBJECTS, "Too many objects of the type have already been created"},
    {VK_ERROR_FORMAT_NOT_SUPPORTED, "A requested format is not supported on this device"},
    {VK_ERROR_FRAGMENTED_POOL, "A pool allocation has failed due to fragmentation"},
    {VK_ERROR_UNKNOWN, "An unknown error has occurred"},

    {VK_ERROR_OUT_OF_POOL_MEMORY, "Out of pool memory"},
    {VK_ERROR_INVALID_EXTERNAL_HANDLE, "Invalid external handle"},
    {VK_ERROR_FRAGMENTATION, "Fragmentation error"},
    {VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS, "Invalid opaque capture address"},
    {VK_PIPELINE_COMPILE_REQUIRED, "Pipeline compile required"},
    {VK_ERROR_NOT_PERMITTED, "Operation not permitted"},

    {VK_ERROR_SURFACE_LOST_KHR, "Surface lost"},
    {VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "Native window in use"},
    {VK_SUBOPTIMAL_KHR, "Suboptimal swapchain"},
    {VK_ERROR_OUT_OF_DATE_KHR, "Swapchain out of date"},
    {VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "Incompatible display"},
    {VK_ERROR_VALIDATION_FAILED_EXT, "Validation failed"},
    {VK_ERROR_INVALID_SHADER_NV, "Invalid shader"},

    {VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR, "Image usage not supported"},
    {VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR, "Video picture layout not supported"},
    {VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR, "Video profile operation not supported"},
    {VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR, "Video profile format not supported"},
    {VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR, "Video profile codec not supported"},
    {VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR, "Video standard version not supported"},

    {VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, "Invalid DRM format modifier plane layout"},
    {VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, "Full screen exclusive mode lost"},

    {VK_THREAD_IDLE_KHR, "Thread idle"},
    {VK_THREAD_DONE_KHR, "Thread done"},
    {VK_OPERATION_DEFERRED_KHR, "Operation deferred"},
    {VK_OPERATION_NOT_DEFERRED_KHR, "Operation not deferred"},

    {VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR, "Invalid video standard parameters"},
    {VK_ERROR_COMPRESSION_EXHAUSTED_EXT, "Compression exhausted"},

    {VK_INCOMPATIBLE_SHADER_BINARY_EXT, "Incompatible shader binary"},
    {VK_PIPELINE_BINARY_MISSING_KHR, "Pipeline binary missing"},
    {VK_ERROR_NOT_ENOUGH_SPACE_KHR, "Not enough space"},

    // Aliases - for completeness
    {VK_ERROR_OUT_OF_POOL_MEMORY_KHR, "Out of pool memory (KHR alias)"},
    {VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR, "Invalid external handle (KHR alias)"},
    {VK_ERROR_FRAGMENTATION_EXT, "Fragmentation error (EXT alias)"},
    {VK_ERROR_NOT_PERMITTED_EXT, "Operation not permitted (EXT alias)"},
    {VK_ERROR_NOT_PERMITTED_KHR, "Operation not permitted (KHR alias)"},
    {VK_ERROR_INVALID_DEVICE_ADDRESS_EXT, "Invalid device address (EXT alias)"},
    {VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR, "Invalid opaque capture address (KHR alias)"},
    {VK_PIPELINE_COMPILE_REQUIRED_EXT, "Pipeline compile required (EXT alias)"},
    {VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT, "Pipeline compile required error (EXT alias)"},
    {VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT, "Incompatible shader binary (EXT alias)"}
    };

    template<typename T, typename... Types>
    struct IndexFromVariadicPack
    {
    private:
        template<typename T, typename... Types>
        struct IndexFromVariadicPackImpl;

        template<typename T, typename First, typename... Rest>
        struct IndexFromVariadicPackImpl<T, First, Rest...> {
            static inline constexpr size_t index = std::is_same_v<T, First> ? 0 : 1 + IndexFromVariadicPackImpl<T, Rest...>::index;
        };

        template<typename T>
        struct IndexFromVariadicPackImpl<T> {
            static inline constexpr size_t index = 0;
        };
    public:
        static inline constexpr size_t index = IndexFromVariadicPackImpl<T, Types...>::index;
        static_assert(index < sizeof...(Types), "Type index not found in variadic pack");
    };

    template<StructureType... structs>
    struct StructChain {
    private:
        std::tuple<EnumToStructTraits_ct<structs>...> m_structs;

        void link() {
            if constexpr (sizeof...(structs) > 1) {
                [this] <size_t... I>(std::index_sequence<I...>) {
                    ((std::get<I>(m_structs).pNext = &std::get<I + 1>(m_structs)), ...);
                }(std::make_index_sequence<sizeof...(structs) - 1>{});
            }
        }

    public:

        StructChain() {
			std::memset(&m_structs, 0, sizeof(m_structs));
            link();
        }

        ~StructChain() = default;

        StructChain(StructChain&& other) {
            m_structs = std::move(other.m_structs);
            link();
        }

        StructChain& operator=(StructChain&& other) {
            if (this == &other)
                return *this;

            m_structs = std::move(other.m_structs);
            link();
            return *this;
        }

        StructChain(const StructChain& other) {
            m_structs = other.m_structs;
            link();
        }

        StructChain& operator=(const StructChain& other) {
            if (this == &other)
                return *this;

            m_structs = other.m_structs;
            link();
            return *this;
        }

        template<size_t index>
        inline auto& get() {
            return std::get<index>(m_structs);
        }

        template<StructureType structure>
        inline auto& get() {
            return std::get<
                IndexFromVariadicPack<StructTraits_t<structure>, StructTraits_t<structs>...>::index
           >(m_structs);
        }

        template<size_t index>
        inline const auto& get() const {
            return std::get<index>(m_structs);
        }

        template<StructureType structure>
        inline const auto& get() const {
            return std::get<
                IndexFromVariadicPack<StructTraits_t<structure>, StructTraits_t<structs>...>::index
            >(m_structs);
        }

        inline auto& getHead() {
            return std::get<0>(m_structs);
        }
    };




} // namespace Graphics