/*
@Title
	C-Autoreleasepool header. C-Autoreleasepool.h

@Author
	H6nry <henry.anonym@gmail.com>

@Version
	Version 0.9

@Description
	An attempt to implement NSAutoreleasepool (or @autoreleasepool{...}) from Objective-C and Cocoa purely in C into the C language.
	I thought this would be useful for someone and implemented this within a weekend or so. I did not find any compareable project
	yet but if you have one which is better, tell me and we can merge together! Email me if you find this useful and think i
	continue to work on this.

@License
	Do what you want with it, just email me if you are going to publish parts of this.
	I also would like to be mentioned in the source code, if published in source code form,
	and credited, if published only in binary form.
	Due to the informality of this license, you are allowed to ignore this if you really want to.

@TODO
	-Implement autoreleasepools within other ones (pool-ception :P)
	-Implement calloc, realloc...(?) and other heap-allocating functions
	-Improve code, especially the gc_free function
	-Improve performance (it gets a bit slow if too much is there)
	-Write a proper test-project as a demo that this stuff really works (does anyone know a REALLY bad C-Dev (except me :P)??)
*/


#pragma once

#ifndef CGC_H
#define CGC_H


#define gc_autoreleasepool(x)	gc_start_autoreleasepool(); x  gc_end_autoreleasepool();

typedef enum gc_pool_state {UNINITIALIZED, OPENED, CLOSED, EDITING, DIFFERENT};

typedef struct gc_s_autoreleasepool {
	size_t pool_volume; //total bytes allocated in this pool
	unsigned int allocated_objects; //object counter of this pool
	enum gc_pool_state state; //maybe for threading...?
	//struct gc_o_autoreleasepool *inheriting_pool; //not used yet. here for the feature "pool-ception"
	struct gc_so_autoreleasepool *first_object; //first object of the pool
} gc_s_autoreleasepool;

typedef struct gc_so_autoreleasepool {
	void *self; //pointer to the allocated object
	unsigned int already_freed:1; //if this was manually freed
	size_t object_size; //the allocated size
	struct gc_so_autoreleasepool *next_object; //next object in the pool
} gc_so_autoreleasepool;

void gc_start_autoreleasepool();
void gc_end_autoreleasepool();
struct gc_s_autoreleasepool current_autoreleasepool;


#define malloc(x) (gc_malloc(x)) //define a preprocessor-directive for the hooked malloc
void *__cdecl gc_malloc(size_t _Size); //define a function to hook malloc with

#define free(x) (gc_free(x))
void __cdecl gc_free(void *_Memory);

#ifdef _INC_STRING //if the given code works with <string.h>. Do we really need this? Does it work for other platforms as well?
#define strdup(x) (gc_strdup(x))
char *__cdecl gc_strdup(const char *_Src);
#endif


#endif