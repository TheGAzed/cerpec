#include <set/frb_set.h>
#include <suite.h>

#define FRB_SET_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, set.length);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, set.size);

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    destroy_frb_set(&set, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, set.length);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, set.size);

    PASS();
}

TEST CLEAR_01(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    clear_frb_set(&set, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, set.length);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, set.size);

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST INSERT_01(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set, &i);
    }

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST INSERT_02(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set, &i);
    }

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set, &i);
    }

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        int buf = 0;
        remove_frb_set(&set, &i, &buf);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, buf);
    }

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set, &i);
    }

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        int buf = 0;
        remove_frb_set(&set, &i, &buf);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, buf);
    }

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set, &i);
    }

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set, &i));
    }

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    frb_set_s set = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set, &i);
    }

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set, &i));
    }

    destroy_frb_set(&set, destroy);

    PASS();
}

TEST UNION_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_union = union_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_union, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_union, destroy);

    PASS();
}

TEST UNION_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_union = union_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_union, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_union, destroy);

    PASS();
}

TEST UNION_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_union = union_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_union, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_union, destroy);

    PASS();
}

TEST UNION_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_union = union_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_union, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_union, destroy);

    PASS();
}

TEST UNION_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 3; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_union = union_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_union, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_union, destroy);

    PASS();
}

TEST UNION_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 3; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_union = union_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_union, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_union, destroy);

    PASS();
}

TEST INTERSECT_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_intersect = intersect_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_intersect, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_intersect = intersect_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_intersect, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_intersect = intersect_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_intersect, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_intersect = intersect_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_intersect, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 3; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_intersect = intersect_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 3; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_intersect, &i));
    }

    for (int i = ((FRB_SET_CHUNK - 1) / 3) * 2; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_intersect, &i));
    }

    for (int i = (FRB_SET_CHUNK - 1) / 3; i < ((FRB_SET_CHUNK - 1) / 3) * 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_intersect, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_intersect, destroy);

    PASS();
}

TEST INTERSECT_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 3; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_intersect = intersect_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK) / 3; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_intersect, &i));
    }

    for (int i = ((FRB_SET_CHUNK) / 3) * 2; i < FRB_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_intersect, &i));
    }

    for (int i = (FRB_SET_CHUNK) / 3; i < ((FRB_SET_CHUNK) / 3) * 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_intersect, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_intersect, destroy);

    PASS();
}

TEST SUBTRACT_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_subtract = subtract_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_subtract, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_subtract = subtract_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_subtract, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_subtract = subtract_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_subtract, &i));
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to be contained.", contains_frb_set(&set_subtract, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_subtract = subtract_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_subtract, &i));
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to be contained.", contains_frb_set(&set_subtract, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 3; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_subtract = subtract_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_subtract, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_subtract, destroy);

    PASS();
}

TEST SUBTRACT_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 3; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_subtract = subtract_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_subtract, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_subtract, destroy);

    PASS();
}

TEST EXCLUDE_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_exclude = exclude_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_exclude, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_exclude = exclude_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERT_FALSEm("[ERROR] Expected element to not be contained.", contains_frb_set(&set_exclude, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_exclude = exclude_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_exclude, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_exclude = exclude_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_exclude, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 3; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_exclude = exclude_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_exclude, &i));
    }

    for (int i = ((FRB_SET_CHUNK - 1) / 3) * 2; i < FRB_SET_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_exclude, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_exclude, destroy);

    PASS();
}

TEST EXCLUDE_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 3; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    frb_set_s set_exclude = exclude_frb_set(&set_one, &set_two, copy);

    for (int i = 0; i < (FRB_SET_CHUNK) / 3; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_exclude, &i));
    }

    for (int i = ((FRB_SET_CHUNK) / 3) * 2; i < FRB_SET_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_frb_set(&set_exclude, &i));
    }

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);
    destroy_frb_set(&set_exclude, destroy);

    PASS();
}

TEST IS_SUBSET_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    ASSERTm("[ERROR] Expected to be subset.", is_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to be subset.", is_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_SUBSET_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to be subset.", is_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
    }

    ASSERTm("[ERROR] Expected to be proper subset.", is_proper_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
    }

    ASSERTm("[ERROR] Expected to be proper subset.", is_proper_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_PROPER_SUBSET_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be proper subset.", is_proper_subset_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_01(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjoint.", is_disjoint_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_02(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_one, &i);
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjoint.", is_disjoint_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_03(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK - 1) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 2; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERTm("[ERROR] Expected to be disjoint.", is_disjoint_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_04(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < (FRB_SET_CHUNK) / 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 2; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERTm("[ERROR] Expected to be disjoint.", is_disjoint_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_05(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK - 1) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK - 1) / 3; i < FRB_SET_CHUNK - 1; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjoint.", is_disjoint_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

TEST IS_DISJOINT_06(void) {
    frb_set_s set_one = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);
    frb_set_s set_two = create_frb_set(sizeof(int), FRB_SET_CHUNK, compare);

    for (int i = 0; i < ((FRB_SET_CHUNK) / 3) * 2; ++i) {
        insert_frb_set(&set_one, &i);
    }

    for (int i = (FRB_SET_CHUNK) / 3; i < FRB_SET_CHUNK; ++i) {
        insert_frb_set(&set_two, &i);
    }

    ASSERT_FALSEm("[ERROR] Expected to not be disjoint.", is_disjoint_frb_set(&set_one, &set_two));

    destroy_frb_set(&set_one, destroy);
    destroy_frb_set(&set_two, destroy);

    PASS();
}

SUITE (frb_set_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02);
    RUN_TEST(UNION_01); RUN_TEST(UNION_02); RUN_TEST(UNION_03);
    RUN_TEST(UNION_04); RUN_TEST(UNION_05); RUN_TEST(UNION_06);
    RUN_TEST(INTERSECT_01); RUN_TEST(INTERSECT_02); RUN_TEST(INTERSECT_03);
    RUN_TEST(INTERSECT_04); RUN_TEST(INTERSECT_05); RUN_TEST(INTERSECT_06);
    RUN_TEST(SUBTRACT_01); RUN_TEST(SUBTRACT_02); RUN_TEST(SUBTRACT_03);
    RUN_TEST(SUBTRACT_04); RUN_TEST(SUBTRACT_05); RUN_TEST(SUBTRACT_06);
    RUN_TEST(EXCLUDE_01); RUN_TEST(EXCLUDE_02); RUN_TEST(EXCLUDE_03);
    RUN_TEST(EXCLUDE_04); RUN_TEST(EXCLUDE_05); RUN_TEST(EXCLUDE_06);
    RUN_TEST(IS_SUBSET_01); RUN_TEST(IS_SUBSET_02); RUN_TEST(IS_SUBSET_03);
    RUN_TEST(IS_SUBSET_04); RUN_TEST(IS_SUBSET_05); RUN_TEST(IS_SUBSET_06);
    RUN_TEST(IS_PROPER_SUBSET_01); RUN_TEST(IS_PROPER_SUBSET_02); RUN_TEST(IS_PROPER_SUBSET_03);
    RUN_TEST(IS_PROPER_SUBSET_04); RUN_TEST(IS_PROPER_SUBSET_05); RUN_TEST(IS_PROPER_SUBSET_06);
    RUN_TEST(IS_DISJOINT_01); RUN_TEST(IS_DISJOINT_02); RUN_TEST(IS_DISJOINT_03);
    RUN_TEST(IS_DISJOINT_04); RUN_TEST(IS_DISJOINT_05); RUN_TEST(IS_DISJOINT_06);
}
