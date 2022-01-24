#include "living.h"

private mapping __Vitals = ([
    "hp": 0,
    "hpMax": 0,
    "sp": 0,
    "spMax": 0,
    "mp": 0,
    "mpMax": 0,
]);

int query_hp () {
    return __Vitals["hp"];
}
int query_sp () {
    return __Vitals["sp"];
}
int query_mp () {
    return __Vitals["mp"];
}

int query_max_hp () {
    return __Vitals["hpMax"];
}
int query_max_sp () {
    return __Vitals["spMax"];
}
int query_max_mp () {
    return __Vitals["mpMax"];
}

void add_hp (int n) {
    __Vitals["hp"] += n;
    if (__Vitals["hp"] > __Vitals["hpMax"]) {
        __Vitals["hp"] = __Vitals["hpMax"];
    }
}
void add_sp (int n) {
    __Vitals["sp"] += n;
    if (__Vitals["sp"] > __Vitals["spMax"]) {
        __Vitals["sp"] = __Vitals["spMax"];
    }
}
void add_mp (int n) {
    __Vitals["mp"] += n;
    if (__Vitals["mp"] > __Vitals["mpMax"]) {
        __Vitals["mp"] = __Vitals["mpMax"];
    }
}

void set_hp (int n) {
    __Vitals["hp"] = n;
    if (__Vitals["hp"] > __Vitals["hpMax"]) {
        __Vitals["hp"] = __Vitals["hpMax"];
    }
}
void set_sp (int n) {
    __Vitals["sp"] = n;
    if (__Vitals["sp"] > __Vitals["spMax"]) {
        __Vitals["sp"] = __Vitals["spMax"];
    }
}
void set_mp (int n) {
    __Vitals["mp"] = n;
    if (__Vitals["mp"] > __Vitals["mpMax"]) {
        __Vitals["mp"] = __Vitals["mpMax"];
    }
}

void update_vitals (int heal) {
    int level = this_object()->query_level() || 1;
    int statHP = this_object()->query_stat("endurance") || 0;
    int statSP = this_object()->query_stat("agility") || 0;
    int statMP = this_object()->query_stat("intelligence") || 0;

    int adjHP = 0 + 0; // @TODO class + species modifiers
    int adjSP = 0 + 0; // @TODO class + species modifiers
    int adjMP = 0 + 0; // @TODO class + species modifiers

    int maxHP = (1 + level + statHP) * (10 + adjHP);
    int maxSP = (1 + level + statSP) * ( 5 + adjSP);
    int maxMP = (1 + level + statMP) * ( 5 + adjMP);

    __Vitals["hpMax"] = maxHP;
    __Vitals["spMax"] = maxSP;
    __Vitals["mpMax"] = maxMP;

    if (heal) {
        set_hp(query_max_hp());
        set_sp(query_max_sp());
        set_mp(query_max_mp());
    }
}