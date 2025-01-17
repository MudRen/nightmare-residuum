inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/color.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_strip_colour () {
    string text = "%^BOLD%^Text%^RESET%^";

    expect_function("strip_colour", testOb);

    expect("strip_colour removes ANSI resets", (: ({
        assert(testOb->strip_colour($(text)), "==", "Text"),
        assert(strlen($(text)), "!=", 4),
        assert(strlen(testOb->strip_colour($(text))), "==", 4),
        assert(strlen(testOb->strip_colour("%^RESET%^%^RESET%^RESET%^")), "==", 0),
    }) :));
}

void test_hex_to_int () {
    expect_function("hex_to_int", testOb);

    expect("hex_to_int handled base 16 to base 10", (: ({
        assert(testOb->hex_to_int("00"), "==", 0),
        assert(testOb->hex_to_int("0A"), "==", 10),
        assert(testOb->hex_to_int("0F"), "==", 15),
        assert(testOb->hex_to_int("A0"), "==", 160),
        assert(testOb->hex_to_int("F0"), "==", 240),
        assert(testOb->hex_to_int("AA"), "==", 170),
        assert(testOb->hex_to_int("BB"), "==", 187),
        assert(testOb->hex_to_int("CC"), "==", 204),
        assert(testOb->hex_to_int("DD"), "==", 221),
        assert(testOb->hex_to_int("EE"), "==", 238),
        assert(testOb->hex_to_int("FF"), "==", 255),
        assert(testOb->hex_to_int("0a"), "==", 10),
        assert(testOb->hex_to_int("0f"), "==", 15),
        assert(testOb->hex_to_int("a0"), "==", 160),
        assert(testOb->hex_to_int("f0"), "==", 240),
        assert(testOb->hex_to_int("aa"), "==", 170),
        assert(testOb->hex_to_int("bb"), "==", 187),
        assert(testOb->hex_to_int("cc"), "==", 204),
        assert(testOb->hex_to_int("dd"), "==", 221),
        assert(testOb->hex_to_int("ee"), "==", 238),
        assert(testOb->hex_to_int("ff"), "==", 255),
        assert(testOb->hex_to_int("-a"), "==", -10),
        assert(testOb->hex_to_int("a"), "==", 10),
        assert(testOb->hex_to_int("A"), "==", 10),
        assert(testOb->hex_to_int("-A"), "==", -10),
        assert(testOb->hex_to_int("FFF"), "==", 4095),
    }) :));
}

private int *Color;
void test_random_color () {
    expect_function("query_random_color", testOb);

    Color = testOb->query_random_color();
    expect("query_random_color returns random triplets", (: ({
        assert(sizeof(Color), "==", 3),
        assert(Color[0] >= 0 && Color[0] <= 255, "==", 1),
        assert(Color[1] >= 0 && Color[1] <= 255, "==", 1),
        assert(Color[2] >= 0 && Color[2] <= 255, "==", 1),
    }) :));
}

void test_sRGB () {
    expect_function("color_to_sRGB", testOb);
    expect_function("color_from_sRGB", testOb);

    expect("converting to sRGB behaves", (: ({
        assert(testOb->color_to_sRGB(0.0), "==", 0),
        assert(testOb->color_to_sRGB(0.25), "==", 137),
        assert(testOb->color_to_sRGB(0.5), "==", 188),
        assert(testOb->color_to_sRGB(0.75), "==", 225),
        assert(testOb->color_to_sRGB(1.0), "==", 255),
    }) :));

    expect("converting from sRGB behaves", (: ({
        assert(testOb->color_from_sRGB(0), "==", 0.0),
        assert(testOb->color_from_sRGB(137), "==", 0.25),
        assert(testOb->color_from_sRGB(188), "==", 0.5),
        assert(testOb->color_from_sRGB(225), "==", 0.75),
        assert(testOb->color_from_sRGB(255), "==", 1.0),
    }) :));
}

void test_lerp () {
    expect_function("color_lerp", testOb);

    expect("color_lerp behaves", (: ({
        assert(testOb->color_lerp(0.0, 255.0, 0.0), "==", 0.0),
        assert(testOb->color_lerp(0.0, 255.0, 0.5), "==", 127.5),
        assert(testOb->color_lerp(0.0, 255.0, 1.0), "==", 255.0),
    }) :));
}