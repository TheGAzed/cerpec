#include <set/fsc_hash_set.h>
#include <dodac.h>

#include <suite.h>

#define FSC_HASH_SET_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    ASSERT_EQ(0, set.length);
    ASSERT_NEQ(0, set.size);
    ASSERT_NEQ(NULL, set.hash);

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    destroy_fsc_hash_set(&set, intdst, NULL);

    ASSERT_EQ(0, set.length);
    ASSERT_EQ(0, set.size);
    ASSERT_EQ(NULL, set.hash);

    PASS();
}

TEST CLEAR_01(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    clear_fsc_hash_set(&set, intdst, NULL);

    ASSERT_EQ(0, set.length);
    ASSERT_NEQ(0, set.size);
    ASSERT_NEQ(NULL, set.hash);

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST INSERT_01(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set, &i);
    }

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST INSERT_02(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set, &i);
    }

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST REMOVE_01(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set, &i);
    }

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        int buf = 0;
        remove_fsc_hash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST REMOVE_02(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set, &i);
    }

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        int buf = 0;
        remove_fsc_hash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST CONTAINS_01(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set, &i);
    }

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set, &i));
    }

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST CONTAINS_02(void) {
    fsc_hash_set_s set = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set, &i);
    }

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set, &i));
    }

    destroy_fsc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST UNION_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_union = union_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set_union, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_union = union_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set_union, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_union = union_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set_union, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_union = union_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set_union, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 3; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_union = union_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set_union, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 3; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_union = union_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set_union, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST INTERSECT_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_intersect = intersect_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set_intersect, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_intersect = intersect_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set_intersect, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_intersect = intersect_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_intersect, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_intersect = intersect_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_intersect, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 3; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_intersect = intersect_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_intersect, &i));
    }

    for (int i = ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_intersect, &i));
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 3; i < ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        ASSERT(contains_fsc_hash_set(&set_intersect, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 3; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_intersect = intersect_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 3; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_intersect, &i));
    }

    for (int i = ((FSC_HASH_SET_CHUNK) / 3) * 2; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_intersect, &i));
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 3; i < ((FSC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        ASSERT(contains_fsc_hash_set(&set_intersect, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST SUBTRACT_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_subtract = subtract_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_subtract, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_subtract = subtract_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_subtract, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_subtract = subtract_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        ASSERT(contains_fsc_hash_set(&set_subtract, &i));
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_subtract, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_subtract = subtract_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        ASSERT(contains_fsc_hash_set(&set_subtract, &i));
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_subtract, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 3; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_subtract = subtract_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT(contains_fsc_hash_set(&set_subtract, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 3; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_subtract = subtract_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 3; ++i) {
        ASSERT(contains_fsc_hash_set(&set_subtract, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST EXCLUDE_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_exclude = exclude_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_exclude, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_exclude = exclude_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_fsc_hash_set(&set_exclude, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_exclude = exclude_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set_exclude, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_exclude = exclude_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set_exclude, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 3; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_exclude = exclude_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT(contains_fsc_hash_set(&set_exclude, &i));
    }

    for (int i = ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_fsc_hash_set(&set_exclude, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 3; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    fsc_hash_set_s set_exclude = exclude_fsc_hash_set(&set_one, &set_two, intcpy);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 3; ++i) {
        ASSERT(contains_fsc_hash_set(&set_exclude, &i));
    }

    for (int i = ((FSC_HASH_SET_CHUNK) / 3) * 2; i < FSC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_fsc_hash_set(&set_exclude, &i));
    }

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);
    destroy_fsc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    ASSERT(is_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    ASSERT(is_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT(is_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT(is_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_01(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_02(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_one, &i);
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_03(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 2; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_04(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < (FSC_HASH_SET_CHUNK) / 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 2; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_05(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK - 1) / 3; i < FSC_HASH_SET_CHUNK - 1; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_06(void) {
    fsc_hash_set_s set_one = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);
    fsc_hash_set_s set_two = create_fsc_hash_set(sizeof(int), FSC_HASH_SET_CHUNK, inthshmurmur, intcmp);

    for (int i = 0; i < ((FSC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_fsc_hash_set(&set_one, &i);
    }

    for (int i = (FSC_HASH_SET_CHUNK) / 3; i < FSC_HASH_SET_CHUNK; ++i) {
        insert_fsc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_fsc_hash_set(&set_one, &set_two));

    destroy_fsc_hash_set(&set_one, intdst, NULL);
    destroy_fsc_hash_set(&set_two, intdst, NULL);

    PASS();
}

SUITE (fsc_hash_set_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02);
    RUN_TEST(UNION_01); RUN_TEST(UNION_02); RUN_TEST(UNION_03);
    RUN_TEST(UNION_04); RUN_TEST(UNION_05); RUN_TEST(UNION_06);
    RUN_TEST(INTERSECT_01); RUN_TEST(INTERSECT_02); RUN_TEST(INTERSECT_03);
    RUN_TEST(INTERSECT_04); RUN_TEST(INTERSECT_05); RUN_TEST(INTERSECT_06);
    RUN_TEST(SUBTRACT_01); RUN_TEST(SUBTRACT_02); RUN_TEST(SUBTRACT_03);
    RUN_TEST(SUBTRACT_04); RUN_TEST(SUBTRACT_05); RUN_TEST(SUBTRACT_06);
    RUN_TEST(EXCLUDE_01); RUN_TEST(EXCLUDE_02); RUN_TEST(EXCLUDE_03);
    RUN_TEST(EXCLUDE_04); RUN_TEST(EXCLUDE_05); RUN_TEST(EXCLUDE_06);
    RUN_TEST(IS_SUBSET_01); RUN_TEST(IS_SUBSET_02); RUN_TEST(IS_SUBSET_03);
    RUN_TEST(IS_SUBSET_04); RUN_TEST(IS_SUBSET_05); RUN_TEST(IS_SUBSET_06);
    RUN_TEST(IS_PROPER_SUBSET_01); RUN_TEST(IS_PROPER_SUBSET_02); RUN_TEST(IS_PROPER_SUBSET_03);
    RUN_TEST(IS_PROPER_SUBSET_04); RUN_TEST(IS_PROPER_SUBSET_05); RUN_TEST(IS_PROPER_SUBSET_06);
    RUN_TEST(IS_DISJOINT_01); RUN_TEST(IS_DISJOINT_02); RUN_TEST(IS_DISJOINT_03);
    RUN_TEST(IS_DISJOINT_04); RUN_TEST(IS_DISJOINT_05); RUN_TEST(IS_DISJOINT_06);
}
