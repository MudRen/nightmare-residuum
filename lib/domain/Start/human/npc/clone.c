inherit STD_NPC;

void create () {
    string name, capName;
    ::create();
    if (secure_random(2)) {
        name = ({ "alex", "amos", "anakin", "apollo", "ash", "ben", "buck", "connor", "duncan", "echo", "finn", "fox", "groot", "han", "harold", "henry", "holden", "jack", "jonas", "julian", "kaidan", "kane", "lando", "lon", "luke", "malcolm", "max", "mickey", "miles", "montgomery", "poe", "peter", "quinn", "richard", "riker", "rocket", "saren", "wesley", "yoda", "ziggy" })[secure_random(40)];
        set_gender("male");
    } else {
        name = ({ "aeryn", "athena", "beverly", "clara", "dana", "dasha", "delenn", "diana", "donna", "elise", "ezri", "gamora", "holly", "inara", "janice", "jenny", "jessica", "kathryn", "kira", "leia", "liara", "lorraine", "maggie", "martha", "maya", "missy", "monica", "moya", "padme", "rey", "ripley", "river", "rose", "serina", "tasha", "theora", "vala", "zephyra", "zyla", "zhora" })[secure_random(40)];
        set_gender("female");
    }
    set_id(({ name, "clone" }));
    capName = capitalize(name);
    set_name(capName);
    set_short(capName + " the clone");
    set_long("A cloned human by the name of " + capName + ".");
    set_level(10);
    set_species("human");
}