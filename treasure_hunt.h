#ifndef __TREASURE_HUNT__
#define __TREASURE_HUNT__

#include "treasure.h"

#include<numeric>
#include<vector>
#include<iostream>
#include<concepts>

template <typename T>
concept treasure_c = requires(T treasure) {
    { Treasure{treasure} } -> std::same_as<T>;
};

template <typename T>
concept member_c = requires(T member) {
    T::strength_t;
    { [] () constexpr { return T::isArmed; }() };
    { member.isArmed } -> std::convertible_to<bool>;
    { member.pay() } -> std::integral;
    member.loot(Treasure<decltype(member.pay()), true)>());
    member.loot(Treasure<decltype(member.pay()), false>());
};

template <typename T>
concept EncounterSide = treasure_c || member_c;

template <EncounterSide sideA, EncounterSide sideB>
struct Encounter {
    sideA &side_a;
    sideB &side_b;
};

template <EncounterSide A, EncounterSide B>
concept encounter_c =
        (treasure_c<A> && member_c<B>) ||
        (member_c<A> && treasure_c<B>) ||
        (member_c<A> && member_c<B>);

// zrobic funkcje pomcniczą na przekazywanie pieniążków
template <member_c sideA, member_c sideB>
void run(Encounter<sideA, sideB> encounter) {
    sideA memberA = encounter.side_a;
    sideB memberB = encounter.side_b;
    if (memberA.isArmed && memberB.isArmed) {
        if (memberA.getStrength() > memberB.getStrength) {
            auto money = memberB.pay();
            memberA.loot(SafeTreasure<decltype(money)> treasure(money));
        }
        else if (memberB.getStrength() > memberA.gerStrength()) {
            auto money = memberA.pay();
            memberB.loot(SafeTreasure<decltype(money)> treasure(money));
        }
    }
    else if (memberA.isArmed && !memberB.isArmed) {
        auto money = memberB.pay();
        memberA.loot(SafeTreasure<decltype(money)> treasure(money));
    }
    else if (!memberA.isArmed && memberB.isArmed) {
        auto money = memberA.pay();
        memberB.loot(SafeTreasure<decltype(money)> treasure(money));
    }
}

template <member_c sideA, treasure_c sideB>
void run(Encounter<sideA, sideB> encounter) {
    sideA member = encounter.side_a;
    member.loot(encounter.side_b);
}

template <treasure_c sideA, member_c sideB>
void run(Encounter<sideA, sideB> encounter) {
    sideB member = encounter.side_b;
    member.loot(encounter.side_a);
}

template <encounter_c... E>
void expedition(E... e) {
    (... , run(e));
}

#endif //__TREASURE_HUNT__
