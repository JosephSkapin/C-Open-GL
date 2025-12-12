/* table.h 
 * Author: Joseph Skapin
 * Lab6: Hash Tables 
 * Version: 2
 * ECE 2230, Fall 2025
 * Description: Header for table.c Declares the hash table structure and all exported functions used by lab6.c and other modules
 */

/* constants used to indicate type of probing.  */
enum ProbeDec_t {LINEAR, DOUBLE, CHAIN}; //Linear = 0, Double = 1, Chain = 2

typedef int  *data_t;   // pointer to the information, I, to be stored in the table
typedef char *hashkey_t; // the key, K, for the pair (K, I)



typedef struct table_entry_tag{
	hashkey_t key;
	data_t data;
	int state; //Empty = 0 , Occupied = 1, Deleted = 2;

} table_entry;

typedef struct chain_node_tag{
	hashkey_t key;
	data_t data;
	struct chain_node_tag *next;

}chain_node;

typedef struct table_tag {
    // you need to fill in details
    // keep track of M, the size of your table 
    int size;
    enum ProbeDec_t probeType; // what type of probing are you using. See ProbeDec_t
    int keys;// how many keys are in the table
    int probeCount; // how many probes for the most recent call to insert, search or remove
    table_entry *open_addr;  // pointer to an array of structers for open addressing 
    chain_node **chains; // pointer to an array of pointers for separate chaining like MP1 
} table_t;
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
table_t *table_construct(int table_size, int probe_type);  

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
void table_rehash(table_t * T, int new_table_size);  

/* Returns the number of entries in the table.  Store this value
 * in the table header because this function is called frequently
 */
int table_entries(table_t *);

/* returns 1 if table is full and 0 if not full. 
 * For separate chaining the table is never full
 */
int table_full(table_t *);

/* Returns the number of table entries marked as deleted.  
 *
 * For separate chaining the number of delete keys is always zero.
 *
 * For lab6 experiments, this function is rarely called.  So it is
 * acceptable for this project to just scan the table and count the 
 * number entries deleted when the function is called.  
 */
int table_deletekeys(table_t *);
   
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
int table_insert(table_t *, hashkey_t K, data_t I);

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
data_t table_delete(table_t *, hashkey_t K); 

/* Given a key, K, retrieve the pointer to the information, I, from the table,
 * but do not remove (K, I) from the table.  Return NULL if the key is not
 * found.
 */
data_t table_search(table_t *, hashkey_t K); 

/* Free all information in the table, the table itself, and any additional
 * headers or other supporting data structures.  
 */
void table_destruct(table_t *);

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
int table_stats(table_t *);  

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
hashkey_t table_peek(table_t *T, int index, int list_position); 

/* Print the table position and keys in a easily readable and compact format.
 * Distinguish between empty and deleted locations.
 * Only useful when the table is small.
 */
void table_debug_print(table_t *T);

