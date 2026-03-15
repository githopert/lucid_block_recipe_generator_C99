#include "table.h"


/* function definitions ----------------------------------------------------- */
void
table_load_from_file(const char* csv_path, struct table* t)
{
	FILE* file;
	size_t size, rows, count;
	char header[INIT_SIZE], line[INIT_SIZE];
	char* token;

	file = fopen(csv_path, "r");

	if (file == NULL)
	{
		printf("Could not open the file: %s\n", csv_path);
		exit(EXIT_FAILURE);
	}

	count = 0;
	size = INIT_SIZE;
	t->rows = 0;
	t->data = (float**)malloc(size * sizeof(float*));

	for (size_t i = 0; i < size; ++i)
	{
		t->data[i] = (float*)calloc(ESSENCE_LENGTH, sizeof(float));
	}

	fgets(header, INIT_SIZE, file);

	while (fgets(line, INIT_SIZE, file) != NULL)
	{
		if (t->rows == size)
		{
			size *= 2;
			t->data = (float**)realloc(t->data, size * sizeof(float*));

			for (size_t i = t->rows; i < size; ++i)
			{
				t->data[i] = (float*)calloc(ESSENCE_LENGTH, sizeof(float));
			}
		}

		count = 0;
		token = strtok(line, ",");

		while (token != NULL)
		{
			sscanf(token, "%f", &t->data[t->rows][count]);
			token = strtok(NULL, ",");
			++count;
		}

		++t->rows;
	}

	fclose(file);

	if (t->rows < size)
	{
		for (size_t i = t->rows; i < size; ++i)
		{
			free(t->data[i]);
		}

		t->data = (float**)realloc(t->data, t->rows * sizeof(float*));
	}

	return;
}

void
table_free(struct table* t)
{
	for (size_t i = 0; i < t->rows; ++i)
	{
		free(t->data[i]);
	}

	free(t->data);

	return;
}

void
table_print(const struct table* t)
{
	for (size_t i = 0; i < t->rows; ++i)
	{
		for (size_t j = 0; j < ESSENCE_LENGTH; ++j)
		{
			printf("%.2f ", t->data[i][j]);
		}

		printf("\n");
	}

	return;
}

void
table_load_names_from_file(const char* csv_path, char names[][NAME_MAX_LEN])
{
	FILE* file;
	size_t count;
	char header[NAME_MAX_LEN], line[NAME_MAX_LEN];


	file = fopen(csv_path, "r");

	if (file == NULL)
	{
		printf("Could not open the file: %s\n", csv_path);
		exit(EXIT_FAILURE);
	}

	count = 0;
	fgets(header, NAME_MAX_LEN, file);

	while (fgets(line, NAME_MAX_LEN, file) != NULL)
	{
		line[strcspn(line, "\r\n")] = '\0';
		strcpy(names[count++], line);
	}

	fclose(file);

	return;
}