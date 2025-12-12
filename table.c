/*
File: Table.c
Author: Joseph Skapin
Course: ECE 2230 - MP6 Hash Tables
Date 12/3/2025

Description: Implements the core hash tabble operations used by the MP6 drivers.
Supports 3 hashing schemes: Linear probing, double hashing, separate chaining.

*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "table.h"
#include "hashes.h"

/*  The empty table is created.  The table must be dynamically allocated and
 *  have a total size of table_size.  The maximum number of (K, I) entries
 *  that can be stored in the table is table_size-1.  For open addressing, 
 *  the table is filled with a special empty key distinct from all other 
 *  nonempty keys (e.g., NULL).  
 *
 *  the probe_type must be one of {LINEAR, DOUBLE, CHAIN}
 *
 *  Do not "correct" the table_size or probe decrement if there is a chance
 *  that the combinaion of table size or probe decrement will not cover
 *  all entries in the table.  Instead we will experiment to determine under
 *  what conditions an incorrect choice of table size and probe decrement
 *  results in poor performance.
 *
 *  Malloc one table for either open addressing or separate 
 *  chaining.  But only allocate one of them depending on the probe_type
 */
table_t *table_construct(int table_size, int probe_type)
{
	table_t *T = malloc(sizeof(table_t));
	if(T == NULL)
	{
		fprintf(stderr, "table_construct: malloc for table_t failed\n");
		return NULL;
	}
	
	T->size = table_size;
	T->probeType = probe_type;
	T->keys = 0;
	T->probeCount = 0;
	T->open_addr = NULL;
	T->chains = NULL;
	
	if(probe_type == LINEAR || probe_type == DOUBLE)
	{
		T->open_addr = calloc(table_size, sizeof(table_entry));
		if(T->open_addr == NULL)
		{
			fprintf(stderr, "table_construct: calloc for open_addr failed \n");
			free(T);
			return NULL;
		}
		
		//Initalize all teble slots as empty
		for(int i = 0; i < table_size; i++)
		{
			T->open_addr[i].key = NULL;
			T->open_addr[i].data = NULL;
			T->open_addr[i].state = 0; //Empty
		}
		
	}else if(probe_type == CHAIN){
		T->chains = calloc(table_size,sizeof(chain_node *));
		if(T->chains == NULL)
		{
			fprintf(stderr, "table_construct: calloc for chains failed \n");
			free(T);
			return NULL;
		}
		
	} else{
		fprintf(stderr, "table_construct: invalid probe_tye %d\n", probe_type);
		free(T);
		return NULL;
	}
	
	return T;
}


/* Sequentially remove each table entry (K, I) and insert into a new
 * empty table with size new_table_size.  Free the memory for the old table.
 * Update the header, T, to point to the new table with its
 * new size.  The probe type should remain the same.
 *
 * Do not rehash the table during an insert or delete function call.  Instead
 * use drivers to verify under what conditions rehashing is required, and
 * call the rehash function in the driver to show how the performance
 * can be improved.
 */
void table_rehash(table_t * T, int new_table_size)
{
	/* 
     * 1. Construct a new table with same probeType and new_table_size
     * 2. For every existing (K, I) in T, insert into the new table
     * 3. Free the old table's storage (but NOT the K/I you moved)
     * 4. Copy the new table header contents back into *T, then free temp header
     */
     assert(T != NULL);
     
     table_t *newT = table_construct(new_table_size, T->probeType);
     if(newT == NULL)
     {
     	fprintf(stderr, "table_rehash: failed to construct new table \n");
     	return;
     }
     if(T->probeType == CHAIN)
     {
     	for(int i = 0; i < T->size; i++)
     	{
     		chain_node *curr = T->chains[i];
     		while(curr != NULL)
     		{
     			chain_node *next = curr->next;
     			
     			int code = table_insert(newT,curr->key,curr->data);
     			assert(code != -1);
     			free(curr);
     			curr = next;
     		}
     	}
     	free(T->chains);
     	T->chains = NULL;
     }
     else{ // open addressing
     	for(int i = 0; i < T->size; i++)
     	{
     		table_entry *e = &T->open_addr[i];
     		if(e->state == 1)
     		{
     			int code = table_insert(newT, e->key, e->data);
     			assert(code != -1);
     		}
     	}
     	free(T->open_addr);
     	T->open_addr = NULL;
     }	
     T->size = newT->size;
     T->keys = newT->keys;
     T->probeCount = 0;
     T->probeType = newT->probeType;
     
     T->open_addr = newT->open_addr;
     T->chains = newT->chains;
     
     newT->open_addr = NULL;
     newT->chains = NULL;
     
     free(newT);
}

/* Returns the number of entries in the table.  Store this value
 * in the table header because this function is called frequently
 */
int table_entries(table_t *T)
{
	return T->keys;


}

/* returns 1 if table is full and 0 if not full. 
 * For separate chaining the table is never full
 */
int table_full(table_t *T)
{
	if(T->probeType == CHAIN)
	{
		return 0; // chaining is never full
	}
	
	//open addressing stores at most size-1 entries
	return (T->keys >= T->size -1) ? 1:0;
}

/* Returns the number of table entries marked as deleted.  
 *
 * For separate chaining the number of delete keys is always zero.
 *
 * For lab6 experiments, this function is rarely called.  So it is
 * acceptable for this project to just scan the table and count the 
 * number entries deleted when the function is called.  
 */
int table_deletekeys(table_t *T)
{
	if(T->probeType == CHAIN)
	{
		return 0;
	}
	
	int count = 0;
	for(int i = 0; i < T->size; i++)
	{
		if(T->open_addr[i].state == 2)
		{
			count++;
		}
	}
	return count;

}
   
/* Insert a new table entry (K, I) into the table provided the table is not
 * already full.  
 * Return:
 *      0 if (K, I) is inserted, 
 *      1 if an older (K, I) is already in the table (update with the new I), or 
 *     -1 if the (K, I) pair cannot be inserted.
 *
 * Note that both K and I are pointers to memory blocks created by malloc().
 *
 * If the return result is 1, you must free the memory blocks that are no
 * longer needed.
 *
 * If the return result is -1, the caller will handle the memory blocks.  The
 * caller may need to examine the blocks to determine what failed. 
 */
int table_insert(table_t *T, hashkey_t K, data_t I)
{
    T->probeCount = 0;

    /* Separate chaining insert */
    if (T->probeType == CHAIN) {
        int index = hashes_table_pos(K, T->size);

        chain_node *curr = T->chains[index];

        // search for existing key
        while (curr != NULL) {
            T->probeCount++;              // count key comparisons
            if (strcmp(curr->key, K) == 0) {
                // key already exists --> replace data
                free(curr->data);
                curr->data = I;
                return 1;
            }
            curr = curr->next;
        }

        // Not found -> create new node at head
        chain_node *N = malloc(sizeof(chain_node));
        N->key  = K;
        N->data = I;
        N->next = T->chains[index];

        T->chains[index] = N;
        T->keys++;
        return 0;

    } else {
        /* Open addressing insert (linear or double hashing) */

        int size = T->size;
        int home = hashes_table_pos(K, size);
        int dec;

        if (T->probeType == LINEAR)
            dec = 1;                          // linear probing
        else
            dec = hashes_probe_dec(K, size);  // double hashing decrement

        int idx = home;
        int first_deleted = -1;               // remember first deleted slot
        int full = table_full(T);             // table at capacity (size-1 entries)?

        while (1) {
            table_entry *e = &T->open_addr[idx];
            T->probeCount++;

            if (e->state == 0) {              // EMPTY slot
                if (full && first_deleted == -1) {
                    /* Table is “full” and this is the one EMPTY sentinel slot.
                     * We never saw the key and have no DELETED slot to reuse,
                     * so this insert must fail.
                     */
                return -1;
                }

                // Either table not full, or we have a DELETED slot to reuse
                int insert_idx = (first_deleted != -1) ? first_deleted : idx;
                e = &T->open_addr[insert_idx];

                e->key  = K;
                e->data = I;
                e->state = 1;                 // OCCUPIED
                T->keys++;
                return 0;
            }

            if (e->state == 1) {              // OCCUPIED
                if (strcmp(e->key, K) == 0) {
                    // duplicate key → replace data
                    free(K);                  // new key not needed
                    free(e->data);
                    e->data = I;
                    return 1;
                }
            } else if (e->state == 2) {       // DELETED
                if (first_deleted == -1)
                    first_deleted = idx;
            }

            // Advance to next probe location
            idx -= dec;
            if (idx < 0)
                idx += size;                  // wrap around

            // If we circled the table
            if (idx == home) {
                if (first_deleted != -1) {
                    table_entry *e_del = &T->open_addr[first_deleted];
                    e_del->key   = K;
                    e_del->data  = I;
                    e_del->state = 1;
                    T->keys++;
                    return 0;
                }
                return -1;
            }
        }
    }
}
/* Delete the table entry (K, I) from the table.  Free the key in the table.
 * Return:
 *     pointer to I, or
 *     null if (K, I) is not found in the table.  
 *
 * Be sure to free(K)
 *
 * See the note on page 490 in Standish's book about marking table entries for
 * deletions when using open addressing.  Review the -d driver for examples
 * of troublesome cases.
 */
data_t table_delete(table_t *T, hashkey_t K)
{
	T->probeCount = 0;

    if (T->probeType == CHAIN) {
        /*chaining delete:
         *  - index = hashes_table_pos(K, T->size)
         *  - walk the list, counting key comparisons
         *  - if found: unlink node, free(node->key), save data pointer,
         *    free node, decrement T->keys, return data
         *  - if not found: return NULL
         */
         
         	int index = hashes_table_pos(K,T->size);
         	chain_node *curr = T->chains[index];
         	chain_node *prev = NULL;
         	
         	while(curr != NULL)
         	{
         		T->probeCount++;
         		if(strcmp(curr->key,K)==0)
         		{
         		//Found the node, unlink it
         			if(prev == NULL)
         			{
         				T->chains[index] = curr->next;
         			}
         			else
         			{
         				prev->next = curr->next;
         			}
         			data_t ret = curr->data;
         			free(curr->key);
         			free(curr);
         			T->keys--;
         			return ret;
         		}
         		prev = curr;
         		curr = curr->next;
         	}
         	return NULL; //not found
         }
         else {
        /*open addressing delete:
         *  - home = hashes_table_pos(K, T->size)
         *  - probe just like search/insert
         *  - for each probe:
         *      - increment T->probeCount
         *      - if slot EMPTY: key not found → return NULL
         *      - if slot OCCUPIED and key matches:
         *          - free(slot.key)
         *          - save slot.data in a local var
         *          - mark slot.state = DELETED, set key/data to NULL,
         *          - decrement T->keys
         *          - return saved data
         *      - if slot DELETED: keep probing
         */
         int size = T->size;
         int home = hashes_table_pos(K,size);
         int dec;
         
         if(T->probeType == LINEAR)
         {
         	dec = 1;
         }
         else
         {
         	dec = hashes_probe_dec(K,size); //double
         }
         int idx = home;
         
         while(1)
         {
         	table_entry *e = &T->open_addr[idx];
         	T->probeCount++;
         	
         	if(e->state == 0)
         	{
         		return NULL; //Hit an empty slot ->Key not in table
         	}
         	if(e->state == 1 && strcmp(e->key,K)==0)
         	{
         		data_t ret = e->data;
         		free(e->key);
         		e->key = NULL;
         		e->data = NULL;
         		e->state = 2;
         		
         		T->keys--;
         		return ret;
         	}
         	
         	idx -= dec;
         	if(idx < 0)
         	{
         		idx += size;
         	}
         	if(idx == home)
         	{
         		return NULL; //full cycle, nothing found
         	}
         }
    }
}

/* Given a key, K, retrieve the pointer to the information, I, from the table,
 * but do not remove (K, I) from the table.  Return NULL if the key is not
 * found.
 */
data_t table_search(table_t *T, hashkey_t K)
{
	T->probeCount = 0;

    if (T->probeType == CHAIN) {
        /*  chaining search:
         *  - index = hashes_table_pos(K, T->size)
         *  - walk list at T->chains[index], compare keys
         *  - increment T->probeCount for each key comparison
         *  - if found: return data pointer
         *  - if not: return NULL
         */
        
		int index = hashes_table_pos(K,T->size);
		chain_node *curr = T->chains[index];
		
		while(curr != NULL)
		{
			T->probeCount++; //count key comparison
			if(strcmp(curr->key,K)==0)
			{
				return curr->data; // found
			}
			curr = curr->next;
		}
		return NULL; //walked entire list, not found
	}
	else {
        /*  open addressing search:
         *  - home = hashes_table_pos(K, T->size)
         *  - probe sequence same as insert/delete
         *  - for each probe:
         *      - increment T->probeCount
         *      - if slot EMPTY: stop → not found → return NULL
         *      - if slot OCCUPIED and key matches: return data
         *      - if slot DELETED: continue probing
         */
         
         int size = T->size;
         int home = hashes_table_pos(K,size);
         int dec;
         
         if(T->probeType == LINEAR)
         {
         	dec = 1;
         }
         else{
         	dec = hashes_probe_dec(K,size); //double hashing
         }
         
         int idx = home;
         
         while(1)
         {
         	table_entry *e = &T->open_addr[idx];
         	T->probeCount++;
         	
         	if(e->state == 0)
         	{
         		return NULL; //Empty slot means key is not in the table
         	}
         	if(e->state == 1 && strcmp(e->key,K)==0)
         	{
         		//Occupied and keys match
         		return e->data;
         	}
         	
         	idx -= dec;
         	if(idx < 0)
         	{
         		idx += size;
         	}
         	if(idx == home)
         	{
         		 return NULL; //came full circle, key was not in the table
         	}
         }
    }
}

/* Free all information in the table, the table itself, and any additional
 * headers or other supporting data structures.  
 */
void table_destruct(table_t *T)
{
	if(T==NULL) return;
	
	if(T->probeType == CHAIN)
	{
		for (int i = 0; i < T->size; i++)
		{
			chain_node *curr = T->chains[i];
			while(curr != NULL)
			{
				chain_node *next = curr->next;
				free(curr->key);
				free(curr->data);
				free(curr);
				curr = next;
			}
		}
		free(T->chains);
	}
	else{
		for(int i = 0; i < T->size; i++)
		{
			table_entry *e = &T->open_addr[i];
			if(e->state == 1)
			{
				free(e->key);
				free(e->data);
			}
		}
		free(T->open_addr);
	}	
	free(T);
}

/* The number of probes for the most recent call to table_search,
 * table_insert, or table_delete 
 *
 * Due to a quirk in the way Standish has defined comparisons in his 
 * performance formulas, we have two different rules for counting probes.  
 * For open addressing the function must return the number of probes into the 
 * hash table (that is, the number of comparisons includes checking if a 
 * position contains an empty key (or is marked as deleted) even if the empty 
 * is represented by a NULL pointer).  For separate chaining, count the number
 * of key comparisons required to insert, search or delete (and don’t count 
 * tests for a NULL pointer).
 */
int table_stats(table_t *T)
{
	return T->probeCount;
}

/* This function is for testing purposes only.  Given an index position into
 * the hash table return the value of the key if data is stored in this 
 * index position.  If the index position does not contain data, then the
 * return value must be zero.  
 *
 * For separate chaining, return the key found in list_position at this index 
 * position.  If the list_position is 0, the function returns the first key in
 * the linked list; if 1 the second key, etc.  If the list_position is greater
 * than the number of items in the list, then return NULL.  Notice 
 * list_position is not used for open addressing
 *
 * Make the first lines of this function 
 *       assert(0 <= index && index < table_size); 
 *       assert(0 <= list_position); 
 */
hashkey_t table_peek(table_t *T, int index, int list_position)
{
	assert(0 <= index && index < T->size);
	assert(0 <= list_position);
	if(T->probeType == CHAIN)
	{
		chain_node *curr = T->chains[index];
		int pos = 0;
		while(curr != NULL && pos < list_position)
		{
			curr = curr->next;
			pos++;
			
		}
		if(curr == NULL)
		{
			return NULL;
		}
		return curr->key;
	}
	else{
		//Open addressing
		if(T->open_addr[index].state == 1)
		{
			return T->open_addr[index].key;
			
		}
		else
		{
			return NULL;
		}
	}
}

/* Print the table position and keys in a easily readable and compact format.
 * Distinguish between empty and deleted locations.
 * Only useful when the table is small.
 */
void table_debug_print(table_t *T)
{
	printf("Table size = %d, keys = %d, type = %d\n",
           T->size, T->keys, T->probeType);

    if (T->probeType == CHAIN) {
        for (int i = 0; i < T->size; i++) {
            printf("[%d]:", i);
            chain_node *curr = T->chains[i];
            while (curr != NULL) {
                printf(" (%s)", curr->key);
                curr = curr->next;
            }
            printf("\n");
        }
    } else {
        for (int i = 0; i < T->size; i++) {
            table_entry *e = &T->open_addr[i];
            printf("[%d]: ", i);
            if (e->state == 0) {
                printf("EMPTY");
            } else if (e->state == 2) {
                printf("DELETED");
            } else {
                printf("KEY=%s", e->key);
            }
            printf("\n");
        }
    }

}
