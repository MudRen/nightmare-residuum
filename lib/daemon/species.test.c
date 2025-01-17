inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/species.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_species_bodies () {
    expect_function("query_bodies", testOb);
    expect_function("query_species", testOb);

    expect("bodies exist", (: ({
        assert(mapp(testOb->query_bodies()), "==", 1),
        assert(sizeof(testOb->query_bodies()), ">", 0),
        // verify common bodytype
        assert(mapp(testOb->query_bodies()["quadruped"]), "==", 1),
    }) :));

    expect("species exist", (: ({
        assert(mapp(testOb->query_species()), "==", 1),
        assert(sizeof(testOb->query_species()), ">", 0),
        // verify common species
        assert(mapp(testOb->query_species()["human"]), "==", 1),
    }) :));
}

void test_setup_body () {
    object ob;
    mapping limbs = ([ ]);

    expect_function("setup_body", testOb);

    // setting a species will setup the body
    ob = new(STD_LIVING);
    ob->set_species("human");
    // store the limb data
    foreach (string limb in ob->query_limbs()) {
        limbs[limb] = ob->query_limb(limb);
    }

    expect("species bodies exist", (: ({
        // verify limbs exist
        assert(sizeof(keys($(limbs))), ">", 0),
        // compare body to limbs
        assert(testOb->setup_body($(ob)), "==", $(limbs)),
    }) :));

    destruct(ob);
}

void test_adjust_stat () {
    expect_function("query_adjust_stat", testOb);

    expect("stats adjustments are queryable", (: ({
        assert(testOb->query_adjust_stat("human", "strength"), "==", 2),

        assert(testOb->query_adjust_stat("unknown species", "strength"), "==", 0),

        assert(testOb->query_adjust_stat("human", "unknown stat"), "==", 0),
    }) :));
}

void test_adjust_vitals () {
    expect_function("query_adjust_hp", testOb);
    expect_function("query_adjust_sp", testOb);
    expect_function("query_adjust_mp", testOb);

    expect("hp adjustments are queryable", (: ({
        assert(testOb->query_adjust_hp("human"), "==", 1),

        assert(testOb->query_adjust_hp("nonexistant"), "==", 0),
    }) :));

    expect("sp adjustments are queryable", (: ({
        assert(testOb->query_adjust_sp("human"), "==", 1),

        assert(testOb->query_adjust_sp("nonexistant"), "==", 0),
    }) :));

    expect("mp adjustments are queryable", (: ({
        assert(testOb->query_adjust_mp("human"), "==", 1),

        assert(testOb->query_adjust_mp("nonexistant"), "==", 0),
    }) :));
}

void test_setup_stats () {
    object ob;

    expect_function("setup_stats", testOb);

    ob = new(STD_LIVING);
    ob->set_species("human");
    expect("stats are setup", (: ({
        assert($(ob)->query_stat("strength"), "==", 0),
        assert($(ob)->query_stat("perception"), "==", 0),
        assert($(ob)->query_stat("endurance"), "==", 0),
        assert($(ob)->query_stat("charisma"), "==", 0),
        assert($(ob)->query_stat("intelligence"), "==", 0),
        assert($(ob)->query_stat("agility"), "==", 0),
        assert($(ob)->query_stat("luck"), "==", 0),

        testOb->setup_stats($(ob)),

        assert($(ob)->query_stat("strength"), ">=", 5),
        assert($(ob)->query_stat("perception"), ">=", 5),
        assert($(ob)->query_stat("endurance"), ">=", 5),
        assert($(ob)->query_stat("charisma"), ">=", 5),
        assert($(ob)->query_stat("intelligence"), ">=", 5),
        assert($(ob)->query_stat("agility"), ">=", 5),
        assert($(ob)->query_stat("luck"), ">=", 5),

        assert($(ob)->handle_remove(), "==", 1),
    }) :));
}