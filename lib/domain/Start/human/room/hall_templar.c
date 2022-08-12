#include "human.h"

inherit ROOM_CLASS_HALL;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a templar's hall");
    set_long("The basement floor of a brick building that consists of the catacombs of the templar. The walls are filled with chambers upon which the dried remains of templars rest. The ground is a mixture of gravel and dirt. A staircase leads back up to the first floor.");
    set_listen("default", "The gravel and dirt mixture crunches lightly underfoot.");
    set_smell("default", "Stale perfume hangs in the humid air.");
    set_exits(([
        "up": HUMAN_ROOM + "intro_templar.c",
    ]));
    set_looks(([
        "floor": "The floor is a mixture of broken stones forming gravel and dirt.",
        "stairs": "The staircase lead back up to the first floor of the temple.",
        ({ "walls", "chambers", "remains" }): "Important templars resting after their duties have completed.",
    ]));
    set_class("templar");
}