#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_requirements(([
        "paladin": ([
            "skills": ([
                "melee attack": 1,
                "psionic attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "melee": 15,
        "psionic": 5,
    ]));
    set_help_text("Smite your opponent.");
}

void handle_hit_msg (object source, object target) {
    message("action", "You smite " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " smites you!\n", target);
    message("action", source->query_cap_name() + " smites " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}