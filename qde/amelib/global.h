#ifndef __CONFIG_H
#define __CONFIG_H

#include <QtCore/QtGlobal>
#include <QtCore/QAtomicPointer>

#define HAVE_GCC_4

#ifdef HAVE_GCC_4
#define AME_NO_EXPORT __attribute__((visibility("hidden")))
#define AME_EXPORT __attribute__((visibility("default")))
#define AME_IMPORT __attribute__((visibility("default")))
#elif defined(_WIN32) || defined(_WIN64)
#define AME_NO_EXPORT
#define AME_EXPORT __declspec(dllexport)
#define AME_IMPORT __declspec(dllimport)
#else
#define AME_NO_EXPORT
#define AME_EXPORT
#define AME_IMPORT
#endif

#if QT_VERSION < 0x040400
#define Q_BASIC_ATOMIC_INITIALIZER Q_ATOMIC_INIT
#define testAndSetOrdered testAndSet
#endif

typedef void (*AmeCleanupFunction)();
class AmeCleanupStatic
{
public:
	AmeCleanupFunction func;
	inline ~AmeCleanupStatic() { func(); }
};

#ifdef Q_CC_MSVC
#define AME_GLOBAL_STATIC_STRUCT_NAME(NAME) _k_##NAME##__LINE__
#else
#define AME_GLOBAL_STATIC_STRUCT_NAME(NAME)
#endif

#define AME_GLOBAL_STATIC(TYPE, NAME) AME_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ())

#define AME_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                                                                   \
static QBasicAtomicPointer<TYPE> _k_static_##NAME = Q_BASIC_ATOMIC_INITIALIZER(0);                                      \
static bool _k_static_##NAME##_destroyed;                                                                               \
static struct AME_GLOBAL_STATIC_STRUCT_NAME(NAME)                                                                       \
{															\
        bool isDestroyed()												\
        {														\
                return _k_static_##NAME##_destroyed;                                                                    \
        }														\
        inline operator TYPE*()												\
        {														\
                return operator->();											\
        }														\
        inline TYPE *operator->()											\
        {														\
                if (!_k_static_##NAME) {										\
                        if (isDestroyed()) {										\
                                qFatal("Fatal error: Accessed global static '%s *%s()' after destruction. "             \
					"Defined at %s:%d", #TYPE, #NAME, __FILE__, __LINE__); 				\
                        }												\
                        TYPE *x = new TYPE ARGS;									\
                        if (!_k_static_##NAME.testAndSetOrdered(0,x) && _k_static_##NAME != x) {                        \
                                delete x;										\
                        } else {											\
				static AmeCleanupStatic cleanupObject = { destroy };					\
                        }												\
                }													\
                return _k_static_##NAME;										\
        }														\
        inline TYPE &operator*()											\
        {														\
                return *operator->();											\
        }														\
        static void destroy()												\
        {														\
                _k_static_##NAME##_destroyed = true;									\
                TYPE *x = _k_static_##NAME;										\
                _k_static_##NAME = 0;											\
                delete x;												\
        }														\
} NAME;


#endif /* __CONFIG_H */

