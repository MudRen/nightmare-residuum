inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR with OBJ", }));
    set_syntax("unlock [door|direction] with [key]");
    set_help_text("Your character will attempt to unlock any doors matching the provided input.");
    set_help_similar(({ "lock", "open", "close", "go", }));
}

/* ----- rule: "" ----- */

mixed can_unlock () {
    return "Unlock what?";
}

/* ----- rule: "STR with OBJ" ----- */

mixed can_unlock_str_with_obj (mixed args...) {
    return 1;
}

mixed do_unlock_str_with_obj (mixed args...) {
    object po = previous_object(), env = environment(po), key;
    string str;

    if (sizeof(args) > 1) {
        str = args[0];
        key = args[1];
    }

    if (member_array(str, env->query_doors()) > -1) {
        if (env->handle_unlock(po, str, key)) return 1;
    }

    message("action", "There is no " + str + " to unlock here.", po);
    return 1;
}