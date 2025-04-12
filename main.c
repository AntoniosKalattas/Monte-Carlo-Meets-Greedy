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
int* algorithm1Rec(int , int , int , int , int **, int , int *, int); //
int* algorithm2Rec(int , int , int , int , int **, int , int *, int); //
int* algorithm3Rec(int , int , int , int , int **, int , int *, int); //
int* algorithm4Rec(int , int , int , int , int **, int , int *, int); //
////////////////////////////////////////////////////////////////////////



//Helper Methods//////////////////////////////////
bool isHittingSet(int *,int, int **, int, int); //
void printT(int **, int, int);                  //
int length(int *, int);                         //
int* swapArrays(int **,int,int);                //
int swapNumbers(int*,int,int);                  //
int clearArrays(int **, int, int,int, int);     //
int criticalNumber(int *,int,int **,int,int);   //
void disp(int *, int);                          //
int smallerSubSet(int **,int,int);              //
void stopThread(int);                           //
//////////////////////////////////////////////////

//BenchMarking Methods///////////////////////////
void generateInstance();
int randomNumberGen(int, int);
Args* readBenchMark1();
void runArgs(Args*);


int main(){
    srand(time(NULL));
    FILE *fp;
    Args *args = (Args *)malloc(sizeof(Args));
    if(flag){
        fp = fopen("test.dat", "r");
        if(fp==NULL){
            perror("Faild to open file sets.dat");
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
    }
    else{
        //create random data.
        // generateInstance();
        free(args);
        args = readBenchMark1();
        runArgs(args);
        return 0;
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
    fclose(fp);
    free(args);
}

void stopThread(int signum){
    if(signum==endThread1)
        printf("Ending thread 1\n");
    if(signum==endThread2)
        printf("Ending thread 2\n");
    if(signum==endThread3)
        printf("Ending thread 3\n");
    if(signum==endThread4)
        printf("Ending thread 4\n");
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

    int *result = algorithm1Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet, endTime);

    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    free(cpyArr);
    free(rtrnSet);
    if(time(NULL)>=endTime)
        printf("(3) Timeout\n");
    return result;
}
void* alg2(void* args){
    Args *arg = (Args *)args;
    int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds

    int *result = algorithm1Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet, endTime);

    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    free(cpyArr);
    free(rtrnSet);
    if(time(NULL)>=endTime)
        printf("(3) Timeout\n");
    return result;
}
void* alg3(void* args){
    Args *arg = (Args *)args;
    int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds

    int *result = algorithm1Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet, endTime);

    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    free(cpyArr);
    free(rtrnSet);
    if(time(NULL)>=endTime)
        printf("(3) Timeout\n");
    return result;
}
void* alg4(void* args){
    Args *arg = (Args *)args;
    int **cpyArr = malloc(arg->m*sizeof(int*));                                                 //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)cpyArr[i] = malloc(arg->c*sizeof(int));                           //allocate space.
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}     //copy each element.

    int *rtrnSet = (int *)malloc(arg->k*sizeof(int));                                           //the returned hitting set.
    for(int i =0;i<arg->k;i++)rtrnSet[i] = 0;                                                   //set all to 0.

    time_t startTime = time(NULL);
    time_t endTime = startTime + 3600; // 1 hour = 3600 seconds

    int *result = algorithm1Rec(arg->n, arg->m, arg->c, arg->k, cpyArr, 0, rtrnSet, endTime);
    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                                  //free the allocated space.
    free(cpyArr);
    free(rtrnSet);
    if(time(NULL)>=endTime)
        printf("(3) Timeout\n");
    return result;
}

int* algorithm1Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m <= 0 && rtrnIndex<k) {
        printf("no elements left1\n");
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

        int *result = algorithm1Rec(n, clearArrays(subSets, randomElement, validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

        if(result != NULL){
            return result;                                                  // Found a valid hitting set
        }
        rtrnSet[rtrnIndex] = 0;                                             // Clear the element.
    }
    return NULL;
}

int* algorithm2Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m<=0 && rtrnIndex<k)
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
    while(validNumbers>0){
        int criticalIndex = criticalNumber(randomSet,validNumbers,subSets,validSubSets,c);      // find the critical number in the randomly selected subset.

        int criticalElement = swapNumbers(randomSet, criticalIndex, validNumbers-1);              // swap the most critical elemen, make it invalid.

        rtrnSet[rtrnIndex] = criticalElement;                                                   //add the element to the hitting set.
        int *result = algorithm2Rec(n, clearArrays(subSets, criticalElement, validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

        if(result!=NULL)
            return result;
        validNumbers--;                                                                         // decrese the valid number, loop to find the next critical number.
    }
    return NULL;
}

int* algorithm3Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m==0 && rtrnIndex<k){
        return rtrnSet;
    }
    if(rtrnIndex>=k)
        return NULL;

    time_t currentTime = time(NULL);
    if(currentTime>=endTime){return (int *)(-1);}


    int validSubSets = m;
    int numberOfSetsTried = validSubSets;
    while(numberOfSetsTried>0){
        unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"
        int random = (m>1)?(rand_r(&seed)%m):0;

        int smallestSetIndex = smallerSubSet(subSets, validSubSets, c);         // find and choose the smallest in length subset.
        int *smallestSet = subSets[smallestSetIndex];
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
        swapArrays(subSets, smallestSetIndex,validSubSets-1);
        numberOfSetsTried--;
    }
    return NULL;
}

int* algorithm4Rec(int n, int m, int c, int k, int **subSets, int rtrnIndex, int *rtrnSet, int endTime){
    if(m<=0 && rtrnIndex<k)
        return rtrnSet;
    if(rtrnIndex>=k)
        return NULL;

    time_t currentTime = time(NULL);
    if(currentTime>=endTime){return (int *)(-1);}

    int validSubSets = m;
    int numberOfSetsTried = validSubSets;
    while(numberOfSetsTried>0){
        unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();         //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"
        int random = (m>1)?(rand_r(&seed)%m):0;

        int smallestSetIndex = smallerSubSet(subSets, validSubSets, c);         // find and choose the smallest in length subset.
        int *smallestSet = subSets[smallestSetIndex];
        random = rand_r(&seed) % (length(smallestSet, c));

        int validNumbers = length(smallestSet, c);
        if(validNumbers<=0)
            return NULL;                                                        // No valid elements to choose from

        while(validNumbers>0){
            int criticalIndex = criticalNumber(smallestSet,validNumbers,subSets,validSubSets,c);      // find the critical number in the randomly selected subset.

            int criticalElement = swapNumbers(smallestSet, criticalIndex, validNumbers-1);              // swap the most critical elemen, make it invalid.

            rtrnSet[rtrnIndex] = criticalElement;                                                   //add the element to the hitting set.
            int *result = algorithm4Rec(n, clearArrays(subSets, criticalElement, validSubSets,m, c), c, k, subSets, rtrnIndex + 1, rtrnSet, endTime);       // try to find the next element from what its left.

            if(result!=NULL)
                return result;
            validNumbers--;                                                                         // decrese the valid number, loop to find the next critical number.
            rtrnSet[rtrnIndex] = 0;
        }
        swapArrays(subSets, smallestSetIndex,validSubSets-1);
        numberOfSetsTried--;
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
/*
This method, will check based on the subsets with the passed possible hitting set, is correct.
*/
bool isHittingSet(int *hittingSet, int kSize, int **subsets, int m, int c) {
    int hittingSetLength = length(hittingSet,kSize);
    for(int j=0;j<m;j++){                               // For each subset
        bool intersects = false;
        for(int x=0;x<c;x++){                           // Check if the subset has any element from the hitting set
            for(int i=0;i<=kSize;i++){
                if(hittingSet[i]==subsets[j][x] && hittingSet[i]!=0){
                    intersects = true;
                    break;
                }
            }
            if(intersects) break;
        }
        if(!intersects) return false;                   // If this subset has no element from the hitting set, return false
    }
    return true;
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
    for(int i=0;i<c;i++) printf("%d ",arr[i]);
    printf("\n");
}
//Given a subSet will find and return the index of the most commond number to other subsets.
int criticalNumber(int *arr, int lengthOfarr,int **subSets,int availableSubSets,int c){
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

    // Find element with highest count
    int bestIndex = 0;
    for(int i=1; i < length(arr,c); i++)
        if(counts[i] > counts[bestIndex])
            bestIndex=i;
    free(counts);
    return bestIndex; // Return the critical number, not just its index
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


void generateInstance(){
    FILE *fp = fopen("test1.dat", "w");
    srand(time(NULL));
    int n = rand()%20000;
    int m = n/0.05;
    int c = 15;
    fprintf(fp,"%d %d %d\n", n,m,c);
    for(int j=0;j<m;j++){
        for(int i=0;i<c;i++)
            fprintf(fp,"%d ", randomNumberGen(1,  20000));
        fprintf(fp,"\n" );
    }

}
int randomNumberGen(int min, int max){
    return min+rand()%(max-min+1);
}

Args* readBenchMark1(){
    FILE *fp;
    Args *args = (Args *)malloc(sizeof(Args));
    fp = fopen("test1.dat", "r");
    if(fp==NULL){
        perror("Faild to open file sets.dat");
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
    int *set1, *set2, *set3, *set4;
    // printf("args n:%d m:%d c:%d k:%d\n", args->n, args->m, args->c, args->k);
    int rtrnK[4] = {0};
    for(int k =1;k<INT_MAX;k++){
        printf("\n \ttrying k=%d\n", k);
        if(rtrnK[0]!=0 && rtrnK[1]!=0 && rtrnK[2]!=0 && rtrnK[3]!=0){
            printf("All done\n");
            break;
        }
        args->k = k;
        for(int j=0;j<3;j++){
            pthread_t t1, t2, t3, t4;
            if(rtrnK[0]==0)
                pthread_create(&t1, NULL, alg1, (void *)args);
            if(rtrnK[1]==0)
                pthread_create(&t2, NULL, alg2, (void *)args);
            if(rtrnK[2]==0)
                pthread_create(&t3, NULL, alg3, (void *)args);
            if(rtrnK[3]==0)
                pthread_create(&t4, NULL, alg4, (void *)args);
            if(rtrnK[3]==0)
                pthread_join(t4, (void **)&set4);
            if(rtrnK[2]==0)
                pthread_join(t3, (void **)&set3);
            if(rtrnK[1]==0)
                pthread_join(t2, (void **)&set2);
            if(rtrnK[0]==0)
                pthread_join(t1, (void **)&set1);

            if(set1){
                printf("\nHitting Set from (1):\n");
                for(int i=0;i<args->k;i++){
                    printf("%d ", set1[i]);
                    rtrnK[0] = k;
                }
                printf("alg1 %d \n", k);

            }
            else printf("(1) failed\n");
            if(set2){
                printf("\n*****Hitting Set from (2):\n");
                for(int i=0;i<args->k;i++){
                    printf("%d ", set2[i]);
                    rtrnK[1] = k;
                }
                printf("alg2 %d \n", k);
            }
            else printf("(2)faild\n");
            if(set3){
                printf("\nHitting Set from (3):\n");
                for(int i=0;i<args->k;i++){
                     printf("%d ", set3[i]);
                     rtrnK[2] = k;
                }
                printf("alg3 %d \n", k);
            }
            else printf("(3) failed\n");
            if(set4){
                printf("\nHitting Set from (4):\n");
                for(int i=0;i<args->k;i++){
                    printf("%d ", set4[i]);
                    rtrnK[3] = k;
                }
                printf("alg4 %d \n", k);
            }
            else printf("(4) failed\n");
        }
    }
    printf("\t k1:%d \t k2:%d \t k3:%d \t k4:%d \n",rtrnK[0], rtrnK[1],rtrnK[2], rtrnK[3]);
    for(int i =0;i<args->m;i++)
        free(args->subsets[i]);
    free(args->subsets);
    free(args->set);
    free(args);
}
