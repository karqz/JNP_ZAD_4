#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include "member.h"

#include <numeric>
#include <vector>
#include <iostream>
#include <concepts>
#include <utility>

template <typename T>
concept treasure_c = requires(T treasure) {
    { Treasure{treasure} } -> std::same_as<T>;
};

template <typename T>
concept member_c = requires(T member) {
    typename T::strength_t;
    { [] () constexpr { return T::isArmed; }() };
    { member.isArmed } -> std::convertible_to<bool>;
    { member.pay() } -> std::integral;
    member.loot(Treasure<decltype(member.pay()), true>(0));
    member.loot(Treasure<decltype(member.pay()), false>(0));
};

template <typename T>
concept EncounterSide = treasure_c<T> || member_c<T>;

template <EncounterSide sideA, EncounterSide sideB>
struct Encounter {
    sideA &side_a;
    sideB &side_b;
};

template <member_c sideA, member_c sideB>
requires (sideA::isArmed && sideB::isArmed)
constexpr void run(Encounter<sideA, sideB> encounter) {
    if (encounter.side_a.getStrength() > encounter.side_b.getStrength()) {
        auto money = encounter.side_b.pay();
        encounter.side_a.loot(SafeTreasure<decltype(money)>(money));
    }
    else if (encounter.side_b.getStrength() > encounter.side_a.getStrength()) {
        auto money = encounter.side_a.pay();
        encounter.side_b.loot(SafeTreasure<decltype(money)>(money));
    }
}

template <member_c sideA, member_c sideB>
requires (!sideA::isArmed || !sideB::isArmed)
constexpr void run(Encounter<sideA, sideB> encounter) {
    if (encounter.side_a.isArmed) {
        auto money = encounter.side_b.pay();
        encounter.side_a.loot(SafeTreasure<decltype(money)>(money));
    }
    else if (encounter.side_b.isArmed) {
        auto money = encounter.side_a.pay();
        encounter.side_b.loot(SafeTreasure<decltype(money)>(money));
    }
}

template <member_c sideA, treasure_c sideB>
constexpr void run(Encounter<sideA, sideB> encounter) {
    encounter.side_a.loot(std::move(encounter.side_b));
}

template <treasure_c sideA, member_c sideB>
constexpr void run(Encounter<sideA, sideB> encounter) {
    encounter.side_b.loot(std::move(encounter.side_a));
}

template <typename E>
concept is_encounter_c = requires(E encounter) {
    { Encounter{encounter} } -> std::same_as<E>;
};

template <is_encounter_c... E>
constexpr void expedition(E... e) {
    (... , run(e));
}

#endif //TREASURE_HUNT_H
