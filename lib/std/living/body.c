#include "living.h"

private string __Gender = "neither";
private string __Species = "unknown";
private int __Level = 1;
mapping __Limbs = ([ ]);
private nosave mapping __Wielded = ([ ]);
private nosave mapping __Worn = ([ ]);
nosave private int __NextHeal;

/* ----- gender and species ----- */

string query_gender () {
    return __Gender;
}
void set_gender (string str) {
    switch(str) {
    case "male": case "female": case "neither":
        __Gender = str;
        break;
    default:
        __Gender = "none";
    }
}

string query_species () {
    return stringp(__Species) ? __Species : "unknown";
}
void set_species (string species) {
    if (!stringp(species)) error("Bad argument 1 to body->set_species");

    __Species = species;
    this_object()->update_vitals(1);
    update_limbs();
}

/* ----- level and experience ----- */

int query_level () {
    return __Level;
}
void set_level (int l) {
    if (l < 0) {
        l = abs(l);
    }
    __Level = l;

    this_object()->update_vitals(1);
    update_limbs(); // limbs require vitals
}

/* ----- limbs ----- */

protected void update_limbs () {
    mapping newLimbs = D_SPECIES->setup_body(this_object());

    foreach (string limb,mapping value in newLimbs || ([ ])) {
        if (__Limbs[limb]) {
            value["damage"] = __Limbs[limb]["damage"];
            value["status"] = __Limbs[limb]["status"];
        }
        __Limbs[limb] = value;
    }
}
string *query_limbs () {
    string *limbs = keys(__Limbs || ([ ]));
    limbs = filter(limbs, (: __Limbs[$1]["status"] != "severed" :));
    return limbs;
}
string *query_severed_limbs () {
    string *limbs = keys(__Limbs || ([ ]));
    limbs = filter(limbs, (: __Limbs[$1]["status"] == "severed" :));
    return limbs;
}
mapping query_limb (string limb) {
    if (limb && __Limbs[limb]) return __Limbs[limb];
    return 0;
}
string query_random_limb () {
    string *limbs = query_limbs();
    if (sizeof(limbs)) return limbs[random(sizeof(limbs))];
    else return 0;
}

void handle_limb_sever (string limb) {
    object limbOb;

    if (!this_object()) {
        return;
    }

    // non-fatal limbs can't be severed multiple times
    if (__Limbs[limb]["status"] == "severed" && __Limbs[limb]["type"] != "FATAL") {
        return;
    }

    if (__Limbs[limb]["type"] == "FATAL") {
        message("combat alert", "Your "+limb+" receives a mortal blow!", this_object());
        message("combat info", possessive_noun(this_object())+" "+limb+" receives a mortal blow!", environment(), this_object());
    } else {
        message("combat alert", "Your "+limb+" is severed!", this_object());
        message("combat info", possessive_noun(this_object())+" "+limb+" is severed!", environment(), this_object());
    }

    // remove wielded weapon
    if (__Wielded[limb]) {
        handle_unwield(__Wielded[limb], limb);
    }
    // remove worn armour
    if (__Worn[limb]) {
        foreach (object ob in __Worn[limb]) {
            handle_unwear(ob);
        }
    }

    // mark limb as severed
    __Limbs[limb]["status"] = "severed";
    __Limbs[limb]["damage"] = -1;

    // create bodypart
    limbOb = new("/std/item/bodypart.c");
    limbOb->setup_bodypart(this_object()->query_cap_name(), limb);
    limbOb->handle_move(environment());

    // deal half limb damage as HP damage
    this_object()->handle_damage(max(({ __Limbs[limb]["damage"], __Limbs[limb]["maxdamage"] }))/2, 0);
    if (__Limbs[limb]["attach"] && __Limbs[limb]["attach"] != limb) {
        handle_limb_sever(__Limbs[limb]["attach"]);
    }
}

void handle_limb_restore (string limb) {
    if (__Limbs[limb]["status"] != "severed") {
        return;
    }
    map_delete(__Limbs[limb], "status");
    __Limbs[limb]["damage"] = 0;
}

void handle_limb_heal (string limb, int n) {
    if (__Limbs[limb]["status"] == "severed") {
        return;
    }
    __Limbs[limb]["damage"] -= n;
    if (__Limbs[limb]["damage"] < 0) {
        __Limbs[limb]["damage"] = 0;
    }
}

/* ----- damage ----- */

varargs int handle_damage (int damage, string limb, object source) {
    int beforeHp = query_hp();

    if (!this_object()) {
        return 0;
    }

    add_hp(-damage);
    if (query_max_hp() < query_hp()) {
        set_hp(query_max_hp());
    }
    if (this_object()->is_character()) {
        message("system", sprintf("hp: %d (%d) sp: %d    mp: %d\n", query_hp(), query_hp() - beforeHp, query_sp(), query_mp()), this_object());
    }

    if (stringp(limb) && limb != "" && this_object()->query_limb(limb)) {
        int limbDamagePct;

        __Limbs[limb]["damage"] += damage / 2;
        if (__Limbs[limb]["damage"] < 0) {
            __Limbs[limb]["damage"] = 0;
        }
        limbDamagePct = __Limbs[limb]["damage"]*100/__Limbs[limb]["maxdamage"];

        if (limbDamagePct >= 100) {
            handle_limb_sever(limb);
        } else if (limbDamagePct >= 75) {
            message("combat alert", "Your "+limb+" is badly damaged!", this_object());
        } else if (limbDamagePct >= 50) {
            message("combat alert", "Your "+limb+" is injured!", this_object());
        }
    }

    return damage;
}

/* ----- wearing ----- */

int query_limb_armor (string limb) {
    int ac = 0;

    if (!mapp(__Worn)) __Worn = ([ ]);

    foreach (object ob in __Worn[limb] || ({ })) {
        ac += ob->query_ac();
    }

    return ac;
}
object *query_all_armor () {
    object *worn = ({ });

    if (!mapp(__Worn)) __Worn = ([ ]);

    foreach (string limb in keys(__Worn)) {
        foreach (object ob in __Worn[limb]) {
            if (member_array(ob, worn) == -1) {
                worn += ({ ob });
            }
        }
    }
    return worn;
}
object query_worn_shield () {
    object shield;
    foreach (string limb in keys(__Worn)) {
        foreach (object ob in __Worn[limb]) {
            if (ob->query_type() == "shield") {
                shield = ob;
            }
        }
    }
    return shield;
}
mixed query_can_wear_armor (object armor) {
    int worn;
    string wielded;

    if (!mapp(__Worn)) {
        __Worn = ([ ]);
    }

    foreach (string limb in armor->query_limbs()) {
        if (armor->query_type() == "shield" && __Wielded[limb]) {
            wielded = limb;
        }
        if (!arrayp(__Worn[limb])) continue;
        foreach (object ob in __Worn[limb]) {
            if (armor->query_type() == ob->query_type()) {
                worn ++;
            }
        }
    }

    switch (armor->query_type()) {
    case "shield":
        if (worn || member_array("shield", query_all_armor()) > -1) {
            return "You are already wearing a " + armor->query_type()+ ".";
        }
        if (wielded) {
            return "You cannot wear a shield while wielding a weapon in your " + wielded + ".";
        }
        break;
    default:
        if (worn) {
            return "You are already wearing a " + armor->query_type()+ ".";
        }
        break;
    }

    return 1;
}

varargs mixed handle_wear (object ob) {
    string limbConj;
    string result;

    if (!mapp(__Worn)) __Worn = ([ ]);

    if (ob->query_worn()) return "You are already wearing " + ob->query_name() + ".";
    result = query_can_wear_armor(ob);
    if (stringp(result)) return result;
    else if (intp(result) != 1) return "You are already wearing " + ob->query_type() + ".";

    foreach (string limb in ob->query_limbs()) {
        if (!arrayp(__Worn[limb])) __Worn[limb] = ({ });
        __Worn[limb] += ({ ob });
    }
    ob->set_worn(this_object());

    limbConj = conjunction(ob->query_limbs());
    message("action", "You wear " + ob->query_name() + " on your " + limbConj + ".", this_object());
    message("action", this_object()->query_cap_name() + " wears " + ob->query_name() + " on " + possessive(this_object()) + " " + limbConj + ".", environment(), this_object());

    return 1;
}
varargs mixed handle_unwear (object ob) {
    string limbConj;
    if (!mapp(__Worn)) __Worn = ([ ]);

    if (!ob->query_worn()) return "You are not wearing " + ob->query_name() + ".";

    foreach (string limb in ob->query_limbs()) {
        if (!arrayp(__Worn[limb])) __Worn[limb] = ({ });
        if (member_array(ob, __Worn[limb]) > -1) {
            __Worn[limb] -= ({ ob });
        }
    }
    ob->set_worn(0);

    limbConj = conjunction(ob->query_limbs());
    message("action", "You remove " + ob->query_name() + " from your " + limbConj + ".", this_object());
    message("action", this_object()->query_cap_name() + " removes " + ob->query_name() + " from " + possessive(this_object()) + " " + limbConj + ".", environment(), this_object());

    return 1;
}

/* ----- wielding ----- */

string *query_wieldable_limbs () {
    string *wieldable = ({ });
    object shield = query_worn_shield();
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["type"] == "WIELD" && !__Wielded[l]) {
            if (!shield || (shield && member_array(l, shield->query_limbs()) == -1)) {
                wieldable += ({ l });
            }
        }
    }
    return wieldable;
}
object query_wielded (string l) {
    if (l && __Limbs[l]["type"] == "WIELD" && __Wielded[l]) return __Wielded[l];
    return 0;
}
object *query_wielded_weapons () {
    object *weapons = ({ });
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["type"] == "WIELD" && __Wielded[l]) weapons += ({ query_wielded(l) });
    }
    return weapons;
}
string *query_wielded_limbs (object ob) {
    string *limbs = ({ });
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["type"] == "WIELD" && __Wielded[l] == ob) {
            limbs += ({ l });
        }
    }
    return limbs;
}

varargs mixed handle_wield (object ob) {
    string *limbs = query_wieldable_limbs(), *hands, limbConj;

    if (!mapp(__Wielded)) __Wielded = ([ ]);

    if (ob->query_wielded()) return "You are already wielding " + ob->query_name() + ".";

    if (ob->query_hands() == 1) {
        if (sizeof(limbs)) {
            hands = limbs[0..0];
        }
    } else if (ob->query_hands() == 2) {
        if (sizeof(limbs) > 1) {
            hands = limbs[0..1];
        }
    }
    if (sizeof(hands) < ob->query_hands()) return "You are out of limbs to wield " + ob->query_name() + ".";

    foreach (string limb in hands) {
        __Wielded[limb] = ob;
    }
    ob->set_wielded(this_object());

    limbConj = conjunction(this_object()->query_wielded_limbs(ob));
    message("action", "You wield " + ob->query_name() + " in your " + limbConj + ".", this_object());
    message("action", this_object()->query_cap_name() + " wields " + ob->query_name() + " in " + possessive(this_object()) + " " + limbConj + ".", environment(), this_object());

    return 1;
}
varargs mixed handle_unwield (object ob) {
    string *limbs, limbConj;

    if (!mapp(__Wielded)) __Wielded = ([ ]);

    if (!ob->query_wielded()) return "You are not wielding " + ob->query_name() + ".";

    limbs = this_object()->query_wielded_limbs(ob);
    limbConj = conjunction(limbs);

    if (!sizeof(limbs)) return 0;

    ob->set_wielded(0);
    foreach (string limb in limbs) {
        __Wielded[limb] = 0;
    }

    message("action", "You unwield " + ob->query_name() + " from your " + limbConj + ".", this_object());
    message("action", this_object()->query_cap_name() + " unwields " + ob->query_name() + " from " + possessive(this_object()) + " " + limbConj + ".", environment(), this_object());

    return 1;
}

/* ----- healing ----- */

void heal (int n) {
    if (n < 0) {
        return;
    }

    add_hp(n);
    add_sp(n);
    add_mp(n);

    n = n / 2;
    foreach (string limb in query_limbs()) {
        if (__Limbs[limb]["damage"] > 0) {
            __Limbs[limb]["damage"] -= n;
            if (__Limbs[limb]["damage"] < 0) {
                __Limbs[limb]["damage"] = 0;
            }
        }
    }
}

private void handle_passive_heal () {
    int amt;

    if (!__NextHeal) {
        __NextHeal = time() + 10;
        return;
    } else if (__NextHeal > time()) {
        return;
    }

    __NextHeal = time() + 10;
    amt = to_int(ceil((query_level() / 5.0) + (query_stat("endurance") / 10.0) + (query_stat("luck") / 20.0)));
    heal(amt);
}

/* ----- object applies ----- */

void heart_beat () {
    handle_passive_heal();
}