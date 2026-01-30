add_library(${PROJECT_NAME} cerpec.c
        sequence/istack.c sequence/iqueue.c sequence/ideque.c
        sequence/fstack.c sequence/fqueue.c sequence/fdeque.c

        list/istraight_list.c list/icircular_list.c list/idouble_list.c
        list/fstraight_list.c list/fcircular_list.c list/fdouble_list.c

        set/ihash_set.c set/irb_set.c set/ibitwise_set.c
        set/fbitwise_set.c set/fhash_set.c set/frb_set.c

        tree/ibsearch_tree.c tree/iavl_tree.c tree/irb_tree.c
        tree/fbsearch_tree.c tree/favl_tree.c

        graph/imatrix_graph.c

        misc/ihash_map.c misc/ibinary_heap.c
        misc/fbinary_heap.c misc/fhash_map.c
        tree/frb_tree.c
)
