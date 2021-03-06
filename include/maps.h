#pragma once

#include "list.h"

extern char *page_zeros;
extern char *page_0xff;
extern char *page_rand;
extern char *page_allocs;

#define MAP_GLOBAL 1
#define MAP_LOCAL 2

struct map {
	struct list_head list;
	void *ptr;
	char *name;
	unsigned long size;
	int prot;
	unsigned char type;
};

void setup_global_mappings(void);
void destroy_global_mappings(void);

void delete_mapping(int childno, struct map *map);

struct map * get_map(void);

struct map * common_set_mmap_ptr_len(int childno);

void dirty_mapping(struct map *map);
