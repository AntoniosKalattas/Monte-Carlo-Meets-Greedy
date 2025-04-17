#include <limits.h>
#include <stdint.h>
#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include<unistd.h>


//////// FLAG ////////////
static int flag = 0;    //
//////////////////////////

// flag = 1 --> Algorithm Check.
// flag = 0 --> BenchMarking.

#define TRUE 1
#define FALSE 0
#define endThread1 700
#define endThread2 701
#define endThread3 702
#define endThread4 703


typedef struct{
    int n,m,c,k;
    int *set;
    int **subsets;
}Args;

//Thread Methods//////
void* alg1(void*);  //
void* alg2(void*);  //
void* alg3(void*);  //
void* alg4(void*);  //
//////////////////////


//Algorithm Implementation//////////////////////////////////////////////
int* algorithm1Rec(int , int , int , int , int **, int , int *, int, int **); //
int* algorithm2Rec(int , int , int , int , int **, int , int *, int); //
int* algorithm3Rec(int , int , int , int , int **, int , int *, int); //
int* algorithm4Rec(int , int , int , int , int **, int , int *, int); //
////////////////////////////////////////////////////////////////////////



//Helper Methods//////////////////////////////////
void printT(int **, int, int);                  //
int length(int *, int);                         //
int* swapArrays(int **,int,int);                //
int swapNumbers(int*,int,int);                  //
int clearArrays(int **, int, int,int, int);     //
void criticalNumber(int *,int,int **,int,int);  //
void disp(int *, int);                          //
int smallerSubSet(int **,int,int);              //
void stopThread(int);                           //
void sortSubSets(int **, int , int);            //
void mapCleaner(int **, int ,int, int, int**);   //
//////////////////////////////////////////////////


//BenchMarking Methods////////////////////////////
void generateInstance();                        //
int randomNumberGen(int, int);                  //
Args* readBenchMark1();                         //
void runArgs(Args*);                            //
void runArgsAlternative(Args *);                //
int benchMarkAlgo1(Args *);                     //
int benchMarkAlgo2(Args *);                     //
int benchMarkAlgo3(Args *);                     //
int benchMarkAlgo4(Args *);                     //
void freeArgs(Args* );                          //
Args* copyArgs(Args*);                          //
//////////////////////////////////////////////////

int main(){
    srand(time(NULL));
    FILE *fp;
    Args *args = (Args *)malloc(sizeof(Args));
    if(flag){
        fp = fopen("test.dat", "r");
        if(fp==NULL){
            perror("Faild to open file test.dat");
            return 0;
        }
        fscanf(fp, "%d %d %d %d",&args->n,&args->m,&args->c,&args->k);
        args->set = malloc(args->n*(sizeof(int)));
        args->subsets = malloc(args->m*sizeof(int*));
        for(int i =0;i<args->m;i++){
            args->subsets[i] = malloc(args->c*sizeof(int));
            for(int j=0;j<args->c;j++)
                args->subsets[i][j] = 0;
        }
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
        pthread_t t1, t2, t3, t4;
        Args *args1 = copyArgs(args);
        Args *args2 = copyArgs(args);
        Args *args3 = copyArgs(args);
        Args *args4 = copyArgs(args);
        pthread_create(&t1, NULL, alg1, (void *)args1);
        pthread_create(&t2, NULL, alg2, (void *)args2);
        pthread_create(&t3, NULL, alg3, (void *)args3);
        pthread_create(&t4, NULL, alg4, (void *)args4);
        int *set1, *set2, *set3, *set4;
        pthread_join(t1, (void **)&set1);
        pthread_join(t2, (void **)&set2);
        pthread_join(t3, (void **)&set3);
        pthread_join(t4, (void **)&set4);

        printf("A:{ ");for(int i=1;i<=args->n;i++) printf("%d ", i); printf("}");
        for(int i =0;i<args->m;i++){
            printf("\nB%d: ",i);
            for(int j=0;j<args->c;j++)
                printf("%d ",args->subsets[i][j]);
            printf("");
        }
        if(set1){
            printf("\nHitting Set from (1):\n");
            for(int i=0;i<args->k;i++){
                printf("%d ", set1[i]);
            }
            free(set1);
        } else printf("\n (1): failed");
        if(set2){
            printf("\nHitting Set from (2):\n");
            for(int i=0;i<args->k;i++){
                printf("%d ", set2[i]);
            }
            free(set2);
        } else printf("\n (2): failed");

        if(set3){
            printf("\nHitting Set from (3):\n");
            for(int i=0;i<args->k;i++){
                 printf("%d ", set3[i]);
            }
            free(set3);
        } else printf("\n (3): failed");

        if(set4){
            printf("\nHitting Set from (4):\n");
            for(int i=0;i<args->k;i++){
                printf("%d ", set4[i]);
            }
            free(set4);
        } else printf("\n (4): failed");

        if(flag){
            for(int i =0;i<args->m;i++)
                free(args->subsets[i]);
            free(args->subsets);
            free(args->set);
        }
        free(args);
    }
    else{
        free(args);
        args = readBenchMark1();
        runArgs(args);
        freeArgs(args);  // Add this line to free the args returned by readBenchMark1
        return 0;
    }
    fclose(fp);
}

void* alg1(void* args){
    Args *arg = (Args *)args;
    int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds
    int **cleaningArr = (int **)(malloc(arg->n*sizeof(int *)));
    for(int i =0;i<arg->n;i++){
        cleaningArr[i]=malloc(sizeof(int)*arg->m);
    }
    mapCleaner(arg->subsets,arg->n,arg->m,arg->c, cleaningArr);

    int *result = algorithm1Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet, endTime, cleaningArr);

    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    free(cpyArr);
    if(time(NULL)>=endTime){
        printf("(1) Timeout\n");
        free(rtrnSet);
        return NULL;
    }
    if(result==NULL)
        free(rtrnSet);
    return result;
}
void* alg2(void* args){
    Args *arg = (Args *)args;
    // int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    // for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    // for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds

    int *result = algorithm2Rec(arg->n, arg->m, arg->c, arg->k, arg->subsets, 0, rtrnSet, endTime);

    // for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    // free(cpyArr);
    if(time(NULL)>=endTime){
        printf("(2) Timeout\n");
        free(rtrnSet);
        return NULL;
    }
    if(result==NULL)
        free(rtrnSet);
    return result;
}
void* alg3(void* args){
    Args *arg = (Args *)args;
    // int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    // for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    // for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds

    sortSubSets(arg->subsets, arg->m, arg->c);
    // printT(cpyArr, arg->m, arg->c);

    int *result = algorithm3Rec(arg->n, arg->m, arg->c, arg->k, arg->subsets, 0, rtrnSet, endTime);

    // for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    // free(cpyArr);
    if(time(NULL)>=endTime){
        printf("(3) Timeout\n");
        free(rtrnSet);
        return NULL;
    }
    if(result==NULL)
        free(rtrnSet);
    return result;
}
void* alg4(void* args){
    Args *arg = (Args *)args;
    // int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    // for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    // for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds

    sortSubSets(arg->subsets, arg->m, arg->c);

    int *result = algorithm4Rec(arg->n, arg->m, arg->c, arg->k, arg->subsets, 0, rtrnSet, endTime);
    //for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    //free(cpyArr);
    if(time(NULL)>=endTime){
        printf("(4) Timeout\n");
        free(rtrnSet);
        return NULL;
    }
    if(result==NULL)
        free(rtrnSet);
    return result;
}

int* algorithm1Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime, int cleaner){
    if(m <= 0 && rtrnIndex<=k) {
        // printf("no elements left (1)\n");
        return rtrnSet;
    }
    if(rtrnIndex>=k)
        return NULL;

    time_t currentTime = time(NULL);
    if(currentTime>=endTime){return (int *)(-1);}


    int validSubSets = m;
    unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"

    int random = (m>1)?(rand_r(&seed)%m):0;

    int *randomSet = swapArrays(subSets, random, validSubSets-1);           //swap the randomly selected SUB-SET with the lowest one.
    random = rand_r(&seed) % (length(randomSet, c));
    int validNumbers = length(randomSet, c);
    if(validNumbers<=0)
        return NULL;                                                        // No valid elements to choose from

    while(validNumbers>0){
        random = rand_r(&seed) % validNumbers;                              // Select a random element
        int randomElement = swapNumbers(randomSet, random, validNumbers - 1);
        validNumbers--;

        rtrnSet[rtrnIndex] = randomElement;                                 //add the element to the hitting set.

        int *result = algorithm1Rec(n, , c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

        if(result != NULL){
            return result;                                                  // Found a valid hitting set
        }
        rtrnSet[rtrnIndex] = 0;                                             // Clear the element.
    }
    return NULL;
}

int* algorithm2Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m<=0 && rtrnIndex<=k)
        return rtrnSet;
    if(rtrnIndex>=k)
        return NULL;

    time_t currentTime = time(NULL);
    if(currentTime>=endTime){return (int *)(-1);}

    int validSubSets = m;
    unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();                             //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"

    int random = (m>1)?(rand_r(&seed)%m):0;

    int *randomSet = swapArrays(subSets, random, validSubSets-1);                               //swap the randomly selected SUB-SET with the lowest one.
    int validNumbers = length(randomSet, c);
    criticalNumber(randomSet, validNumbers, subSets, m, c);
    while(validNumbers>0){
        swapNumbers(randomSet, 0, validNumbers-1);              // swap the most critical elemen, make it invalid

        rtrnSet[rtrnIndex] = randomSet[0];                                                   //add the element to the hitting set.
        int *result = algorithm2Rec(n, clearArrays(subSets, randomSet[0], validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

        if(result!=NULL)
            return result;
        validNumbers--;                                                                         // decrese the valid number, loop to find the next critical number.
    }
    return NULL;
}

int* algorithm3Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m==0 && rtrnIndex<=k){
        return rtrnSet;
    }
    if(rtrnIndex>=k)
        return NULL;

    time_t currentTime = time(NULL);
    if(currentTime>=endTime){return (int *)(-1);}


    int validSubSets = m;
    int numberOfSetsTried = 0;

    while(numberOfSetsTried<m){
        unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"
        int random = (m>1)?(rand_r(&seed)%m):0;

        int *smallestSet = subSets[0];
        random = rand_r(&seed) % (length(smallestSet, c));

        int validNumbers = length(smallestSet, c);
        if(validNumbers<=0)
            return NULL;                                                        // No valid elements to choose from

        while(validNumbers>0){
            random = rand_r(&seed) % validNumbers;                              // Select a random element
            int randomElement = swapNumbers(smallestSet, random, validNumbers - 1);
            validNumbers--;

            rtrnSet[rtrnIndex] = randomElement;                                 //add the element to the hitting set.

            int *result = algorithm3Rec(n, clearArrays(subSets, randomElement, validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

            if(result != NULL){
                return result;                                                  // Found a valid hitting set
            }
            rtrnSet[rtrnIndex] = 0;                                             // Clear the element.
        }
         swapArrays(subSets, 0,validSubSets-1);
        numberOfSetsTried++;
    }
    return NULL;
}

int* algorithm4Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m<=0 && rtrnIndex<=k)
        return rtrnSet;
    if(rtrnIndex>=k)
        return NULL;

    time_t currentTime = time(NULL);
    if(currentTime>=endTime){return (int *)(-1);}

    int validSubSets = m;
    int numberOfSetsTried = 0;
    while(numberOfSetsTried<m){
        unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"
        int random = (m>1)?(rand_r(&seed)%m):0;

        int *smallestSet = subSets[0];
        random = rand_r(&seed) % (length(smallestSet, c));

        int validNumbers = length(smallestSet, c);
        if(validNumbers<=0)
            return NULL;                                                        // No valid elements to choose from
        criticalNumber(smallestSet, validNumbers, subSets, m, c);
        while(validNumbers>0){
            int criticalElement = swapNumbers(smallestSet, 0, validNumbers-1);              // swap the most critical elemen, make it invalid.

            rtrnSet[rtrnIndex] = criticalElement;                                                   //add the element to the hitting set.
            int *result = algorithm4Rec(n, clearArrays(subSets, criticalElement, validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

            if(result!=NULL)
                return result;
            validNumbers--;                                                                         // decrese the valid number, loop to find the next critical number.
            rtrnSet[rtrnIndex] = 0;
        }
        swapArrays(subSets, 0,validSubSets-1);
        numberOfSetsTried++;
    }
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
    for(int i=0;i<size;i++){
        if(arr[i]==0) break;
        len++;
    }
    return len;
}

// Given a 2D array, it will swap the bottom row with the current row (bottom row → lowest available row). It returns the array we just swapped(Randomly selected).
int* swapArrays(int **arr, int cur, int dest){
    int *rtrnSet = arr[cur];                                       // randomly selected array.
    arr[cur] = arr[dest];
    arr[dest] = rtrnSet;
    return rtrnSet;
}
// Same as above. Given a 1D array, it will swap the current number with the furthest one (furthest → rightmost available number). Returns the randomly selected number.
int swapNumbers(int *arr, int cur, int dest){
    int temp = arr[cur];
    arr[cur] = arr[dest];
    arr[dest] = temp;
    return temp;
}
void mapCleaner(int **arr, int n, int m, int c, int **cleaner){
    for(int i=1;i<n;i++){
        int j=0;
        for(int x=0;x<m;x++){
            for(int h=0;h<length(arr[x], c);h++){
                if(arr[x][h]==i){
                    cleaner[i][j]=x;
                    break;
                }
            }
            j++;
        }
    }
}

int clearArrays(int **arr, int target, int validSubSets, int m, int c) {
    // Early return if target is invalid or there are no valid subsets
    if (target == 0 || validSubSets <= 0) return validSubSets;

    int validArrays = validSubSets;
    int i = 0;

    while (i < validArrays && i < m) {
        bool found = false;
        // Only search until we find the target or reach end
        for (int j = 0; j < c && arr[i][j] != 0; j++) {
            if (arr[i][j] == target) {
                found = true;
                break;
            }
        }

        if (found) {
            // Only swap if not already at the position being swapped with
            if (i != validArrays - 1) {
                swapArrays(arr, i, validArrays - 1);
            }
            validArrays--;
            // Don't increment i since we need to check the newly swapped array
        } else {
            i++; // Move to next array if target not found
        }
    }

    return validArrays;
}
// Used to debug, will output the content of the given array.
void disp(int *arr, int c){
    for(int i=0;i<c;i++) printf("%d ",arr[i]);
    printf("\n");
}
//Given a subSet will find and return the index of the most commond number to other subsets.
void criticalNumber(int *arr, int lengthOfarr,int **subSets,int availableSubSets,int c){
    // Track count of each element in arr.
    int *counts = (int*)malloc(length(arr,c)*sizeof(int));
    for(int i=0;i<length(arr,c);i++){
        counts[i] = 0;
        for(int j=0;j<availableSubSets;j++){
            for(int x=0;x<length(subSets[j],c);x++){
                if(subSets[j][x]==arr[i]){
                    counts[i]++;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < lengthOfarr - 1; i++) {
            for (int j = 0; j < lengthOfarr - i - 1; j++) {
                if (counts[j] < counts[j + 1]) {
                    // Swap arr values.
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    // Swap corresponding counts.
                    int temp2 = counts[j];
                    counts[j] = counts[j + 1];
                    counts[j + 1] = temp2;
                }
            }
        }
    free(counts);
}
// This method, based on the given available to choose subsets will return the index of the smallest one.
int smallerSubSet(int **subSets, int availableSubSets, int c){
    int smallestIndex=0;
    int smallestLength = INT_MAX;
    for(int i=0;i<availableSubSets;i++){
        if(length(subSets[i], c)<smallestLength){
            smallestIndex = i;
            smallestLength = length(subSets[i],c);
        }
    }
    return smallestIndex;
}

void sortSubSets(int **subSets, int m, int c) {
    for(int i=0;i<m-1;i++){
        for(int j=0;j<m-i-1;j++){
            if(length(subSets[j],c)>length(subSets[j+1],c)){
                int *temp = subSets[j];
                subSets[j] = subSets[j+1];
                subSets[j+1] = temp;
            }
        }
    }
}

Args* readBenchMark1(){

    FILE *fp;
    Args *args = (Args *)malloc(sizeof(Args));
    fp = fopen("test1.dat", "r");
    if(fp==NULL){
        perror("Faild to open file test1.dat");
        return 0;
    }
    fscanf(fp, "%d %d %d",&args->n,&args->m,&args->c);
    args->set = malloc(args->n*(sizeof(int)));
    args->subsets = malloc(args->m*sizeof(int*));
    for(int i=0;i<args->m;i++){
        args->subsets[i] = malloc(args->c*sizeof(int));
        for(int j=0;j<args->c;j++)
            args->subsets[i][j]=0;
    }
    int i=0,j=0;
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
    fclose(fp);
    return args;
}

void runArgs(Args* args){
    pthread_t runAlg1, runAlg2, runAlg3, runAlg4;
    Args *args1 = copyArgs(args);
    Args *args2 = copyArgs(args);
    Args *args3 = copyArgs(args);
    Args *args4 = copyArgs(args);

    pthread_create(&runAlg1, NULL, (void *)benchMarkAlgo1, (void *)args1);
    pthread_create(&runAlg2, NULL, (void *)benchMarkAlgo2, (void *)args2);
    pthread_create(&runAlg3, NULL, (void *)benchMarkAlgo3, (void *)args3);
    pthread_create(&runAlg4, NULL, (void *)benchMarkAlgo4, (void *)args4);
    int avgTime=0;
    pthread_join(runAlg4, (void *)&avgTime);printf("avg time for algo 4: %d\n", (int)avgTime);
    pthread_join(runAlg3, (void *)&avgTime);printf("avg time for algo 3: %d\n", (int)avgTime);
    pthread_join(runAlg2, (void *)&avgTime);printf("avg time for algo 2: %d\n", (int)avgTime);
    pthread_join(runAlg1, (void *)&avgTime);printf("avg time for algo 1: %d\n", (int)avgTime);
}

int benchMarkAlgo1(Args *args){
    Args *arg = (Args *)args;
    time_t startTime = time(NULL);
    int *set1;
    int timePerK[3] = {0};
    int hit=0;
    int k=13;
    while(hit<3){
        args->k = k;
        for(int j=0;j<3;j++){
            pthread_t t1;
            pthread_create(&t1, NULL, alg1, (void *)args);
            pthread_join(t1, (void **)&set1);
            if(set1){
                timePerK[hit]=(int)(time(NULL)-startTime);
                startTime = time(NULL);
                hit++;
                if(hit==3)
                    break;
                printf("\nHitting Set from (1):\n");
                for(int i=0;i<args->k;i++){
                    printf("%d ", set1[i]);
                }
                printf("alg1 %d \n", k);
                free(set1);
            }
            else printf("(1) failed k:%d\n", k);
        }
        k++;
        if(hit==3)
            break;
    }

    freeArgs(args);
    return(timePerK[0]+timePerK[1]+timePerK[2])/3;
}
int benchMarkAlgo2(Args *args){
    Args *arg = (Args *)args;
    time_t startTime = time(NULL);
    int *set1;
    int timePerK[3] = {0};
    int hit=0;
    int k=13;
    while(hit<3){
        args->k = k;
        for(int j=0;j<3;j++){
            pthread_t t1;
            pthread_create(&t1, NULL, alg2, (void *)args);
            pthread_join(t1, (void **)&set1);
            if(set1){
                timePerK[hit]=(int)(time(NULL)-startTime);
                startTime = time(NULL);
                hit++;
                printf("\nHitting Set from (2):\n");
                for(int i=0;i<args->k;i++){
                    printf("%d ", set1[i]);
                }
                printf("alg2 %d \n", k);
                free(set1);
            }
            else printf("(2) failed k:%d\n", k);
        }
        k++;
    }
    freeArgs(args);
    return(timePerK[0]+timePerK[1]+timePerK[2])/3;
}
int benchMarkAlgo3(Args *args){
    Args *arg = (Args *)args;
    time_t startTime = time(NULL);
    int *set1;
    int timePerK[3] = {0};
    int hit=0;
    int k=13;
    while(hit<3){
        args->k = k;
        for(int j=0;j<3;j++){
            pthread_t t1;
            pthread_create(&t1, NULL, alg3, (void *)args);
            pthread_join(t1, (void **)&set1);
            if(set1){
                timePerK[hit]=(int)(time(NULL)-startTime);
                startTime = time(NULL);
                hit++;
                printf("\nHitting Set from (3):\n");
                for(int i=0;i<args->k;i++){
                    printf("%d ", set1[i]);
                }
                printf("alg3 %d \n", k);
                free(set1);
            }
            else printf("(3) failed k: %d\n", k);
        }
        k++;
    }

    freeArgs(args);
    return(timePerK[0]+timePerK[1]+timePerK[2])/3;
}
int benchMarkAlgo4(Args *args){
    Args *arg = (Args *)args;
    time_t startTime = time(NULL);
    int *set1;
    int timePerK[3] = {0};
    int hit=0;
    int k=13;
    while(hit<3){
        args->k = k;
        for(int j=0;j<3;j++){
            pthread_t t1;
                pthread_create(&t1, NULL, alg4, (void *)args);
            pthread_join(t1, (void **)&set1);
            if(set1){
                timePerK[hit]=(int)(time(NULL)-startTime);
                startTime = time(NULL);
                hit++;
                printf("\nHitting Set from (4) k:%d:\n",k);
                for(int i=0;i<args->k;i++){
                    printf("%d ", set1[i]);
                }
                printf("alg4 %d \n", k);
                free(set1);
            }
            else printf("(4) failed k:%d\n", k);
        }
        k++;
    }
    freeArgs(args);
    // printf("returning %d\n",(timePerK[0]+timePerK[1]+timePerK[2])/3);
    return(timePerK[0]+timePerK[1]+timePerK[2])/3;
}

Args* copyArgs(Args* original) {
    Args* copy = (Args*)malloc(sizeof(Args));
    copy->n = original->n;
    copy->m = original->m;
    copy->c = original->c;
    copy->k = original->k;

    copy->set = malloc(copy->n * sizeof(int));
    memcpy(copy->set, original->set, copy->n * sizeof(int));

    copy->subsets = malloc(copy->m * sizeof(int*));
    for(int i = 0; i < copy->m; i++) {
        copy->subsets[i] = malloc(copy->c * sizeof(int));
        memcpy(copy->subsets[i], original->subsets[i], copy->c * sizeof(int));
    }
    return copy;
}

void freeArgs(Args* args) {
    for(int i = 0; i < args->m; i++)
        free(args->subsets[i]);
    free(args->subsets);
    free(args->set);
    free(args);
}
