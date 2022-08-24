inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/ability.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_name () {
    expect_function("query_name", testOb);

    expect("handles ability name", (: ({
        assert(testOb->query_name(), "==", "ability"),
    }) :));
}

void test_skill_powers () {
    expect_function("query_skill_powers", testOb);
    expect_function("set_skill_powers", testOb);

    expect("handles skill powers", (: ({
        assert(testOb->query_skill_powers(), "==", ([ ])),

        testOb->set_skill_powers(([ "melee": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "melee": 5, ])),

        testOb->set_skill_powers(([ "ranged": 5, "psionic": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "ranged": 5, "psionic": 5, ])),
    }) :));

    expect("set_skill_powers handles invalid argument 1", (: ({
        assert((: testOb->set_skill_powers(this_object()) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers(1) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers(1.0) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers("") :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers(({})) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers((: 1 :)) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
    }) :));
}

void test_difficulty_factor () {
    expect_function("query_difficulty_factor", testOb);
    expect_function("set_difficulty_factor", testOb);

    expect("handles difficulty factor", (: ({
        // defaults to 100
        assert(testOb->query_difficulty_factor(), "==", 100),

        // check lowering difficulty factor
        testOb->set_difficulty_factor(90),
        assert(testOb->query_difficulty_factor(), "==", 90),

        // check raising difficulty factor
        testOb->set_difficulty_factor(110),
        assert(testOb->query_difficulty_factor(), "==", 110),
    }) :));

    expect("set_difficulty_factor handles invalid argument 1", (: ({
        assert((: testOb->set_difficulty_factor(this_object()) :), "catch", "*Bad argument 1 to ability->set_difficulty_factor\n"),
        assert((: testOb->set_difficulty_factor(1.0) :), "catch", "*Bad argument 1 to ability->set_difficulty_factor\n"),
        assert((: testOb->set_difficulty_factor("") :), "catch", "*Bad argument 1 to ability->set_difficulty_factor\n"),
        assert((: testOb->set_difficulty_factor(({})) :), "catch", "*Bad argument 1 to ability->set_difficulty_factor\n"),
        assert((: testOb->set_difficulty_factor(([])) :), "catch", "*Bad argument 1 to ability->set_difficulty_factor\n"),
        assert((: testOb->set_difficulty_factor((: 1 :)) :), "catch", "*Bad argument 1 to ability->set_difficulty_factor\n"),
    }) :));
}

void test_help_text () {
    expect_function("query_help_text", testOb);
    expect_function("set_help_text", testOb);

    expect("handles help text", (: ({
        assert(testOb->query_help_text(), "==", UNDEFINED),

        testOb->set_help_text("Help text."),
        assert(testOb->query_help_text(), "==", "Help text."),

        testOb->set_help_text("Different help text."),
        assert(testOb->query_help_text(), "==", "Different help text."),
    }) :));

    expect("set_help_text handles invalid argument 1", (: ({
        assert((: testOb->set_help_text(this_object()) :), "catch", "*Bad argument 1 to ability->set_help_text\n"),
        assert((: testOb->set_help_text(1.0) :), "catch", "*Bad argument 1 to ability->set_help_text\n"),
        assert((: testOb->set_help_text(1) :), "catch", "*Bad argument 1 to ability->set_help_text\n"),
        assert((: testOb->set_help_text(({})) :), "catch", "*Bad argument 1 to ability->set_help_text\n"),
        assert((: testOb->set_help_text(([])) :), "catch", "*Bad argument 1 to ability->set_help_text\n"),
        assert((: testOb->set_help_text((: 1 :)) :), "catch", "*Bad argument 1 to ability->set_help_text\n"),
    }) :));
}