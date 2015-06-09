/*
@Title
	C-Autoreleasepool code. C-Autoreleasepool.c

@Other stuff
	Look in C-Autoreleasepool.h for the rest of the paper-stuff
*/


#include <stdio.h> //include stdio for output
#include <stdlib.h> //include stdlib for malloc function
#include <string.h> //include string for strup+...? functions
#include "C-Autoreleasepool.h" //include the header
#define CGC_DEBUG //define that we should debug. does anyone know a global DEF for this?
#undef CGC_DEBUG //comment out to enable the debug message stuff!

static unsigned int gc_allocate_memory_in_pool(size_t size, void *returned_obj, struct gc_s_autoreleasepool *pool); //the pool argument is unused yet, for the feature "pool-ception"


#undef malloc //undefine the malloc-hook to prevent recursive, forever-loops
void *__cdecl gc_malloc(size_t _Size) {
	void *return_type = malloc(_Size);
	if (current_autoreleasepool.state == OPENED && return_type != NULL && _Size != 0) {
		gc_allocate_memory_in_pool(_Size, return_type, NULL);
	}

	return return_type;
} //the function to hook malloc with

//TODO: Implement calloc, realloc...

#undef free
void __cdecl gc_free(void *_Memory) {
	struct gc_so_autoreleasepool *object;

	if (current_autoreleasepool.state == OPENED && current_autoreleasepool.first_object != NULL) {
		object = current_autoreleasepool.first_object;
		if (object->next_object != NULL) {
			while (object->self != _Memory) {
				object = object->next_object;
				if (object->next_object == NULL) break;
			}
		}
		if (object->self == _Memory) { //bad style. sorry for this, fix asap.
			object->already_freed = 1;
			object->self = NULL;

			current_autoreleasepool.allocated_objects--;
			current_autoreleasepool.pool_volume -= object->object_size;
			#ifdef CGC_DEBUG
			fprintf(stderr, "Cgc: Free was called! Free'd size:%i  Allocated objects in pool:%i  Pool volume:%i\n", object->object_size, current_autoreleasepool.allocated_objects, current_autoreleasepool.pool_volume);
			#endif
		} else {
			#ifdef CGC_DEBUG
			fprintf(stderr, "Cgc: Free was called for an object which is not in the autoreleasepool! This is no problem, but may look weird in your source code.\n");
			#endif
		}
	}

	free(_Memory);
}

#undef strdup
char *__cdecl gc_strdup(const char *_Src) {
	char *return_type = strdup(_Src);
	if (current_autoreleasepool.state == OPENED && return_type != NULL) {
		gc_allocate_memory_in_pool(strlen(_Src)+1, return_type, NULL);
	}

	return return_type;
}

//TODO: Implement some other heap-functions


void gc_start_autoreleasepool() {
	if (current_autoreleasepool.state != OPENED && current_autoreleasepool.state != EDITING) {
		current_autoreleasepool.pool_volume = 0;
		current_autoreleasepool.allocated_objects = 0;
		current_autoreleasepool.state = OPENED;
		current_autoreleasepool.first_object = NULL;
		#ifdef CGC_DEBUG
		fprintf(stderr, "Cgc: Built new autoreleasepool.\n");
		#endif
	} else {
		#ifdef CGC_DEBUG
		fprintf(stderr, "Cgc: Seems like an autoreleasepool is initialized already, TODO for the dev to allow more pools at once. For now, everything is just thrown in one pool.\n");
		#endif
	}
}

void gc_end_autoreleasepool() {
	struct gc_so_autoreleasepool *next_free;
	struct gc_so_autoreleasepool *free_o;

	if (current_autoreleasepool.state == OPENED && current_autoreleasepool.pool_volume != 0 && current_autoreleasepool.allocated_objects != 0) {
		current_autoreleasepool.state = EDITING;

		for (free_o = current_autoreleasepool.first_object; free_o != NULL; free_o = next_free) {
			next_free = free_o->next_object;
			if (free_o->already_freed == 0) {
				current_autoreleasepool.allocated_objects--;
				current_autoreleasepool.pool_volume -= free_o->object_size;
				free(free_o->self);
			}
			free(free_o);
		}

		current_autoreleasepool.state = CLOSED;
		#ifdef CGC_DEBUG
		fprintf(stderr, "Cgc: Drained autoreleasepool. Allocated objects in pool:%i  Pool volume:%i\n", current_autoreleasepool.allocated_objects, current_autoreleasepool.pool_volume);
		#endif
	}
}

static unsigned int gc_allocate_memory_in_pool(size_t size, void *returned_obj, struct gc_s_autoreleasepool *pool) {
	struct gc_so_autoreleasepool *object;
	struct gc_so_autoreleasepool *n_object;

	if (current_autoreleasepool.state == OPENED && returned_obj != NULL && size != 0) {
		current_autoreleasepool.state = EDITING;
		if (current_autoreleasepool.first_object == NULL) {
			object = (gc_so_autoreleasepool *)malloc(sizeof(gc_so_autoreleasepool));
			object->self = returned_obj;
			object->already_freed = 0;
			object->object_size = size;
			object->next_object = NULL;
			current_autoreleasepool.first_object = object;
		} else {
			object = current_autoreleasepool.first_object;
			while (object->next_object != NULL) {
				object = object->next_object;
			}
			n_object = (gc_so_autoreleasepool *)malloc(sizeof(gc_so_autoreleasepool));
			n_object->self = returned_obj;
			n_object->already_freed = 0;
			n_object->object_size = size;
			n_object->next_object = NULL;
			object->next_object = n_object;
		}
		current_autoreleasepool.allocated_objects++;
		current_autoreleasepool.pool_volume += size;
		#ifdef CGC_DEBUG
		fprintf(stderr, "Cgc: Pushed object with size: %i into autoreleasepool. Allocated objects in pool: %i  Pool volume: %i\n", size, current_autoreleasepool.allocated_objects, current_autoreleasepool.pool_volume);
		#endif
		current_autoreleasepool.state = OPENED;
		return 0;
	} else {
		#ifdef CGC_DEBUG
		fprintf(stderr, "Cgc: Some error occured while pushing object:%p of size:%i into pool:%p\n", returned_obj, size, pool);
		#endif
		return 1;
	}
}
