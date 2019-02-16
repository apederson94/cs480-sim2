#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

//all data structures created by me

//stores all of the config values provided by the configuration file
struct config_values{
    float ver;

    char *mdf_path;
    char *cpu_sched;
    char *log_path;
    char *log_to;

    int quantum_time;
    int memory_available;
    int cpu_cycle_time;
    int io_cycle_time;
};

//stores an action specified in the meta data file as well as the action after it
struct sim_action{
    char command_letter;
    char operation_string[20];
    int assoc_val;
    struct sim_action *next;
};

//frees all memory associated with sim_action linked list
void free_actions(struct sim_action *head) {
    struct sim_action *tmp;

    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

//prints all relevant sim_action info.
void print_sim_action(struct sim_action *src) {
    printf("Op code letter: %c\n", src->command_letter);
    printf("Op code name  : %s\n", src->operation_string);
    printf("Op code value : %d\n", src->assoc_val);
}

//prints all relevant sim_action info for all sim_actions in a list
void print_sim_actions(struct sim_action *head) {
    while (head->next) {
        print_sim_action(head);
        printf("\n");
        head = head->next;
    }
}

//prints all relevant config_values info
void print_config_values(struct config_values *src) {
    printf("Version                : %f\n", src->ver);
    printf("Program file name      : %s\n", src->mdf_path);
    printf("CPU schedule selection : %s\n", src->cpu_sched);
    printf("Quantum time           : %d\n", src->quantum_time);
    printf("Memory Available       : %d\n", src->memory_available);
    printf("Process cycle rate     : %d\n", src->cpu_cycle_time);
    printf("I/O cycle rate         : %d\n", src->io_cycle_time);
    printf("Log to selection       : %s\n", src->log_to);
    printf("Log file name          : %s\n", src->log_path);
}

#endif