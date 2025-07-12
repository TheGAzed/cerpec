#include <sequence/deque.h>
#include <suite.h>

TEST CREATE_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(test_t), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected head to be NULL.", NULL, test.head);

    destroy_deque(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    destroy_deque(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_NEQm("[ERROR] Expected size to be zero.", sizeof(test_t), test.size);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected head to be NULL.", NULL, test.head);

    PASS();
}

TEST CLEAR_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    clear_deque(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(test_t), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);

    destroy_deque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_02(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_03(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_back_deque(&test, &i);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_02(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_back_deque(&test, &i);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_03(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_back_deque(&test, &i);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);

        test_t a = 0;
        peek_front_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_02(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);

        test_t a = 0;
        peek_front_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_03(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);

        test_t a = 0;
        peek_front_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_04(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_back_deque(&test, &i);

        test_t a = 0;
        peek_front_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_05(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_back_deque(&test, &i);

        test_t a = 0;
        peek_front_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_06(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_back_deque(&test, &i);

        test_t a = 0;
        peek_front_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);

        test_t a = 0;
        peek_back_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_02(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);

        test_t a = 0;
        peek_back_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_03(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);

        test_t a = 0;
        peek_back_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_04(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_back_deque(&test, &i);

        test_t a = 0;
        peek_back_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_05(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_back_deque(&test, &i);

        test_t a = 0;
        peek_back_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_06(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_back_deque(&test, &i);

        test_t a = 0;
        peek_back_deque(test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_01(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    for (test_t i = DEQUE_CHUNK - 2; i >= 0; --i) {
        test_t a = 0;
        dequeue_front_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_02(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);
    }

    for (test_t i = DEQUE_CHUNK - 1; i >= 0; --i) {
        test_t a = 0;
        dequeue_front_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_03(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    for (test_t i = DEQUE_CHUNK; i >= 0; --i) {
        test_t a = 0;
        dequeue_front_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_04(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_back_deque(&test, &i);
    }

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue_front_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_05(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_back_deque(&test, &i);
    }

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue_front_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_06(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_back_deque(&test, &i);
    }

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue_front_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_01(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_02(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);
    }

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_03(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_04(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_back_deque(&test, &i);
    }

    for (test_t i = DEQUE_CHUNK - 2; i >= 0; --i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_05(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_back_deque(&test, &i);
    }

    for (test_t i = DEQUE_CHUNK - 1; i >= 0; --i)  {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_06(void) {
    deque_s test = create_deque(sizeof(test_t));


    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_back_deque(&test, &i);
    }

    for (test_t i = DEQUE_CHUNK; i >= 0; --i)  {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST FOREACH_FRONT_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    test_t value = 1;
    foreach_front_deque(test, increment, &value);

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST FOREACH_FRONT_02(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);
    }

    test_t value = 1;
    foreach_front_deque(test, increment, &value);

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST FOREACH_FRONT_03(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    test_t value = 1;
    foreach_front_deque(test, increment, &value);

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST FOREACH_BACK_01(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    test_t value = 1;
    foreach_back_deque(test, increment, &value);

    for (test_t i = 0; i < DEQUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST FOREACH_BACK_02(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        enqueue_front_deque(&test, &i);
    }

    test_t value = 1;
    foreach_back_deque(test, increment, &value);

    for (test_t i = 0; i < DEQUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

TEST FOREACH_BACK_03(void) {
    deque_s test = create_deque(sizeof(test_t));

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        enqueue_front_deque(&test, &i);
    }

    test_t value = 1;
    foreach_back_deque(test, increment, &value);

    for (test_t i = 0; i < DEQUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue_back_deque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_deque(&test, destroy);

    PASS();
}

SUITE (deque_test) {
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
