#include <stdio.h>
#include "str_utils.h"
#include "booleans.h"
#include "data_structures.h"
#ifndef FILE_UTILS
#define FILE_UTILS

//all file utilities created by me

//reads values in from config file 
int read_config_file(FILE* config, struct config_values *settings) {
    char line[100];
    int pos;
    int len;
    int val_iter = 0;
    while (fgets(line, 100, config)) {
        char *value = (char*) malloc(sizeof(char) * 100);
        //gets position of the termination of the line name
        pos = sub_str_pos(line, ":");
        //continues only if the line contains a ":" symbol
        if (pos > 0) {
            //gets the length of hte line
            len = str_len(line);
            //gets a sub string from the position of the line name end to the end of the line
            sub_str(line, pos, len, value);
            if (str_contains(line, "Version/Phase")) {
                remove_non_chars(value);
                float ver = a2f(value);
                //returns error code 1 if versioning is incorrect
                if (ver > 10.0f || ver < 0.0f) {
                    return 4;
                }
                settings->ver = ver;

            } else if (str_contains(line, "Log File")) {
                remove_non_chars(value);
                //stores log path value if File or Both is chosen, N/A otherwise
                if (str_cmp(settings->log_to, "File") 
                || str_cmp(settings->log_to, "Both")) {
                    str_copy(value, settings->log_path);
                } else {
                    settings->log_path = "N/A";
                }

            } else if (str_contains(line, "CPU Scheduling Code")) {
                remove_non_chars(value);
                /*returns error code 2 if cpu scheduling code is incorrect
                FCFS-N is chosen if NONE was supplised by the config file*/
                if (str_cmp(value, "NONE")) {
                    settings->cpu_sched = "FCFS-N";
                } else if (!check_cpu_sched(value)) {
                    return 5;
                 } else {
                    settings->cpu_sched = value;
                 }

            } else if (str_contains(line, "Quantum Time")) {
                remove_non_chars(value);
                int num = a2i(value);
                //returns error code 3 if quantum time supplied is incorrect
                if (num > 100 || num < 0) {
                    return 6;
                }
                settings->quantum_time = num;

            } else if (str_contains(line, "Memory Available")) {
                remove_non_chars(value);
                int num = a2i(value);
                //returns error code 4 if too much or too little memory available
                if (num < 0 || num > 102400) {
                    return 7;
                }
                settings->memory_available = a2i(value);

            } else if (str_contains(line, "Processor Cycle Time")) {
                remove_non_chars(value);
                int num = a2i(value);
                //return error code 5 if processor cycle time too high or too low
                if (num < 1 || num > 1000) {
                    return 8;
                }
                settings->cpu_cycle_time = num;

            } else if (str_contains(line, "I/O Cycle Time")) {
                remove_non_chars(value);
                int num = a2i(value);
                //returns error code 6 if I/O cycle time too high or too low
                if (num < 1 || num > 10000) {
                    return 9;
                }
                settings->io_cycle_time = num;

            } else if (str_contains(line, "Log To")) {
                remove_non_chars(value);
                //returns error code 7 if log to line contains an unsupported option
                if (!check_log_to(value)) {
                    return 10;
                }
                settings->log_to = value;

            } else if (str_contains(line, "File Path:")) {
                remove_non_chars(value);
                //returns error code 8 if file path contains directories
                if (str_contains(value, "/")) {
                    return 11;
                }
                settings->mdf_path = value;
            }
        }
        val_iter++;
    }
    return 0;
}

//TODO: MIX VALGRIND ERRORS

//sets action data based on provided string
int set_action_data(char *command, struct sim_action *action) {
    int cmd_length, i, op_flag, op_iter, value;
    char current_char, op_str[20];

    cmd_length = str_len(command);
    i = 0;
    value = 0;
    op_iter = 0;
    op_flag = FALSE;
    current_char = command[i];

    if (!str_contains(command, "(") || !str_contains(command, ")")
    || !str_contains(command, ";")) {
        return 18;
    }

    while (i < cmd_length) {
        if (!(current_char == ' ') 
        && char_is_upper(current_char)) {
            action->command_letter = command[i];
        } else if (current_char == '(') {
            op_flag = TRUE;
        } else if (current_char == ')') {
            op_str[op_iter] = '\0';
            if ((!str_cmp(op_str, "start") && !str_cmp(op_str, "end")
            && (action->command_letter == 'S' || action->command_letter == 'A'))) {
                return 12;
            } else if (!str_cmp(op_str, "run") && action->command_letter == 'P') {
                return 13;
            } else if (!str_cmp(op_str, "hard drive") && !str_cmp(op_str, "keyboard")
            && action->command_letter == 'I') {
                return 14;
            } else if (!str_cmp(op_str, "hard drive") && !str_cmp(op_str, "printer")
            && !str_cmp(op_str, "monitor") && action->command_letter == 'O') {
                return 15;
            } else if (!str_cmp(op_str, "allocate") && !str_cmp(op_str, "access") 
            && action->command_letter == 'M') {
                return 16;
            }
            str_copy(op_str, action->operation_string);
            op_flag = FALSE;
        } else if (op_flag) {
            op_str[op_iter] = current_char;
            op_iter++;
        } else if (!op_flag 
        && !(current_char == ';') 
        && char_is_num(current_char)) {
            value *= 10;
            value += (current_char - 48);
        }
        i++;
        current_char = command[i];
    }
    if (value < 0) {
        return 17;
    }
    action->assoc_val = value;
    return 0;
}

//reads in all information from the meta data file
int read_meta_data_file(FILE *mdf, struct sim_action **first_action) {
    char line[100];
    int line_length, i, cmd_iter, set_data_res;
    cmd_iter = 0;
    char command[40];
    struct sim_action *current = (struct sim_action*) malloc(sizeof(struct sim_action));

    current = *first_action;

    while (fgets(line, 100, mdf)) {
        if (!str_contains(line, "Start Program Meta-Data") && !str_contains(line, "End Program Meta-Data")) {
            line_length = str_len(line);
            for (i = 0; i < line_length; i++) {
                command[cmd_iter] = line[i];
                cmd_iter++;
                if (line[i] == ';') {
                    struct sim_action *next = (struct sim_action*) malloc(sizeof(struct sim_action*));
                    command[cmd_iter] = '\0';
                    cmd_iter = 0;
                    set_data_res = set_action_data(command, current);
                    if (set_data_res > 0) {
                        return set_data_res;
                    }
                    current->next = next;
                    current = next;
                }
            }
        }
    }
    return 0;
}

#endif