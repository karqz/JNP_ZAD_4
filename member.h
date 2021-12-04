#ifndef __MEMBER__
#define __MEMBER__

#include "treasure.h"
#include <cstdint>

namespace {

    constexpr uint32_t fibonacci(uint32_t n) {
        if (n == 0)
            return 0;
        if (n == 1)
            return 1;

        uint32_t fib[n + 1];
        fib[0] = 0;
        fib[1] = 1;
        for (std::size_t i = 2; i <= n; i++)
            fib[i] = fib[i - 1] + fib[i - 2];

        return fib[n];
    }

} // anonymous namespace

// Adventurer
template<ValueType T, bool IsArmed>
class Adventurer {};

template<ValueType T>
class Adventurer<T, true> {
public:
    using strength_t = uint32_t;

    constexpr static bool isArmed = true;

    constexpr Adventurer(strength_t strength):strength(strength),value(0) {};

    constexpr strength_t getStrength() const {
        return strength;
    }

    template<bool IsTrapped>
    constexpr void loot(Treasure<T, IsTrapped> &&treasure) {
        if (!IsTrapped) {
            value += treasure.getLoot();
        }
        else if (strength > 0) {
            value += treasure.getLoot();
            strength /= 2;
        }
    }

    constexpr T pay() {
        T valuables = value;
        value = 0;
        return valuables;
    }

private:
    strength_t strength;
    T value;
};

template<ValueType T>
class Adventurer<T, false> {
public:
    using strength_t = uint32_t;

    constexpr static bool isArmed = false;

    constexpr Adventurer():value(0) {};

    template<bool IsTrapped>
    constexpr void loot(Treasure<T, IsTrapped> &&treasure) {
        if constexpr (!IsTrapped) {
            value += treasure.getLoot();
        }
    }

    constexpr T pay() {
        T valuables = value;
        value = 0;
        return valuables;
    };

private:
    T value;
};
// Adventurer

template<ValueType T>
using Explorer = Adventurer<T, false>;

// Veteran
template<ValueType T, std::size_t CompletedExpeditions>
requires (CompletedExpeditions < 25)
class Veteran {
public:
    using strength_t = uint32_t;

    constexpr static bool isArmed = true;

    constexpr Veteran():strength(fibonacci(CompletedExpeditions)),value(0) {};

    constexpr strength_t getStrength() const {
        return strength;
    }

    template<bool IsTrapped>
    constexpr void loot(Treasure<T, IsTrapped> &&treasure) {
        if ((IsTrapped && strength > 0) || !IsTrapped) {
            value += treasure.getLoot();
        }
    }

    constexpr T pay() {
        T valuables = value;
        value = 0;
        return valuables;
    };

private:
    strength_t strength;
    T value;
};
// Veteran

#endif //__MEMBER__
