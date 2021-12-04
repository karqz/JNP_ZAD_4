#ifndef __TREASURE__
#define __TREASURE__

#include<concepts>
#include<type_traits>

template<typename T>
concept ValueType = std::is_integral<T>::value;

template<ValueType T, bool IsTrapped>
class Treasure {
public:
    constexpr static bool isTrapped = IsTrapped;

    constexpr Treasure(T value):value(value){};

    constexpr T evaluate() {
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
using SafeTreasure = Treasure<T, false>;

template<ValueType T>
using TrappedTreasure = Treasure<T, true>;

#endif //__TREASURE__
