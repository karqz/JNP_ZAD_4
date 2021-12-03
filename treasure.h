#ifndef __TREASURE__
#define __TREASURE__

#include<concepts>
#include<type_traits>

template<typename T>
concept ValueType = std::integral<T>;

template<ValueType T, bool IsTrapped>
class Treasure {};

template<ValueType T>
class Treasure<T, true> {
    public:
        using isTrapped = std::integral_constant<bool,true>;
        
        constexpr Treasure(T value):value(value){};

        constexpr T evaluate() const {
            return value;
        };

        constexpr T getLoot() {
            T help = value;
            value = 0;
            return help;
        };

    private:
        const T value;
};

template<ValueType T>
class Treasure<T, false> {
    public:
        using isTrapped = std::integral_constant<bool, false>;
        
        constexpr Treasure(T value):value(value){};

        constexpr T evaluate() const {
            return value;
        };

        constexpr T getLoot() {
            T help = value;
            value = 0;
            return help;
        };

    private:
        T value;
};

template<ValueType T>
class SafeTreasure : public Treasure<T, false>{};

template<ValueType T>
class TrappedTreasure : public Treasure<T, true>{};

#endif //__TREASURE__