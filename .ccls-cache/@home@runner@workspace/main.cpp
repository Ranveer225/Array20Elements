#include <iostream>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>   
#include <ctime>   

using namespace std;

int findMin(int arr[], int start, int end) {
    int minVal = arr[start];
    for (int i = start + 1; i < end; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}
int main() {
    pid_t p;
    int arr[20], pipefd[2];
    srand(time(0));  

    printf("Generated Array: ");
    for (int i = 0; i < 20; i++) {
        arr[i] = rand() % 100 + 1;
        printf("%d ", arr[i]);
    }
    printf("\n");

    pipe(pipefd);  
    p = fork();    

    if (p == 0) { 
        close(pipefd[0]); 
        int minChild = findMin(arr, 10, 20);
        write(pipefd[1], &minChild, sizeof(minChild));
        close(pipefd[1]); 
        printf("Child Process ID: %d, Minimum number in second half: %d\n", getpid(), minChild);
    } 
    else { 
        wait(NULL); 
        close(pipefd[1]); 
        int minChild, minParent = findMin(arr, 0, 10);
        read(pipefd[0], &minChild, sizeof(minChild));
        close(pipefd[0]); 
        printf("Parent Process ID: %d, Minimum number in first half: %d\n", getpid(), minParent);
        printf("Minimum number of array: %d\n", (minChild < minParent) ? minChild : minParent);
    }
    return 0;
}
