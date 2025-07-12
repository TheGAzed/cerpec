#include <sequence/queue.h>
#include <suite.h>

TEST CREATE_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(test_t), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected tail to be NULL.", NULL, test.tail);

    destroy_queue(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    destroy_queue(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_NEQm("[ERROR] Expected size to be zero.", sizeof(test_t), test.size);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected tail to be NULL.", NULL, test.tail);

    PASS();
}

TEST CLEAR_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    clear_queue(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(test_t), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);

    destroy_queue(&test, destroy);

    PASS();
}

TEST ENQUEUE_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        enqueue(&test, &i);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST ENQUEUE_02(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        enqueue(&test, &i);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST ENQUEUE_03(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        enqueue(&test, &i);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST PEEK_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        enqueue(&test, &i);

        test_t a = 0;
        peek_queue(test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", 0, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST PEEK_02(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        enqueue(&test, &i);

        test_t a = 0;
        peek_queue(test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", 0, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST PEEK_03(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        enqueue(&test, &i);

        test_t a = 0;
        peek_queue(test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", 0, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST DEQUEUE_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        enqueue(&test, &i);
    }

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST DEQUEUE_02(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        enqueue(&test, &i);
    }

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST DEQUEUE_03(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        enqueue(&test, &i);
    }

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST FOREACH_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        enqueue(&test, &i);
    }

    test_t value = 1;
    foreach_queue(test, increment, &value);

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST FOREACH_02(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        enqueue(&test, &i);
    }

    test_t value = 1;
    foreach_queue(test, increment, &value);

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST FOREACH_03(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        enqueue(&test, &i);
    }

    test_t value = 1;
    foreach_queue(test, increment, &value);

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST MAP_01(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        enqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    map_queue(test, sort, &cmp);

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        enqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    map_queue(test, sort, &cmp);

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST MAP_03(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        enqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    map_queue(test, sort, &cmp);

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST MAP_04(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK - 1; ++i) {
        enqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    map_queue(test, sort, &cmp);

    for (test_t i = QUEUE_CHUNK - 2; i >= 0; --i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST MAP_05(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK; ++i) {
        enqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    map_queue(test, sort, &cmp);

    for (test_t i = QUEUE_CHUNK - 1; i >= 0; --i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

TEST MAP_06(void) {
    queue_s test = create_queue(sizeof(test_t));

    for (test_t i = 0; i < QUEUE_CHUNK + 1; ++i) {
        enqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    map_queue(test, sort, &cmp);

    for (test_t i = QUEUE_CHUNK; i >= 0; --i) {
        test_t a = 0;
        dequeue(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_queue(&test, destroy);

    PASS();
}

SUITE (queue_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(ENQUEUE_01); RUN_TEST(ENQUEUE_02); RUN_TEST(ENQUEUE_03);
    RUN_TEST(PEEK_01); RUN_TEST(PEEK_02); RUN_TEST(PEEK_03);
    RUN_TEST(DEQUEUE_01); RUN_TEST(DEQUEUE_02); RUN_TEST(DEQUEUE_03);
    RUN_TEST(FOREACH_01); RUN_TEST(FOREACH_02); RUN_TEST(FOREACH_03);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02); RUN_TEST(MAP_03); RUN_TEST(MAP_04); RUN_TEST(MAP_05); RUN_TEST(MAP_06);
}
