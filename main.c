#include <stdio.h>
#include <stdlib.h>

#include "table.h"
#include "fuser.h"


int main(int argc, char** argv)
{
	struct table t;
	size_t fuse_result = 0;
	size_t items[2] = {0, 0};
	size_t counts[2] = {1, 1};
	size_t recipe_num, stack_size_1, stack_size_2;
	struct recipe* recipes;
	char item_names[441][NAME_MAX_LEN];


	table_load_from_file("essences.csv", &t);
	table_load_names_from_file("names.csv", item_names);

	for (size_t i = 0; i < t.rows; ++i)
	{
		fprintf(stderr, "%lld/%lld\r", i + 1, 441);

		items[0] = i;
		counts[0] = 0;
		counts[1] = 0;
		stack_size_1 = t.data[i][STACK_SIZE_IDX];

		if (stack_size_1 > 3)
		{
			stack_size_1 = 3;
		}

		// fprintf(stderr, "%lld\n", stack_size_1);

		for (size_t n1 = 1; n1 <= stack_size_1; ++n1)
		{
			for (size_t j = i; j < t.rows; ++j)
			{
				items[1] = j;
				stack_size_2 = t.data[j][STACK_SIZE_IDX];

				if (stack_size_2 > 3)
				{
					stack_size_2 = 3;
				}

				for (size_t n2 = 1; n2 <= stack_size_2; ++n2)
				{
					counts[0] = n1;
					counts[1] = n2;

					fuse_result = get_fusion_result(2, items, counts, &t);

					printf(
						"%lld %s + %lld %s = %s\n",
						n1,
						item_names[items[0]],
						n2,
						item_names[items[1]],
						item_names[fuse_result]
					);
				}
			}
		}
	}

	table_free(&t);

	return EXIT_SUCCESS;
}