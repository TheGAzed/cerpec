#include <misc/ibinary_heap.h>
#include <suite.h>

TEST CREATE_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, heap.capacity);
    ASSERT_NEQm("[ERROR] Expected compare function to not be NULL.", NULL, heap.compare);
    ASSERT_EQm("[ERROR] Expected elements array to be NULL.", NULL, heap.elements);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, heap.length);
    ASSERT_NEQm("[ERROR] Expected element size to not be zero.", 0, heap.size);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    destroy_ibinary_heap(&heap, destroy);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, heap.capacity);
    ASSERT_EQm("[ERROR] Expected compare function to be NULL.", NULL, heap.compare);
    ASSERT_EQm("[ERROR] Expected elements array to be NULL.", NULL, heap.elements);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, heap.length);
    ASSERT_EQm("[ERROR] Expected element size to be zero.", 0, heap.size);

    PASS();
}

TEST CLEAR_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    clear_ibinary_heap(&heap, destroy);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, heap.capacity);
    ASSERT_NEQm("[ERROR] Expected compare function to not be NULL.", NULL, heap.compare);
    ASSERT_EQm("[ERROR] Expected elements array to be NULL.", NULL, heap.elements);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, heap.length);
    ASSERT_NEQm("[ERROR] Expected element size to not be zero.", 0, heap.size);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST COPY_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    ibinary_heap_s replica = copy_ibinary_heap(&heap, copy);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        int a = 0, b = 0;
        pop_ibinary_heap(&heap, &a);
        pop_ibinary_heap(&replica, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", a, b);
    }

    destroy_ibinary_heap(&heap, destroy);
    destroy_ibinary_heap(&replica, destroy);

    PASS();
}

TEST COPY_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    ibinary_heap_s replica = copy_ibinary_heap(&heap, copy);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        int a = 0, b = 0;
        pop_ibinary_heap(&heap, &a);
        pop_ibinary_heap(&replica, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", a, b);
    }

    destroy_ibinary_heap(&heap, destroy);
    destroy_ibinary_heap(&replica, destroy);

    PASS();
}

TEST COPY_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    ibinary_heap_s replica = copy_ibinary_heap(&heap, copy);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        int a = 0, b = 0;
        pop_ibinary_heap(&heap, &a);
        pop_ibinary_heap(&replica, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", a, b);
    }

    destroy_ibinary_heap(&heap, destroy);
    destroy_ibinary_heap(&replica, destroy);

    PASS();
}

TEST PUSH_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PUSH_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PUSH_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST POP_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        int a = 0;
        pop_ibinary_heap(&heap, &a);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", a, i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST POP_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        int a = 0;
        pop_ibinary_heap(&heap, &a);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", a, i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST POP_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        int a = 0;
        pop_ibinary_heap(&heap, &a);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", a, i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PEEP_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int a = 42;
    peep_ibinary_heap(&heap, &a);
    ASSERT_EQm("[ERROR] Expected elements to be equal.", 0, a);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PEEP_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int a = 42;
    peep_ibinary_heap(&heap, &a);
    ASSERT_EQm("[ERROR] Expected elements to be equal.", 0, a);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PEEP_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int a = 42;
    peep_ibinary_heap(&heap, &a);
    ASSERT_EQm("[ERROR] Expected elements to be equal.", 0, a);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

SUITE (ibinary_heap_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03);
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02); RUN_TEST(PUSH_03);
    RUN_TEST(POP_01); RUN_TEST(POP_02); RUN_TEST(POP_03);
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02); RUN_TEST(PEEP_03);
}
