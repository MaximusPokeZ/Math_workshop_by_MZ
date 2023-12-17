
typedef struct PriorityQueue
{
    /* data */
} PriorityQueue;


void heapifyDown(PriorityQueue* pq, int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;

    if (leftChild < pq->size && pq->array[leftChild] > pq->array[largest]) {
        largest = leftChild;
    }

    if (rightChild < pq->size && pq->array[rightChild] > pq->array[largest]) {
        largest = rightChild;
    }

    if (largest != index) {
        swap(&pq->array[index], &pq->array[largest]);
        heapifyDown(pq, largest);
    }
}