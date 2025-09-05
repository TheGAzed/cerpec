#include <set/ibitwise_set.h>
#include <suite.h>

TEST CREATE_01(void) {
    ibitwise_set_s set = create_ibitwise_set();

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, set.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, set.length);

    destroy_ibitwise_set(&set);

    PASS();
}

TEST DESTROY_01(void) {
    ibitwise_set_s set = create_ibitwise_set();

    destroy_ibitwise_set(&set);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, set.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, set.length);

    PASS();
}

TEST CLEAR_01(void) {
    ibitwise_set_s set = create_ibitwise_set();

    clear_ibitwise_set(&set);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, set.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, set.length);

    destroy_ibitwise_set(&set);

    PASS();
}

TEST INSERT_01(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set, i);
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST INSERT_02(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set, i);
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST INSERT_03(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set, i);
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST REMOVE_01(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set, i);
    }

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        remove_ibitwise_set(&set, i);
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST REMOVE_02(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set, i);
    }

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        remove_ibitwise_set(&set, i);
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST REMOVE_03(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set, i);
    }

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        remove_ibitwise_set(&set, i);
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST CONTAINS_01(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set, i);
    }

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set, i));
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST CONTAINS_02(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set, i);
    }

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set, i));
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST CONTAINS_03(void) {
    ibitwise_set_s set = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set, i);
    }

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set, i));
    }

    destroy_ibitwise_set(&set);

    PASS();
}

TEST UNION_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 3; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 3; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST UNION_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 3; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_union = union_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_union, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_union);

    PASS();
}

TEST INTERSECT_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 3; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 3; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    for (size_t i = ((IBITWISE_SET_CHUNK - 1) / 3) * 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 3; i < ((IBITWISE_SET_CHUNK - 1) / 3) * 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 3; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 3; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    for (size_t i = ((IBITWISE_SET_CHUNK) / 3) * 2; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 3; i < ((IBITWISE_SET_CHUNK) / 3) * 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST INTERSECT_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 3; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_intersect = intersect_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 3; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    for (size_t i = ((IBITWISE_SET_CHUNK + 1) / 3) * 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_intersect, i));
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 3; i < ((IBITWISE_SET_CHUNK + 1) / 3) * 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_intersect, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_intersect);

    PASS();
}

TEST SUBTRACT_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 3; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 3; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST SUBTRACT_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 3; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_subtract = subtract_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_subtract, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_subtract);

    PASS();
}

TEST EXCLUDE_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 3; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    for (size_t i = ((IBITWISE_SET_CHUNK - 1) / 3) * 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 3; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    for (size_t i = ((IBITWISE_SET_CHUNK) / 3) * 2; i < IBITWISE_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST EXCLUDE_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 3; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ibitwise_set_s set_exclude = exclude_ibitwise_set(set_one, set_two);

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    for (size_t i = ((IBITWISE_SET_CHUNK + 1) / 3) * 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_ibitwise_set(set_exclude, i));
    }

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);
    destroy_ibitwise_set(&set_exclude);

    PASS();
}

TEST IS_SUBSET_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_SUBSET_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to be subset.", is_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    ASSERTm("[ERROR] Expected to be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    ASSERTm("[ERROR] Expected to be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    ASSERTm("[ERROR] Expected to be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_PROPER_SUBSET_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_01(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_02(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_03(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_one, i);
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_04(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK - 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 2; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERTm("[ERROR] Expected to be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_05(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 2; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERTm("[ERROR] Expected to be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_06(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < (IBITWISE_SET_CHUNK + 1) / 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 2; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERTm("[ERROR] Expected to be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_07(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK - 1) / 3; i < IBITWISE_SET_CHUNK - 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_08(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK) / 3; i < IBITWISE_SET_CHUNK; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

TEST IS_DISJOINT_09(void) {
    ibitwise_set_s set_one = create_ibitwise_set();
    ibitwise_set_s set_two = create_ibitwise_set();

    for (size_t i = 0; i < ((IBITWISE_SET_CHUNK + 1) / 3) * 2; ++i) {
        insert_ibitwise_set(&set_one, i);
    }

    for (size_t i = (IBITWISE_SET_CHUNK + 1) / 3; i < IBITWISE_SET_CHUNK + 1; ++i) {
        insert_ibitwise_set(&set_two, i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjosize_t.", is_disjoint_ibitwise_set(set_one, set_two));

    destroy_ibitwise_set(&set_one);
    destroy_ibitwise_set(&set_two);

    PASS();
}

SUITE (ibitwise_set_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
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
