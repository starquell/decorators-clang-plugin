/// Some macro routine here, because decorate attribute is not implemented directly temporary,
/// it works via clang::annotate("decorate", ...)
#define decorate1(a) clang::annotate("decorate", #a)
#define decorate2(a, b)  clang::annotate("decorate", #a, #b)
#define decorate3(a, b, c)  clang::annotate("decorate", #a, #b, #c)

#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
#define decorate(...) GET_MACRO(__VA_ARGS__, decorate4, decorate3, decorate2, decorate1)(__VA_ARGS__)

#define return_if_needed(call) \
        if constexpr (std::is_same_v<decltype(call), void>) { \
            call; \
        } \
        else { \
            return call;  \
        }

#include <utility>
