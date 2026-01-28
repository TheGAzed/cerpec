#include <set/ihash_set.h>
#include <suite.h>

TEST CREATE_01(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    ASSERT_EQ(0, set.capacity);
    ASSERT_EQ(0, set.length);
    ASSERT_NEQ(0, set.size);
    ASSERT_NEQ(NULL, set.hash);

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    destroy_ihash_set(&set, destroy);

    ASSERT_EQ(0, set.capacity);
    ASSERT_EQ(0, set.length);
    ASSERT_EQ(0, set.size);
    ASSERT_EQ(NULL, set.hash);

    PASS();
}

TEST CLEAR_01(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    clear_ihash_set(&set, destroy);

    ASSERT_EQ(0, set.capacity);
    ASSERT_EQ(0, set.length);
    ASSERT_NEQ(0, set.size);
    ASSERT_NEQ(NULL, set.hash);

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST INSERT_01(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set, &i);
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST INSERT_02(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set, &i);
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST INSERT_03(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set, &i);
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set, &i);
    }

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        int buf = 0;
        remove_ihash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set, &i);
    }

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        int buf = 0;
        remove_ihash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST REMOVE_03(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set, &i);
    }

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        int buf = 0;
        remove_ihash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set, &i);
    }

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set, &i));
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set, &i);
    }

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set, &i));
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST CONTAINS_03(void) {
    ihash_set_s set = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set, &i);
    }

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set, &i));
    }

    destroy_ihash_set(&set, destroy);

    PASS();
}

TEST UNION_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 3; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 3; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST UNION_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 3; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_union = union_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set_union, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_union, destroy);

    PASS();
}

TEST INTERSECT_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 3; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    for (int i = ((IHASH_SET_CHUNK - 1) / 3) * 2; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 3; i < ((IHASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        ASSERT(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 3; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 3; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    for (int i = ((IHASH_SET_CHUNK) / 3) * 2; i < IHASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    for (int i = (IHASH_SET_CHUNK) / 3; i < ((IHASH_SET_CHUNK) / 3) * 2; ++i) {
        ASSERT(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 3; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_intersect = intersect_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 3; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    for (int i = ((IHASH_SET_CHUNK + 1) / 3) * 2; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_intersect, &i));
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 3; i < ((IHASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        ASSERT(contains_ihash_set(&set_intersect, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_intersect, destroy);

    PASS();
}

TEST SUBTRACT_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        ASSERT(contains_ihash_set(&set_subtract, &i));
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        ASSERT(contains_ihash_set(&set_subtract, &i));
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        ASSERT(contains_ihash_set(&set_subtract, &i));
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_subtract, &i));
    }


    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 3; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 3; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 3; ++i) {
        ASSERT(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 3; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_subtract = subtract_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 3; ++i) {
        ASSERT(contains_ihash_set(&set_subtract, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_subtract, destroy);

    PASS();
}

TEST EXCLUDE_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 3; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    for (int i = ((IHASH_SET_CHUNK - 1) / 3) * 2; i < IHASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 3; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 3; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    for (int i = ((IHASH_SET_CHUNK) / 3) * 2; i < IHASH_SET_CHUNK; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 3; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ihash_set_s set_exclude = exclude_ihash_set(&set_one, &set_two, copy);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 3; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    for (int i = ((IHASH_SET_CHUNK + 1) / 3) * 2; i < IHASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_ihash_set(&set_exclude, &i));
    }

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);
    destroy_ihash_set(&set_exclude, destroy);

    PASS();
}

TEST IS_SUBSET_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    ASSERT(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    ASSERT(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    ASSERT(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_01(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_02(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_03(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_one, &i);
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_04(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK - 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 2; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_05(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 2; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_06(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < (IHASH_SET_CHUNK + 1) / 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 2; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_07(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK - 1) / 3; i < IHASH_SET_CHUNK - 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_08(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK) / 3; i < IHASH_SET_CHUNK; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_09(void) {
    ihash_set_s set_one = create_ihash_set(sizeof(int), hash, compare);
    ihash_set_s set_two = create_ihash_set(sizeof(int), hash, compare);

    for (int i = 0; i < ((IHASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ihash_set(&set_one, &i);
    }

    for (int i = (IHASH_SET_CHUNK + 1) / 3; i < IHASH_SET_CHUNK + 1; ++i) {
        insert_ihash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_ihash_set(&set_one, &set_two));

    destroy_ihash_set(&set_one, destroy);
    destroy_ihash_set(&set_two, destroy);

    PASS();
}

SUITE (ihash_set_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02); RUN_TEST(INSERT_03);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02); RUN_TEST(REMOVE_03);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02); RUN_TEST(CONTAINS_03);
    RUN_TEST(UNION_01); RUN_TEST(UNION_02); RUN_TEST(UNION_03);
    RUN_TEST(UNION_04); RUN_TEST(UNION_05); RUN_TEST(UNION_06);
    RUN_TEST(UNION_07); RUN_TEST(UNION_08); RUN_TEST(UNION_09);
    RUN_TEST(INTERSECT_01); RUN_TEST(INTERSECT_02); RUN_TEST(INTERSECT_03);
    RUN_TEST(INTERSECT_04); RUN_TEST(INTERSECT_05); RUN_TEST(INTERSECT_06);
    RUN_TEST(INTERSECT_07); RUN_TEST(INTERSECT_08); RUN_TEST(INTERSECT_09);
    RUN_TEST(SUBTRACT_01); RUN_TEST(SUBTRACT_02); RUN_TEST(SUBTRACT_03);
    RUN_TEST(SUBTRACT_04); RUN_TEST(SUBTRACT_05); RUN_TEST(SUBTRACT_06);
    RUN_TEST(SUBTRACT_07); RUN_TEST(SUBTRACT_08); RUN_TEST(SUBTRACT_09);
    RUN_TEST(EXCLUDE_01); RUN_TEST(EXCLUDE_02); RUN_TEST(EXCLUDE_03);
    RUN_TEST(EXCLUDE_04); RUN_TEST(EXCLUDE_05); RUN_TEST(EXCLUDE_06);
    RUN_TEST(EXCLUDE_07); RUN_TEST(EXCLUDE_08); RUN_TEST(EXCLUDE_09);
    RUN_TEST(IS_SUBSET_01); RUN_TEST(IS_SUBSET_02); RUN_TEST(IS_SUBSET_03);
    RUN_TEST(IS_SUBSET_04); RUN_TEST(IS_SUBSET_05); RUN_TEST(IS_SUBSET_06);
    RUN_TEST(IS_SUBSET_07); RUN_TEST(IS_SUBSET_08); RUN_TEST(IS_SUBSET_09);
    RUN_TEST(IS_PROPER_SUBSET_01); RUN_TEST(IS_PROPER_SUBSET_02); RUN_TEST(IS_PROPER_SUBSET_03);
    RUN_TEST(IS_PROPER_SUBSET_04); RUN_TEST(IS_PROPER_SUBSET_05); RUN_TEST(IS_PROPER_SUBSET_06);
    RUN_TEST(IS_PROPER_SUBSET_07); RUN_TEST(IS_PROPER_SUBSET_08); RUN_TEST(IS_PROPER_SUBSET_09);
    RUN_TEST(IS_DISJOINT_01); RUN_TEST(IS_DISJOINT_02); RUN_TEST(IS_DISJOINT_03);
    RUN_TEST(IS_DISJOINT_04); RUN_TEST(IS_DISJOINT_05); RUN_TEST(IS_DISJOINT_06);
    RUN_TEST(IS_DISJOINT_07); RUN_TEST(IS_DISJOINT_08); RUN_TEST(IS_DISJOINT_09);
}
