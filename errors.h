#ifndef ERRORS
#define ERRORS

enum 
{
    NON_NUMERIC_STRING_ERROR,
    NUM_ARGS_ERROR,
    FILE_TYPE_ERROR,
    CONFIG_FILE_OPEN_ERROR,
    VERSION_PHASE_VALUE_ERROR,
    UNSUPPORTED_CPU_SCHED_ERROR,
    QUANTUM_TIME_VALUE_ERROR,
    MEMORY_AVAIL_VALUE_ERROR,
    CPU_CYCLE_TIME_VALUE_ERROR,
    IO_CYCLE_TIME_VALUE_ERROR,
    INVALID_LOG_TO_ERROR,
    MDF_LOCATION_ERROR,
    MDF_MEMORY_UNIT_ERROR,
    MDF_OPEN_ERROR,
    SA_OP_STRING_ERROR,
    P_OP_STRING_ERROR,
    I_OP_STRING_ERROR,
    O_OP_STRING_ERROR,
    M_OP_STRING_ERROR,
    NEGATIVE_MDF_VALUE_ERROR,
    CORRUPTED_MDF_ERROR
};

//DISPLAYS ERRORS
void displayError(int err);

#endif