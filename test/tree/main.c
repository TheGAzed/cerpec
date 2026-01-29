#include <suite.h>

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(ibsearch_tree_test);
    RUN_SUITE(iavl_tree_test);
    RUN_SUITE(irb_tree_test);

    RUN_SUITE(fbsearch_tree_test);
    //RUN_SUITE(favl_tree_test);
    //RUN_SUITE(frb_tree_test);

    GREATEST_MAIN_END();
}
