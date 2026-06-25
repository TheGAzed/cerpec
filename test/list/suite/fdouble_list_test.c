#include <list/fdouble_list.h>
#include <dodac.h>

#include <suite.h>

#define NIL ((size_t)(-1))

#define FDOUBLE_LIST_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(NULL, test.elements);
    ASSERT_NEQ(NULL, test.node[FDL_NEXT]);
    ASSERT_NEQ(NULL, test.node[FDL_PREV]);

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);
    destroy_fdouble_list(&test, intdst, NULL);

    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    clear_fdouble_list(&test, intdst, NULL);

    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(NULL, test.elements);
    ASSERT_NEQ(NULL, test.node[FDL_NEXT]);
    ASSERT_NEQ(NULL, test.node[FDL_PREV]);

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    fdouble_list_s replica = copy_fdouble_list(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);

    for (size_t i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0, r = 0;
        get_fdouble_list(&test, i, &t);
        get_fdouble_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&replica, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    fdouble_list_s replica = copy_fdouble_list(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);

    for (size_t i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0, r = 0;
        get_fdouble_list(&test, i, &t);
        get_fdouble_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&replica, intdst, NULL);

    PASS();
}

TEST IS_EMPTY_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    ASSERT(is_empty_fdouble_list(&test));

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST IS_EMPTY_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    const int a = 0;
    insert_at_fdouble_list(&test, &a, test.length);

    ASSERT_FALSE(is_empty_fdouble_list(&test));

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST INSERT_AT_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST INSERT_AT_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST GET_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fdouble_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST GET_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_fdouble_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_FIRST_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_first_fdouble_list(&test, &i, &t, intcmp, NULL);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_FIRST_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        remove_first_fdouble_list(&test, &i, &t, intcmp, NULL);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_AT_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_fdouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_AT_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_fdouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST REVERSE_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    reverse_fdouble_list(&test);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_fdouble_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST REVERSE_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    reverse_fdouble_list(&test);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_fdouble_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_NEXT_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fdouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_fdouble_list(&test, 1);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_NEXT_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_fdouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_fdouble_list(&test, 1);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_PREV_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        shift_prev_fdouble_list(&test, 1);

        int t = 0;
        get_fdouble_list(&test, 0, &t);
        ASSERT_EQ(FDOUBLE_LIST_CHUNK - 1 - 1 - i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_PREV_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        shift_prev_fdouble_list(&test, 1);

        int t = 0;
        get_fdouble_list(&test, 0, &t);
        ASSERT_EQ(FDOUBLE_LIST_CHUNK - 1 - i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST SPLICE_01(void) {
    fdouble_list_s one = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);
    fdouble_list_s two = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < (FDOUBLE_LIST_CHUNK - 1) / 2; ++i) {
        insert_at_fdouble_list(&one, &i, one.length);
    }

    for (int i = (FDOUBLE_LIST_CHUNK - 1) / 2; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&two, &i, two.length);
    }

    splice_fdouble_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fdouble_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&one, intdst, NULL);
    destroy_fdouble_list(&two, intdst, NULL);

    PASS();
}

TEST SPLICE_02(void) {
    fdouble_list_s one = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);
    fdouble_list_s two = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK / 2; ++i) {
        insert_at_fdouble_list(&one, &i, one.length);
    }

    for (int i = FDOUBLE_LIST_CHUNK / 2; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&two, &i, two.length);
    }

    splice_fdouble_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_fdouble_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&one, intdst, NULL);
    destroy_fdouble_list(&two, intdst, NULL);

    PASS();
}

TEST SPLIT_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    fdouble_list_s split = slice_fdouble_list(&test, 0, test.length / 2, test.max, test.max);

    for (int i = 0; i < (FDOUBLE_LIST_CHUNK - 1) / 2; ++i) {
        int s = 0;
        get_fdouble_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = (FDOUBLE_LIST_CHUNK - 1) / 2; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fdouble_list(&test, (size_t)(i) - (FDOUBLE_LIST_CHUNK - 1) / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    fdouble_list_s split = slice_fdouble_list(&test, 0, test.length / 2, test.max, test.max);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK / 2; ++i) {
        int s = 0;
        get_fdouble_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = FDOUBLE_LIST_CHUNK / 2; i < FDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_fdouble_list(&test, (size_t)(i) - FDOUBLE_LIST_CHUNK / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_04(void) {
    const int array[] = { 0, 1, 2, 3, 4 };
    size_t const length = sizeof(array) / sizeof(int);
    fdouble_list_s test = create_fdouble_list(sizeof(int), length);

    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_fdouble_list(&test, array + i, test.length);
    }

    fdouble_list_s split = slice_fdouble_list(&test, 4, 1, length, length);

    // test list assert
    int got = -1;
    get_fdouble_list(&test, 0, &got);
    ASSERT_EQ(0, got);

    get_fdouble_list(&test, 1, &got);
    ASSERT_EQ(1, got);

    get_fdouble_list(&test, 2, &got);
    ASSERT_EQ(2, got);

    get_fdouble_list(&test, 3, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_fdouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_05(void) {
    const int array[] = { 0, 1, 2, 3, 4 };
    size_t const length = sizeof(array) / sizeof(int);
    fdouble_list_s test = create_fdouble_list(sizeof(int), length);

    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_fdouble_list(&test, array + i, test.length);
    }

    fdouble_list_s split = slice_fdouble_list(&test, 4, 2, length, length);

    // test list assert
    int got = -1;
    get_fdouble_list(&test, 0, &got);
    ASSERT_EQ(1, got);

    get_fdouble_list(&test, 1, &got);
    ASSERT_EQ(2, got);

    get_fdouble_list(&test, 2, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_fdouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    get_fdouble_list(&split, 1, &got);
    ASSERT_EQ(0, got);

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_06(void) {
    const int array[] = { 0, 1, 2, 3, 4 };
    size_t const length = sizeof(array) / sizeof(int);
    fdouble_list_s test = create_fdouble_list(sizeof(int), length);

    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_fdouble_list(&test, array + i, test.length);
    }

    fdouble_list_s split = slice_fdouble_list(&test, 4, 3, length, length);

    // test list assert
    int got = -1;
    get_fdouble_list(&test, 0, &got);
    ASSERT_EQ(2, got);

    get_fdouble_list(&test, 1, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_fdouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    get_fdouble_list(&split, 1, &got);
    ASSERT_EQ(0, got);

    get_fdouble_list(&split, 2, &got);
    ASSERT_EQ(1, got);

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_07(void) {
    const int array[5] = { 0, 1, 2, 3, 4 };
    size_t const length = sizeof(array) / sizeof(int);
    fdouble_list_s test = create_fdouble_list(sizeof(int), length);

    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_fdouble_list(&test, array + i, test.length);
    }

    fdouble_list_s split = slice_fdouble_list(&test, 4, 4, length, length);

    // test list assert
    int got = -1;

    get_fdouble_list(&test, 0, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_fdouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    get_fdouble_list(&split, 1, &got);
    ASSERT_EQ(0, got);

    get_fdouble_list(&split, 2, &got);
    ASSERT_EQ(1, got);

    get_fdouble_list(&split, 3, &got);
    ASSERT_EQ(2, got);

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&split, intdst, NULL);

    PASS();
}

TEST EXTRACT_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    fdouble_list_s extract = extract_fdouble_list(&test, intfilodd, NULL, test.max, test.max);
    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; i += 2) {
        int s = 0;
        get_fdouble_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < FDOUBLE_LIST_CHUNK - 1; i += 2) {
        int e = 0;
        get_fdouble_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&extract, intdst, NULL);

    PASS();
}

TEST EXTRACT_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    fdouble_list_s extract = extract_fdouble_list(&test, intfilodd, NULL, test.max, test.max);
    for (int i = 0; i < FDOUBLE_LIST_CHUNK; i += 2) {
        int s = 0;
        get_fdouble_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < FDOUBLE_LIST_CHUNK; i += 2) {
        int e = 0;
        get_fdouble_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_fdouble_list(&test, intdst, NULL);
    destroy_fdouble_list(&extract, intdst, NULL);

    PASS();
}

TEST MAP_NEXT_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_next_fdouble_list(&test, intincrement, &value);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_NEXT_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_next_fdouble_list(&test, intincrement, &value);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_PREV_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_prev_fdouble_list(&test, intincrement, &value);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_PREV_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_prev_fdouble_list(&test, intincrement, &value);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_01(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    apply_fdouble_list(&test, intqsort, NULL);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_02(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        insert_at_fdouble_list(&test, &i, test.length);
    }

    apply_fdouble_list(&test, intqsort, NULL);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_03(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int reverse = (FDOUBLE_LIST_CHUNK - 1) - i - 1;
        insert_at_fdouble_list(&test, &reverse, test.length);
    }

    apply_fdouble_list(&test, intqsort, NULL);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_04(void) {
    fdouble_list_s test = create_fdouble_list(sizeof(int), FDOUBLE_LIST_CHUNK);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int reverse = (FDOUBLE_LIST_CHUNK) - i - 1;
        insert_at_fdouble_list(&test, &reverse, test.length);
    }

    apply_fdouble_list(&test, intqsort, NULL);

    for (int i = 0; i < FDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_fdouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fdouble_list(&test, intdst, NULL);

    PASS();
}

SUITE (fdouble_list_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(IS_EMPTY_01); RUN_TEST(IS_EMPTY_02);
    RUN_TEST(INSERT_AT_01); RUN_TEST(INSERT_AT_02);
    RUN_TEST(GET_01); RUN_TEST(GET_02);
    RUN_TEST(REMOVE_FIRST_01); RUN_TEST(REMOVE_FIRST_02);
    RUN_TEST(REMOVE_AT_01); RUN_TEST(REMOVE_AT_02);
    RUN_TEST(REVERSE_01); RUN_TEST(REVERSE_02);
    RUN_TEST(SHIFT_NEXT_01); RUN_TEST(SHIFT_NEXT_02);
    RUN_TEST(SHIFT_PREV_01); RUN_TEST(SHIFT_PREV_02);
    RUN_TEST(SPLICE_01); RUN_TEST(SPLICE_02);
    RUN_TEST(SPLIT_01); RUN_TEST(SPLIT_02);
    RUN_TEST(SPLIT_04); RUN_TEST(SPLIT_05); RUN_TEST(SPLIT_06);
    RUN_TEST(SPLIT_07);
    RUN_TEST(EXTRACT_01); RUN_TEST(EXTRACT_02);
    RUN_TEST(MAP_NEXT_01); RUN_TEST(MAP_NEXT_02);
    RUN_TEST(MAP_PREV_01); RUN_TEST(MAP_PREV_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
