#include <suite.h>

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(ibitwise_set_test);
    RUN_SUITE(ihash_set_test);
    RUN_SUITE(irb_set_test);

    GREATEST_MAIN_END();
}
