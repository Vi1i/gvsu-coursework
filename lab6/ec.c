

struct semNode {
    PCB         *PCB;

}

struct semaphore {
    int             value;
    struct semNode  * head,
                    * tail;
}
