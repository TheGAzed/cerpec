#include <suite.h>

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(istack_test);
    RUN_SUITE(iqueue_test);
    RUN_SUITE(ideque_test);

    GREATEST_MAIN_END();
}
