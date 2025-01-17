nosave private mapping __Reset, __Objects;
nosave private int __Resets;

mapping query_reset () {
    return __Reset;
}
mapping query_objects () {
    return __Objects;
}
int query_resets () {
    return __Resets;
}

void create () {
    __Reset = ([ ]);
    __Objects = ([ ]);
    __Resets = 0;
}
void reset () {
    mapping counts = ([ ]);
    int count;
    string name;
    object ob;

    // remove destructed tracked objects
    __Objects = filter_mapping(__Objects, (: !!$2 :));

    // count objects in this container
    foreach (ob in all_inventory()) {
        name = base_name(ob) + ".c";
        if (!__Reset[name]) continue;
        if (sizeof(__Objects[name]) > 0) continue;
        if (!counts[name]) counts[name] = 0;
        counts[name] += 1;
    }
    foreach (string key, mixed val in __Reset) {
        if (functionp(val)) {
            val = evaluate(val);
        }
        if (!intp(val)) continue;

        count = 0;
        if (counts[key] && (count = counts[key]) >= val) continue;
        for (; count < val; count ++) {
            if (__Objects[key + count]) continue;

            ob = clone_object(key);
            // track wandering objects
            if (ob->query_wander()) {
                __Objects[key + count] = ob;
            }

            // prevent objects from leaking if can't move
            if (!ob->handle_move(this_object())) {
                destruct(ob);
            } else {
                ob->reset();
            }
        }
    }
    __Resets ++;
}

void set_reset (mapping reset) {
    __Reset = ([ ]);
    foreach (string key, mixed val in reset) {
        __Reset[key] = val;
    }
    reset();
}