add_library(${PROJECT_NAME} cerpec.c
        sequence/istack.c
        sequence/iqueue.c
        sequence/ideque.c

        list/istraight_list.c
        list/icircular_list.c
        list/idouble_list.c

        set/ihash_set.c
        set/irb_set.c
        set/ibitwise_set.c

        tree/ibsearch_tree.c
        tree/iavl_tree.c
        tree/irb_tree.c

        graph/imatrix_graph.c

        misc/ihash_table.c
        misc/ibinary_heap.c

        sequence/fstack.c
        sequence/fqueue.c
)
