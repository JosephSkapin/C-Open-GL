/* llist.c                  <<-- A template to update and 
 * Joseph Skapin         <<-- change
 * jskapin                 <<-- change
 * JosephSkapin   <--- replace with your github name
 * ECE 2230 Fall 2025
 * MP3
 *
 * Propose: A template for llist.c. You will make many changes.
 *
 * Assumptions: Many details are incomplete.  
 *
 * Bugs: Many details have not been implemented.
 
  MP3 Functions Begin on Line 505
  Creating Sorting algorithms for our two way linked list using insertion sort,selection sort, merge sort and qsort.
 * debug_validate funciton has been removed from most functions to improve sorting performance
 *
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "llist.h"        // defines public functions for list ADT

// definitions for private constants used in llist.c only

#define LLIST_SORTED    989898
#define LLIST_UNSORTED -898989

// prototypes for private functions used in llist.c only 

void llist_debug_validate(llist_t *L);

/* ----- below are the functions  ----- */

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from llist_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.
 */
data_t * llist_access(llist_t *list_ptr, int pos_index)
{
    llist_elem_t *rover = NULL;
 
    // debugging function to verify that the structure of the list is valid
    //llist_debug_validate(list_ptr);

    /* handle special cases.
     *   1.  The list is empty
     *   2.  Asking for the head 
     *   3.  Asking for the tail
     *   4.  specifying a position that is out of range.  This is not defined
     *       to be an error in this function, but instead it is assumed the 
     *       calling function correctly specifies the position index
     */
    if (list_ptr->ll_entry_count == 0) {
        return NULL;  // list is empty
    }
    else if (pos_index == LLPOSITION_FRONT || pos_index == 0) { //asking for head
        return list_ptr->ll_front->data_ptr;
    }
    else if (pos_index == LLPOSITION_BACK || pos_index == list_ptr->ll_entry_count - 1) {
        return list_ptr->ll_back->data_ptr; //asking for tail
    }
    else if (pos_index < 0 || pos_index >= list_ptr->ll_entry_count)
        return NULL;   // does not correspond to position in list

    // loop through the list until find correct position index
    int count=0;
    rover = list_ptr->ll_front;
    while(count!=pos_index)
    {
    rover = rover->ll_next;
    count++;
    
    }
 
    // already verified that pos_index should be valid so rover better not be null
    assert(rover != NULL);
    assert(rover->data_ptr != NULL);
    return rover->data_ptr;
}

/* Allocates a new, empty list 
 *
 * If the comparison function is NULL, then the list is unsorted.
 *
 * Otherwise, the list is initially assumed to be sorted.  Note that if 
 * list_insert is used the list is changed to unsorted.  
 *
 * The field sorted can only take values LLIST_SORTED or LLIST_UNSORTED
 *
 * Use llist_destruct to remove and deallocate all elements on a list 
 * and the header block.
 *
 * (This function is written and no changes needed. It provides an example
 *  of how save the comparison function pointer.  See other examples in this
 *  file for how to use compare_fun.)
 */
llist_t * llist_construct(int (*fcomp)(const data_t *, const data_t *))
{
    llist_t *new_list;
    new_list = (llist_t *) malloc(sizeof(llist_t));
    new_list->ll_front = NULL;
    new_list->ll_back = NULL;
    new_list->ll_entry_count = 0;
    new_list->compare_fun = fcomp;
    if (fcomp == NULL)
        new_list->ll_sorted_state = LLIST_UNSORTED;
    else
        new_list->ll_sorted_state = LLIST_SORTED;

    //llist_debug_validate(new_list);
    return new_list;
}

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 * 
 * fcomp: function to test if elem_ptr is a match to an item in the list.
 *        Returns 0 if they match and any value not equal to 0 if they do not match.
 *
 * NOTICE: pos_index is returned and is not an input value!
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1. 
 *
 * pos_index: used as a return value for the position index of matching element
 *
 */
data_t * llist_elem_find(llist_t *list_ptr, data_t *elem_ptr, int *pos_index,
        int (*fcomp)(const data_t *, const data_t *))
{

    assert(list_ptr != NULL);
    //assert(fcomp != NULL);
    
    llist_elem_t *curr = list_ptr->ll_front;
    int count = 0;
    
    while(curr !=NULL)
    {
        if(fcomp(curr->data_ptr,elem_ptr)==0)
        {
            if(pos_index !=NULL)
            {
                *pos_index = count;
            }
            return curr->data_ptr;
           
            
        }
        curr = curr->ll_next;
        count++;
    }
    
    //llist_debug_validate(list_ptr);

    // fix the return values 
    if(pos_index != NULL)
    {
        *pos_index = -1;
    }   
    return NULL;
}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 */
void llist_destruct(llist_t *list_ptr)
{
    //list_ptr has access to entry count, sorted state, front and back.
    //go to front then move along the list
    
    // the first line must validate the list 
    //llist_debug_validate(list_ptr);

    // Your code starts here
    if(list_ptr == NULL)
    {
        return;
    }
    
    llist_elem_t *current = list_ptr->ll_front;
    llist_elem_t *next;
    
    while (current != NULL)
    {
        next = current->ll_next;
        free(current->data_ptr);
        free(current);
        current = next;
    }
    
    free(list_ptr);
    
}

/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * llist_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the 
 *            list.  Index starts at 0 at head of the list, and incremented by 
 *            one until the tail is reached.  The index can also be equal
 *            to LLPOSITION_FRONT or LLPOSITION_BACK (these are special negative 
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list, 
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the llist_ptr->ll_sorted_state must be equal 
 * to LLIST_UNSORTED.
 */
void llist_insert(llist_t *list_ptr, data_t *elem_ptr, int pos_index)
{
    assert(list_ptr != NULL);
    assert(pos_index == LLPOSITION_FRONT || pos_index == LLPOSITION_BACK || pos_index >= 0);
    // note you MUST handle the case that pos_index is LLPOSITION_FRONT or BACK
    
    
    // insert your code here
    if(list_ptr == NULL || elem_ptr ==NULL){
        return; //invalid input
    }
    llist_elem_t *new_elem = malloc(sizeof(llist_elem_t));
    new_elem->data_ptr = elem_ptr;
    new_elem->ll_next = NULL;
    new_elem->ll_prev = NULL;
    
    //Case 1: pos_index == LLPOSITION_FRONT
    if(pos_index == LLPOSITION_FRONT || pos_index ==0)
    {
        new_elem->ll_next = list_ptr->ll_front;
        if(list_ptr->ll_front != NULL)
        {
            list_ptr->ll_front->ll_prev = new_elem;
        }
        else //list was empty
        {
           list_ptr->ll_back = new_elem;
        }
        list_ptr->ll_front = new_elem;
       
    }
    
    //Case 2: pos_index == LLPOSITION_BACK
    else if(pos_index == LLPOSITION_BACK || pos_index >= list_ptr->ll_entry_count)
    {
        new_elem->ll_prev = list_ptr->ll_back;
        if(list_ptr->ll_back !=NULL)
        {
            list_ptr->ll_back->ll_next = new_elem;
        }
        else
        {
            list_ptr->ll_front = new_elem;
        }
        list_ptr->ll_back=new_elem;
        
        
    }   
    
    //Case 3: Insert directly at index position, middle of list
    else{
        llist_elem_t *curr = list_ptr->ll_front;
        int count =0;
        while(curr != NULL && count < pos_index)
        {
            curr = curr->ll_next;
            count++;
        }
    
    
        new_elem->ll_next = curr;
        new_elem->ll_prev = curr->ll_prev;
        if(curr->ll_prev !=NULL)
        {
            curr->ll_prev->ll_next = new_elem;
        }
        curr->ll_prev = new_elem;
    
        if(curr == list_ptr->ll_front)
        {
            list_ptr->ll_front = new_elem;
        }
       }
       list_ptr->ll_entry_count++;
    
    
    
 
    // the last three lines of this function must be the following 
    if (list_ptr->ll_sorted_state == LLIST_SORTED) 
        list_ptr->ll_sorted_state = LLIST_UNSORTED;
    //llist_debug_validate(list_ptr);
    
    return;
}

/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the compare_fun function pointer found in the list header:
 *     list_ptr->compare_fun(A, B)
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use llist_insert_sorted, the list preserves its sorted nature.
 *
 * If you use llist_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call llist_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 *
 * Note: if the element to be inserted is equal in rank to an element already
 *       in the list, the newly inserted element will be placed after all the 
 *       elements of equal rank that are already in the list.
 */
void llist_insert_sorted(llist_t *list_ptr, data_t *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->ll_sorted_state == LLIST_SORTED);
    //insert your code here
    if(list_ptr->ll_sorted_state == LLIST_UNSORTED ||elem_ptr == NULL)
    {
        return;
    }
    //Allocate new node to store the data_t being passed in
    llist_elem_t *new_elem = malloc(sizeof(llist_elem_t));
    assert(new_elem != NULL);
    
    new_elem->data_ptr = elem_ptr;
    new_elem->ll_next = NULL;
    new_elem->ll_prev = NULL;
    
    //1: If list is empty
    if(list_ptr->ll_front ==NULL)
    {
        list_ptr->ll_front = new_elem;
        list_ptr->ll_back = new_elem;
        list_ptr->ll_entry_count = 1;
        llist_debug_validate(list_ptr);
        return;
    }
    
    //2. traverse to find insertion point
    llist_elem_t *curr = list_ptr->ll_front;
    while(curr != NULL && list_ptr->compare_fun(elem_ptr,curr->data_ptr) <=0)
    {
        curr = curr->ll_next;
    }
    
    if(curr==NULL)//insert at back 
    {
        new_elem->ll_prev = list_ptr->ll_back;
        list_ptr->ll_back->ll_next=new_elem;
        list_ptr->ll_back = new_elem;
        
    }
    else if(curr->ll_prev ==NULL)//insert at front
    {
        new_elem->ll_next = curr;
        curr->ll_prev = new_elem;
        list_ptr ->ll_front = new_elem;
            
    }
    else{
        //insert between
        new_elem->ll_prev = curr->ll_prev;
        new_elem->ll_next = curr;
        curr ->ll_prev->ll_next = new_elem;
        curr->ll_prev = new_elem;
        
    }
    list_ptr->ll_entry_count++;
    // the last line checks if the new list is correct 
    //llist_debug_validate(list_ptr);
}

/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
data_t * llist_remove(llist_t *list_ptr, int pos_index)
{
    assert(list_ptr != NULL);
    assert(pos_index == LLPOSITION_FRONT || pos_index == LLPOSITION_BACK || pos_index >= 0);
    // note you MUST handle the case that pos_index is LLPOSITION_FRONT or BACK
 
    // insert your code here 
    if(pos_index > list_ptr->ll_entry_count)
    {
        return NULL;
    }
    llist_elem_t *curr = list_ptr->ll_front;
    int count =0;
    //Case 1: pos_index == LLPOSITION_FRONT
    if(pos_index == LLPOSITION_FRONT || pos_index == 0)
    {
        curr = list_ptr->ll_front;
    }
    //Case 2: pos_index == LLPOSITION_BACK
    else if(pos_index == LLPOSITION_BACK || pos_index == list_ptr->ll_entry_count-1)
    {
        curr = list_ptr->ll_back;
    }
  
    //Case 3: pos_index is between front and back of the list
    else{
        
        while(curr !=NULL && count < pos_index)
        {
            curr = curr->ll_next;
            count++;
        }
        
    }
    
    if(curr->ll_prev !=NULL)
    {
       curr->ll_prev->ll_next = curr->ll_next; 
    }
    else
    {
        list_ptr->ll_front = curr->ll_next;
    }
    
    if(curr->ll_next !=NULL)
    {
        curr->ll_next->ll_prev = curr->ll_prev;
    }
    else{
        list_ptr->ll_back = curr->ll_prev;
    }
    
    data_t *removed_data = curr->data_ptr;
    free(curr);
    
    list_ptr->ll_entry_count--;
   
    // the last line should verify the list is valid after the remove
    //llist_debug_validate(list_ptr);
    return removed_data;  // you must fix the return value
}

/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 *
 * (This function is already written, so no changes necessary.)
 */
int llist_entries(llist_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->ll_entry_count >= 0);
    return list_ptr->ll_entry_count;
}


// Start of MP3 Functions
//Converting Standish.c functions to work with two way linked list
/*
   Sort Types: 
   1: Insertion
   2: Recursive Selection
   3: Iterative Selection
   4: Merge
   
   List Types: (delt with by geninput.c)
   1: Random adresses
   2: Ascending and sequential 
   3: Descending and sequential
*/

//defining function prototypes
void insertion_sort(llist_t *list_ptr);
void SelectionSort(llist_t *A, llist_elem_t *m, llist_elem_t *n);
void IterativeSelectionSort(llist_t *A, llist_elem_t *m, llist_elem_t *n);
void MergeSort(llist_t *list_ptr);
void qsort_llist(llist_t *list_ptr);
void llist_split(llist_t *list_ptr, llist_t *LL, llist_t *RL);
void Merge(llist_t *LL, llist_t *RL, llist_t *result,
           int (*fcomp)(const data_t *, const data_t *));


void llist_sort(llist_t *list_ptr, int sort_type,int (*fcomp)(const data_t *, const data_t *))
{
    if(list_ptr == NULL)
    {
        return;
    }
    list_ptr->compare_fun = fcomp;
 
    if(sort_type == 1)
    {
    insertion_sort(list_ptr);
    
    }
    else if(sort_type == 2)
    {
        SelectionSort(list_ptr,list_ptr->ll_front,list_ptr->ll_back);
    }
    else if(sort_type == 3)
    {
        IterativeSelectionSort(list_ptr,list_ptr->ll_front,list_ptr->ll_back);
    }
    else if(sort_type == 4)
    {
        MergeSort(list_ptr);
    }
    else if(sort_type == 5)
    {
        qsort_llist(list_ptr);
    }
    


    list_ptr->ll_sorted_state = LLIST_SORTED;
    llist_debug_validate(list_ptr);
    return;
}
//Insertion sort changed to work with two way linked list
void insertion_sort(llist_t *list_ptr)
{
    if(list_ptr == NULL || list_ptr->ll_front == NULL)
    {
        return;
    }
    
    llist_t *sorted = llist_construct(list_ptr->compare_fun);
    
    while(list_ptr->ll_front != NULL)
    {
        data_t *item = llist_remove(list_ptr,LLPOSITION_FRONT);
        
        llist_insert_sorted(sorted,item);
    }
    
    *list_ptr = *sorted;
    
    free(sorted);
    

}
//finds the maximum item in a list and returns it
llist_elem_t * FindMax(llist_t *A, llist_elem_t *m, llist_elem_t *n)   /* assume m<n */
{ 
   llist_elem_t *i = m;      /* i is an index that visits all positions from m to n */
   llist_elem_t *j = m;     /* j is an index that saves the position of the largest */
            
                     
   do {
      i = i->ll_next;                      /* advance i to point to next number A[i] */
      if (A->compare_fun(i->data_ptr, j->data_ptr)==1) {         
                                             /* if A[i] > largest previous A[j] then */
         j = i;              /* save the position, i, of the largest in j */
      }
   } while (i != n);           /* stop when all i in m:n have been tested */

   return j;                       /* return j == position of the largest */
}  
//selection sort changed to work with two way linked list
void SelectionSort(llist_t *A, llist_elem_t *m, llist_elem_t *n)
{
    if(A == NULL)
    {
        return;
    }
    llist_elem_t *MaxPosition;    /* MaxPosition is the index of A's biggest item  */
    data_t *temp;                     /* temp is used to exchange items in A */
    if (m != n) {               /* if there is more than one number to sort */
   
      /* Let MaxPosition be the index of the largest number in A */
         MaxPosition = FindMax(A,m,n);
      
      /* exchange m <--> Maxposition* ( change the data_ptr(s))*/
         temp = m->data_ptr;
         m->data_ptr = MaxPosition->data_ptr;
         MaxPosition->data_ptr = temp;
      
      /* SelectionSort the subarray, move to the next entry. */
         SelectionSort(A, m->ll_next, n);
   }
}

//similar to selection sort, without recursion
void IterativeSelectionSort(llist_t *A, llist_elem_t *m, llist_elem_t *n)
{
   if(A == NULL)
   {
    return;
   }
   llist_elem_t *MaxPosition;
   data_t *temp;
   llist_elem_t *i;

   while (m != n) {
   
      i = m; 
      MaxPosition = m;

      do {
         i = i->ll_next;
         if (A->compare_fun(i->data_ptr, MaxPosition->data_ptr) == 1)
         {
            MaxPosition = i;
         }
      } while (i != n);
   
      temp = m->data_ptr;
      m->data_ptr = MaxPosition->data_ptr; 
      MaxPosition->data_ptr = temp;
   
      m = m->ll_next;
   }
}
//requires two helper functions, llist_split, and Merge.
/*
Splits main list into two lists, sorts those lists then merges them into the main list now sorted
return early if the list doesnt have more than 1 entry.
*/
void MergeSort(llist_t *list_ptr)
{
    
    if(list_ptr == NULL || list_ptr->ll_entry_count <= 1)
    {
        return;
    }
    
    llist_t RL;
    llist_t LL; 
    
    
    //Split main list into two lists
    llist_split(list_ptr,&LL,&RL);
    
    
    //Recsursion to sort the left/right lists
    MergeSort(&LL);
    MergeSort(&RL);
    
    //Merge back into single list
    llist_t merged;
    Merge(&LL,&RL,&merged,list_ptr->compare_fun);
    
    
    *list_ptr = merged;
}
//Splitting list_ptr into two halves
void llist_split(llist_t *list_ptr, llist_t *LL, llist_t *RL)
{
    int half = list_ptr->ll_entry_count / 2;  
    llist_elem_t *curr = list_ptr->ll_front; //rover to traverse list_ptr

    // move curr to the mid point, assign fronts, then break the list into two halves
    for (int i = 1; i < half; i++) {
        curr = curr->ll_next;
    }

    // assign fronts
    LL->ll_front = list_ptr->ll_front;
    RL->ll_front = curr->ll_next;
    if (RL->ll_front)
    {
        RL->ll_front->ll_prev = NULL;
    }

    // break connection
    curr->ll_next = NULL;
    
    LL->ll_back = curr;
    RL->ll_back = list_ptr->ll_back;

    // Giving LL/RL respective count sizes
    LL->ll_entry_count = half;
    RL->ll_entry_count = list_ptr->ll_entry_count - half;

    // give LL/RL list_ptr fcomp and sorted state
    LL->compare_fun = list_ptr->compare_fun;
    RL->compare_fun = list_ptr->compare_fun;
    LL->ll_sorted_state = list_ptr->ll_sorted_state;
    RL->ll_sorted_state = list_ptr->ll_sorted_state;
}

//Takes in left list, right list, result list along with comparison function so we can sort.
void Merge(llist_t *LL, llist_t *RL, llist_t *result, int (*fcomp)(const data_t *, const data_t *))
{
    llist_elem_t *L = LL->ll_front;
    llist_elem_t *R = RL->ll_front;
    llist_elem_t *head = NULL, *tail = NULL;

    while (L != NULL && R != NULL) 
    {
        //comparison function decises what chosen will become
        llist_elem_t *chosen;
        if(fcomp(L->data_ptr, R->data_ptr) >= 0)
        {
            chosen = L;
        }
        else
        {
            chosen = R;
        }
        if (chosen == L) 
        {
            L = L->ll_next;
        }       
        else
        {
            R = R->ll_next;
        }

        chosen->ll_next = NULL;
        if (!head) 
        {
            head = tail = chosen;
            chosen->ll_prev = NULL;
        } else 
        {
            tail->ll_next = chosen;
            chosen->ll_prev = tail;
            tail = chosen;
        }
    }

    // deal with leftover nodes
    if (L != NULL) 
    {
        if (!head)
        {
            head = L;
        } 
        else
        { 
            tail->ll_next = L;
            L->ll_prev = tail; 
        }
        tail = LL->ll_back;
    }
     else if (R != NULL) 
     {
        if (head == NULL)
        {
            head = R;
        }
        else 
        { 
            tail->ll_next = R; 
            R->ll_prev = tail; 
        }
        tail = RL->ll_back;
     }
    //assigning proper data to result list
    result->ll_front = head;
    result->ll_back = tail;
    result->ll_entry_count = LL->ll_entry_count + RL->ll_entry_count;
    result->compare_fun = fcomp;
    result->ll_sorted_state = LLIST_SORTED;
}


int qsort_compare(const void *p_a, const void *p_b, void * lptr)
{
    llist_t *list_ptr = (llist_t *) lptr;
    return list_ptr->compare_fun (*(data_t **) p_b, *(data_t **) p_a);
}


void qsort_llist(llist_t *list_ptr) 
{
    int i, Asize = llist_entries(list_ptr);
    assert(list_ptr->compare_fun != NULL);
    data_t ** QsortA = (data_t **) malloc(Asize*sizeof(data_t *));

    for (i = 0; i < Asize; i++) {
        QsortA[i] = llist_remove(list_ptr, LLPOSITION_FRONT);
    }

    qsort_r(QsortA, Asize, sizeof(data_t *), qsort_compare, list_ptr);

    for (i = 0; i < Asize; i++) {
        llist_insert(list_ptr, QsortA[i], LLPOSITION_BACK);
    }
    free(QsortA);
}

/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void llist_debug_validate(llist_t *L)
{
    llist_elem_t *N;
    int count = 0;
    assert(L != NULL);
    if (L->ll_front == NULL)
        assert(L->ll_back == NULL && L->ll_entry_count == 0);
    if (L->ll_back == NULL)
        assert(L->ll_front == NULL && L->ll_entry_count == 0);
    if (L->ll_entry_count == 0)
        assert(L->ll_front == NULL && L->ll_back == NULL);
    if (L->ll_entry_count == 1) {
        assert(L->ll_front == L->ll_back && L->ll_front != NULL);
        assert(L->ll_front->ll_next == NULL && L->ll_front->ll_prev == NULL);
        assert(L->ll_front->data_ptr != NULL);
    }
    if (L->ll_front == L->ll_back && L->ll_front != NULL)
        assert(L->ll_entry_count == 1);
    assert(L->ll_sorted_state == LLIST_SORTED || L->ll_sorted_state == LLIST_UNSORTED);
    if (L->ll_entry_count > 1) {
        assert(L->ll_front != L->ll_back && L->ll_front != NULL && L->ll_back != NULL);
        N = L->ll_front;
        assert(N->ll_prev == NULL);
        while (N != NULL) {
            assert(N->data_ptr != NULL);
            if (N->ll_next != NULL) assert(N->ll_next->ll_prev == N);
            else assert(N == L->ll_back);
            count++;
            N = N->ll_next;
        }
        assert(count == L->ll_entry_count);
    }
    if (L->ll_sorted_state == LLIST_SORTED && L->ll_front != NULL) {
        N = L->ll_front;
        while (N->ll_next != NULL) {
            assert(L->compare_fun(N->data_ptr, N->ll_next->data_ptr) != -1);   // A <= B
            N = N->ll_next;
        }
    }
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
