#pragma once
#include "Enums.h"
#include "Flags.h"
#include "Wrappers/TaskTables/FeatureEnum.h"
#include "Wrappers/TaskTables/PropertyEnum.h"
#include "Wrappers/TaskTables/QueuePropertyEnum.h"
#include "Common.h"

namespace Graphics {
    struct Version
    {
        uint32_t version;

        operator uint32_t() const
        {
            return version;
        }

        operator const uint32_t& () const
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

        CopyRegion& setSrcOffset(size_t srcOffset) { this->srcOffset = srcOffset; return *this; }
        CopyRegion& setDstOffset(size_t dstOffset) { this->dstOffset = dstOffset; return *this; }
        CopyRegion& setSize(size_t size) { this->size = size; return *this; }
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

        static Color Empty() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
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

    using Scissor = Rect2D; //for better readability

    struct RenderRegion {
    private:
        Viewport viewport;
        Scissor scissor;

    public:
        RenderRegion() = default;
        ~RenderRegion() = default;

        RenderRegion(const RenderRegion&) noexcept = default;
        RenderRegion(RenderRegion&&) noexcept = default;

        RenderRegion& operator=(const RenderRegion&) = default;
        RenderRegion& operator=(RenderRegion&&) noexcept = default;

        Viewport getViewport() const { return viewport; }
        Rect2D getScissor() const { return scissor; }

        void setViewport(const Viewport& viewport) { this->viewport = viewport; }
        void setScissor(const Scissor& scissor) { this->scissor = scissor; }

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

    class MemoryRequirements : public StructBase<VkMemoryRequirements, MemoryRequirements>
    {
        using Base = StructBase<VkMemoryRequirements, MemoryRequirements>;
    public:
        using Base::Base;
        size_t getSize() const { return static_cast<size_t>(this->size); }
        size_t getAlignment() const { return static_cast<size_t>(this->alignment); }
        uint32_t getMemoryTypeBits() const { return this->memoryTypeBits; }
    };

    class PushConstantRange : public StructBase<VkPushConstantRange, PushConstantRange>
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

    class ClearColorValue : public UnionBase<VkClearColorValue, ClearColorValue,
        std::array<float, 4>, std::array<int32_t, 4>, std::array<uint32_t, 4>>
    {
        using Base = UnionBase<VkClearColorValue, ClearColorValue,
            std::array<float, 4>, std::array<int32_t, 4>, std::array<uint32_t, 4>>;
    public:
        using Base::Base;

        ClearColorValue& setClearValue(const std::array<float, 4>& value) {
            getUnion().float32[0] = value[0];
            getUnion().float32[1] = value[1];
            getUnion().float32[2] = value[2];
            getUnion().float32[3] = value[3];
            return *this;
        }

        ClearColorValue& setClearValue(const std::array<int32_t, 4>& value) {
            getUnion().int32[0] = value[0];
            getUnion().int32[1] = value[1];
            getUnion().int32[2] = value[2];
            getUnion().int32[3] = value[3];
            return *this;
        }

        ClearColorValue& setClearValue(const std::array<uint32_t, 4>& value) {
            getUnion().uint32[0] = value[0];
            getUnion().uint32[1] = value[1];
            getUnion().uint32[2] = value[2];
            getUnion().uint32[3] = value[3];
            return *this;
        }

        std::span<const float, 4> getFloat32() const { return getUnion().float32; }
        std::span<const int32_t, 4> getInt32() const { return getUnion().int32; }
        std::span<const uint32_t, 4> getUint32() const { return getUnion().uint32; }
    };

    class ClearDepthStencilValue : public StructBase<VkClearDepthStencilValue, ClearDepthStencilValue>
    {
        using Base = StructBase<VkClearDepthStencilValue, ClearDepthStencilValue>;
    public:
        using Base::Base;

        ClearDepthStencilValue& setDepth(float depth) {
            this->depth = depth;
            return *this;
        }

        ClearDepthStencilValue& setStencil(uint32_t stencil) {
            this->stencil = stencil;
            return *this;
        }

        float getDepth() const { return this->depth; }
        uint32_t getStencil() const { return this->stencil; }
    };

    class ClearValue : public UnionBase<VkClearValue, ClearValue,
        ClearColorValue, ClearDepthStencilValue>
    {
        using Base = UnionBase<VkClearValue, ClearValue,
            ClearColorValue, ClearDepthStencilValue>;
    public:
        using Base::Base;

        ClearValue& setClearValue(const ClearColorValue& color) {
            getUnion().color = color;
            return *this;
        }

        ClearValue& setClearValue(const ClearDepthStencilValue& depthStencil) {
            getUnion().depthStencil = depthStencil;
            return *this;
        }

        const ClearColorValue& getClearColor() const { return getUnion().color; }
        const ClearDepthStencilValue& getClearDepthStencil() const { return getUnion().depthStencil; }
    }
}