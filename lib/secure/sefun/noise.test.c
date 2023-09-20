inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/noise.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_order () {
    return ({
        "test_noise_perlin_helpers",
        "test_generate_permutation",
        "test_noise_perlin",
        "test_noise_simplex",
     });
}

void test_noise_perlin_helpers () {
    expect("noise_perlin_fade smooths values", (: ({
        assert(testOb->noise_perlin_fade(0.0), "==", 0.0),
        assert(testOb->noise_perlin_fade(0.20), "==", 0.057920),
        assert(testOb->noise_perlin_fade(0.25), "==", 0.103516),
        assert(testOb->noise_perlin_fade(0.40), "==", 0.317440),
        assert(testOb->noise_perlin_fade(0.5), "==", 0.5),
        assert(testOb->noise_perlin_fade(0.60), "==", 0.682560),
        assert(testOb->noise_perlin_fade(0.75), "==", 0.896484),
        assert(testOb->noise_perlin_fade(0.80), "==", 0.942080),
        assert(testOb->noise_perlin_fade(1.0), "==", 1.0),
    }) :));
    expect("noise_perlin_grad provides directional information", (: ({
        assert(testOb->noise_perlin_grad(0,  1.0, 1.0, 1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(1,  1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(2,  1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(3,  1.0, 1.0, 1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(4,  1.0, 1.0, 1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(5,  1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(6,  1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(7,  1.0, 1.0, 1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(8,  1.0, 1.0, 1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(9,  1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(10, 1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(11, 1.0, 1.0, 1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(12, 1.0, 1.0, 1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(13, 1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(14, 1.0, 1.0, 1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(15, 1.0, 1.0, 1.0), "==", -2.0),

        assert(testOb->noise_perlin_grad(0,  -1.0, -1.0, -1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(1,  -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(2,  -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(3,  -1.0, -1.0, -1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(4,  -1.0, -1.0, -1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(5,  -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(6,  -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(7,  -1.0, -1.0, -1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(8,  -1.0, -1.0, -1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(9,  -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(10, -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(11, -1.0, -1.0, -1.0), "==",  2.0),
        assert(testOb->noise_perlin_grad(12, -1.0, -1.0, -1.0), "==", -2.0),
        assert(testOb->noise_perlin_grad(13, -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(14, -1.0, -1.0, -1.0), "==",  0.0),
        assert(testOb->noise_perlin_grad(15, -1.0, -1.0, -1.0), "==",  2.0),
    }) :));
    expect("noise_perlin_lerp weights two numbers", (: ({
        assert(testOb->noise_perlin_lerp(-0.5, 0.0, 10.0), "==", 0.0),
        assert(testOb->noise_perlin_lerp(0.0, 0.0, 10.0), "==", 0.0),
        assert(testOb->noise_perlin_lerp(0.25, 0.0, 10.0), "==", 2.5),
        assert(testOb->noise_perlin_lerp(0.5, 0.0, 10.0), "==", 5.0),
        assert(testOb->noise_perlin_lerp(0.75, 0.0, 10.0), "==", 7.5),
        assert(testOb->noise_perlin_lerp(1.0, 0.0, 10.0), "==", 10.0),
        assert(testOb->noise_perlin_lerp(1.5, 0.0, 10.0), "==", 10.0),
    }) :));
}

void test_generate_permutation () {
    expect("noise_generate_permutation returns different permutations", (: ({
        // default permutation
        assert(testOb->noise_generate_permutation(0), "==", ({ 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 })),

        // seeded permutations
        assert(testOb->noise_generate_permutation("test"), "==", ({ 63, 198, 254, 50, 16, 207, 216, 8, 89, 208, 158, 26, 13, 70, 33, 194, 111, 61, 121, 153, 52, 188, 93, 150, 180, 105, 172, 41, 4, 135, 146, 209, 173, 211, 95, 12, 239, 40, 225, 20, 200, 56, 215, 78, 27, 15, 228, 250, 179, 238, 139, 161, 42, 132, 248, 19, 3, 127, 81, 77, 141, 73, 119, 253, 166, 74, 156, 167, 171, 45, 202, 148, 255, 125, 168, 195, 227, 147, 165, 185, 94, 193, 226, 96, 124, 97, 243, 169, 246, 106, 98, 51, 102, 75, 245, 242, 190, 219, 17, 131, 71, 31, 130, 88, 25, 68, 82, 210, 117, 159, 164, 57, 231, 244, 84, 72, 251, 249, 36, 186, 62, 35, 59, 183, 223, 90, 92, 205, 163, 138, 54, 7, 21, 46, 201, 206, 235, 34, 213, 87, 230, 38, 196, 85, 100, 101, 217, 240, 157, 212, 182, 187, 116, 247, 203, 2, 154, 23, 222, 53, 145, 60, 152, 11, 114, 214, 22, 18, 143, 49, 142, 113, 86, 189, 109, 191, 48, 108, 0, 140, 24, 162, 134, 76, 175, 32, 66, 43, 120, 197, 44, 149, 233, 14, 29, 224, 107, 1, 234, 252, 218, 65, 64, 178, 129, 126, 118, 83, 112, 99, 144, 104, 67, 58, 91, 155, 80, 103, 181, 199, 137, 28, 128, 136, 30, 204, 160, 192, 39, 37, 151, 241, 6, 9, 170, 110, 79, 123, 69, 237, 232, 10, 174, 55, 5, 177, 115, 229, 236, 184, 221, 220, 122, 176, 133, 47 })),
        assert(testOb->noise_generate_permutation("seed"), "==", ({ 160, 49, 46, 164, 239, 171, 18, 113, 78, 172, 65, 20, 75, 17, 221, 188, 29, 99, 124, 252, 44, 112, 82, 233, 98, 62, 225, 244, 241, 85, 116, 101, 33, 144, 143, 7, 201, 150, 159, 211, 3, 246, 108, 80, 235, 206, 102, 230, 92, 12, 142, 87, 86, 19, 0, 156, 251, 199, 146, 2, 81, 197, 198, 77, 203, 122, 176, 16, 1, 195, 51, 105, 141, 214, 194, 32, 175, 134, 119, 133, 40, 9, 93, 169, 157, 174, 79, 226, 27, 245, 54, 41, 4, 180, 254, 114, 121, 193, 181, 154, 182, 155, 131, 22, 45, 250, 126, 95, 94, 66, 111, 153, 227, 213, 162, 118, 209, 223, 117, 231, 74, 236, 8, 53, 215, 68, 152, 67, 242, 161, 208, 186, 63, 212, 248, 220, 166, 59, 170, 149, 129, 57, 84, 253, 21, 255, 56, 36, 47, 240, 139, 130, 91, 58, 69, 128, 191, 25, 37, 42, 179, 173, 34, 73, 38, 228, 23, 247, 127, 14, 234, 151, 163, 123, 177, 147, 232, 115, 107, 243, 50, 6, 132, 96, 185, 61, 10, 187, 136, 238, 183, 189, 5, 70, 48, 55, 26, 137, 229, 145, 165, 210, 110, 120, 71, 140, 178, 217, 39, 13, 135, 168, 76, 88, 104, 100, 222, 207, 125, 249, 148, 28, 224, 237, 216, 219, 200, 97, 52, 138, 167, 15, 64, 24, 31, 35, 90, 30, 196, 11, 83, 204, 184, 109, 218, 202, 106, 158, 43, 89, 72, 103, 190, 60, 205, 192 })),
        assert(testOb->noise_generate_permutation("a really long seed"), "==", ({ 43, 65, 80, 97, 110, 114, 74, 219, 105, 229, 218, 251, 117, 75, 211, 68, 180, 85, 174, 235, 197, 249, 46, 56, 158, 194, 163, 69, 112, 231, 195, 102, 208, 72, 49, 70, 237, 165, 124, 193, 58, 53, 5, 254, 92, 41, 183, 30, 57, 115, 196, 130, 188, 94, 156, 77, 170, 13, 222, 135, 173, 162, 181, 88, 187, 139, 73, 59, 4, 176, 79, 21, 160, 104, 81, 245, 137, 91, 50, 189, 3, 93, 201, 39, 168, 76, 205, 164, 11, 238, 83, 171, 126, 153, 18, 10, 217, 82, 111, 67, 166, 186, 113, 42, 198, 28, 8, 27, 149, 78, 178, 16, 84, 33, 128, 98, 38, 15, 157, 145, 25, 144, 36, 99, 106, 127, 200, 233, 240, 234, 51, 255, 190, 230, 192, 241, 209, 9, 167, 32, 236, 227, 221, 35, 250, 204, 7, 199, 182, 133, 119, 6, 34, 141, 63, 108, 120, 138, 123, 242, 116, 184, 226, 239, 29, 175, 253, 202, 109, 207, 26,  185, 161, 89, 71, 64, 19, 54, 134, 232, 154, 152, 246, 61, 103, 143, 179, 159, 40, 121, 169, 252, 125, 22, 147, 206, 1, 55, 131, 31, 48, 203, 45, 140, 225, 52, 12, 14, 44, 172, 224, 247, 2, 214, 17, 136, 20, 215, 62, 90, 107, 66, 132, 146, 177, 129, 87, 122, 191, 151, 100, 37, 244, 248, 101, 212, 96, 23, 228, 213, 150, 86, 210, 243, 148, 220, 223, 60, 24, 0, 47, 155, 118, 216, 95, 142 })),
    }) :));
    expect("noise_generate_permutation_simplex returns different permutations", (: ({
        // default permutation
        assert(testOb->noise_generate_permutation_simplex(0), "==", ([
            "p": ({ 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 }),
            "w": ({ -1, 1, -1, 0, 0, -1, -1, -1, -1, 0, 1, -1, 1, -1, -1, -1, 1, 1, -1, 0, -1, 1, 1, -1, -1, 1, -1, 1, -1, 0, 1, 1, -1, 0, 1, -1, 1, 0, -1, 0, 0, 0, 0, -1, -1, -1, -1, 1, 0, -1, -1, 0, -1, -1, 0, -1, 1, 1, 0, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 0, 1, -1, 1, 0, -1, -1, -1, -1, 0, 0, -1, -1, 1, 0, -1, 0, -1, -1, 1, -1, 1, 1, 1, -1, 1, -1, 0, 0, -1, -1, 0, 1, -1, -1, 1, 1, 0, 1, 0, 1, -1, 1, 1, -1, 1, 1, 0, 0, 1, 1, 1, -1, 1, -1, 0, -1, 1, 1, 0, 1, 0, 0, 0, -1, 1, 1, -1, 1, 0, 0, 1, -1, 1, -1, 1, 0, -1, -1, 1, 0, -1, 1, 0, 0, 1, 0, -1, 1, -1, -1, 0, 0, 1, 1, 0, -1, 1, 0, 0, -1, 0, -1, -1, 1, -1, -1, 1, -1, 0, -1, 1, 1, 1, -1, -1, 1, 1, 1, 0, 1, 1, 0, 1, -1, 1, 0, 1, 1, -1, 1, 1, 1, 1, 0, -1, 1, -1, -1, -1, -1, -1, 0, 1, -1, -1, -1, 1, 1, 0, -1, -1, 1, 0, 0, 1, 1, 1, -1, 0, 1, -1, 0, 1, 1, 0, 1, 1, -1, 0, 0, 1, -1, 0, 0, 1, -1, -1, 1, -1, 1, 1, -1, 0, 0, 1 }),
            "x": ({ -1, 0, 1, 1, 1, -1, 0, -1, 1, -1, 0, -1, 0, 1, 0, 0, -1, 0, 0, -1, 0, -1, 1, 0, 0, 1, -1, 1, -1, -1, 0, -1, -1, 1, 1, 1, 0, 1, 0, -1, -1, -1, 1, 1, -1, 0, 1, 0, 1, 1, 0, 1, -1, -1, 1, -1, -1, -1, -1, 1, 1, 1, 0, -1, 1, 0, 0, 0, 1, 1, 1, 0, -1, 1, -1, 0, 1, -1, 0, 1, -1, 1, 0, 0, -1, 1, -1, 1, -1, -1, -1, 1, -1, 0, -1, -1, 0, 1, -1, 0, 0, 1, 1, 1, 1, -1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1, 0, 0, -1, 0, -1, 1, 0, 0, -1, 1, 0, 1, -1, 1, 0, 1, 0, 1, -1, -1, -1, 1, -1, -1, -1, -1, 1, 0, -1, 1, 1, 1, -1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, 0, -1, 1, 0, 0, -1, 1, 0, 1, 0, 1, -1, 1, 0, -1, 0, -1, 1, 0, -1, -1, -1, 1, 0, 1, 1, 1, 1, 1, 1, -1, 0, 0, 1, 0, 1, 0, -1, 1, 1, -1, -1, 1, 0, 1, 1, 1, 1, 1, 1, -1, -1, 1, 1, 0, -1, -1, -1, 0, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, -1, 0, -1, -1, 1, -1, -1, -1, -1, 1, 0, -1, 1, 1, 1, -1, 0, 0, -1, 0, -1, -1, -1, -1 }),
            "y": ({ -1, 1, 0, -1, -1, 0, 1, 0, 0, -1, 1, 1, 1, 0, -1, 1, 0, -1, -1, -1, -1, 0, 0, 1, -1, 1, 1, 0, -1, -1, -1, 1, -1, 1, 0, 0, 1, -1, -1, -1, -1, 1, -1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, -1, 0, 1, 1, 0, 0, 0, -1, 0, 0, -1, -1, -1, 0, 1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, 1, -1, -1, -1, 1, 0, 1, 0, -1, 0, 1, 0, 1, -1, 0, -1, 1, 1, -1, 1, 1, 1, 1, 0, 1, 0, -1, -1, 1, 1, -1, 0, 0, -1, -1, 1, 1, 1, -1, -1, -1, -1, 0, -1, 0, -1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 1, 1, 0, 0, -1, 1, 0, 1, -1, 1, -1, 1, 1, 0, -1, -1, 0, 1, -1, 1, 1, 1, 0, -1, 1, -1, 1, 1, -1, -1, -1, 0, 0, 0, 1, -1, -1, 1, -1, 1, 0, 0, 0, 1, 1, 0, -1, 1, 1, 0, -1, -1, 1, -1, -1, 1, -1, 1, 0, -1, 1, 0, -1, -1, 1, 1, 1, -1, 1, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, -1, 0, 1, 1, 1, 0, 1, -1, 1, -1, 0, -1, -1, -1, -1, 0, 0, 0, 1, -1, -1, 1, 1, 1, 0, -1, 0, 1, 1, 0, 1, -1, 1, 1, 1 }),
            "z": ({ 0, 1, 1, -1, 1, -1, -1, 1, 1, -1, 1, 0, -1, 1, -1, 1, 1, 1, -1, 1, 1, -1, 1, -1, 1, 0, 0, -1, 0, 1, -1, 0, 0, 1, -1, -1, 1, 1, 1, 1, 1, 1, -1, -1, 0, -1, -1, 1, -1, 0, 1, 1, 1, 0, 1, 0, -1, 0, -1, 1, -1, 1, 1, -1, -1, 1, -1, -1, -1, 0, -1, -1, 1, 0, 1, -1, 0, -1, 1, 1, 1, 0, 1, -1, 1, 1, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0, 1, -1, -1, 1, 1, 1, 0, 1, 0, 1, 1, -1, 0, -1, 0, 1, 1, 1, -1, -1, 0, 1, -1, 0, 1, 1, 1, -1, 1, 1, -1, 1, 0, -1, -1, 1, 1, -1, 1, -1, -1, 0, 0, 1, -1, 0, 0, 0, -1, -1, -1, -1, -1, 0, 1, 0, 0, -1, 1, -1, -1, 0, -1, 0, 0, 1, 1, -1, 1, 1, -1, -1, -1, -1, 1, -1, -1, -1, 1, 1, 1, 0, -1, -1, 0, -1, 1, -1, -1, 0, 1, 1, 0, -1, 0, 1, 1, 0, 1, 1, -1, -1, 0, 1, -1, 0, 0, 1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 1, 0, -1, 0, -1, -1, -1, 1, 0, 1, 0, 0, -1, 0, 1, -1, 1, 0, 1, -1, 1, 1, -1, 1, 0, -1, -1, 1, 1, 0, 1, 1, -1, -1, -1, 0, -1, 1, 0 })
        ])),


         // seeded permutations
        assert(testOb->noise_generate_permutation_simplex("test"), "==", ([
            "p": ({ 63, 198, 254, 50, 16, 207, 216, 8, 89, 208, 158, 26, 13, 70, 33, 194, 111, 61, 121, 153, 52, 188, 93, 150, 180, 105, 172, 41, 4, 135, 146, 209, 173, 211, 95, 12, 239, 40, 225, 20, 200, 56, 215, 78, 27, 15, 228, 250, 179, 238, 139, 161, 42, 132, 248, 19, 3, 127, 81, 77, 141, 73, 119, 253, 166, 74, 156, 167, 171, 45, 202, 148, 255, 125, 168, 195, 227, 147, 165, 185, 94, 193, 226, 96, 124, 97, 243, 169, 246, 106, 98, 51, 102, 75, 245, 242, 190, 219, 17, 131, 71, 31, 130, 88, 25, 68, 82, 210, 117, 159, 164, 57, 231, 244, 84, 72, 251, 249, 36, 186, 62, 35, 59, 183, 223, 90, 92, 205, 163, 138, 54, 7, 21, 46, 201, 206, 235, 34, 213, 87, 230, 38, 196, 85, 100, 101, 217, 240, 157, 212, 182, 187, 116, 247, 203, 2, 154, 23, 222, 53, 145, 60, 152, 11, 114, 214, 22, 18, 143, 49, 142, 113, 86, 189, 109, 191, 48, 108, 0, 140, 24, 162, 134, 76, 175, 32, 66, 43, 120, 197, 44, 149, 233, 14, 29, 224, 107, 1, 234, 252, 218, 65, 64, 178, 129, 126, 118, 83, 112, 99, 144, 104, 67, 58, 91, 155, 80, 103, 181, 199, 137, 28, 128, 136, 30, 204, 160, 192, 39, 37, 151, 241, 6, 9, 170, 110, 79, 123, 69, 237, 232, 10, 174, 55, 5, 177, 115, 229, 236, 184, 221, 220, 122, 176, 133, 47 }),
            "x": ({ -1, 0, -1, 1, 1, -1, 1, 1, 1, 1, -1, 1, -1, 0, 0, 0, -1, -1, 1, 1, -1, -1, -1, -1, -1, 1, -1, 1, 0, 0, 1, 1, -1, 1, -1, -1, -1, 1, 0, -1, 1, 1, -1, -1, 1, -1, 0, 1, 1, -1, 1, 0, 1, 0, 1, 1, 0, -1, 1, -1, -1, 1, -1, -1, 0, 1, -1, 0, 1, -1, 1, -1, -1, -1, 1, 0, 0, 1, 0, 1, -1, 0, 0, 0, -1, 0, 1, 1, -1, 1, 0, 1, 0, 1, -1, 1, -1, 1, 1, 0, 0, -1, 0, 1, 1, 0, 1, 1, -1, -1, 0, 1, 0, -1, -1, 1, 1, 1, 0, 1, -1, 0, 1, -1, -1, 1, -1, -1, 0, 1, -1, 0, -1, -1, 1, -1, 1, 0, -1, -1, 0, 0, 0, -1, 0, 0, 1, 1, -1, -1, -1, 1, -1, -1, 1, 0, 1, -1, -1, -1, 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1, -1, -1, 1, -1, 0, -1, 1, 0, 0, -1, -1, 0, 0, 1, 1, 0, -1, -1, 1, -1, -1, 0, 1, 0, 1, -1, 1, 0, 0, 1, 0, -1, -1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, -1, 0, 1, -1, 0, 1, -1, -1, 0, 0, 0, 0, -1, 1, 0, 1, 1, -1, -1, 1, 0, -1, 1, 1, -1, -1, 0, 1, 1, 0, -1, 1, -1, -1, 1, 1, 0, -1 }),
            "y": ({ -1, -1, -1, -1, 1, 0, 1, 0, 1, 1, -1, -1, 0, -1, 1, 1, 0, 1, 1, 1, 1, 1, 1, -1, 1, 0, 0, 0, -1, -1, -1, 1, 0, -1, -1, 0, 0, 0, 1, 1, 0, 1, -1, 0, -1, 0, -1, -1, -1, 0, 0, 1, 0, -1, 1, -1, 1, -1, 1, 0, 0, 0, -1, 1, -1, 0, 1, -1, 0, 0, 0, 1, -1, 1, 0, 1, 1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 0, -1, 0, 1, -1, -1, 0, 1, -1, -1, -1, 1, 1, -1, -1, 1, 1, 1, -1, -1, -1, 1, -1, -1, 1, -1, 1, 1, 0, -1, 1, -1, -1, -1, 1, -1, -1, -1, -1, 1, 0, 1, 0, -1, -1, 1, 0, 0, 0, 0, 1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1, 1, 1, -1, -1, 1, -1, 0, 1, -1, -1, -1, 1, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 0, 1, -1, 1, 0, -1, 1, 0, 1, 0, 1, 1, -1, 0, 0, 1, 1, 0, 1, -1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, -1, 1, 1, -1, 1, -1, -1, -1, 1, 1, 1, 0, 1, -1, -1, -1, 1, -1, 1, -1, 0, 1, 1, 0, -1, 0, 1, 1, -1, -1, -1, 1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 1, -1, -1, 0, 1, 1, 1, -1, 1, -1, 0 }),
            "z": ({ -1, -1, 1, 0, 0, -1, 1, 1, -1, 0, 1, 1, 1, -1, 1, -1, -1, -1, -1, -1, 0, 1, -1, 0, 0, 1, 1, 1, 1, -1, 0, 0, 1, 0, -1, 1, -1, 1, 1, 0, 1, 1, 0, -1, -1, -1, 1, 1, 0, -1, -1, 1, -1, 1, 1, 0, -1, -1, 0, 1, 1, 1, 0, -1, -1, -1, 1, -1, -1, 1, -1, 0, -1, -1, 1, -1, -1, 0, 1, -1, 1, 1, -1, 1, 1, 1, 0, 1, 0, -1, -1, 0, -1, -1, 0, 0, 1, -1, 0, -1, -1, -1, -1, 1, -1, 1, 0, 0, 0, -1, 1, -1, -1, 0, 0, 1, -1, -1, 1, 1, 1, -1, -1, 0, -1, 1, 1, 1, -1, -1, 0, -1, 0, -1, 1, -1, -1, -1, 0, 0, -1, -1, 1, 0, 1, 1, -1, 0, -1, 0, 0, -1, 0, 0, -1, -1, 1, 0, 1, 0, 0, 1, 1, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, -1, 1, -1, 0, 1, 1, 1, 1, -1, -1, 1, -1, 1, -1, -1, 1, 1, 1, 0, 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, -1, 0, 1, -1, 1, -1, -1, 0, -1, 0, -1, 1, 1, 1, 1, 1, 1, 1, 1, -1, 1, 0, 0, -1, 1, -1, -1, -1, -1, 1, 1, 1, -1, -1, 0, 1, 0, 0, 1, 1, 1, -1, 1, 1, 0, 1, -1 }),
            "w": ({ 0, 1, 0, 1, 1, -1, 0, 1, 0, 1, 0, 0, -1, 1, -1, 1, -1, 0, 0, 0, 1, 0, 0, 1, 1, -1, 1, -1, 1, -1, 1, -1, -1, -1, 0, 1, -1, 1, -1, 1, 1, 0, -1, 1, 0, -1, 1, 0, -1, 1, -1, -1, 1, 1, 0, -1, -1, 0, -1, -1, -1, -1, -1, 0, 1, 1, 0, -1, -1, -1, 1, 1, 0, 0, 1, -1, -1, -1, -1, 0, 0, -1, 1, 1, 0, -1, -1, -1, 1, 1, 1, -1, 1, -1, -1, 1, 0, 0, -1, -1, -1, 0, 1, 0, 0, 1, 1, 1, -1, 0, 1, 0, -1, 1, 1, 1, 0, 0, 1, 0, 0, -1, 0, -1, 0, 0, 0, -1, -1, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1, 1, 1, 1, -1, 1, -1, 0, 1, 0, 1, 1, 0, 1, -1, -1, 1, 0, -1, 0, -1, -1, 0, 0, -1, 1, 1, 1, 1, -1, -1, 1, -1, 1, 0, -1, 0, 1, 1, 1, 1, 0, 1, 1, 1, -1, 1, 1, -1, 0, -1, 1, -1, -1, 1, 0, 1, -1, -1, 1, 0, 0, -1, 1, 1, -1, 0, 1, -1, 1, -1, 1, 1, -1, 0, 0, 0, 1, -1, -1, -1, -1, 0, 1, 1, 0, 1, 1, 1, -1, -1, -1, -1, 1, -1, 1, 1, -1, 0, -1, -1, 1, 1, 1, -1, -1, -1, -1, -1, 1, 0, 0, 0, 0, 1, -1, -1 }),
        ])),
        assert(testOb->noise_generate_permutation_simplex("seed"), "==", ([
            "p": ({ 160, 49, 46, 164, 239, 171, 18, 113, 78, 172, 65, 20, 75, 17, 221, 188, 29, 99, 124, 252, 44, 112, 82, 233, 98, 62, 225, 244, 241, 85, 116, 101, 33, 144, 143, 7, 201, 150, 159, 211, 3, 246, 108, 80, 235, 206, 102, 230, 92, 12, 142, 87, 86, 19, 0, 156, 251, 199, 146, 2, 81, 197, 198, 77, 203, 122, 176, 16, 1, 195, 51, 105, 141, 214, 194, 32, 175, 134, 119, 133, 40, 9, 93, 169, 157, 174, 79, 226, 27, 245, 54, 41, 4, 180, 254, 114, 121, 193, 181, 154, 182, 155, 131, 22, 45, 250, 126, 95, 94, 66, 111, 153, 227, 213, 162, 118, 209, 223, 117, 231, 74, 236, 8, 53, 215, 68, 152, 67, 242, 161, 208, 186, 63, 212, 248, 220, 166, 59, 170, 149, 129, 57, 84, 253, 21, 255, 56, 36, 47, 240, 139, 130, 91, 58, 69, 128, 191, 25, 37, 42, 179, 173, 34, 73, 38, 228, 23, 247, 127, 14, 234, 151, 163, 123, 177, 147, 232, 115, 107, 243, 50, 6, 132, 96, 185, 61, 10, 187, 136, 238, 183, 189, 5, 70, 48, 55, 26, 137, 229, 145, 165, 210, 110, 120, 71, 140, 178, 217, 39, 13, 135, 168, 76, 88, 104, 100, 222, 207, 125, 249, 148, 28, 224, 237, 216, 219, 200, 97, 52, 138, 167, 15, 64, 24, 31, 35, 90, 30, 196, 11, 83, 204, 184, 109, 218, 202, 106, 158, 43, 89, 72, 103, 190, 60, 205, 192 }),
            "x": ({ 0, 1, -1, 0, -1, 1, 1, 1, -1, -1, 0, -1, 1, 1, -1, -1, -1, 0, -1, -1, -1, 1, 1, 1, 0, -1, 0, -1, 1, -1, -1, 0, 0, 1, -1, 0, 1, -1, -1, 1, 0, -1, -1, 1, 1, -1, 0, 0, -1, -1, -1, -1, -1, 1, 0, -1, 1, 0, 1, 0, 1, 0, 0, -1, 1, 1, 1, 1, 0, 0, 1, 1, -1, -1, 0, 0, -1, 0, -1, 0, 1, 1, -1, 1, -1, -1, -1, 0, 1, -1, -1, 1, 0, -1, -1, 1, 1, 0, -1, 1, -1, 1, 0, -1, -1, 1, -1, -1, -1, 0, -1, 1, 0, -1, 0, -1, 1, -1, -1, 0, 1, -1, 1, -1, -1, 0, 1, 0, 1, 0, 1, 1, -1, -1, 1, -1, 0, 1, 1, -1, 0, 1, -1, -1, -1, -1, 1, 0, -1, 1, 1, 0, 1, 1, 0, 0, -1, 1, 0, 1, 1, -1, 0, 1, 0, 0, -1, -1, -1, -1, 1, -1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, -1, 1, 1, 1, -1, -1, -1, 0, 0, 1, -1, 1, 1, 0, 1, 0, 1, -1, 1, 0, -1, 1, 1, 0, -1, 0, 1, -1, 1, 1, 0, -1, -1, -1, 1, -1, -1, 0, -1, 1, 1, 1, 0, -1, 1, 0, -1, 0, 1, -1, 0, 1, -1, 0, 1, 1, -1, 1, -1, 1, 1, 1, -1, 1, 1, 1, 0, -1, -1, -1, 0 }),
            "y": ({ 1, 1, 0, -1, 0, 0, -1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, -1, 0, 1, -1, 1, 1, 1, 1, 1, -1, 1, 1, 0, -1, 0, -1, -1, -1, 1, -1, 0, 1, 0, 0, -1, -1, 1, 0, 0, -1, -1, -1, 1, 1, -1, -1, -1, 1, 1, -1, -1, 0, 0, -1, 1, 1, 1, 1, -1, 0, 0, -1, 1, 1, 0, -1, -1, -1, 0, 0, 1, 0, 1, 0, 0, 1, -1, 1, -1, 0, -1, 1, -1, -1, 1, 1, 1, -1, -1, -1, 1, -1, 0, -1, -1, -1, -1, 1, 0, 1, 1, 1, 1, -1, 1, -1, 1, -1, 0, 0, 0, 1, -1, -1, 1, 1, -1, 1, 1, -1, -1, 1, 1, 1, -1, -1, 0, 1, 1, 1, 1, 1, 1, -1, 1, -1, 0, 1, 0, 1, -1, -1, -1, 1, -1, 1, -1, 0, -1, 0, 1, 0, -1, -1, -1, -1, -1, 0, 0, -1, 1, -1, 1, -1, 0, -1, 0, -1, -1, -1, -1, 1, 1, 1, 0, -1, 0, 0, -1, 1, -1, -1, 1, -1, -1, 0, -1, 1, -1, -1, 0, 1, -1, 0, -1, 1, -1, 0, -1, 0, 0, 1, 0, -1, -1, 0, 1, 1, 1, 1, 1, 0, 1, -1, 0, 1, 1, 0, -1, 0, 1, 1, -1, 1, -1, -1, -1, 0, -1, 0, 1, 0, -1, 0, 0, -1, 0, 1, 0, -1, -1, 1, 0, 1 }),
            "z": ({ 1, 0, -1, 1, -1, -1, 0, 0, -1, 1, 1, 0, -1, 0, -1, 1, -1, -1, 1, 1, 1, 0, 0, 1, -1, 1, 1, 0, 0, 0, 0, 1, 1, 0, -1, -1, 1, 0, -1, 0, -1, 0, 1, 0, -1, -1, -1, -1, 1, 1, -1, 0, 0, 0, 1, 1, -1, -1, 0, -1, 0, 1, -1, 1, -1, 1, 0, 0, 1, -1, 0, 1, 1, 0, -1, 1, -1, -1, 0, 1, 1, 1, -1, 1, -1, -1, -1, -1, -1, 0, 0, 1, 1, 0, 1, 0, -1, 1, 0, 1, 0, -1, -1, 0, 1, 1, 1, -1, 1, -1, -1, -1, -1, 0, -1, 0, 0, -1, 0, -1, -1, 1, 1, 0, 0, 1, 1, -1, 0, 1, 0, 1, -1, 0, 1, 1, -1, -1, -1, 0, 1, -1, 0, -1, 0, -1, 1, 1, -1, 0, -1, -1, -1, 1, 1, 1, -1, -1, 1, -1, 0, 1, -1, 1, -1, 1, 0, 0, -1, -1, -1, 0, -1, -1, 0, 0, 1, 0, -1, 0, 0, -1, 1, 1, -1, -1, -1, -1, 1, -1, 0, -1, 1, -1, 0, 0, 1, 1, 1, 0, 1, 0, -1, 1, -1, 1, 0, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, -1, -1, 0, 1, 1, 1, 1, -1, 1, 1, 0, -1, -1, -1, 1, 1, -1, -1, 1, 1, 1, -1, 0, 1, 1, 1, 1, -1, -1, 1, -1, -1, 1, -1, 1, 1, 1, 1 }),
            "w": ({ 1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 0, 0, 0, -1, 0, 0, 1, 1, 1, -1, 1, 0, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, -1, 1, 0, -1, -1, 1, 1, 1, -1, 1, 1, 1, 0, 1, 1, -1, 1, -1, 1, 0, 0, -1, 1, 1, -1, -1, 1, -1, -1, 0, 1, 1, -1, -1, -1, -1, -1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 0, -1, 0, 1, -1, 1, 0, -1, 1, -1, 1, 1, 0, 1, 0, -1, -1, 0, 1, 0, -1, 1, -1, 0, 0, 0, 0, 1, -1, 0, -1, -1, 1, 1, -1, 0, -1, -1, 1, 1, 1, -1, -1, 1, 0, -1, 1, -1, 1, 0, 0, 1, 0, 0, 1, 0, 1, -1, -1, 0, 1, 0, -1, 0, 0, 1, -1, 1, -1, 1, 0, 0, -1, 1, 0, 0, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 0, 1, 1, -1, -1, 0, -1, -1, 1, -1, -1, -1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, -1, 0, -1, 1, 1, -1, 0, -1, -1, -1, -1, 1, 1, 0, -1, 1, 1, 0, -1, -1, -1, 1, 1, 0, 1, 1, 0, -1, 0, 0, 1, 0, 1, -1, 0, 0, 1, -1, 1, 1, -1, -1, 1, 0, 0, -1, 0, 0, 1, -1, -1, 1, 0, -1, 0, 1, 1, 0, -1, 0, 1, -1, 0, 0, -1, 1 }),
        ])),
        assert(testOb->noise_generate_permutation_simplex("a really long seed"), "==", ([
            "p": ({ 43, 65, 80, 97, 110, 114, 74, 219, 105, 229, 218, 251, 117, 75, 211, 68, 180, 85, 174, 235, 197, 249, 46, 56, 158, 194, 163, 69, 112, 231, 195, 102, 208, 72, 49, 70, 237, 165, 124, 193, 58, 53, 5, 254, 92, 41, 183, 30, 57, 115, 196, 130, 188, 94, 156, 77, 170, 13, 222, 135, 173, 162, 181, 88, 187, 139, 73, 59, 4, 176, 79, 21, 160, 104, 81, 245, 137, 91, 50, 189, 3, 93, 201, 39, 168, 76, 205, 164, 11, 238, 83, 171, 126, 153, 18, 10, 217, 82, 111, 67, 166, 186, 113, 42, 198, 28, 8, 27, 149, 78, 178, 16, 84, 33, 128, 98, 38, 15, 157, 145, 25, 144, 36, 99, 106, 127, 200, 233, 240, 234, 51, 255, 190, 230, 192, 241, 209, 9, 167, 32, 236, 227, 221, 35, 250, 204, 7, 199, 182, 133, 119, 6, 34, 141, 63, 108, 120, 138, 123, 242, 116, 184, 226, 239, 29, 175, 253, 202, 109, 207, 26,  185, 161, 89, 71, 64, 19, 54, 134, 232, 154, 152, 246, 61, 103, 143, 179, 159, 40, 121, 169, 252, 125, 22, 147, 206, 1, 55, 131, 31, 48, 203, 45, 140, 225, 52, 12, 14, 44, 172, 224, 247, 2, 214, 17, 136, 20, 215, 62, 90, 107, 66, 132, 146, 177, 129, 87, 122, 191, 151, 100, 37, 244, 248, 101, 212, 96, 23, 228, 213, 150, 86, 210, 243, 148, 220, 223, 60, 24, 0, 47, 155, 118, 216, 95, 142 }),
            "x": ({ 1, 0, 1, 0, -1, 1, 1, 1, 1, 0, 1, 1, -1, 1, 1, 0, -1, -1, -1, 1, 0, 1, -1, 1, -1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, -1, 0, -1, 0, 1, -1, 0, -1, -1, 1, -1, -1, 1, 1, 0, 0, -1, -1, -1, -1, 1, -1, -1, 0, -1, 0, -1, 1, 1, 1, 1, 1, 0, 1, -1, -1, 0, 1, 1, -1, 1, 1, 1, -1, 0, -1, 1, 0, 1, -1, -1, 0, 1, -1, 1, 1, -1, 1, 1, 1, 1, 1, -1, 0, 0, 1, 1, 1, 0, -1, 1, 1, -1, -1, 1, 1, -1, 0, 0, 0, 0, -1, -1, 1, 1, 1, 0, 0, 1, -1, 1, 1, 1, 1, 1, -1, -1, 0, 0, 1, 1, 1, 0, 0, -1, 0, -1, 0, 1, -1, 0, 0, -1, 0, -1, 0, 0, -1, -1, -1, 1, 1, 1, 1, -1, 1, 0, -1, -1, -1, -1, 1, -1, -1, 1, 1, 0, 1, 0, 0, 1, -1, 0, 1, 1, 1, -1, -1, 0, -1, 1, -1, 1, 1, 1, -1, -1, -1, 1, -1, 0, -1, 0, -1, 1, 1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, 0, -1, 1, 1, -1, -1, -1, 1, 1, 0, 0, 1, 1, 0, -1, 1, -1, -1, 0, 0, -1, 1, 0, -1, 0, -1, 0, -1, -1, -1, 1, 1, -1, -1, -1, -1, 1, 0, -1, 1, -1, 1, -1, -1 }),
            "y": ({ 0, 1, 1, 1, 0, -1, 0, -1, 0, -1, -1, -1, 1, 0, -1, -1, 1, 1, 0, 0, -1, 1, 0, 1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 0, 1, -1, 0, -1, 1, 1, -1, 1, -1, -1, 1, 0, -1, -1, 1, -1, -1, 1, 1, -1, 1, 0, 0, 0, -1, -1, 0, 1, 1, 1, -1, 0, 0, -1, -1, 1, 0, 1, 1, 0, 1, 1, 0, -1, -1, 1, 1, 1, 0, -1, 0, 0, 0, -1, 0, 0, -1, 0, -1, 1, -1, 0, 1, -1, 0, 1, -1, -1, 1, 0, -1, 1, 0, -1, 1, 0, -1, 1, 1, 1, 1, 1, -1, 0, 1, 1, 1, 1, -1, 1, 0, -1, 0, 0, 1, 0, -1, -1, -1, -1, 1, 1, 1, 0, -1, 1, 0, 1, 1, 1, -1, 0, -1, -1, -1, -1, -1, -1, 1, 0, -1, 0, 1, 0, -1, -1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, -1, 1, 1, 1, -1, 1, -1, -1, -1, 0, -1, 1, -1, 1, -1, 0, -1, -1, 0, 1, 0, 1, 1, -1, -1, 0, 1, -1, 1, -1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, -1, 1, -1, 1, 0, 1, -1, -1, -1, 0, 1, -1, -1, 1, 1, -1, -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, -1, -1, 1, -1, -1, -1, -1, 1, 1, -1, 1, 1, 1, 0, -1, -1, 1, -1, 0 }),
            "z": ({ -1, 1, 0, 1, -1, 0, -1, -1, 1, 1, 1, -1, 0, -1, 0, 1, 0, 0, -1, -1, 1, -1, -1, 1, 1, -1, -1, 1, 0, -1, -1, -1, 0, 1, 0, -1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 0, -1, 0, 1, 1, 0, 0, 1, -1, 0, -1, -1, -1, 1, -1, 1, 1, 1, 1, -1, -1, 0, -1, 1, -1, 0, -1, -1, 0, -1, -1, -1, 1, 0, -1, -1, 1, 1, -1, 0, -1, 0, 0, 0, 1, 1, -1, -1, -1, -1, 0, -1, 0, 1, -1, -1, -1, 1, 1, 0, -1, 0, -1, 1, -1, 1, 0, 0, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1, -1, 0, 1, 0, -1, -1, 1, -1, 1, 1, -1, -1, 0, 0, 1, -1, -1, -1, -1, -1, -1, 1, -1, 1, -1, 1, -1, -1, 1, 0, 0, -1, 1, 1, 1, 0, -1, -1, -1, 0, -1, 1, -1, 1, 1, -1, 0, 0, -1, 1, 0, -1, -1, 0, -1, 1, 1, 1, 0, 1, -1, 1, 1, 1, 0, -1, 0, 0, 1, 0, 0, 1, 1, -1, -1, 1, 0, 0, 1, 0, 1, -1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, -1, 1, 1, 1, -1, -1, 0, 1, -1, -1 }),
            "w": ({ -1, -1, 1, -1, 1, 1, 1, 0, -1, -1, 0, 0, -1, -1, -1, 1, 1, -1, 1, -1, -1, 0, 1, 0, 0, 1, -1, -1, 1, -1, -1, 1, 1, 1, -1, 1, -1, -1, 0, -1, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 1, 1, 0, 0, 0, -1, 1, -1, 0, -1, -1, 1, -1, 0, 0, -1, -1, 0, 1, 1, -1, -1, 1, 1, -1, -1, -1, 0, 1, 0, -1, 0, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, 0, 0, 1, 1, 0, 1, -1, -1, 1, 0, -1, 1, 1, 0, 1, 0, -1, 1, 1, 1, 1, -1, 1, 1, 1, -1, 0, -1, 0, 1, 1, -1, 1, 0, 1, -1, 1, 1, -1, 0, 0, 1, 1, -1, -1, -1, -1, 1, 1, -1, 0, -1, 0, 1, -1, -1, 1, -1, -1, 1, 1, -1, 0, 1, 0, 1, 0, 1, 1, 0, 1, -1, 0, -1, 0, 1, -1, -1, 0, 0, -1, 0, -1, 1, -1, 1, 1, 1, 0, 0, 1, 0, -1, -1, -1, 0, 1, 0, -1, 0, 0, 1, -1, 1, -1, -1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 0, 0, -1, 1, 1, 1, -1, -1, -1, 0, 0, -1, 1, -1, 1, 0, -1, 1, 1, -1, 1, -1, 1, 1, 1, -1, 1, 0, 0, 0, 0, 1, -1, 0, 1, 0, 0, 1 }),
        ])),
    }) :));
}

void test_noise_perlin () {
    int *seedZero = testOb->noise_generate_permutation(0);
    int *seedTest = testOb->noise_generate_permutation("test");

    // https://rosettacode.org/wiki/Perlin_noise
    // Perlin noise of 3D point 3.14, 42.0, 7.0 is 0.13691995878400012
    // But Perlin noise of 2D point 3.14, 42.0 is -0.018920253184000042

    expect("noise_perlin_2d_permutation returns correct non-seeded value", (: ({
        assert(testOb->noise_perlin_2d_permutation(3.14, 42.0, $(seedZero)), "==", -0.018920),
    }) :));
    expect("noise_perlin_3d_permutation returns correct non-seeded value", (: ({
        assert(testOb->noise_perlin_3d_permutation(3.14, 42.0, 7.0, $(seedZero)), "==", 0.136920),
        assert(testOb->noise_perlin_3d_permutation(3.14, 42.0, 0.0, $(seedZero)), "==", 0.136920),
    }) :));

    expect("noise_perlin_2d_permutation returns seeded value", (: ({
        assert(testOb->noise_perlin_2d_permutation(3.14, 42.0, $(seedTest)), "==", 0.155840),
    }) :));
    expect("noise_perlin_3d_permutation returns seeded value", (: ({
        assert(testOb->noise_perlin_3d_permutation(3.14, 42.0, 7.0, $(seedTest)), "==", -0.018920),
    }) :));

    expect("noise_perlin_2d returns non-seeded value", (: ({
        assert(testOb->noise_perlin_2d(3.14, 42.0, $(seedZero), 1, 1.0), "==", -0.018920),
        assert(testOb->noise_perlin_2d(3.14, 42.0, $(seedZero), 8, 15.0), "==", 0.211000),
    }) :));
    expect("perlin_noise_3d returns non-seeded value", (: ({
        assert(testOb->perlin_noise_3d(3.14, 42.0, 7.0, $(seedZero), 1, 1.0), "==", 0.136920),
        assert(testOb->perlin_noise_3d(3.14, 42.0, 7.0, $(seedZero), 8, 15.0), "==", 0.133774),
    }) :));

    expect("noise_perlin_2d returns seeded value", (: ({
        assert(testOb->noise_perlin_2d(3.14, 42.0, $(seedTest), 1, 1.0), "==", 0.155840),
        assert(testOb->noise_perlin_2d(3.14, 42.0, $(seedTest), 8, 15.0), "==", -0.024632),
    }) :));
    expect("perlin_noise_3d returns seeded value", (: ({
        assert(testOb->perlin_noise_3d(3.14, 42.0, 7.0, $(seedTest), 1, 1.0), "==", -0.018920),
        assert(testOb->perlin_noise_3d(3.14, 42.0, 7.0, $(seedTest), 8, 15.0), "==", -0.306174),
    }) :));
}

void test_noise_simplex () {
    mapping seedZero = testOb->noise_generate_permutation_simplex(0);
    mapping seedTest = testOb->noise_generate_permutation_simplex("test");

    expect("noise_simplex_4d_permutation returns correct non-seeded value", (: ({
        assert(testOb->noise_simplex_4d_permutation(3.14, 42.0, 7.0, 0.0, $(seedZero)), "==", 0.653330),
    }) :));
    expect("noise_simplex_4d_permutation returns seeded value", (: ({
        assert(testOb->noise_simplex_4d_permutation(3.14, 42.0, 7.0, 0.0, $(seedTest)), "==", -0.480029),
    }) :));
    expect("noise_simplex_4d returns non-seeded value", (: ({
        assert(testOb->noise_simplex_4d(3.14, 42.0, 7.0, 0.0, $(seedZero), 1, 1.0), "==", 0.653330),
        assert(testOb->noise_simplex_4d(3.14, 42.0, 7.0, 0.0, $(seedZero), 8, 15.0), "==", 0.108415),
    }) :));
    expect("noise_simplex_4d returns seeded value", (: ({
        assert(testOb->noise_simplex_4d(3.14, 42.0, 7.0, 0.0, $(seedTest), 1, 1.0), "==", -0.480029),
        assert(testOb->noise_simplex_4d(3.14, 42.0, 7.0, 0.0, $(seedTest), 8, 15.0), "==", 0.514248),
    }) :));
}