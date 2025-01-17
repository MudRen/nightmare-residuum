#include <verb.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/verb.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

nosave private int __Disable, __Busy;
int query_disable () { return __Disable; }
int query_busy () { return __Busy; }

void test_requirements () {
    expect_function("set_requirements", testOb);
    expect_function("query_requirements", testOb);
    expect_function("can_verb_rule", testOb);
    expect_function("check_busy", testOb);
    expect_function("check_disable", testOb);

    expect("requirements should be settable and queryable", (: ({
        assert(testOb->query_requirements(), "==", REQUIREMENT_NONE),

        testOb->set_requirements(REQUIREMENT_BUSY),
        assert(testOb->query_requirements(), "==", REQUIREMENT_BUSY),

        testOb->set_requirements(REQUIREMENT_DISABLE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_DISABLE),

        testOb->set_requirements(REQUIREMENT_BUSY & REQUIREMENT_DISABLE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_BUSY & REQUIREMENT_DISABLE),
    }) :));

    expect("can_verb_rule should use requirements", (: ({
        // test singular requirements
        testOb->set_requirements(REQUIREMENT_NONE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_NONE),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1), // passes

        testOb->set_requirements(REQUIREMENT_BUSY),
        assert(testOb->query_requirements(), "==", REQUIREMENT_BUSY),
        assert(testOb->check_busy(), "==", 1),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),

        __Busy = 1,
        assert(testOb->check_busy(), "regex", "^You are too busy"),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),

        testOb->set_requirements(REQUIREMENT_DISABLE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_DISABLE),
        assert(testOb->check_disable(), "==", 1),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),

        __Disable = 1,
        assert(testOb->check_disable(), "regex", "^You are not able"),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are not able"),

        // test multiple requirements
        testOb->set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE),
        __Busy = 0,
        __Disable = 0,
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),
        // busy
        __Busy = 1,
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),
        // both
        __Disable = 1,
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),
        // disable
        __Busy = 0,
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are not able"),
    }) :));
}