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
#include <span>


#include "StructTraits.h"

#define GRAPHICS_API_ATTR VKAPI_ATTR
#define GRAPHICS_API_CALL VKAPI_CALL
#define GRAPHICS_API_PTR  VKAPI_PTR
#define GRAPHICS_VERIFY(condition, message) Graphics::verify(condition, message, __FILE__, __LINE__, __FUNCTION__)

#ifdef GRAPHICS_NO_VERIFY && GRAPHICS_ALWAYS_VERIFY
#error "GRAPHICS_NO_VERIFY and GRAPHICS_ALWAYS_VERIFY cannot be defined at the same time
#endif

namespace Graphics {

    using DeviceSize_t = VkDeviceSize;

    static inline void verify(bool condition,
        const std::string& message, const char* file, int line, const char* func) {
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
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

	using SampleMask = VkSampleMask;

    template<typename T>
    concept StructHasNext = requires(T t) { t.pNext; };

    template<typename T>
    concept StructHasSType = requires(T t) { t.sType; };

    template<typename T, typename Derived>
    class StructBase : public T {
    public:
        using T::T;

        StructBase() : T() {
            std::memset(static_cast<T*>(this), 0, sizeof(T));
            if constexpr (StructHasSType<T>) {
                T::sType = static_cast<typename std::remove_const<decltype(T::sType)>::type>(StructToEnumTraits_v<T>);
            }
            //if constexpr (StructHasNext<T>) {
            //    T::pNext = nullptr;
            //}
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

    template<typename T, typename... Ts>
    struct TupleHasTypeTrait {};

    template<typename T, typename First, typename... Ts>
    struct TupleHasTypeTrait<T, First, Ts...> {
        static constexpr bool value = std::is_same_v<T, First> || TupleHasTypeTrait<T, Ts...>::value;
    };

    template<typename T, typename Last>
    struct TupleHasTypeTrait<T, Last> {
        static constexpr bool value = std::is_same_v<T, Last> || false;
    };

    template<typename T, typename... Ts>
    static constexpr bool TupleHasTypeTrait_v = TupleHasTypeTrait<T, Ts...>::value;

    template<typename T, typename Derived, typename... Ts>
    class UnionBase {
    protected:
        T m_data;

    public:

        UnionBase() = default;
        ~UnionBase() = default;
        UnionBase(const T& other) : m_data(other.m_data) {}
        UnionBase(T&& other) : m_data(std::move(other.m_data)) {}
        UnionBase(const UnionBase&) = default;
        UnionBase(UnionBase&&) = default;

        UnionBase& operator=(const UnionBase&) = default;
        UnionBase& operator=(UnionBase&&) = default;

        UnionBase& operator=(const T& other) { m_data = other; return *this; };
        UnionBase& operator=(T&& other) { m_data = std::exchange(other, T()); return *this; };

        operator T& () { return m_data; }
        operator const T& () const { return m_data; }
        const T* getUnderlyingPointer() const { return &m_data; };
        T* getUnderlyingPointer() { return &m_data; };
        static const T* underlyingCast(const UnionBase* ptr) { return reinterpret_cast<const T*>(ptr); };
        static T* underlyingCast(UnionBase* ptr) { return reinterpret_cast<T*>(ptr); };

        const T& getUnion() const { return *this; };
        T& getUnion() { return *this; };

        static const Derived* underlyingCast(const T* ptr) { return reinterpret_cast<const Derived*>(ptr); };
        static Derived* underlyingCast(T* ptr) { return reinterpret_cast<Derived*>(ptr); };

        template<typename U>
        const U& get() const { 
            static_assert(TupleHasTypeTrait_v<U, Ts...>, "Type not found in union");
            return *reinterpret_cast<const U*>(&m_data); 
        };

        template<typename U>
        U& get() {
            static_assert(TupleHasTypeTrait_v<U, Ts...>, "Type not found in union");
            return *reinterpret_cast<U*>(&m_data);
        };
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

    template <typename T>
    concept RefType = requires {
        T::s_typeName;
        requires std::is_same_v<decltype(T::s_typeName), const std::string> ||
            std::is_same_v<decltype(T::s_typeName), const char*>;
    };

    template<typename T, RefType Ref>
    class VerificatorComponent : public Ref {
    public:
        VerificatorComponent() { this->setHandle(nullptr); };

        VerificatorComponent(VerificatorComponent&& other) {
            this->setHandle(other.getHandle());
            other.reset();
        }

        VerificatorComponent& operator=(VerificatorComponent&& other) {
            if (this == &other)
                return *this;
            GRAPHICS_VERIFY(!isValid(), std::string("Trying to move to valid object: ") + Ref::s_typeName);
            this->setHandle(other.getHandle());
            other.reset();
            return *this;
        }

        ~VerificatorComponent() {
            GRAPHICS_VERIFY(!isValid(), std::string("Valid object was not destroyed: ") + Ref::s_typeName);
        };

        VerificatorComponent(T&& other) {
            this->setHandle(std::move(other));
        };

        VerificatorComponent& operator=(T&& other) {
            if (this == &other)
                return *this;
            GRAPHICS_VERIFY(!isValid(), std::string("Trying to move to valid object: ") + Ref::s_typeName);
            this->setHandle(std::move(other));
            return *this;
        }

        VerificatorComponent(const VerificatorComponent&) = delete;
        VerificatorComponent& operator=(const VerificatorComponent&) = delete;
        VerificatorComponent(const T&) = delete;
        VerificatorComponent& operator=(const T&) = delete;

        bool isValid() const { return this->isSet(); };

        Ref getReference() const { return Ref(this->getHandle()); };
    };

    using bool32_t = VkBool32;

    

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

    template<typename T, typename... Types>
    static inline const auto IndexFromVariadicPack_v = IndexFromVariadicPack<T, Types...>::index;

    template<int32_t index, typename... Types>
    struct TypeFromVariadicPackIndex;

    template<int32_t index, typename First, typename... Rest>
    struct TypeFromVariadicPackIndex<index, First, Rest...> {
        using Type = std::conditional_t<(index == 0), First, typename TypeFromVariadicPackIndex<index - 1, Rest...>::Type>;
    };

    template<int32_t index>
    struct TypeFromVariadicPackIndex<index> {
        using Type = void;
        static_assert(index < 0, "Index out of bounds");
    };

    template<int32_t index, typename... Types>
    using TypeFromVariadicPackIndex_t = typename TypeFromVariadicPackIndex<index, Types...>::Type;

    template<StructureType... structs>
    struct StructChain {
    private:
        std::tuple<EnumToStructTraits_ct<structs>...> m_structs;

        void link() {
            if constexpr (sizeof...(structs) > 1) {
                [this] <size_t... I>(std::index_sequence<I...>) {
                    ((std::get<I>(m_structs).pNext = &std::get<I + 1>(m_structs)), ...);
                    ((std::get<I>(m_structs).sType = static_cast<VkStructureType>(
                        StructToEnumTraits_v<TypeFromVariadicPackIndex_t<I, EnumToStructTraits_ct<structs>...>>
                    )), ...);
                }(std::make_index_sequence<sizeof...(structs) - 1>{});
            }
            std::get<sizeof...(structs) - 1>(m_structs).sType = static_cast<VkStructureType>(
                StructToEnumTraits_v<TypeFromVariadicPackIndex_t<sizeof...(structs) - 1, EnumToStructTraits_ct<structs>...>>);
        }

    public:
        using HeadType = std::tuple_element_t<0, std::tuple<EnumToStructTraits_ct<structs>...>>;

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
                IndexFromVariadicPack_v<EnumToStructTraits_ct<structure>, EnumToStructTraits_ct<structs>...>
           >(m_structs);
        }

        template<size_t index>
        inline const auto& get() const {
            return std::get<index>(m_structs);
        }

        template<StructureType structure>
        inline const auto& get() const {
            return std::get<
                IndexFromVariadicPack_v<EnumToStructTraits_ct<structure>, EnumToStructTraits_ct<structs>...>
            >(m_structs);
        }

        inline HeadType& getHead() {
            return std::get<0>(m_structs);
        }

        inline const HeadType& getHead() const {
            return std::get<0>(m_structs);
        }
    };




} // namespace Graphics