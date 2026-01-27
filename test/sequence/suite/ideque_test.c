#include <sequence/ideque.h>
#include <suite.h>

TEST CREATE_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);
    ASSERT_EQ(NULL, test.head);

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    destroy_ideque(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(sizeof(int), test.size);
    ASSERT_EQ(0, test.size);
    ASSERT_EQ(NULL, test.head);

    PASS();
}

TEST CLEAR_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    clear_ideque(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_ideque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);

        int a = 0;
        peek_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);

        int a = 0;
        peek_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);

        int a = 0;
        peek_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_04(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);

        int a = 0;
        peek_front_ideque(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_05(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);

        int a = 0;
        peek_front_ideque(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_06(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);

        int a = 0;
        peek_front_ideque(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);

        int a = 0;
        peek_back_ideque(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);

        int a = 0;
        peek_back_ideque(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);

        int a = 0;
        peek_back_ideque(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_04(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);

        int a = 0;
        peek_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_05(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);

        int a = 0;
        peek_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_06(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);

        int a = 0;
        peek_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_01(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    for (int i = IDEQUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_02(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    for (int i = IDEQUE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        dequeue_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_03(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    for (int i = IDEQUE_CHUNK; i >= 0; --i) {
        int a = 0;
        dequeue_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_04(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_05(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_06(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_front_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_01(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_02(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_03(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_04(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    for (int i = IDEQUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_05(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    for (int i = IDEQUE_CHUNK - 1; i >= 0; --i)  {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_06(void) {
    ideque_s test = create_ideque(sizeof(int));


    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    for (int i = IDEQUE_CHUNK; i >= 0; --i)  {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST MAP_FRONT_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    each_front_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST MAP_FRONT_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    each_front_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST MAP_FRONT_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    each_front_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST MAP_BACK_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    each_back_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST MAP_BACK_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    each_back_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST MAP_BACK_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    each_back_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_ideque(&test, sort, &cmp);

    for (int i = IDEQUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_ideque(&test, sort, &cmp);

    for (int i = IDEQUE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_ideque(&test, sort, &cmp);

    for (int i = IDEQUE_CHUNK; i >= 0; --i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_ideque(&test, sort, &cmp);

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST APPLY_05(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_ideque(&test, sort, &cmp);

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST APPLY_06(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_back_ideque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_ideque(&test, sort, &cmp);

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

SUITE (ideque_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(ENQUEUE_FRONT_01); RUN_TEST(ENQUEUE_FRONT_02); RUN_TEST(ENQUEUE_FRONT_03);
    RUN_TEST(ENQUEUE_BACK_01); RUN_TEST(ENQUEUE_BACK_02); RUN_TEST(ENQUEUE_BACK_03);
    RUN_TEST(PEEK_FRONT_01); RUN_TEST(PEEK_FRONT_02); RUN_TEST(PEEK_FRONT_03);
    RUN_TEST(PEEK_FRONT_04); RUN_TEST(PEEK_FRONT_05); RUN_TEST(PEEK_FRONT_06);
    RUN_TEST(PEEK_BACK_01); RUN_TEST(PEEK_BACK_02); RUN_TEST(PEEK_BACK_03);
    RUN_TEST(PEEK_BACK_04); RUN_TEST(PEEK_BACK_05); RUN_TEST(PEEK_BACK_06);
    RUN_TEST(DEQUEUE_FRONT_01); RUN_TEST(DEQUEUE_FRONT_02); RUN_TEST(DEQUEUE_FRONT_03);
    RUN_TEST(DEQUEUE_FRONT_04); RUN_TEST(DEQUEUE_FRONT_05); RUN_TEST(DEQUEUE_FRONT_06);
    RUN_TEST(DEQUEUE_BACK_01); RUN_TEST(DEQUEUE_BACK_02); RUN_TEST(DEQUEUE_BACK_03);
    RUN_TEST(DEQUEUE_BACK_04); RUN_TEST(DEQUEUE_BACK_05); RUN_TEST(DEQUEUE_BACK_06);
    RUN_TEST(MAP_FRONT_01); RUN_TEST(MAP_FRONT_02); RUN_TEST(MAP_FRONT_03);
    RUN_TEST(MAP_BACK_01); RUN_TEST(MAP_BACK_02); RUN_TEST(MAP_BACK_03);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04); RUN_TEST(APPLY_05); RUN_TEST(APPLY_06);
}
