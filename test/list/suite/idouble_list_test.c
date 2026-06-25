#include <list/idouble_list.h>
#include <dodac.h>

#include <suite.h>

#define NIL ((size_t)(-1))

TEST CREATE_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_EQ(NULL, test.elements);
    ASSERT_EQ(NULL, test.node[IDL_NEXT]);
    ASSERT_EQ(NULL, test.node[IDL_PREV]);

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));
    destroy_idouble_list(&test, intdst, NULL);

    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    clear_idouble_list(&test, intdst, NULL);

    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_EQ(NULL, test.elements);
    ASSERT_EQ(NULL, test.node[IDL_NEXT]);
    ASSERT_EQ(NULL, test.node[IDL_PREV]);

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s replica = copy_idouble_list(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);

    for (size_t i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0, r = 0;
        get_idouble_list(&test, i, &t);
        get_idouble_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&replica, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s replica = copy_idouble_list(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);

    for (size_t i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0, r = 0;
        get_idouble_list(&test, i, &t);
        get_idouble_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&replica, intdst, NULL);

    PASS();
}

TEST COPY_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s replica = copy_idouble_list(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);

    for (size_t i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0, r = 0;
        get_idouble_list(&test, i, &t);
        get_idouble_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&replica, intdst, NULL);

    PASS();
}

TEST IS_EMPTY_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    ASSERT(is_empty_idouble_list(&test));

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST IS_EMPTY_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    const int a = 0;
    insert_at_idouble_list(&test, &a, test.length);

    ASSERT_FALSE(is_empty_idouble_list(&test));

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST INSERT_AT_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST INSERT_AT_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST INSERT_AT_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST GET_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_idouble_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST GET_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_idouble_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST GET_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        get_idouble_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_FIRST_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_first_idouble_list(&test, &i, &t, intcmp, NULL);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_FIRST_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        remove_first_idouble_list(&test, &i, &t, intcmp, NULL);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_FIRST_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        remove_first_idouble_list(&test, &i, &t, intcmp, NULL);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_AT_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_idouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_AT_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_idouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REMOVE_AT_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        remove_at_idouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REVERSE_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    reverse_idouble_list(&test);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_idouble_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REVERSE_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    reverse_idouble_list(&test);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_idouble_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST REVERSE_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    reverse_idouble_list(&test);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        remove_at_idouble_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_NEXT_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_idouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_idouble_list(&test, 1);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_NEXT_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_idouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_idouble_list(&test, 1);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_NEXT_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        get_idouble_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_idouble_list(&test, 1);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_PREV_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        shift_prev_idouble_list(&test, 1);

        int t = 0;
        get_idouble_list(&test, 0, &t);
        ASSERT_EQ(IDOUBLE_LIST_CHUNK - 1 - 1 - i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_PREV_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        shift_prev_idouble_list(&test, 1);

        int t = 0;
        get_idouble_list(&test, 0, &t);
        ASSERT_EQ(IDOUBLE_LIST_CHUNK - 1 - i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SHIFT_PREV_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }


    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        shift_prev_idouble_list(&test, 1);

        int t = 0;
        get_idouble_list(&test, 0, &t);
        ASSERT_EQ(IDOUBLE_LIST_CHUNK + 1 - 1 - i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST SPLICE_01(void) {
    idouble_list_s one = create_idouble_list(sizeof(int));
    idouble_list_s two = create_idouble_list(sizeof(int));

    for (int i = 0; i < (IDOUBLE_LIST_CHUNK - 1) / 2; ++i) {
        insert_at_idouble_list(&one, &i, one.length);
    }

    for (int i = (IDOUBLE_LIST_CHUNK - 1) / 2; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&two, &i, two.length);
    }

    splice_idouble_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_idouble_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&one, intdst, NULL);
    destroy_idouble_list(&two, intdst, NULL);

    PASS();
}

TEST SPLICE_02(void) {
    idouble_list_s one = create_idouble_list(sizeof(int));
    idouble_list_s two = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK / 2; ++i) {
        insert_at_idouble_list(&one, &i, one.length);
    }

    for (int i = IDOUBLE_LIST_CHUNK / 2; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&two, &i, two.length);
    }

    splice_idouble_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_idouble_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&one, intdst, NULL);
    destroy_idouble_list(&two, intdst, NULL);

    PASS();
}

TEST SPLICE_03(void) {
    idouble_list_s one = create_idouble_list(sizeof(int));
    idouble_list_s two = create_idouble_list(sizeof(int));

    for (int i = 0; i < (IDOUBLE_LIST_CHUNK + 1) / 2; ++i) {
        insert_at_idouble_list(&one, &i, one.length);
    }

    for (int i = (IDOUBLE_LIST_CHUNK + 1) / 2; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&two, &i, two.length);
    }

    splice_idouble_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        get_idouble_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&one, intdst, NULL);
    destroy_idouble_list(&two, intdst, NULL);

    PASS();
}

TEST SPLIT_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 0, test.length / 2);

    for (int i = 0; i < (IDOUBLE_LIST_CHUNK - 1) / 2; ++i) {
        int s = 0;
        get_idouble_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = (IDOUBLE_LIST_CHUNK - 1) / 2; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_idouble_list(&test, (size_t)(i) - (IDOUBLE_LIST_CHUNK - 1) / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 0, test.length / 2);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK / 2; ++i) {
        int s = 0;
        get_idouble_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = IDOUBLE_LIST_CHUNK / 2; i < IDOUBLE_LIST_CHUNK; ++i) {
        int t = 0;
        get_idouble_list(&test, (size_t)(i) - IDOUBLE_LIST_CHUNK / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 0, test.length / 2);

    for (int i = 0; i < (IDOUBLE_LIST_CHUNK + 1) / 2; ++i) {
        int s = 0;
        get_idouble_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = (IDOUBLE_LIST_CHUNK + 1) / 2; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int t = 0;
        get_idouble_list(&test, (size_t)(i) - (IDOUBLE_LIST_CHUNK + 1) / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_04(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    const int array[] = { 0, 1, 2, 3, 4 };
    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_idouble_list(&test, array + i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 4, 1);

    // test list assert
    int got = -1;
    get_idouble_list(&test, 0, &got);
    ASSERT_EQ(0, got);

    get_idouble_list(&test, 1, &got);
    ASSERT_EQ(1, got);

    get_idouble_list(&test, 2, &got);
    ASSERT_EQ(2, got);

    get_idouble_list(&test, 3, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_idouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_05(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    const int array[] = { 0, 1, 2, 3, 4 };
    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_idouble_list(&test, array + i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 4, 2);

    // test list assert
    int got = -1;
    get_idouble_list(&test, 0, &got);
    ASSERT_EQ(1, got);

    get_idouble_list(&test, 1, &got);
    ASSERT_EQ(2, got);

    get_idouble_list(&test, 2, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_idouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    get_idouble_list(&split, 1, &got);
    ASSERT_EQ(0, got);

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_06(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    const int array[] = { 0, 1, 2, 3, 4 };
    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_idouble_list(&test, array + i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 4, 3);

    // test list assert
    int got = -1;
    get_idouble_list(&test, 0, &got);
    ASSERT_EQ(2, got);

    get_idouble_list(&test, 1, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_idouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    get_idouble_list(&split, 1, &got);
    ASSERT_EQ(0, got);

    get_idouble_list(&split, 2, &got);
    ASSERT_EQ(1, got);

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST SPLIT_07(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    const int array[] = { 0, 1, 2, 3, 4 };
    for (size_t i = 0; i < sizeof(array) / test.size; ++i) {
        insert_at_idouble_list(&test, array + i, test.length);
    }

    idouble_list_s split = slice_idouble_list(&test, 4, 4);

    // test list assert
    int got = -1;

    get_idouble_list(&test, 0, &got);
    ASSERT_EQ(3, got);

    // sliced list assert
    get_idouble_list(&split, 0, &got);
    ASSERT_EQ(4, got);

    get_idouble_list(&split, 1, &got);
    ASSERT_EQ(0, got);

    get_idouble_list(&split, 2, &got);
    ASSERT_EQ(1, got);

    get_idouble_list(&split, 3, &got);
    ASSERT_EQ(2, got);

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&split, intdst, NULL);

    PASS();
}

TEST EXTRACT_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s extract = extract_idouble_list(&test, intfilodd, NULL);
    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; i += 2) {
        int s = 0;
        get_idouble_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < IDOUBLE_LIST_CHUNK - 1; i += 2) {
        int e = 0;
        get_idouble_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&extract, intdst, NULL);

    PASS();
}

TEST EXTRACT_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s extract = extract_idouble_list(&test, intfilodd, NULL);
    for (int i = 0; i < IDOUBLE_LIST_CHUNK; i += 2) {
        int s = 0;
        get_idouble_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < IDOUBLE_LIST_CHUNK; i += 2) {
        int e = 0;
        get_idouble_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&extract, intdst, NULL);

    PASS();
}

TEST EXTRACT_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    idouble_list_s extract = extract_idouble_list(&test, intfilodd, NULL);
    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; i += 2) {
        int s = 0;
        get_idouble_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < IDOUBLE_LIST_CHUNK + 1; i += 2) {
        int e = 0;
        get_idouble_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_idouble_list(&test, intdst, NULL);
    destroy_idouble_list(&extract, intdst, NULL);

    PASS();
}

TEST MAP_NEXT_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_next_idouble_list(&test, intincrement, &value);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_NEXT_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_next_idouble_list(&test, intincrement, &value);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_NEXT_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_next_idouble_list(&test, intincrement, &value);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_PREV_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_prev_idouble_list(&test, intincrement, &value);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_PREV_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_prev_idouble_list(&test, intincrement, &value);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST MAP_PREV_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    int value = 1;
    each_prev_idouble_list(&test, intincrement, &value);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_01(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    apply_idouble_list(&test, intqsort, NULL);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_02(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    apply_idouble_list(&test, intqsort, NULL);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_03(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        insert_at_idouble_list(&test, &i, test.length);
    }

    apply_idouble_list(&test, intqsort, NULL);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_04(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int reverse = (IDOUBLE_LIST_CHUNK - 1) - i - 1;
        insert_at_idouble_list(&test, &reverse, test.length);
    }

    apply_idouble_list(&test, intqsort, NULL);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_05(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int reverse = (IDOUBLE_LIST_CHUNK) - i - 1;
        insert_at_idouble_list(&test, &reverse, test.length);
    }

    apply_idouble_list(&test, intqsort, NULL);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

TEST APPLY_06(void) {
    idouble_list_s test = create_idouble_list(sizeof(int));

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int reverse = (IDOUBLE_LIST_CHUNK + 1) - i - 1;
        insert_at_idouble_list(&test, &reverse, test.length);
    }

    apply_idouble_list(&test, intqsort, NULL);

    for (int i = 0; i < IDOUBLE_LIST_CHUNK + 1; ++i) {
        int a = 0;
        get_idouble_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_idouble_list(&test, intdst, NULL);

    PASS();
}

SUITE (idouble_list_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03);
    RUN_TEST(IS_EMPTY_01); RUN_TEST(IS_EMPTY_02);
    RUN_TEST(INSERT_AT_01); RUN_TEST(INSERT_AT_02); RUN_TEST(INSERT_AT_03);
    RUN_TEST(GET_01); RUN_TEST(GET_02); RUN_TEST(GET_03);
    RUN_TEST(REMOVE_FIRST_01); RUN_TEST(REMOVE_FIRST_02); RUN_TEST(REMOVE_FIRST_03);
    RUN_TEST(REMOVE_AT_01); RUN_TEST(REMOVE_AT_02); RUN_TEST(REMOVE_AT_03);
    RUN_TEST(REVERSE_01); RUN_TEST(REVERSE_02); RUN_TEST(REVERSE_03);
    RUN_TEST(SHIFT_NEXT_01); RUN_TEST(SHIFT_NEXT_02); RUN_TEST(SHIFT_NEXT_03);
    RUN_TEST(SHIFT_PREV_01); RUN_TEST(SHIFT_PREV_02); RUN_TEST(SHIFT_PREV_03);
    RUN_TEST(SPLICE_01); RUN_TEST(SPLICE_02); RUN_TEST(SPLICE_03);
    RUN_TEST(SPLIT_01); RUN_TEST(SPLIT_02); RUN_TEST(SPLIT_03);
    RUN_TEST(SPLIT_04); RUN_TEST(SPLIT_05); RUN_TEST(SPLIT_06);
    RUN_TEST(SPLIT_07);
    RUN_TEST(EXTRACT_01); RUN_TEST(EXTRACT_02); RUN_TEST(EXTRACT_03);
    RUN_TEST(MAP_NEXT_01); RUN_TEST(MAP_NEXT_02); RUN_TEST(MAP_NEXT_03);
    RUN_TEST(MAP_PREV_01); RUN_TEST(MAP_PREV_02); RUN_TEST(MAP_PREV_03);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04); RUN_TEST(APPLY_05); RUN_TEST(APPLY_06);
}
