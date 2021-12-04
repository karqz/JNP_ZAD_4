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
class SafeTreasure : public Treasure<T, false>{};

template<ValueType T>
class TrappedTreasure : public Treasure<T, true>{};

#endif //__TREASURE__