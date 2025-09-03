target_sources(CERPEC
        PRIVATE sequence/istack.c
        PRIVATE sequence/iqueue.c
        PRIVATE sequence/ideque.c

        PRIVATE list/istraight_list.c
        PRIVATE list/icircular_list.c
        PRIVATE list/idouble_list.c

        PRIVATE priority/ibinary_heap.c

        PRIVATE table/ihash_table.c

        PRIVATE set/ihash_set.c
        PRIVATE set/irb_set.c
        PRIVATE set/ibitwise_set.c

        PRIVATE tree/ibsearch_tree.c
        PRIVATE tree/iavl_tree.c
        PRIVATE tree/irb_tree.c

        PRIVATE misc/itrie.c
)
