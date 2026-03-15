#ifndef _FUSER_H
#define _FUSER_H

#include <float.h>
#include <math.h>

#include "global.h"
#include "table.h"


/* function prototypes ------------------------------------------------------ */
size_t get_fusion_result(
	size_t item_num,
	const size_t* items,
	const size_t* counts,
	const struct table* t
); 

static float fuser_mood_distance(const float* l1, const float* l2);
static float fuser_tag_priority(const float* l1, const float* l2);
static float fuser_tag_similarity(const float* l1, const float* l2);
static float fuser_color_distance(const float* l1, const float* l2);

#endif  /* _FUSER_H */