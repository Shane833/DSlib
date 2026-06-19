#include <lcthw/PQueue.h>

// Forward Function Declaration
static int LeftChild(PQueue* q, int parent_index);
static int RightChild(PQueue* q, int parent_index);
static int Parent(PQueue* q, int child_index);
static void BubbleUp(PQueue* q, int element_index);
static void BubbleDown(PQueue* q, int element_index);
static void PQ_Swap(PQueue* q, const int a_index, const int b_index);

// Additional Function - Defining this function here bcz its just being used 
// for one specific task and shouldn't really be available for the outside user
static inline void* PQueue_Get(PQueue* q, int index)
{
	check(index <= q->end, "Invalid Index!")
	
	return q->array[index];
error:
	return NULL;
}

// Function Definitions
PQueue* PQueue_Create(size_t size, PQueue_Compare cmp)
{
	PQueue *q = (PQueue*)malloc(sizeof(PQueue));
	check_mem(q);
	q->max = size;
	q->end = -1;
	q->cmp = cmp;
	
	q->array = calloc(size, sizeof(void*));
	check_mem(q->array);
	
	return q;
error:
	return NULL;
}

PQueue* PQueue_CreateFromArray(void** array, size_t size, PQueue_Compare cmp)
{
	check(array != NULL, "Invalid Array!");
	
	PQueue* q = PQueue_Create(size, cmp);
	check_mem(q);
	
	for(int i = 0;i < size;i++){
		PQueue_Enqueue(q, array[i]);
	}

	return q;
error:
	return NULL;
}

PQueue* PQueue_CreateFromStaticArray(void** array, size_t size, PQueue_Compare cmp)
{
	check(array != NULL, "Invalid Array!");
	
	PQueue* q = PQueue_Create(size, cmp);
	check_mem(q);
	
	for(int i = 0;i < size;i++){
		PQueue_Enqueue(q, &array[i]);
	}
	
	return q;
error:
	return NULL;
}

void PQueue_Enqueue(PQueue* q, void* element)
{
	check(q != NULL, "Invalid Queue!");
	check(q->end < (int)q->max, "Overflow!"); // Bug fixed : Don't compare -ve int with size_t 

	q->array[++q->end] = element;
	BubbleUp(q, q->end);
	
error: // fallthrough
	return;
}

void* PQueue_Dequeue(PQueue* q)
{
	check(q != NULL, "Invalid Queue!");
	check(q->end >= 0, "Underflow!");
	
	void* element = q->array[0];
	
	q->array[0] = q->array[q->end--];
	BubbleDown(q, 0);

	return element;
error: // fallthrough
	return NULL;
}

// Static Functions Defintion
static int Parent(PQueue* q, int child_index)
{
	check(child_index <= q->end || child_index > 0, "Invalid Child Index!");
	
	int parent_index = (int)((child_index - 1) / 2); // taking the floor
	
	return parent_index;
error:
	return -1;
}

static int LeftChild(PQueue* q, int parent_index)
{
	check(parent_index <= q->end, "Invalid Parent Index!");
	
	int child_index = 2 * parent_index + 1;
	if(child_index > q->end) goto error;
	
	return child_index;
error:
	return -1;
}

static int RightChild(PQueue* q, int parent_index)
{
	check(parent_index <= q->end, "Invalid Parent Index!");
	
	int child_index = 2 * parent_index + 2;
	if(child_index > q->end) goto error;
	
	return child_index;
error:
	return -1;
}

static void BubbleUp(PQueue* q, int element_index)
{
	// We will compare the parent of the current element with itself
	// and if the parent is not dominant than we swap
	int parent_index = Parent(q, element_index);
	
	if(parent_index == -1) return; // basically reaching the root
	
	if(q->cmp(q->array[element_index], q->array[parent_index]) < 0)
	{
		// swap and call the function again with the updated index
		PQ_Swap(q, element_index, parent_index);
		BubbleUp(q, parent_index);
	}
}

static void BubbleDown(PQueue* q, int element_index)
{
	// we will check either of the child if they will dominate the
	// current element then we swap and continue till leaf nodes
	
	if(PQueue_Size(q) == 0) return; // When no element remains
	
	int dominant = element_index; // assuming this to be dominant element
	
	if(LeftChild(q, element_index) != -1){
		int left = LeftChild(q, element_index);
		dominant = q->cmp(q->array[dominant], q->array[left]) < 0 ? dominant : left;
	}
	if(RightChild(q, element_index) != -1){
		int right = RightChild(q, element_index);
		dominant = q->cmp(q->array[dominant], q->array[right]) < 0 ? dominant : right;
	}
	
	if(dominant != element_index){
		PQ_Swap(q, dominant, element_index);
		BubbleDown(q, dominant);
	}
}

static void PQ_Swap(PQueue* q, const int a_index, const int b_index)
{
	void* temp = q->array[a_index];
	q->array[a_index] = q->array[b_index];
	q->array[b_index] = temp;
}

// Destroy Functions

void PQueue_Destroy(PQueue* q)
{
	if(q){
		if(q->array){
			free(q->array);
		}
		free(q);
	}
}

void  PQueue_ClearDestroy(PQueue* q)
{
	if(q){
		if(q->array){
			for(int i = 0;i < PQueue_Size(q);i++){
				if(PQueue_Get(q, i)){
				free(PQueue_Get(q,i));
				}
			}
			free(q->array);
		}
		free(q);
	}
}
