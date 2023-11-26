#pragma once

#include <string_view>
#include <hook64/And64InlineHook.hpp>
#include <dlfcn.h>
#include "logger.h"
#include "string_utils.h"

namespace hooks {

    constexpr FixedString LIBINPUT = "libinput.so";
    constexpr FixedString LIBINPUT_READER = "libinputreader.so";
    constexpr FixedString LIBINPUT_FLIENGER = "libinputflinger.so";
    constexpr FixedString LIBINPUT_FLIENGER_BASE = "libinputflinger_base.so";

    void setupFunctionHooks(void *moduleBase);
}


#define VA_EXPAND(...) __VA_ARGS__

template<FixedString Mod, FixedString Fn, typename ret, typename... p>
static auto __imp_Call() {
    static void *funcCache = nullptr;
    if (funcCache == nullptr) {
        logger::info("InputInject/Hooking", "creating dlsym cache");
        auto handle = dlopen(Mod, RTLD_NOW);
        if (handle == nullptr) {
            logger::info("InputInject/Hooking", "dlsym_cache handle not found %s", Mod.c_str());
        }
        funcCache = dlsym(handle, Fn);
        if (funcCache == nullptr) {
            logger::info("InputInject/Hooking", "dlsym_cache func not found %s: %s", Mod.c_str(), Fn.c_str());
        }
        logger::info("InputInject/Hooking", "dlsym_cache cached %s: %s at %p", Mod.c_str(), Fn.c_str(), funcCache);
    }
    return ((ret(*)(p...)) (funcCache));
}

#define SymCall(mod, fn, ret, ...) (__imp_Call<mod, fn, ret, __VA_ARGS__>())

class THookRegister {
public:
    THookRegister(void *address, void *hook, void **org) {
        A64HookFunction(reinterpret_cast<void *const>(address), reinterpret_cast<void *const>(hook),
                        reinterpret_cast<void **>(org));
    }

    THookRegister(const char *module, const char *sym, void *hook, void **org) {
        auto handle = dlopen(module, RTLD_NOW);
        auto func = dlsym(handle, sym);
        if (func == nullptr) {
            logger::info("InputInject/Hooking", "func not found %s: %s", module, sym);
        } else {
            A64HookFunction(reinterpret_cast<void *const>(func), reinterpret_cast<void *const>(hook),
                            reinterpret_cast<void **>(org));
            logger::info("InputInject/Hooking", "Hooked %s: %s", module, sym);
        }
    }

    template<typename T>
    THookRegister(const char *module, const char *sym, T hook, void **org) {
        union {
            T a;
            void *b;
        } hookUnion;
        hookUnion.a = hook;
        THookRegister(module, sym, hookUnion.b, org);
    }

    template<typename T>
    THookRegister(void *address, T hook, void **org) {
        union {
            T a;
            void *b;
        } hookUnion;
        hookUnion.a = hook;
        THookRegister(address, hookUnion.b, org);
    }
};

#define VA_EXPAND(...) __VA_ARGS__
template<uint64_t, uint64_t>
struct THookTemplate;
template<uint64_t, uint64_t>
extern THookRegister THookRegisterTemplate;

#define _TInstanceHook(class_inh, pclass, iname, mod, sym, ret, ...)                         \
    template <>                                                                              \
    struct THookTemplate<do_hash(iname), do_hash(mod)> class_inh {                           \
        typedef ret (THookTemplate::*original_type)(__VA_ARGS__);                            \
        static original_type& _original() {                                                  \
            static original_type storage;                                                    \
            return storage;                                                                  \
        }                                                                                    \
        template <typename... Params>                                                        \
        static ret original(pclass* _this, Params&&... params) {                             \
            return (((THookTemplate*)_this)->*_original())(std::forward<Params>(params)...); \
        }                                                                                    \
        ret _hook(__VA_ARGS__);                                                              \
    };                                                                                       \
    template <>                                                                              \
    static THookRegister THookRegisterTemplate<do_hash(iname), do_hash(mod)>{                \
        mod,sym, &THookTemplate<do_hash(iname), do_hash(mod)>::_hook,                        \
        (void**)&THookTemplate<do_hash(iname), do_hash(mod)>::_original()};                  \
    ret THookTemplate<do_hash(iname), do_hash(mod)>::_hook(__VA_ARGS__)

#define _TInstanceDefHook(iname, mod, sym, ret, type, ...) \
    _TInstanceHook(                                   \
        : public type, type, iname, mod, sym, ret, VA_EXPAND(__VA_ARGS__))
#define _TInstanceNoDefHook(iname, sym, ret, ...) \
    _TInstanceHook(, void, iname, sym, ret, VA_EXPAND(__VA_ARGS__))

#define THook2(iname, ret, mod, sym, ...) _TStaticNoDefHook(iname, mod, sym, ret, VA_EXPAND(__VA_ARGS__))
#define THook(ret, mod, sym, ...) THook2(sym, ret, mod, sym, VA_EXPAND(__VA_ARGS__))
#define TStaticHook2(iname, ret, mod, sym, type, ...) \
    _TStaticDefHook(iname, mod, sym, ret, type, VA_EXPAND(__VA_ARGS__))
#define TStaticHook(ret, sym, type, ...) TStaticHook2(sym, ret, sym, type, VA_EXPAND(__VA_ARGS__))
#define TInstanceHook2(iname, ret, mod, sym, type, ...) \
    _TInstanceDefHook(iname, mod, sym, ret, type, VA_EXPAND(__VA_ARGS__))
#define TInstanceHook(ret, mod, sym, type, ...) \
    TInstanceHook2(sym, ret, mod, sym, type, VA_EXPAND(__VA_ARGS__))
