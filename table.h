#ifndef _TABLE_H
#define _TABLE_H

#include "global.h"


/* structs, enums, typedefs ------------------------------------------------- */
struct table {
	float** data;
	size_t rows;
};

/* function prototypes ------------------------------------------------------ */
void table_load_from_file(const char* csv_path, struct table* t);
void table_free(struct table* t);
void table_print(const struct table* t);

void table_load_names_from_file(const char* csv_path, char names[][NAME_MAX_LEN]);

#endif  /* _TABLE_H */