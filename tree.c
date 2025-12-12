
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "tree.h"
/*
Define and implement all functions from tree.h
*/

//Must make my own debug and print functions this time around.
void tree_debug_print_tree(tree_t *);
void tree_debug_validate(tree_t *);
int debug_val_rec(tree_node *N, int min, int max, int *count);
void ugly_print(tree_node *N, int level);

tree_t * tree_construct()
{
	tree_t* new_tree;
	new_tree = (tree_t*) malloc(sizeof(tree_t));
	new_tree->root = NULL;
	new_tree->keys = 0;
	new_tree->key_comparisons = 0;
	
	return new_tree;
}


/*
Free All items stored in the tree including the memory block with the data and the node_in_tree_t structure. Also frees the header block.
*/
void tree_destruct(tree_t *T)
{

	tree_node *N = T->root;
	free_nodes(N);
	free(T);

}

void free_nodes(tree_node *N)
{
	//post order recursive search to free each node. ( Left, Right, Root)
	if(N != NULL)
	{	
		free_nodes(N->LChild);
		free_nodes(N->RChild);
		free(N->data_ptr);
		free(N);
	}
}


/*
Insert the memory block pointed to by DataPointer into the tree with the associated key.
The function must return 0 if the key is already in the tree(in which case the data 
memory block is replaced). The function must return 1 if they key was not already in the tree but was instead added to the tree.
Inserted such that the tree becomes and retains being a binary search tree. left children are less than roots and right children are greater than roots. 
*/
int tree_insert(tree_t *T, tree_key_t Key, data_t *DataPointer)
{
	T->key_comparisons = 0;
	
	if(T->root == NULL)//first insertion --> the root
	{
		tree_node *newNode = (tree_node*) malloc(sizeof(tree_node));
		newNode->unique_key = Key;
		newNode->data_ptr = DataPointer;
		newNode->LChild = NULL;
		newNode->RChild = NULL;
		T->root = newNode;
		T->keys++;
		return 1;
	}
	
	tree_node * current = T->root; //now need to deal with non first node insertion
	tree_node * parent = NULL;
	
	while(current != NULL && current->unique_key != Key)
	{
		T->key_comparisons++;
		if(Key < current->unique_key)
		{
			parent = current;
			current = current->LChild;
			
		}
		else
		{
			parent = current;
			current = current->RChild;
			
		}
		T->key_comparisons++;
		
	}
	//Key exists within tree already
	if(current != NULL)
	{
		free(current->data_ptr);
		current->data_ptr = DataPointer;
		T->key_comparisons++;
		return 0;
	}
	else //current == NULL, meaning this key does not exist within the tree already
	{
		tree_node *newNode = (tree_node *) malloc(sizeof(tree_node));
		newNode->unique_key = Key;
		newNode->data_ptr = DataPointer;
		newNode->RChild = NULL;
		newNode->LChild = NULL;
		if(Key < parent -> unique_key)
		{
			parent->LChild = newNode;
			T->keys++;
			return 1;
		}
		else{ //assign parent Right child to newNode
			parent->RChild = newNode;
			T->keys++;
			return 1;
		}
	
	}


}


/*
Remove the item in the tree with the matching key. Return the pointer to the data memory block and free the tree_node memory block. If the key is not found in the tree,
then return NULL.
*/
data_t *tree_remove(tree_t *T, tree_key_t Key)
{
	//Need a parent node to follow the current node in order to do proper node removal.
	T->key_comparisons = 0;
	//need to use same search loop from tree insert
	tree_node * current = T->root;
	tree_node * parent = NULL;
	
	while(current != NULL && current->unique_key != Key)
	{
		T->key_comparisons++;
		if(Key < current->unique_key)
		{
			parent = current;
			current = current->LChild;
			
		}
		else
		{
			parent = current;
			current = current->RChild;
			
		}
		T->key_comparisons++;
		
	}
	//Key exists within tree already
	if(current != NULL)
	{	
		T->key_comparisons++;
		data_t * ret = current->data_ptr;
		tree_node * left = current->LChild;
		tree_node * right = current->RChild;
		
		//Case 1 , left and right children are both NULL -> dealing with a leaf
		if(left == NULL && right == NULL)
		{
			if(parent == NULL) //only root exists, so remove root
			{
				T->root = NULL;
				free(current);
				T->keys--;
				return ret;
			}
			else
			{
				//to determine which child of parent we are dealing with
				if(parent ->LChild == current)
				{
					parent->LChild = NULL;
				}
				else
				{
					parent -> RChild = NULL;
				}
				free(current);
				T->keys--;
				return ret;
			}
		}
		//case 2: There exists 1 child to current
		if((left == NULL && right != NULL) || (left != NULL && right == NULL))
		{
			//checks which child is not null and assigns it
			tree_node *child = (left!= NULL) ? left: right;
			if(parent == NULL)
			{
				T->root = child;
			}	
			else
			{
				if(parent->LChild == current)
				{
					parent->LChild = child;
				}
				else{
					parent->RChild = child;
				}
			}
			free(current);
			T->keys--;
			return ret;
		}
		//case 3, There exists 2 children for current node
		if(left != NULL && right != NULL)
		{
			//successor is the smallest node in the right subtree(from current)
			tree_node *succ = current->RChild;
			tree_node *parent_succ = current;
			while(succ->LChild != NULL)
			{
				parent_succ = succ;
				succ = succ->LChild;
			}
			//now the successor was found
			
			current->unique_key = succ->unique_key;
			current->data_ptr = succ->data_ptr;
			
			tree_node * replacement = succ->RChild;
			if(parent_succ == current)
			{
				current->RChild = replacement;
			}
			else{
				parent_succ->LChild = replacement;
			}
			free(succ);
			T->keys--;
			return ret;
		}
		
		
		
	}
	else //current == NULL, meaning this key does not exist within the tree already
	{
		return NULL;
	}
	
	
	return NULL;
}


/*
Find the tree element with the matching key and return a pointer to the data memory 
block that is stored in this node in the tree. If the key is not found in the tree then return NULL
*/
data_t *tree_search(tree_t *T, tree_key_t Key) 
{
	//Need to traverse and compare unique_key to the key passed in, if passed in key is less, move left
	//if passed in key is greater then unique key, move right.
	
	//This has to do with resetting the key_comparison count every search
	T->key_comparisons = 0;
	tree_node *current = T->root;
	while(current != NULL && current->unique_key != Key)
	{
		T->key_comparisons++;
		if(Key < current->unique_key)
		{
			
			current = current->LChild;
			
		}
		else
		{
			current = current->RChild;
			
		}
		T->key_comparisons++;
		
	}
	//adding 1 to the counter if key was found
	if(current != NULL)
	{
		T->key_comparisons++;
	}
	//if key wasnt found then just return null
	if(current == NULL)
	{
		return NULL;
	}
	data_t *data = current->data_ptr;
	return data;
}


/*
Find the tree element with the matching key and return the level number in the tree where the key is found. Recall that the key in the root node is at level 0. If the key is not found in the tree then return the level number where the key would have been found (that is, in the spot where it would have been inserted with your insert function).

*/
int tree_level(tree_t *T,tree_key_t Key)
{	
	//Could do a count of sorts to keep track of how many nodes we have traversed to find the node
	//Total number of keys is stored in tree_t which is the total number of nodes in the tree.
	tree_node *current = T->root;
	int level = 0;
	while(current != NULL && current->unique_key != Key)
	{
		
		if(Key < current->unique_key)
		{
			
			current = current->LChild;
			level++;
		}
		else
		{
			current = current->RChild;
			level++;
		}
		
		
	}
	
	//if key wasnt found then just return level
	if(current == NULL)
	{
		return level;
	}
	
	return level;
}

//returns the number of keys in the tree
int tree_size(tree_t *T)
{
	if(T == NULL)
	{
		return 0;
	}
	return T->keys;
}

/*
Return key_comparisons, the number of key comparisons for the most recent call of tree_insert, tree_remove, or tree_search.
*/
int tree_stats(tree_t *T)
{
	if(T == NULL)
	{
		return 0;
	}
	return T->key_comparisons;

}
//Return the internal path length of the tree
int tree_internal_path_len(tree_t *T)
{	if(T == NULL)
	{
		return 0;
	}
	return depth_search(T->root,0);

}

int depth_search(tree_node *N,int depth)
{
 	
 	if(N != NULL)
 	{
 		return depth + depth_search(N->LChild,depth+1)+ depth_search(N->RChild,depth+1);	
 	}

	return 0;
}

//Basic Validations from the pdf
int debug_val_rec(tree_node *N, int min, int max, int *count)
{
	if (N == NULL) return true;
	if (N->unique_key <= min || N->unique_key >= max) return false;
	assert(N->data_ptr != NULL);
	*count += 1;
	return debug_val_rec(N->LChild, min, N->unique_key, count) &&
	debug_val_rec(N->RChild, N->unique_key, max, count);
}

void tree_debug_validate(tree_t *T)
{
	int size = 0;
	if(T->root == NULL)
	{
		assert(T->keys == 0);
		return;
	}
	assert(debug_val_rec(T->root, INT_MIN, INT_MAX, &size) == true);
	assert(size == T->keys);
}

/*
void ugly_print(tree_node *N, int level) 
{
	if (N == NULL) return ;
	ugly_print(N->RChild, level+1) ;
	for (int i=0; i<level; i++) printf(" "); // print 5 spaces
	printf("%5d\n", N->unique_key); // field width is 5
	ugly_print(N->LChild, level+1);
}
void tree_debug_print_tree(tree_t *T) 
{
	printf("Tree with %d keys\n", T->keys);
	ugly_print(T->root, 0);
	printf("\n");
	tree_debug_validate(T);
}
*/
static void pretty_print(tree_node *node, int depth, char branch);

static void pretty_print(tree_node *node, int depth, char branch)
{
    if (node == NULL)
        return;

    // Print right subtree first (so it appears on top)
    pretty_print(node->RChild, depth + 1, '/');

    // Print indentation
    for (int i = 0; i < depth; i++)
        printf("        "); // 8 spaces per level

    // Print the current node
    if (branch == '/')
        printf(" /----");
    else if (branch == '\\')
        printf(" \\----");
    else
        printf(" root->");

    printf("[%d]\n", node->unique_key);

    // Print left subtree
    pretty_print(node->LChild, depth + 1, '\\');
}

void tree_debug_print_tree(tree_t *T)
{
    if (T == NULL || T->root == NULL) {
        printf("(empty tree)\n");
        return;
    }

    printf("\nTree with %d keys:\n", T->keys);
    pretty_print(T->root, 0, ' ');
    printf("\n");

    tree_debug_validate(T); 
}

