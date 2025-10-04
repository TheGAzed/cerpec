#include <tree/irb_tree.h>
#include <suite.h>

TEST CREATE_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, tree.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, tree.length);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, tree.size);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    destroy_irb_tree(&tree, destroy);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, tree.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, tree.length);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, tree.size);
    ASSERT_EQm("[ERROR] Expected elements to be NULL.", NULL, tree.elements);

    PASS();
}

TEST CLEAR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    clear_irb_tree(&tree, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, tree.length);
    ASSERT_NEQm("[ERROR] Expected size to be zero.", 0, tree.size);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST INSERT_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST INSERT_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST INSERT_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = IRB_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_irb_tree(&tree, &i, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = IRB_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_irb_tree(&tree, &i, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = IRB_TREE_CHUNK; i >= 0; --i) {
        int a = 0;
        remove_irb_tree(&tree, &i, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_irb_tree(&tree, &i));
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_irb_tree(&tree, &i));
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST CONTAINS_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_irb_tree(&tree, &i));
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_MAX_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_max_irb_tree(&tree, &a);
        remove_irb_tree(&tree, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_MAX_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_max_irb_tree(&tree, &a);
        remove_irb_tree(&tree, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_MAX_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = 0, b = 0;

        get_max_irb_tree(&tree, &a);
        remove_irb_tree(&tree, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_MIN_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_min_irb_tree(&tree, &a);
        remove_irb_tree(&tree, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_MIN_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_min_irb_tree(&tree, &a);
        remove_irb_tree(&tree, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_MIN_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = 0, b = 0;

        get_min_irb_tree(&tree, &a);
        remove_irb_tree(&tree, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_MAX_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = IRB_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_max_irb_tree(&tree, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_MAX_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = IRB_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_max_irb_tree(&tree, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_MAX_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = IRB_TREE_CHUNK; i >= 0; --i) {
        int a = 0;
        remove_max_irb_tree(&tree, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_MIN_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = 0;
        remove_min_irb_tree(&tree, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_MIN_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = 0;
        remove_min_irb_tree(&tree, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_MIN_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = 0;
        remove_min_irb_tree(&tree, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_FLOOR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    const int a = (IRB_TREE_CHUNK - 1) / 2;
    int buf = 0;
    remove_irb_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_irb_tree(&tree, &a, &floor);

    ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, floor);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_FLOOR_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }
    const int a = (IRB_TREE_CHUNK) / 2;
    int buf = 0;
    remove_irb_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_irb_tree(&tree, &a, &floor);

    ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, floor);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_FLOOR_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    const int a = (IRB_TREE_CHUNK + 1) / 2;
    int buf = 0;
    remove_irb_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_irb_tree(&tree, &a, &floor);

    ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, floor);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    const int a = (IRB_TREE_CHUNK - 1) / 2;
    int buf = 0;
    remove_irb_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_irb_tree(&tree, &a, &ceil);

    ASSERT_EQm("[ERROR] Expected elements to be equal.", a + 1, ceil);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }
    const int a = (IRB_TREE_CHUNK) / 2;
    int buf = 0;
    remove_irb_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_irb_tree(&tree, &a, &ceil);

    ASSERT_EQm("[ERROR] Expected elements to be equal.", a + 1, ceil);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    const int a = (IRB_TREE_CHUNK + 1) / 2;
    int buf = 0;
    remove_irb_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_irb_tree(&tree, &a, &ceil);

    ASSERT_EQm("[ERROR] Expected elements to be equal.", a + 1, ceil);

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    int a = 0;
    remove_max_irb_tree(&tree, &a);

    while (!is_empty_irb_tree(&tree)) {
        int b = 0;
        remove_floor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);

        a = b;
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }
    int a = 0;
    remove_max_irb_tree(&tree, &a);

    while (!is_empty_irb_tree(&tree)) {
        int b = 0;
        remove_floor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);

        a = b;
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    int a = 0;
    remove_max_irb_tree(&tree, &a);

    while (!is_empty_irb_tree(&tree)) {
        int b = 0;
        remove_floor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);

        a = b;
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    int a = 0;
    remove_min_irb_tree(&tree, &a);

    while (!is_empty_irb_tree(&tree)) {
        int b = 0;
        remove_ceil_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a + 1, b);

        a = b;
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }
    int a = 0;
    remove_min_irb_tree(&tree, &a);

    while (!is_empty_irb_tree(&tree)) {
        int b = 0;
        remove_ceil_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a + 1, b);

        a = b;
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }
    int a = 0;
    remove_min_irb_tree(&tree, &a);

    while (!is_empty_irb_tree(&tree)) {
        int b = 0;
        remove_ceil_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a + 1, b);

        a = b;
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = i - 1, b = 0;
        get_successor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = i - 1, b = 0;
        get_successor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = i - 1, b = 0;
        get_successor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = IRB_TREE_CHUNK - 1 - i, b = 0;
        get_predecessor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = IRB_TREE_CHUNK - i, b = 0;
        get_predecessor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = IRB_TREE_CHUNK + 1 - i, b = 0;
        get_predecessor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = i - 1, b = 0;
        remove_successor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = i - 1, b = 0;
        remove_successor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = i - 1, b = 0;
        remove_successor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_01(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK - 1; ++i) {
        int a = IRB_TREE_CHUNK - 1 - i, b = 0;
        remove_predecessor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_02(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK; ++i) {
        int a = IRB_TREE_CHUNK - i, b = 0;
        remove_predecessor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_03(void) {
    irb_tree_s tree = create_irb_tree(sizeof(int), compare);

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        insert_irb_tree(&tree, &i);
    }

    for (int i = 0; i < IRB_TREE_CHUNK + 1; ++i) {
        int a = IRB_TREE_CHUNK + 1 - i, b = 0;
        remove_predecessor_irb_tree(&tree, &a, &b);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", a - 1, b);
    }

    destroy_irb_tree(&tree, destroy);

    PASS();
}

SUITE (irb_tree_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02); RUN_TEST(INSERT_03);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02); RUN_TEST(REMOVE_03);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02); RUN_TEST(CONTAINS_03);
    RUN_TEST(GET_MAX_01); RUN_TEST(GET_MAX_02); RUN_TEST(GET_MAX_03);
    RUN_TEST(GET_MIN_01); RUN_TEST(GET_MIN_02); RUN_TEST(GET_MIN_03);
    RUN_TEST(REMOVE_MAX_01); RUN_TEST(REMOVE_MAX_02); RUN_TEST(REMOVE_MAX_03);
    RUN_TEST(REMOVE_MIN_01); RUN_TEST(REMOVE_MIN_02); RUN_TEST(REMOVE_MIN_03);
    RUN_TEST(GET_FLOOR_01); RUN_TEST(GET_FLOOR_02); RUN_TEST(GET_FLOOR_03);
    RUN_TEST(GET_CEIL_01); RUN_TEST(GET_CEIL_02); RUN_TEST(GET_CEIL_03);
    RUN_TEST(REMOVE_FLOOR_01); RUN_TEST(REMOVE_FLOOR_02); RUN_TEST(REMOVE_FLOOR_03);
    RUN_TEST(REMOVE_CEIL_01); RUN_TEST(REMOVE_CEIL_02); RUN_TEST(REMOVE_CEIL_03);
    RUN_TEST(GET_SUCCESSOR_01); RUN_TEST(GET_SUCCESSOR_02); RUN_TEST(GET_SUCCESSOR_03);
    RUN_TEST(GET_PREDECESSOR_01); RUN_TEST(GET_PREDECESSOR_02); RUN_TEST(GET_PREDECESSOR_03);
    RUN_TEST(REMOVE_SUCCESSOR_01); RUN_TEST(REMOVE_SUCCESSOR_02); RUN_TEST(REMOVE_SUCCESSOR_03);
    RUN_TEST(REMOVE_PREDECESSOR_01); RUN_TEST(REMOVE_PREDECESSOR_02); RUN_TEST(REMOVE_PREDECESSOR_03);
}
