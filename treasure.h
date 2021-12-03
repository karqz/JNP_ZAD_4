#ifndef __TREASURE__
#define __TREASURE__

#include<concepts>
#include<type_traits>

template<typename T>
concept ValueType = std::integral<T>;

template<typename ValueType, bool IsTrapped>
class Treasure {};

template<typename ValueType>
class Treasure<ValueType, true> {
    public:
        using isTrapped = std::integral_constant<bool,true>;
        
        constexpr Treasure(ValueType value):value(value){};

        constexpr ValueType evaluate() const {
            return value;
        };

        constexpr ValueType getLoot() {
            ValueType help = value;
            value = 0;
            return help;
        };

    private:
        ValueType value;
};

template<typename ValueType>
class Treasure<ValueType, false> {
    public:
        using isTrapped = std::integral_constant<bool,false>;

        constexpr Treasure(ValueType value):value(value){};

        constexpr ValueType evaluate() const {
            return value;
        };

        constexpr ValueType getLoot() {
            ValueType help = value;
            value = 0;
            return help;
        };
        
    private:
        const ValueType value;
};

template<typename ValueType>
class SafeTreasure : public Treasure<ValueType, true>{};

template<typename ValueType>
class TrappedTreasure : public Treasure<ValueType, false>{};

#endif //__TREASURE__