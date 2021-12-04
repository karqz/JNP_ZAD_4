#ifndef __MEMBER__
#define __MEMBER__

#include "treasure.h"
#include <cstdint>

template<std::size_t v>
concept CompletedExpeditions = v < 25;

constexpr uint32_t fibonacci(uint32_t n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

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
            T to_pay = value;
            value = 0;
            return to_pay;
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
            if (!IsTrapped) {
                value += treasure.getLoot();
            }
        }

        constexpr T pay() {
            T to_pay = value;
            value = 0;
            return to_pay;
        };

    private:
        T value;
};

template<ValueType T>
using Explorer = Adventurer<T, false>;


template<ValueType T, std::size_t CompletedExpeditions>
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
            if (strength > 0) {
                value += treasure.getLoot();
            }
        }

        constexpr T pay() {
            T to_pay = value;
            value = 0;
            return to_pay;
        };

    private:
        strength_t strength;
        T value;
};

#endif //__MEMBER__