#include <sequence/ideque.h>
#include <suite.h>

TEST CREATE_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(int), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected head to be NULL.", NULL, test.head);

    destroy_ideque(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    destroy_ideque(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_NEQm("[ERROR] Expected size to be zero.", sizeof(int), test.size);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected head to be NULL.", NULL, test.head);

    PASS();
}

TEST CLEAR_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    clear_ideque(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(int), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);

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
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
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
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST FOREACH_FRONT_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    map_front_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST FOREACH_FRONT_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    map_front_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST FOREACH_FRONT_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    map_front_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST FOREACH_BACK_01(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    map_back_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST FOREACH_BACK_02(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    map_back_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_ideque(&test, destroy);

    PASS();
}

TEST FOREACH_BACK_03(void) {
    ideque_s test = create_ideque(sizeof(int));

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        enqueue_front_ideque(&test, &i);
    }

    int value = 1;
    map_back_ideque(&test, increment, &value);

    for (int i = 0; i < IDEQUE_CHUNK + 1; ++i) {
        int a = 0;
        dequeue_back_ideque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
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
    RUN_TEST(FOREACH_FRONT_01); RUN_TEST(FOREACH_FRONT_02); RUN_TEST(FOREACH_FRONT_03);
    RUN_TEST(FOREACH_BACK_01); RUN_TEST(FOREACH_BACK_02); RUN_TEST(FOREACH_BACK_03);
    //RUN_TEST(MAP_01); RUN_TEST(MAP_02); RUN_TEST(MAP_03); RUN_TEST(MAP_04); RUN_TEST(MAP_05); RUN_TEST(MAP_06);
}
