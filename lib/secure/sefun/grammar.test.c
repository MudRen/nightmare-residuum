inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/grammar.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_remove_article () {
    expect_function("remove_article", testOb);

    expect("articles are trimmed", (: ({
        assert(testOb->remove_article(""), "==", ""),

        assert(testOb->remove_article("thing"), "==", "thing"),

        assert(testOb->remove_article("the thing"), "==", "thing"),
        assert(testOb->remove_article("a thing"), "==", "thing"),
        assert(testOb->remove_article("an item"), "==", "item"),
    }) :));
}
void test_add_article () {
    expect_function("add_article", testOb);

    expect("articles are prepended", (: ({
        assert(testOb->add_article(""), "==", ""),

        assert(testOb->add_article("item"), "==", "an item"),
        assert(testOb->add_article("item", 1), "==", "the item"),

        assert(testOb->add_article("thing"), "==", "a thing"),
        assert(testOb->add_article("thing", 1), "==", "the thing"),
    }) :));
}

void test_conjunction () {
    expect_function("conjunction", testOb);

    expect("conjunction handles list", (: ({
        assert(testOb->conjunction(({ "1" })), "==", "1"),
        assert(testOb->conjunction(({ "1", "2" })), "==", "1 and 2"),
        assert(testOb->conjunction(({ "1", "2", "3" })), "==", "1, 2, and 3"),
        assert(testOb->conjunction(({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" })), "==", "1, 2, 3, 4, 5, 6, 7, 8, 9, and 10"),
    }) :));
}

void test_cardinal () {
    expect_function("cardinal", testOb);

    expect("cardinal handles numbers", (: ({
        assert(testOb->cardinal(0), "==", "zero"),
        assert(testOb->cardinal(1), "==", "one"),
        assert(testOb->cardinal(-1), "==", "negative one"),
        assert(testOb->cardinal(10), "==", "ten"),
        assert(testOb->cardinal(-10), "==", "negative ten"),
        assert(testOb->cardinal(100), "==", "one hundred"),
        assert(testOb->cardinal(1000), "==", "one thousand"),
        assert(testOb->cardinal(10000), "==", "ten thousand"),
        assert(testOb->cardinal(12345), "==", "twelve thousand, three hundred and forty-five"),
        assert(testOb->cardinal(-12345), "==", "negative twelve thousand, three hundred and forty-five"),
        assert(testOb->cardinal(100000), "==", "many"),
        assert(testOb->cardinal(-100000), "==", "negative many"),
    }) :));
}

void test_ordinal () {
    expect_function("ordinal", testOb);

    expect("ordinal handles words for 0..9 range", (: ({
        assert(testOb->ordinal(0), "==", "zeroth"),
        assert(testOb->ordinal(1), "==", "first"),
        assert(testOb->ordinal(2), "==", "second"),
        assert(testOb->ordinal(3), "==", "third"),
        assert(testOb->ordinal(4), "==", "fourth"),
        assert(testOb->ordinal(5), "==", "fifth"),
        assert(testOb->ordinal(6), "==", "sixth"),
        assert(testOb->ordinal(7), "==", "seventh"),
        assert(testOb->ordinal(8), "==", "eighth"),
        assert(testOb->ordinal(9), "==", "ninth"),
    }) :));

    expect("ordinal handles numbers outside 11..14 range", (: ({
        assert(testOb->ordinal(11), "==", "11th"),
        assert(testOb->ordinal(12), "==", "12th"),
        assert(testOb->ordinal(13), "==", "13th"),
        assert(testOb->ordinal(14), "==", "14th"),
    }) :));

    expect("ordinal handles numbers outside 0..14 range", (: ({
        assert(testOb->ordinal(15), "==", "15th"),
        assert(testOb->ordinal(22), "==", "22nd"),
        assert(testOb->ordinal(23), "==", "23rd"),
        assert(testOb->ordinal(24), "==", "24th"),
    }) :));
}

void test_pluralize () {
    object ob;

    expect_function("pluralize", testOb);

    expect("pluralize handles strings", (: ({
        assert(testOb->pluralize("elf"), "==", "elves"),
        assert(testOb->pluralize("dwarf"), "==", "dwarves"),

        assert(testOb->pluralize("staff"), "==", "staves"),
        assert(testOb->pluralize("scoff"), "==", "scoffs"),
        assert(testOb->pluralize("bluff"), "==", "bluffs"),
        assert(testOb->pluralize("quaff"), "==", "quaffs"),
        assert(testOb->pluralize("chaff"), "==", "chaffs"),
        assert(testOb->pluralize("graff"), "==", "graffs"),

        assert(testOb->pluralize("gun"), "==", "guns"),
        assert(testOb->pluralize("guns"), "==", "guns"),

        assert(testOb->pluralize("a red house"), "==", "red houses"),
        assert(testOb->pluralize("a sack of rice"), "==", "sacks of rice"),
        assert(testOb->pluralize("plumbus"), "==", "plumbi"),
        assert(testOb->pluralize("gooblebox"), "==", "goobleboxes"),
        assert(testOb->pluralize("flooblecrank"), "==", "flooblecranks"),
        assert(testOb->pluralize("bloobleyank"), "==", "bloobleyanks"),
    }) :));

    ob = new(STD_OBJECT);
    expect("pluralize handles objects", (: ({
        assert($(ob)->set_name("tester"), "==", 0),
        assert(testOb->pluralize($(ob)), "==", "testers"),
        assert($(ob)->set_name("staff"), "==", 0),
        assert(testOb->pluralize($(ob)), "==", "staves"),
    }) :));
    destruct(ob);

    expect("pluralize handles some abnormal overridden strings", (: ({
        // handle entire string is abnormal override
        assert(testOb->pluralize("die"), "==", "dies"),
        assert(testOb->pluralize("were"), "==", "was"),
        assert(testOb->pluralize("boots"), "==", "boots"),
        assert(testOb->pluralize("robes"), "==", "robes"),
        assert(testOb->pluralize("gloves"), "==", "gloves"),
        assert(testOb->pluralize("shoes"), "==", "shoes"),

        // handle only last word is override string
        assert(testOb->pluralize("blue robes"), "==", "blue robes"),
        assert(testOb->pluralize("canvas shoes"), "==", "canvas shoes"),
    }) :));
}

void test_consolidate () {
    expect_function("consolidate", testOb);

    expect("consolidate handles basic words", (: ({
        assert(testOb->consolidate(5, "elf"), "==", "five elves"),
        assert(testOb->consolidate(0, "giraffe"), "==", "zero giraffes"),
        assert(testOb->consolidate(25, "cat"), "==", "twenty-five cats"),
        assert(testOb->consolidate(54321, "dog"), "==", "fifty-four thousand, three hundred and twenty-one dogs"),
        assert(testOb->consolidate(-54321, "care"), "==", "negative fifty-four thousand, three hundred and twenty-one cares"),
        assert(testOb->consolidate(2, "Paul"), "==", "two Pauls"),
    }) :));
    expect("consolidate handles parenthesis tags after words", (: ({
        assert(testOb->consolidate(2, "a bronze dagger (wielded)"), "==", "two bronze daggers (wielded)"),
        assert(testOb->consolidate(2, "a bronze ring (worn)"), "==", "two bronze rings (worn)"),
        assert(testOb->consolidate(3, "a monster (injured)"), "==", "three monsters (injured)"),
    }) :));
}

void test_possessive_noun () {
    object ob;

    expect_function("possessive_noun", testOb);

    expect("possessive_noun handles names", (: ({
        assert(testOb->possessive_noun(0), "==", "Its"),
        assert(testOb->possessive_noun("Name"), "==", "Name's"),
        assert(testOb->possessive_noun("Hermes"), "==", "Hermes'"),
        assert(testOb->possessive_noun("Shax"), "==", "Shax'"),
        assert(testOb->possessive_noun("Chaz"), "==", "Chaz'"),
    }) :));

    ob = new(STD_OBJECT);
    expect("possessive_noun handles objects", (: ({
        assert($(ob)->set_name("Name"), "==", 0),
        assert(testOb->possessive_noun($(ob)), "==", "Name's"),
        assert($(ob)->set_name("Hermes"), "==", 0),
        assert(testOb->possessive_noun($(ob)), "==", "Hermes'"),
        assert($(ob)->set_name("Shax"), "==", 0),
        assert(testOb->possessive_noun($(ob)), "==", "Shax'"),
        assert($(ob)->set_name("Chaz"), "==", 0),
        assert(testOb->possessive_noun($(ob)), "==", "Chaz'"),
    }) :));
    destruct(ob);
}

void test_subjective () {
    object ob;

    expect_function("subjective", testOb);

    expect("subjective handles names", (: ({
        assert(testOb->subjective(0), "==", "it"),
        assert(testOb->subjective("male"), "==", "he"),
        assert(testOb->subjective("female"), "==", "she"),
        assert(testOb->subjective("neither"), "==", "they"),
        assert(testOb->subjective("nonexistant"), "==", "it"),
    }) :));

    ob = new(STD_LIVING);
    expect("subjective handles objects", (: ({
        assert($(ob)->set_gender("male"), "==", 0),
        assert(testOb->subjective($(ob)), "==", "he"),
        assert($(ob)->set_gender("female"), "==", 0),
        assert(testOb->subjective($(ob)), "==", "she"),
        assert($(ob)->set_gender("neither"), "==", 0),
        assert(testOb->subjective($(ob)), "==", "they"),
        assert($(ob)->set_gender("nonexistant"), "==", 0),
        assert(testOb->subjective($(ob)), "==", "it"),
    }) :));
    destruct(ob);
}

void test_objective () {
    object ob;

    expect_function("objective", testOb);

    expect("objective handles names", (: ({
        assert(testOb->objective(0), "==", "it"),
        assert(testOb->objective("male"), "==", "him"),
        assert(testOb->objective("female"), "==", "her"),
        assert(testOb->objective("neither"), "==", "them"),
        assert(testOb->objective("nonexistant"), "==", "it"),
    }) :));

    ob = new(STD_LIVING);
    expect("objective handles objects", (: ({
        assert($(ob)->set_gender("male"), "==", 0),
        assert(testOb->objective($(ob)), "==", "him"),
        assert($(ob)->set_gender("female"), "==", 0),
        assert(testOb->objective($(ob)), "==", "her"),
        assert($(ob)->set_gender("neither"), "==", 0),
        assert(testOb->objective($(ob)), "==", "them"),
        assert($(ob)->set_gender("nonexistant"), "==", 0),
        assert(testOb->objective($(ob)), "==", "it"),
    }) :));
    destruct(ob);
}

void test_possessive () {
    object ob;

    expect_function("possessive", testOb);

    expect("possessive handles names", (: ({
        assert(testOb->possessive(0), "==", "its"),
        assert(testOb->possessive("male"), "==", "his"),
        assert(testOb->possessive("female"), "==", "her"),
        assert(testOb->possessive("neither"), "==", "their"),
        assert(testOb->possessive("nonexistant"), "==", "its"),
    }) :));

    ob = new(STD_LIVING);
    expect("possessive handles objects", (: ({
        assert($(ob)->set_gender("male"), "==", 0),
        assert(testOb->possessive($(ob)), "==", "his"),
        assert($(ob)->set_gender("female"), "==", 0),
        assert(testOb->possessive($(ob)), "==", "her"),
        assert($(ob)->set_gender("neither"), "==", 0),
        assert(testOb->possessive($(ob)), "==", "their"),
        assert($(ob)->set_gender("nonexistant"), "==", 0),
        assert(testOb->possessive($(ob)), "==", "its"),
    }) :));
    destruct(ob);
}

void test_reflexive () {
    object ob;

    expect_function("reflexive", testOb);

    expect("reflexive handles names", (: ({
        assert(testOb->reflexive(0), "==", "itself"),
        assert(testOb->reflexive("male"), "==", "himself"),
        assert(testOb->reflexive("female"), "==", "herself"),
        assert(testOb->reflexive("neither"), "==", "themself"),
        assert(testOb->reflexive("nonexistant"), "==", "itself"),
    }) :));

    ob = new(STD_LIVING);
    expect("reflexive handles objects", (: ({
        assert($(ob)->set_gender("male"), "==", 0),
        assert(testOb->reflexive($(ob)), "==", "himself"),
        assert($(ob)->set_gender("female"), "==", 0),
        assert(testOb->reflexive($(ob)), "==", "herself"),
        assert($(ob)->set_gender("neither"), "==", 0),
        assert(testOb->reflexive($(ob)), "==", "themself"),
        assert($(ob)->set_gender("nonexistant"), "==", 0),
        assert(testOb->reflexive($(ob)), "==", "itself"),
    }) :));
    destruct(ob);
}