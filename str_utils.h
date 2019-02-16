#include "booleans.h"
#include <math.h>
#ifndef STR_UTILS
#define STR_UTILS

//string utility functions I have created

//gets string length
int str_len(const char *string) {
    char curr = string[0];
    int len = 0;
    while (curr != '\0') {
        len += 1;
        curr = string[len];
    }

    return len;
}

//copies from one string into another
void str_copy(char *src, char *dest) {
    int len = str_len(src);
    int i;
    if (src && dest) {
        for (i = 0; i < len; i++) {
            dest[i] = src[i];
        }
        dest[i] = '\0';
    }
}

//reads 3 characters after a period to determine a file extension
const char* get_file_ext(const char *string, char *ext) {
    int iter = 4;
    ext[4] = '\0';

    for (int i = str_len(string); i > 0; i--) {
        ext[iter] = string[i];
        iter--;
    }

    return ext;
}

//compares two strings to each other
int str_cmp(const char *src, char *target) {
    int iter = 0;

    if (str_len(src) != str_len(target)) {
        return FALSE;
    }

    while (src[iter]) {
        if (src[iter] != target[iter]) {
            return FALSE;
        }
        iter++;
    }

    return TRUE;
}

//checks to see if string contains another string
int str_contains(char *src, char *target) {
    int iter = 0;
    int target_iter = 0;
    char curr = src[0];
    char target_curr = target[0];

    while (curr && target_curr) {
        curr = src[iter];
        if (curr == target_curr) {
            target_iter++;
            target_curr = target[target_iter];
            iter++;
        } else {
            if (target_iter == 0) {
                iter++;
            }
            target_iter = 0;
            target_curr = target[0];
        }
    }

    //if target_curr == end of string, this will equate to true
    if (!target_curr) {
        return TRUE;
    }

    return FALSE;
}

//returns position of a substring in a string
int sub_str_pos(char *src, char *target) {
    int iter = 0;
    int target_iter = 0;
    char curr = src[0];
    char target_curr = target[0];

    while (curr && target_curr) {
        curr = src[iter];
        
        if (curr == target_curr) {
            target_iter++;
            target_curr = target[target_iter];
            iter++;
        } else {
            if (target_iter == 0) {
                iter++;
            }
            target_iter = 0;
            target_curr = target[0];
        }
    }

    if (target_curr) {
        return -1;
    }

    return iter;
}

//reverses a string in place
void rev_str(char *src) {
    char tmp;
    //ignore last character because it is the null stirng termiantor
    int len = str_len(src) - 1;
    int half_len = len / 2;
    if (len % 2 != 0) {
        half_len++;
    }
    for (int i = 0; i < half_len; i++) {
        tmp = src[len - i];
        src[len - i] = src[i];
        src[i] = tmp;
    }
}

//creates a substring from start to end indices and puts it in the substr parameter
void sub_str(char *src, int start, int end, char *substr) {
    int iter = 0;
    for (int i = start; i < end; i++) {
        substr[iter] = src[i];
        iter++;
    }
    substr[iter] = '\0';
}

//removes newline characters
void remove_newline(char *src) {
    int pos = sub_str_pos(src, "\n");
    int i;
    char *tmp = (char*) malloc(sizeof(char) * 100);
    pos--;
    while (pos > 0) {
        for (i = 0; i < pos; i++) {
            tmp[i] = src[i];
        }
        tmp[i] = '\0';
        str_copy(tmp, src);
        pos = sub_str_pos(src, "\n");
    }
    free(tmp);
}

//removes whitespace from string start and end
void remove_whitespace(char *src) {
    int len = str_len(src);
    int iter = 0;
    int i;
    char curr;
    char *tmp = (char*) malloc(sizeof(char) * 100);
    for (i = 0; i < len; i++) {
        curr = src[i];
        if (curr > 32 && curr < 127) {
            tmp[iter] = curr;
            iter++;
        }
    }
    tmp[i] = '\0';
    str_copy(tmp, src);
    free(tmp);
}

//removes both whitespace and newline chars from string
void remove_non_chars(char *src) {
    remove_whitespace(src);
    remove_newline(src);
}

//converts a string into a float value
float a2f(char *src) {
    int iter = 0;
    char curr_char = src[0];
    char target[1] = {'.'};
    int dec_pos = sub_str_pos(src, target);
    int places = dec_pos - 2;
    float num = 0.0f;

    while (curr_char) {
        if (curr_char - 48 >= 0 && curr_char - 48 < 10) {
            num += (curr_char - 48) * pow(10, places);
            places--;
        }
        iter++;
        curr_char = src[iter];
        
    }
    return num;
}

//converts a string into an int value
int a2i(char *src) {
    int len = str_len(src);
    int num = 0;
    int curr_num;
    for (int i = 0; i < len; i++) {
        num *= 10;
        curr_num = src[i] - 48;
        if (curr_num > 0 && curr_num < 48) {
            num += curr_num;
        }
    }
    return num;
}

//returns TRUE if a vaild cpu scheduler has been chosen
int check_cpu_sched(char *src) {
    if (str_cmp(src, "FCFS-N")
    || str_cmp(src, "SJF-N")
    || str_cmp(src, "SRTF-P")
    || str_cmp(src, "FCFS-P") 
    || str_cmp(src, "RR-P")
    ) {
        return TRUE;
    }
    return FALSE;
}

//returns TRUE if a valid log to option has been chosen
int check_log_to(char *src) {
    if (str_cmp(src, "Monitor") 
    || str_cmp(src, "File") 
    || str_cmp(src, "Both")
    ) {
        return TRUE;
    }
    return FALSE;
}

//clears all values in a string by replacing everything up to cap with the null character
void str_clear(char *src, int cap) {
    for (int i = 0; i < cap; i++) {
        src[i] = '\0';
    }
}

//removes all non-numbers from string
void str_rm_non_numbers(char *src) {
    int i, len, num;
    i = 0;
    len = str_len(src);
    for (i = 0; i < len; i++) {
        num = src[i] - 48;
        if (num > 9 || num < 0) {
            src[i] = src[i+1];
            printf("ERROR\n");
           
        }
    }
}

//returns true if character is a number
int char_is_num(char src) {
    if (src >= 48 && src <= 58) {
        return TRUE;
    }
    return FALSE;
}

//returns true if character is upper case
int char_is_upper(char src) {
    if (src > 64 && src < 91) {
        return TRUE;
    }
    return FALSE;
}

#endif