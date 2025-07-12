#include <sequence/stack.h>
#include <suite.h>

TEST CREATE_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.capacity);
    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected capacity to be zero.", sizeof(test_t), test.size);
    ASSERT_NEQm("[ERROR] Expected capacity to be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected capacity to be NULL.", NULL, test.elements);

    destroy_stack(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    destroy_stack(&test, destroy);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.capacity);
    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.length);
    ASSERT_NEQm("[ERROR] Expected capacity to be zero.", sizeof(test_t), test.size);
    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected capacity to be NULL.", NULL, test.elements);

    PASS();
}

TEST CLEAR_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    clear_stack(&test, destroy);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected capacity to be zero.", sizeof(test_t), test.size);
    ASSERT_NEQm("[ERROR] Expected capacity to be zero.", 0, test.size);

    destroy_stack(&test, destroy);

    PASS();
}

TEST PUSH_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        push_stack(&test, &i);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST PUSH_02(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        push_stack(&test, &i);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST PUSH_03(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        push_stack(&test, &i);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST PEEP_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        push_stack(&test, &i);

        test_t a = 0;
        peep_stack(test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST PEEP_02(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        push_stack(&test, &i);

        test_t a = 0;
        peep_stack(test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST PEEP_03(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        push_stack(&test, &i);

        test_t a = 0;
        peep_stack(test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST POP_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        push_stack(&test, &i);
    }

    for (test_t i = STACK_CHUNK - 2; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST POP_02(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        push_stack(&test, &i);
    }

    for (test_t i = STACK_CHUNK - 1; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST POP_03(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        push_stack(&test, &i);
    }

    for (test_t i = STACK_CHUNK; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST FOREACH_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        push_stack(&test, &i);
    }

    test_t value = 1;
    foreach_stack(test, increment, &value);

    for (test_t i = STACK_CHUNK - 2; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST FOREACH_02(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        push_stack(&test, &i);
    }

    test_t value = 1;
    foreach_stack(test, increment, &value);

    for (test_t i = STACK_CHUNK - 1; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST FOREACH_03(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        push_stack(&test, &i);
    }

    test_t value = 1;
    foreach_stack(test, increment, &value);

    for (test_t i = STACK_CHUNK; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST MAP_01(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        push_stack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    map_stack(test, sort, &cmp);

    for (test_t i = STACK_CHUNK - 2; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        push_stack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    map_stack(test, sort, &cmp);

    for (test_t i = STACK_CHUNK - 1; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST MAP_03(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        push_stack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    map_stack(test, sort, &cmp);

    for (test_t i = STACK_CHUNK; i >= 0; --i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST MAP_04(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        push_stack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    map_stack(test, sort, &cmp);

    for (test_t i = 0; i < STACK_CHUNK - 1; ++i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST MAP_05(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        push_stack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    map_stack(test, sort, &cmp);

    for (test_t i = 0; i < STACK_CHUNK; ++i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

TEST MAP_06(void) {
    stack_s test = create_stack(sizeof(test_t));

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        push_stack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    map_stack(test, sort, &cmp);

    for (test_t i = 0; i < STACK_CHUNK + 1; ++i) {
        test_t a = 0;
        pop_stack(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_stack(&test, destroy);

    PASS();
}

SUITE (stack_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02); RUN_TEST(PUSH_03);
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02); RUN_TEST(PEEP_03);
    RUN_TEST(POP_01); RUN_TEST(POP_02); RUN_TEST(POP_03);
    RUN_TEST(FOREACH_01); RUN_TEST(FOREACH_02); RUN_TEST(FOREACH_03);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02); RUN_TEST(MAP_03); RUN_TEST(MAP_04); RUN_TEST(MAP_05); RUN_TEST(MAP_06);
}
