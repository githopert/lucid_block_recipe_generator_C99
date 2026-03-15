#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* macros ------------------------------------------------------------------- */
#define ESSENCE_LENGTH 77
#define NUM_PROPERTIES 13
#define NUM_TAGS 28
#define COLOR_WEIGHT 0.25
#define SAMEY_PUNISHMENT 0.35
#define INIT_SIZE 2048

#define ENERGY_IDX 13
#define FUSEABLE_IDX 14
#define BIAS_IDX 16
#define STACK_SIZE_IDX 17

#define COLOR_IDX_START 18
#define TAGS_IDX_START 21
#define OUTPUT_TAGS_IDX_START 49

#define NAME_MAX_LEN 32

#endif  /* _GLOBAL_H */