/* tree.h
*  Public functions and data structures for trees
*/

typedef int tree_key_t;
typedef int data_t;

typedef struct NodeTag
{
	tree_key_t unique_key;
	data_t * data_ptr;
	struct NodeTag *LChild;
	struct NodeTag *RChild;
} tree_node;
typedef struct TreeTag
{
	tree_node *root;	//root of the tree of course
	int keys;            //Stores current number of keys in the tree
	int key_comparisons; //stores the number of key comparisons during the most recent search, insert, or remove
} tree_t;

tree_t * tree_construct();
void tree_destruct(tree_t *);
int tree_insert(tree_t *, tree_key_t Key,data_t *DataPointer);
data_t *tree_remove(tree_t *, tree_key_t Key);
data_t *tree_search(tree_t *, tree_key_t Key);
int tree_level(tree_t *,tree_key_t Key);
int tree_size(tree_t *);
int tree_stats(tree_t *);
int tree_internal_path_len(tree_t *);
void free_nodes(tree_node *);
int depth_search(tree_node *,int);
int debug_val_rec(tree_node *, int , int , int *);
void ugly_print(tree_node *, int );
void tree_debug_print_tree(tree_t *);
void tree_debug_validate(tree_t *);


