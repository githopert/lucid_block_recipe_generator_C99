#include "fuser.h"


size_t
get_fusion_result(size_t item_num, const size_t* items, const size_t* counts, const struct table* t)
{
	size_t used_ids[6];
	size_t used_id_idx, current_item, closest_item;

	float total_energy, ratio, closest_distance;
	float mood_sum[NUM_PROPERTIES], tag_sum[NUM_TAGS], output_tag_sum[NUM_TAGS], avg_color[3];


	/* initializing --------------------------------------------------------- */
	used_id_idx = 0;
	total_energy = 0.0;
	ratio = 0.0;
	closest_distance = FLT_MAX;
	closest_item = 0;

	for (size_t i = 0; i < 6; ++i)
	{
		used_ids[i] = 0.0;
	}

	for (size_t i = 0; i < 3; ++i)
	{
		avg_color[i] = 0.0;
	}

	for (size_t i = 0; i < NUM_PROPERTIES; ++i)
	{
		mood_sum[i] = 0.0;
	}

	for (size_t i = 0; i < NUM_TAGS; ++i)
	{
		tag_sum[i] = 0.0;
		output_tag_sum[i] = 0.0;
	}

	/* processing ----------------------------------------------------------- */
	/* sum all items -------------------------------------------------------- */
	for (size_t i = 0; i < item_num; ++i)
	{
		current_item = items[i];
		total_energy += t->data[current_item][ENERGY_IDX];
	}

	if (total_energy <= 0)
	{
		total_energy = 0.01;
	}

	for (size_t i = 0; i < item_num; ++i)
	{
		current_item = items[i];
		used_ids[used_id_idx++] = items[i];

		ratio = t->data[current_item][ENERGY_IDX] * counts[i] / total_energy;

		for (size_t j = 0; j < NUM_PROPERTIES; ++j)
		{
			mood_sum[j] += ratio * t->data[current_item][j];
		}

		for (size_t j = 0; j < NUM_TAGS; ++j)
		{
			if (t->data[current_item][TAGS_IDX_START + j] == 1)
			{
				tag_sum[j] = 1;
			}

			if (t->data[current_item][OUTPUT_TAGS_IDX_START + j] == 1)
			{
				output_tag_sum[j] = 1;
			}
		}

		for (size_t j = 0; j < 3; ++j)
		{
			avg_color[j] += ratio * t->data[current_item][COLOR_IDX_START + j];
		}

		// Debug
		// fprintf(stderr, "CURRENT ITEM: %lld\n", current_item);
	}

	/* search for closest item ---------------------------------------------- */
	float mood_dist, color_dist, tag_sameness, tag_output_match;

	for (size_t i = 0; i < t->rows; ++i)
	{
		if (t->data[i][FUSEABLE_IDX] == 0)
		{
			// fprintf(stderr, "FOUND NOT FUSEABLE ITEM: %lld\n", i);
			continue;
		}

		tag_sameness = fuser_tag_similarity(&t->data[i][TAGS_IDX_START], tag_sum);
		tag_output_match = fuser_tag_priority(&t->data[i][OUTPUT_TAGS_IDX_START], output_tag_sum);
		mood_dist = fuser_mood_distance(&t->data[i][0], mood_sum);
		color_dist = fuser_color_distance(&t->data[i][COLOR_IDX_START], avg_color);

		mood_dist -= tag_sameness + tag_output_match;
		mood_dist += color_dist	* COLOR_WEIGHT;
		mood_dist -= 2.5 * t->data[i][BIAS_IDX];

		for (size_t j = 0; j < used_id_idx; ++j)
		{
			if (used_ids[j] == i)
			{
				mood_dist += SAMEY_PUNISHMENT;
				break;
			}
		}

		if (mood_dist < closest_distance)
		{
			closest_distance = mood_dist;
			closest_item = i;
		}
	}

	return closest_item;
} 

static float
fuser_mood_distance(const float* l1, const float* l2)
{
	float distance_squared;


	distance_squared = 0.0f;

	for (size_t i = 0; i < NUM_PROPERTIES; ++i)
	{
		distance_squared += (l1[i] - l2[i]) * (l1[i] - l2[i]);
	}

	return sqrt(distance_squared);
}

static float
fuser_tag_priority(const float* l1, const float* l2)
{
	float similarity;


	similarity = 0.0f;

	for (size_t i = 0; i < NUM_TAGS; ++i)
	{
		if (l1[i] > 0 && l2[i] > 0)
		{
			similarity += 1000.0f;
		}
	}

	return similarity;
}

static float
fuser_tag_similarity(const float* l1, const float* l2)
{
	float similarity;


	similarity = 0.0f;

	for (size_t i = 0; i < NUM_TAGS; ++i)
	{
		if (l1[i] > 0 && l2[i] > 0)
		{
			similarity += 1.0;
		}
	}

	return similarity;
}

static float
fuser_color_distance(const float* l1, const float* l2)
{
	float dr, dg, db;


	dr = l1[0] - l2[0];
	dg = l1[1] - l2[1];
	db = l1[2] - l2[2];

	return sqrt(dr * dr + dg * dg + db * db);
}