/*
 * Files: test_harness.c
 * ---------------------
 * Reads and interprets text-based script files containing a sequence of
 * allocator requests. Runs the allocator on the script, validating for
 * for correctness.
 *
 * jzelenski, updated Sun Nov 26 12:32:10 PST 2017
 */

#include <error.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "allocator.h"
#include "segment.h"

// Alignment requirement
#define ALIGNMENT 8

// struct for a single allocator request
typedef struct {
    enum {ALLOC=1, FREE, REALLOC} op;	// type of request
    int id;		        // id for free() to use later
    size_t size;        // num bytes for alloc/realloc request
    int lineno;         // which line in file
} request_t;

// struct for facts about a single malloc'ed node
typedef struct {
    void *ptr;
    size_t size;
} block_t;

// struct for info for one script file
typedef struct {
    char name[128];		// short name of script
    request_t *ops;	    // array of requests read from script
    int num_ops;		// number of requests
    int num_ids;		// number of distinct block ids
    block_t *blocks;    // array of blocks returned by malloc when executing
    size_t peak, lifetime, segment_size;
} script_t;


static void parse_script(const char *filename, script_t *script);
static bool eval_correctness(script_t *script);
static bool verify_block(void *ptr, size_t size, script_t *script, int lineno);
static bool verify_payload(void *ptr, size_t size, int id, script_t *script, int lineno, char *op);
static void allocator_error(script_t *script, int lineno, char* format, ...);

int main(int argc, char *argv[])
{
    if (argc < 2) 
        error(1, 0, "Missing argument. Please supply one or more script files.");

    setvbuf(stdout, NULL, _IONBF, 0); // disable stdout buffering, all printfs display to terminal immediately

    for (int i = 1; i < argc; i++) {
        script_t script;
        parse_script(argv[i], &script);
        printf("\nEvaluating allocator on %s...", script.name);
        bool valid = eval_correctness(&script);
        if (valid) printf(" successfully serviced %d requests.\n", script.num_ops);
        free(script.ops);
        free(script.blocks);
    }
    return 0;
}


/* Function: read_line
 * --------------------
 * Reads one line from file and stores in buf. Skips lines that are all-white or beginning with
 * comment char #. Increments pass-by-ref counter of number of lines read/skipped. Removes
 * trailing newline. Returns true if did read valid line, false otherwise.
 */
static bool read_line(char buf[], size_t bufsz, FILE *fp, int *pnread)
{
    while (true) {
        if (fgets(buf, bufsz, fp) == NULL) return false;
        (*pnread)++;
        if (buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] ='\0'; // remove trailing newline
        char ch;
        if (sscanf(buf, " %c", &ch) == 1 && ch != '#') // scan first non-white char, check not #
            return true;
    }
}

/*
 * Fuction: parse_script
 * ---------------------
 * Parse a script file and store sequence of requests for later execution.
 */
static void parse_script(const char *path, script_t *script)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
        error(1, 0, "Could not open script file \"%s\".", path);

    const char *basename = strrchr(path, '/') ? strrchr(path, '/') + 1 : path;
    strncpy(script->name, basename, sizeof(script->name)-1); // copy basename (up to limit)
    script->name[sizeof(script->name)-1] = '\0';    // null terminate
    script->ops = NULL;
    script->num_ops = 0;
    int lineno = 0, nallocated = 0, maxid = 0;
    char buf[1024];

    for (int i = 0; read_line(buf, sizeof(buf), fp, &lineno) ; i++) {
        if (i == nallocated) {
            nallocated += 500;
            script->ops = realloc(script->ops, nallocated*sizeof(request_t));
            if (!script->ops)
                error(1, 0, "Libc heap exhausted. Cannot continue.");
        }
        script->ops[i].lineno = lineno;
        char request;
        script->ops[i].op = script->ops[i].size = 0;
        int nscanned = sscanf(buf, " %c %d %zu", &request, &script->ops[i].id, &script->ops[i].size);
        if (request == 'a' && nscanned == 3)
            script->ops[i].op = ALLOC;
        else if (request == 'r' && nscanned == 3)
            script->ops[i].op = REALLOC;
        else if (request == 'f' && nscanned == 2)
            script->ops[i].op = FREE;
        if (!script->ops[i].op || script->ops[i].id < 0 || script->ops[i].size > INT_MAX)
            error(1, 0, "Line %d of script file '%s' is malformed.", lineno, script->name);
        if (script->ops[i].id > maxid) maxid = script->ops[i].id;
        script->num_ops = i+1;
    }
    fclose(fp);

    script->num_ids = maxid + 1;
    script->blocks = calloc(script->num_ids, sizeof(block_t));
    if (!script->blocks)
        error(1, 0, "Libc heap exhausted. Cannot continue.");

    script->lifetime = script->peak = 0;   // count lifetime allocation sizes, compute peak in-use
    size_t cur = 0;
    for (int req = 0; req < script->num_ops; req++) {
        int id = script->ops[req].id;
        size_t requested_size = script->ops[req].size;
        size_t old_size = script->blocks[id].size;
        switch (script->ops[req].op) {
            case ALLOC:
                script->blocks[id].size = requested_size;
                script->lifetime += requested_size;
                cur += requested_size;
                break;
            case FREE:
                cur -= old_size;
                break;
            case REALLOC:
                script->lifetime += requested_size;
                cur += (requested_size - old_size);
                script->blocks[id].size = requested_size;
                break;
        }
        if (cur > script->peak) script->peak = cur;
    }
    script->segment_size = 2*script->peak;
}


/* Function: eval_correctness
 * --------------------------
 * Check the allocator for correctness on given script. Interprets the earlier parsed
 * script operation-by-operation and reports if it detects any "obvious"
 * errors (returning blocks outside the heap, unaligned, overlapping blocks, etc.)
 */
static bool eval_correctness(script_t *script)
{
    init_heap_segment(script->segment_size);
    if (!myinit(heap_segment_start(), heap_segment_size())) {
        allocator_error(script, 0, "myinit() returned false");
        return false;
    }
    if (!validate_heap()) { // check heap consistency after init
        allocator_error(script, 0, "validate_heap() returned false, called after myinit");
        return false;
    }
    memset(script->blocks, 0, script->num_ids*sizeof(script->blocks[0]));

    for (int req = 0; req < script->num_ops; req++) {
        int id = script->ops[req].id;
        size_t requested_size = script->ops[req].size;
        size_t old_size = script->blocks[id].size;
        void *p, *newp, *oldp = script->blocks[id].ptr;

        switch (script->ops[req].op) {

            case ALLOC:
                if ((p = mymalloc(requested_size)) == NULL && requested_size != 0) {
                    allocator_error(script, script->ops[req].lineno, "heap exhausted, malloc returned NULL");
                    return false;
                }
                // Test new block for correctness: must be properly aligned
                // and must not overlap any currently allocated block.
                if (!verify_block(p, requested_size, script, script->ops[req].lineno))
                    return false;

                // Fill new block with the low-order byte of new id
                // can be used later to verify data copied when realloc'ing
                memset(p, id & 0xFF, requested_size);
                script->blocks[id] = (block_t){.ptr = p, .size = requested_size};
                break;

            case REALLOC:
                if (!verify_payload(oldp, old_size, id, script, script->ops[req].lineno, "realloc-ing"))
                    return false;
                if ((newp = myrealloc(oldp, requested_size)) == NULL && requested_size != 0) {
                    allocator_error(script, script->ops[req].lineno, "heap exhausted, realloc returned NULL");
                    return false;
                }

                old_size = script->blocks[id].size;
                script->blocks[id].size = 0;
                if (!verify_block(newp, requested_size, script, script->ops[req].lineno))
                    return false;
                // Verify new block contains the data from the old block
                for (size_t j = 0; j < (old_size < requested_size ? old_size : requested_size); j++) {
                    if (*((unsigned char *)newp + j) != (id & 0xFF)) {
                        allocator_error(script, script->ops[req].lineno, "realloc did not preserve the data from old block");
                        return false;
                    }
                }
                // Fill new block with the low-order byte of new id
                memset(newp, id & 0xFF, requested_size);
                script->blocks[id] = (block_t){.ptr = newp, .size = requested_size};
                break;

            case FREE:
                old_size = script->blocks[id].size;
                p = script->blocks[id].ptr;
                // verify payload intact before free
                if (!verify_payload(p, old_size, id, script, script->ops[req].lineno, "freeing"))
                    return false;
                script->blocks[id] = (block_t){.ptr = NULL, .size = 0};
                myfree(p);
                break;
        }

        if (!validate_heap()) { // check heap consistency after each request
            allocator_error(script, script->ops[req].lineno, "validate_heap() returned false, called in-between requests");
            return false;   // stop at first sign of error
        }
    }

    // verify payload is still intact for any block still allocated
    for (int id = 0;  id < script->num_ids;  id++)
        if (!verify_payload(script->blocks[id].ptr, script->blocks[id].size, id, script, -1, "at exit"))
            return false;
    return true;
}


/* Function: verify_block
 * ----------------------
 * Does some simple checks on the block returned by allocator to try to
 * verify correctness.  If any problem shows up, reports an allocator error
 * with details and line from script file. The checks it performs are:
 *  -- verify block address is correctly aligned
 *  -- verify block address is within heap segment
 *  -- verify block address + size doesn't overlap any existing allocated block
 */
static bool verify_block(void *ptr, size_t size, script_t *script, int lineno)
{
    // address must be ALIGNMENT-byte aligned
    if (((uintptr_t)ptr) % ALIGNMENT != 0) {
        allocator_error(script, lineno, "New block (%p) not aligned to %d bytes",
                        ptr, ALIGNMENT);
        return false;
    }
    if (ptr == NULL && size == 0) return true;

    // block must lie within the extent of the heap
    void *end = (char *)ptr + size;
    void *heap_end = (char *)heap_segment_start() + heap_segment_size();
    if (ptr < heap_segment_start() || end > heap_end) {
        allocator_error(script, lineno, "New block (%p:%p) not within heap segment (%p:%p)",
                        ptr, end, heap_segment_start(), heap_end);
        return false;
    }
    // block must not overlap any other blocks
    for (int i = 0; i < script->num_ids; i++) {
        if (script->blocks[i].ptr == NULL || script->blocks[i].size == 0) continue;
        void *other_start = script->blocks[i].ptr;
        void *other_end = (char *)other_start + script->blocks[i].size;
        if ((ptr >= other_start && ptr < other_end) || (end > other_start && end < other_end) ||
            (ptr < other_start && end >= other_end)){
            allocator_error(script, lineno, "New block (%p:%p) overlaps existing block (%p:%p)",
                            ptr, end, other_start, other_end);
            return false;
        }
    }
    return true;
}

/* Function: verify_payload
 * ------------------------
 * When a block is allocated, the payload is filled with a simple repeating pattern
 * Later when realloc'ing or freeing that block, check the payload to verify those
 * contents are still intact, otherwise raise allocator error.
 */
static bool verify_payload(void *ptr, size_t size, int id, script_t *script, int lineno, char *op)
{
    for (size_t i = 0; i < size; i++) {
        if (*((unsigned char *)ptr + i) != (id & 0xFF)) {
            allocator_error(script, lineno, "invalid payload data detected when %s address %p", op, ptr);
            return false;
        }
    }
    return true;
}

// Report errors from invoking student's allocator functions (non-fatal)
static void allocator_error(script_t *script, int lineno, char* format, ...)
{
    va_list args;
    fprintf(stdout, "\nALLOCATOR FAILURE [%s, line %d]: ", script->name, lineno);
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fprintf(stdout,"\n");
}
