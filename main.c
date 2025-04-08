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

void* alg1(void* args);
void* alg2(void* args);
void* alg3(void* args);
void* alg4(void* args);

int *algorithm1(int ,int ,int ,int , int **);

void printT(int **, int, int);
int length(int *, int);

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
        // printf("%d", args->m);
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
    int **cpyArr = malloc(arg->m*sizeof(int*));                                     //copy the array since we are going to be making changes to it.
    for(int i =0;i<arg->m;i++)                                                      // allocate space.
        cpyArr[i] = malloc(arg->c*sizeof(int));
    for(int i=0;i<arg->m;i++){for(int j=0;j<arg->c;j++) cpyArr[i][j] = arg->subsets[i][j];}    // copy each element.
    int *rtrnTbl = algorithm1(arg->n, arg->m, arg->c, arg->k, cpyArr);
    for(int i=0;i<arg->m;i++) free(cpyArr[i]);                                      // free the allocated space.

    free(cpyArr);
    return rtrnTbl;
}

// TODO
// [] make it recursive.
// [] we never added an element in rtrnSet.


int *algorithm1(int n,int m,int c,int k, int **arr){
    int rtrnIndex=0;
    int *rtrnSet = (int *)malloc(k*sizeof(int));                                                     // the returned hitting set.
    for(int i =0;i<k;i++)rtrnSet[i] = 0;                                // set all to 0.
    unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();     //os based. If linux try "unsigned int seed = time(NULL) ^ pthread_self();"
    int numberOfValidSubSets = m;
    // here we will randomly choose a subset out of the availle substes, SWAP it and put it to the bottom of the array with the available sub sets.
    // then we will decrease the numberOfValidSubsets so we will then choose only from the valid ones.
    while(numberOfValidSubSets>1){
        int random = rand_r(&seed) % (numberOfValidSubSets-1);
        // printf("random is %d\n", random);
        // printf("----Beforer----\n");
        // printT(cpyArr, arg->m, arg->c);
        int *randomSet = arr[random];
        arr[random] = arr[numberOfValidSubSets-1];
        arr[numberOfValidSubSets-1] = randomSet;
        // printf("----AFTER----\n");
        // printT(cpyArr, arg->m, arg->c);
        numberOfValidSubSets--;
        // now we will find the random element that we will select.
        random = rand_r(&seed) % (length(randomSet, c));
        int randomElement = randomSet[random];

        // for each valid sub set left we will check each one and if they contain the same random element we will remove it.
        int i = 0;
        while (i < numberOfValidSubSets) {
            bool found = false;
            for (int j = 0; j < c; j++) {
                if (arr[i][j] == randomElement) {
                    found = true;
                    break;
                }
            }
            if (found) {
                int *temp = arr[i];
                arr[i] = arr[numberOfValidSubSets - 1];
                arr[numberOfValidSubSets - 1] = temp;
                numberOfValidSubSets--;
            } else {
                i++;
            }
        }

    }
    return rtrnSet;
}


void printT(int **t, int ii, int jj){
    for(int i =0;i<ii;i++){
        for(int j=0;j<jj;j++)
            printf("%d ",t[i][j]);
        printf("\n");
    }
    return;
}
int length(int *arr, int size){
    int len = 0;
    for(int i =0;i<size;i++){
        if(arr[i]!=0) len++;                    // calculate the length of the array.
        else return len;
    }
    return len;
}






void* alg2(void* args){
    return NULL;
}
void* alg3(void* args){
    return NULL;
}
void* alg4(void* args){
    return NULL;
}
