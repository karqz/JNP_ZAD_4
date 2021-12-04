#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include "member.h"

#include <numeric>
#include <vector>
#include <iostream>
#include <concepts>

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
    member.loot(Treasure<decltype(member.pay()), true>());
    member.loot(Treasure<decltype(member.pay()), false>());
};

template <typename T>
concept EncounterSide = treasure_c<T> || member_c<T>;

template <EncounterSide sideA, EncounterSide sideB>
struct Encounter {
    sideA &side_a;
    sideB &side_b;
};

// zrobic funkcje pomcniczą na przekazywanie pieniążków
template <member_c sideA, member_c sideB>
void run(Encounter<sideA, sideB> encounter) {
    sideA memberA = encounter.side_a;
    sideB memberB = encounter.side_b;
    if (memberA.isArmed && memberB.isArmed) {
        if (memberA.getStrength() > memberB.getStrength) {
            auto money = memberB.pay();
            memberA.loot(SafeTreasure<decltype(money)>(money));
        }
        else if (memberB.getStrength() > memberA.getStrength()) {
            auto money = memberA.pay();
            memberB.loot(SafeTreasure<decltype(money)>(money));
        }
    }
    else if (memberA.isArmed && !memberB.isArmed) {
        auto money = memberB.pay();
        memberA.loot(SafeTreasure<decltype(money)>(money));
    }
    else if (!memberA.isArmed && memberB.isArmed) {
        auto money = memberA.pay();
        memberB.loot(SafeTreasure<decltype(money)>(money));
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

template <typename E>
concept is_encounter_c = requires(E encounter) {
    { Encounter{encounter} } -> std::same_as<E>;
};

template <is_encounter_c... E>
void expedition(E... e) {
    (... , run(e));
}

#endif //TREASURE_HUNT_H
