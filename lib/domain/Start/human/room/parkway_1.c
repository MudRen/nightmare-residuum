#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a wide village street");
    set_long("A wide street traveling northwest to southeast with green spaces instead of buildings. Birch trees branch over the street and open green spaces. Flower beds are prepared near street intersections. An obelisk can be seen to the southeast. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Birds chirp from the trees.");
    set_smell("default", "Fresh air smelling of grass.");
    set_exits(([
        "northwest": HUMAN_ROOM + "parkway_2.c",
        "southeast": HUMAN_ROOM + "square.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled Parkway.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_ITEM + "artwork_1.c": 1,
        HUMAN_ITEM + "bench.c": 1,
        HUMAN_NPC + "ant.c": !random(3),
        HUMAN_NPC + "bee.c": !random(3),
        HUMAN_NPC + "beetle.c": !random(3),
        HUMAN_NPC + "squirrel.c": !random(3),
        HUMAN_NPC + "wasp.c": !random(3),
        HUMAN_NPC + "pigeon.c": !random(2),
        HUMAN_NPC + "sparrow.c": !random(2),
    ]));
}