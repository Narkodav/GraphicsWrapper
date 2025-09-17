#pragma once
#include <array>

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace Graphics {

    template<typename Func>
    struct FunctionTypeTraits : std::false_type {};

    template<typename Ret, typename... Params>
    struct FunctionTypeTraits<Ret(Params...)> {
        using ReturnType = Ret;
        static constexpr size_t s_numParams = sizeof...(Params);

        template<size_t index>
        using ParamType = std::tuple_element_t<index, std::tuple<Params...>>;
        using ParamTypes = std::tuple<Params...>;

        static constexpr bool s_isVoid = std::is_void_v<Ret>;
        static constexpr bool s_hasParams = s_numParams > 0;

        template<typename T>
        static constexpr bool s_hasParamType = (std::is_same_v<T, Params> || ...);

        template<size_t N>
        static constexpr bool s_hasNParams = (sizeof...(Params) == N);
    };

    template<typename Ret, typename... Params>
    struct FunctionTypeTraits<Ret(*)(Params...)> : FunctionTypeTraits<Ret(Params...)> {};

    template<typename Ret, typename... Params>
    struct FunctionTypeTraits<Ret(&)(Params...)> : FunctionTypeTraits<Ret(Params...)> {};

    template<typename Class, typename Ret, typename... Params>
    struct FunctionTypeTraits<Ret(Class::*)(Params...)> : FunctionTypeTraits<Ret(Params...)> {
        using ClassType = Class;
    };

    template<typename Func, typename... Args>
    struct IsInvocableWithConvertibleArgs : std::false_type {};

    template<typename Ret, typename... Params, typename... Args>
    struct IsInvocableWithConvertibleArgs<Ret(Params...), Args...>
        : std::conjunction<
        std::bool_constant<sizeof...(Params) == sizeof...(Args)>,
        std::is_convertible<Args, Params>...
        > {
    };

    template<typename Ret, typename... Params, typename... Args>
    struct IsInvocableWithConvertibleArgs<Ret(*)(Params...), Args...>
        : std::conjunction<
        std::bool_constant<sizeof...(Params) == sizeof...(Args)>,
        std::is_convertible<Args, Params>...
        > {
    };

    template<typename Func, typename... Args>
    static inline const bool IsInvocableWithConvertibleArgs_v = IsInvocableWithConvertibleArgs<Func, Args...>::value;

    template<typename FunctionEnum, template <FunctionEnum> class FunctionTraits, 
    typename PassedType, typename GetProcAddrType>
    class FunctionTable
    {
    private:

        std::array<PFN_vkVoidFunction, static_cast<size_t>(FunctionEnum::Num)> m_functions = { nullptr };
        GetProcAddrType m_getProcAddr;

    public:

        FunctionTable() = default;
        FunctionTable(GetProcAddrType getProcAddr) : m_getProcAddr(getProcAddr) {};

        FunctionTable(const FunctionTable&) = delete;
        FunctionTable(FunctionTable&&) = default;

        FunctionTable& operator=(const FunctionTable&) = delete;
        FunctionTable& operator=(FunctionTable&&) = default;

        void loadAllFunctions(PassedType obj) {
            [this, &obj] <std::size_t... Is>(std::index_sequence<Is...>) {
                ((loadFunction<static_cast<FunctionEnum>(Is)>(obj)), ...);
            }(std::make_index_sequence<static_cast<size_t>(FunctionEnum::Num)>{});            
        }

        template<FunctionEnum... funcs>
        void loadFunctions(PassedType obj) {
            (loadFunction<funcs>(obj), ...);
        }

        template<FunctionEnum func>
        void loadFunction(PassedType obj) {
            m_functions[static_cast<size_t>(func)] = m_getProcAddr(obj, FunctionTraits<func>::name);
        }

        template<FunctionEnum func, typename... Args>
        typename FunctionTypeTraits<typename FunctionTraits<func>::Type>::ReturnType
            execute(Args&&... args) const {
            static_assert(IsInvocableWithConvertibleArgs_v<typename FunctionTraits<func>::Type, Args...>,
                "Arguments are not convertible to function parameter types");
            auto funcPtr = get<func>();
#ifdef _DEBUG
            if (!funcPtr) {
                throw std::runtime_error(std::string("Vulkan function not loaded: ") +
                    FunctionTraits<func>::name);
            }
#endif
            return funcPtr(std::forward<Args>(args)...);
        }

        template<FunctionEnum func>
        typename FunctionTraits<func>::Type get() const {
            return reinterpret_cast<typename FunctionTraits<func>::Type>(
                m_functions[static_cast<size_t>(func)]);
        }

        template<FunctionEnum func>
        bool isLoaded() const {
            return m_functions[static_cast<size_t>(func)] != nullptr;
        }

        template<FunctionEnum func>
        static constexpr const char* getName() {
            return FunctionTraits<func>::name;
        }
    };
}