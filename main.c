#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include<unistd.h>


//////// FLAG ////////////
static int flag = 1;    //
//////////////////////////


#define TRUE 1
#define FALSE 0

//Thread Methods//////
void* alg1(void*);  //
void* alg2(void*);  //
void* alg3(void*);  //
void* alg4(void*);  //
//////////////////////


//Algorithm Implementation////////////////////////////////////////
int *algorithm1(int ,int ,int ,int , int **);                   //
int* algorithm1Rec(int , int , int , int, int **, int , int *); //
int* algorithm2Rec(int , int , int , int, int **, int , int *); //
//////////////////////////////////////////////////////////////////


//Helper Methods//////////////////////////////////
bool isHittingSet(int *, int **, int, int);     //
void printT(int **, int, int);                  //
int length(int *, int);                         //
int* swapArrays(int **,int,int);                //
int swapNumbers(int*,int,int);                  //
int clearArrays(int **, int, int,int, int);     //
int criticalNumber(int *,int **,int,int);       //
void disp(int *, int);                          //
//////////////////////////////////////////////////
typedef struct{
    int n,m,c,k;
    int *set;
    int **subsets;
}Args;

int main(){
    srand(time(NULL));
    FILE *fp;
    Args *args = (Args *)malloc(sizeof(Args));
    if(flag){
        fp = fopen("sets.dat", "r");
        if(fp==NULL){
            perror("Faild to open file sets.dat");
            return 0;
        }
        fscanf(fp, "%d %d %d %d",&args->n,&args->m,&args->c,&args->k);
        args->set = malloc(args->n*(sizeof(int)));
        args->subsets = malloc(args->m*sizeof(int*));
        for(int i =0;i<args->m;i++)
            args->subsets[i] = malloc(args->c*sizeof(int));
        int i=0, j=0;
        for(i=1;i<=args->n;i++)args->set[i-1] = i;                              // initaliise the main set.
        i=0;j=0;
        while(TRUE){                                                            // read all subsets from file
            int num;
            if(fscanf(fp,"%d",&num)==EOF)
                break;
            else{
                args->subsets[i][j]=num;
                j++;
            }
            char e;
            if(fscanf(fp, "%c",&e)==1 && e=='\n'){
            i++;
            j=0;
            }
        }
    }
    else{
        //create random data.
    }

    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, alg1, (void *)args);
    pthread_create(&t2, NULL, alg2, (void *)args);
    pthread_create(&t3, NULL, alg3, (void *)args);
    pthread_create(&t4, NULL, alg4, (void *)args);
    int *set1, *set2, *set3, *set4;
    pthread_join(t1, (void **)&set1);
    pthread_join(t2, (void **)&set2);
    pthread_join(t3, (void **)&set3);
    pthread_join(t4, (void **)&set4);

    printf("for the array: ");for(int i=1;i<=args->n;i++) printf("%d ", i);
    for(int i =0;i<args->m;i++){
        printf("\nB%d: ",i);
        for(int j=0;j<args->c;j++)
            printf("%d ",args->subsets[i][j]);
        printf("");
    }
    printf("\nHitting Set:\n");
    for(int i=0;i<args->k;i++){
         printf("%d ", set1[i]);
    }
    if(flag){
        fclose(fp);
        free(args);
    }
    free(set1);
}


void* alg1(void* args){
    Args *arg = (Args *)args;
    int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    algorithm1Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet);

    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    free(cpyArr);
    return rtrnSet;
}
void* alg2(void* args){
    Args *arg = (Args *)args;
    int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    algorithm2Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet);


    return rtrnSet;
}
void* alg3(void* args){
    return NULL;
}
void* alg4(void* args){
    return NULL;
}

// TODO
// [x] make it recursive.
// [x] we never added an element in rtrnSet.
int* algorithm1Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet){
    if(rtrnIndex>=k){
        printf("Re: rtr %d m %d k %d\n", rtrnIndex, m, k);
        if(isHittingSet(rtrnSet,subSets,m,c))
            return rtrnSet;
        return NULL;
    }
    if(m<=0){
        printf("No more subsets: rtr %d m %d k %d\n", rtrnIndex, m, k);
        return NULL;
    }
    int validSubSets = m;
    unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"

    int random = (m>1)?(rand_r(&seed)%m):0;

    int *randomSet = swapArrays(subSets, random, validSubSets-1);           //swap the randomly selected SUB-SET with the lowest one.
    random = rand_r(&seed) % (length(randomSet, c));
    int validNumbers = length(randomSet, c);
    if(validNumbers<=0)
        return NULL; // No valid elements to choose from

    while(validNumbers>0){
        // Select a random element
        random = rand_r(&seed) % validNumbers;
        int randomElement = swapNumbers(randomSet, random, validNumbers - 1);
        validNumbers--;

        rtrnSet[rtrnIndex] = randomElement;                                                 //add the element to the hitting set.

        int *result = algorithm1Rec(n, clearArrays(subSets, randomElement, validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet);       // try to find the next element from what its left.

        if(result != NULL){
            return result; // Found a valid hitting set
        }

        //this means that the element didn't work
        rtrnSet[rtrnIndex] = 0; // Clear the element.
    }
    printf("faild\n");
    return NULL;
}

// Todo
// [ ]Implement Recursion.
// [x] Find critical Number.
int* algorithm2Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet){
    if(rtrnIndex>=k){
        printf("Re: rtr %d m %d k %d\n", rtrnIndex, m, k);
        if(isHittingSet(rtrnSet,subSets,m,c))
            return rtrnSet;
        return NULL;
    }
    if(m<=0){
        printf("No more subsets: rtr %d m %d k %d\n", rtrnIndex, m, k);
        return NULL;
    }
    int validSubSets = m;
    unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"

    int random = (m>1)?(rand_r(&seed)%m):0;

    int *randomSet = swapArrays(subSets, random, validSubSets-1);           //swap the randomly selected SUB-SET with the lowest one.
    disp(randomSet,c);
    printf("crit: %d----------------------\n",randomSet[criticalNumber(randomSet,subSets,validSubSets,c)]);

    return NULL;
}

/*
Debugging Method, Used to display the 2d array with the subsets.
*/
void printT(int **t, int ii, int jj){
    for(int i =0;i<ii;i++){
        for(int j=0;j<jj;j++)
            printf("%d ",t[i][j]);
        printf("\n");
    }
    return;
}
/*
Method used to calculate the length of the given array. Length: Then number of consequently non-zero numbers. For the array 1 2 3 0 5 6,
the returned number will be 3
*/
int length(int *arr, int size){
    int len = 0;
    for(int i =0;i<size;i++){
        if(arr[i]!=0) len++;                    // calculate the length of the array.
        else return len;
    }
    return len;
}
/*
This method, will check based on the subsets with the passed possible hitting set, is correct.
*/
bool isHittingSet(int *hittingSet, int **subsets, int m, int c) {
    int hittingSetLength = length(hittingSet,c);
    for (int j = 0; j < m; j++) {                           // For each subset
        bool intersects = false;
        for (int x = 0; x < c; x++) {                       // Check if the subset has any element from the hitting set
            for (int i = 0; i < hittingSetLength; i++) {
                if (hittingSet[i] == subsets[j][x] && hittingSet[i]!=0) {
                    intersects = true;
                    break;
                }
            }
            if (intersects) break;
        }
        if (!intersects) return false;                      // If this subset has no element from the hitting set, return false
    }
    return true;
}
// Given a 2D array, it will swap the bottom row with the current row (bottom row → lowest available row). It returns the array we just swapped(Randomly selected).
int* swapArrays(int **arr, int cur, int dest){
    int *randomSet = arr[cur];                                       // randomly selected array.
    arr[cur] = arr[dest];
    arr[dest] = randomSet;
    return randomSet;
}
// Same as above. Given a 1D array, it will swap the current number with the furthest one (furthest → rightmost available number). Returns the randomly selected number.
int swapNumbers(int *arr, int cur, int dest){
    int temp = arr[cur];
    arr[cur] = arr[dest];
    arr[dest] = temp;
    return temp;
}

// This will swap the arrays that include the 'target' number with the botton ones. They return the new available subsets, number.
int clearArrays(int **arr, int target, int validSubSets, int m, int c) {
    int validArrays = validSubSets;
    int i = 0;
    while(i<validArrays && i<m){
        bool found=false;
        for(int j=0;j<c;j++)
            if(arr[i][j]==target){
                found = true;
                break;
            }
        if(found){
            swapArrays(arr, i, validArrays - 1);    //Dont increment i because we need to check the new swapped array.
            validArrays--;
        }
        else
            i++;                                    //if we didn't find the target.
    }
    return validArrays;
}
// Used to debug, will output the content of the given array.
void disp(int *arr, int c){
    for(int i =0;i<c;i++) printf("%d ",arr[i] );
    printf("\n");
}

//Given a subSet will find and return the index of the most commond number to other subsets.
int criticalNumber(int *arr,int **subSets,int availableSubSets, int c){
    // Track count of each element in arr
    int *counts = (int*)malloc(length(arr,c)*sizeof(int));
    for(int i=0;i<length(arr,c);i++){
        counts[i] = 0;
        for (int j = 0; j < availableSubSets; j++) {
            for (int x = 0; x <length(subSets[j],c); x++) {
                if (subSets[j][x] == arr[i]) {
                    counts[i]++;
                    break;
                }
            }
        }
    }

    // Find element with highest count
    int bestIndex = 0;
    for (int i = 1; i < length(arr,c); i++) {
        if (counts[i] > counts[bestIndex]) {
            bestIndex = i;
        }
    }
    free(counts);
    return bestIndex; // Return the critical number, not just its index
}
