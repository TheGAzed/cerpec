#include <set/isc_hash_set.h>
#include <dodac.h>

#include <suite.h>

TEST CREATE_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    ASSERT_EQ(0, set.capacity);
    ASSERT_EQ(0, set.length);
    ASSERT_NEQ(0, set.size);
    ASSERT_NEQ(NULL, set.hash);

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    destroy_isc_hash_set(&set, intdst, NULL);

    ASSERT_EQ(0, set.capacity);
    ASSERT_EQ(0, set.length);
    ASSERT_EQ(0, set.size);
    ASSERT_EQ(NULL, set.hash);

    PASS();
}

TEST CLEAR_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    clear_isc_hash_set(&set, intdst, NULL);

    ASSERT_EQ(0, set.capacity);
    ASSERT_EQ(0, set.length);
    ASSERT_NEQ(0, set.size);
    ASSERT_NEQ(NULL, set.hash);

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    isc_hash_set_s replica = copy_isc_hash_set(&set, intcpy, NULL);

    ASSERT_EQ(set.allocator, replica.allocator);
    ASSERT_EQ(set.compare, replica.compare);
    ASSERT_EQ(set.ac, replica.ac);
    ASSERT_EQ(set.hash, replica.hash);
    ASSERT_EQ(set.length, replica.length);
    ASSERT_EQ(set.size, replica.size);

    destroy_isc_hash_set(&set, intdst, NULL);
    destroy_isc_hash_set(&replica, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    isc_hash_set_s replica = copy_isc_hash_set(&set, intcpy, NULL);

    ASSERT_EQ(set.allocator, replica.allocator);
    ASSERT_EQ(set.compare, replica.compare);
    ASSERT_EQ(set.ac, replica.ac);
    ASSERT_EQ(set.hash, replica.hash);
    ASSERT_EQ(set.length, replica.length);
    ASSERT_EQ(set.size, replica.size);

    destroy_isc_hash_set(&set, intdst, NULL);
    destroy_isc_hash_set(&replica, intdst, NULL);

    PASS();
}

TEST COPY_03(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    isc_hash_set_s replica = copy_isc_hash_set(&set, intcpy, NULL);

    ASSERT_EQ(set.allocator, replica.allocator);
    ASSERT_EQ(set.compare, replica.compare);
    ASSERT_EQ(set.ac, replica.ac);
    ASSERT_EQ(set.hash, replica.hash);
    ASSERT_EQ(set.length, replica.length);
    ASSERT_EQ(set.size, replica.size);

    destroy_isc_hash_set(&set, intdst, NULL);
    destroy_isc_hash_set(&replica, intdst, NULL);

    PASS();
}

TEST INSERT_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST INSERT_02(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST INSERT_03(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST REMOVE_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        int buf = 0;
        remove_isc_hash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST REMOVE_02(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        int buf = 0;
        remove_isc_hash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST REMOVE_03(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        int buf = 0;
        remove_isc_hash_set(&set, &i, &buf);
        ASSERT_EQ(i, buf);
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST CONTAINS_01(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set, &i));
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST CONTAINS_02(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set, &i));
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST CONTAINS_03(void) {
    isc_hash_set_s set = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set, &i);
    }

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set, &i));
    }

    destroy_isc_hash_set(&set, intdst, NULL);

    PASS();
}

TEST UNION_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 3; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 3; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST UNION_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 3; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_union = union_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_union, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_union, intdst, NULL);

    PASS();
}

TEST INTERSECT_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 3; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    for (int i = ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 3; i < ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        ASSERT(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 3; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 3; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    for (int i = ((ISC_HASH_SET_CHUNK) / 3) * 2; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 3; i < ((ISC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        ASSERT(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST INTERSECT_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 3; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_intersect = intersect_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 3; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    for (int i = ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_intersect, &i));
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 3; i < ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        ASSERT(contains_isc_hash_set(&set_intersect, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_intersect, intdst, NULL);

    PASS();
}

TEST SUBTRACT_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        ASSERT(contains_isc_hash_set(&set_subtract, &i));
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        ASSERT(contains_isc_hash_set(&set_subtract, &i));
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        ASSERT(contains_isc_hash_set(&set_subtract, &i));
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_subtract, &i));
    }


    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 3; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 3; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 3; ++i) {
        ASSERT(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST SUBTRACT_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 3; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_subtract = subtract_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 3; ++i) {
        ASSERT(contains_isc_hash_set(&set_subtract, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_subtract, intdst, NULL);

    PASS();
}

TEST EXCLUDE_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT_FALSE(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 3; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 3; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    for (int i = ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 3; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 3; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    for (int i = ((ISC_HASH_SET_CHUNK) / 3) * 2; i < ISC_HASH_SET_CHUNK; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST EXCLUDE_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 3; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    isc_hash_set_s set_exclude = exclude_isc_hash_set(&set_one, &set_two, intcpy, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 3; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    for (int i = ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        ASSERT(contains_isc_hash_set(&set_exclude, &i));
    }

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);
    destroy_isc_hash_set(&set_exclude, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    ASSERT(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    ASSERT(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    ASSERT(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_SUBSET_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    ASSERT(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_PROPER_SUBSET_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_proper_subset_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_01(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_02(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_03(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_one, &i);
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_04(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK - 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 2; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_05(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 2; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_06(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < (ISC_HASH_SET_CHUNK + 1) / 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 2; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_07(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK - 1) / 3; i < ISC_HASH_SET_CHUNK - 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_08(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK) / 3; i < ISC_HASH_SET_CHUNK; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

TEST IS_DISJOINT_09(void) {
    isc_hash_set_s set_one = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);
    isc_hash_set_s set_two = create_isc_hash_set(sizeof(int), inthshmurmur, NULL, intcmp, NULL);

    for (int i = 0; i < ((ISC_HASH_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_isc_hash_set(&set_one, &i);
    }

    for (int i = (ISC_HASH_SET_CHUNK + 1) / 3; i < ISC_HASH_SET_CHUNK + 1; ++i) {
        insert_isc_hash_set(&set_two, &i);
    }

    ASSERT_FALSE(is_disjoint_isc_hash_set(&set_one, &set_two));

    destroy_isc_hash_set(&set_one, intdst, NULL);
    destroy_isc_hash_set(&set_two, intdst, NULL);

    PASS();
}

SUITE (isc_hash_set_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03);
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
