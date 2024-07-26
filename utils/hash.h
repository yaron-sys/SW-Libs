#ifndef HASH_H_		
#define HASH_H_		
		
#include <stddef.h> /* size_t */		
		
typedef struct hash hash_t;		
		
		
hash_t *HashCreate(size_t hash_size,		
				   size_t (*hash_function)(const void *data),		
				   int (*is_match)(const void *data1, const void *data2, void *params));		
		
void HashDestroy(hash_t *hash_table);		
		
int HashInsert(hash_t *hash_table, void *to_insert);		
		
void *HashRemove(hash_t *hash_table, const void *to_remove, void *params);		
		
void *HashFind(const hash_t *hash_table, const void *to_find, void *params);		
		
int HashForEach(const hash_t *hash_table, int (*do_func)(void *data, void *params), void *params);		
		
int HashIsEmpty(const hash_t *hash_table);		
		
size_t HashSize(const hash_t *hash_table);		


#endif /* HASH_H_ */
