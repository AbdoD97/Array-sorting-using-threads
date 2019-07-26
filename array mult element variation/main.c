#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>                // for gettimeofday()



#define bufSize 1024
 double elapsedTime;
int *Mat1, *Mat2, *Mat3;
int M1RC,M1CC,M2RC, M2CC, M3RC, M3CC;
pthread_t *threads;
struct  Pairs
{
    int i;
    int j;
};
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void *MatrixMul(void *vargp)
{
//struct arg_struct args = (struct arg_struct  *)vargp;
    //struct Pairs *my_pair = (struct Pairs*)vargp;

//int i = args ->arg1;
//int j = args ->arg2;
 // struct arg_struct my_arg;
//memcpy(&my_arg, args, sizeof (*args));
struct Pairs *my_pair = (struct Pairs*)vargp;
    //printf("\nThread %ld, %ld", (*my_pair).i, (*my_pair).j);

//printf("first arg &d second arg %d" ,this_arg.arg1,this_arg.arg2);
int sum =0;



// printf("d assdaasdasd %d" , p);
    {
      /*for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++) {
        for (k = 0; k < p; k++) {*/
        {
            for (int k = 0; k < M1CC; k++)
            {
                //sum +=  Mat1[c][k]*Mat2[k][d];
                sum +=  (*(Mat1 + ( (*my_pair).i)*M1CC + k)) * (*(Mat2 + k*M2CC + ( (*my_pair).j)))  ;
            }
//printf("smu %d " , sum);
            //  multiply[c][d] = sum;
            (*(Mat3 + ( (*my_pair).i)*M3CC + ( (*my_pair).j))) = sum;
            //sum = 0;
        }
    }
//pthread_join(threads[12], NULL);
   pthread_exit(NULL);


}

void counter(int * array){
     FILE *fp;
     char output[]="output.txt";
     int n;

     fp=fopen(output,"w");
fprintf(fp,"%d\t %d \n" , M3RC , M3CC);
     for(int j=0; j<M3RC; j++) {
    for(int i=0; i<M3CC; i++)
           fprintf(fp,"%d\t",(*(Mat3 + ( j)*M3CC + ( i))) ); //fprintf(fp,"%d\n",array[j][i]);

           fprintf(fp,"\n" );
}
     /*for(n=0;n<10;n++) {
        fprintf(fp,"%d\n",array[n]);
     }*/

fprintf(fp,"Elapsed time %lf" , elapsedTime);
     fclose(fp);

}


int main()
{
    int dummyCounter = 0;

    struct  timeval t1, t2;



    // start timer
    gettimeofday(&t1, NULL);

    int fileFlag= 0;
    FILE* fp;
    char buf[bufSize];
    char** tokens;



    if ((fp = fopen("input.txt", "r")) == NULL)
    {
        perror("fopen source-file");
        return 1;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strlen(buf) - 1] = '\0';
        //printf("%s\n", buf);
        if(fileFlag==0)
        {
            tokens = str_split(buf, ' ');
            M1RC = atoi(tokens[0]);
            M1CC = atoi(tokens[1]);
            Mat1 = (int *)malloc(M1RC * M1CC * sizeof(int));

        }
        else if(fileFlag==M1RC+1)
        {
            tokens = str_split(buf, ' ');
            M2RC = atoi(tokens[0]);
            M2CC = atoi(tokens[1]);
            Mat2 = (int *)malloc(M2RC * M2CC * sizeof(int));
            Mat3 = (int *)malloc(M1RC * M2CC * sizeof(int));
            M3RC = M1RC;
            M3CC = M2CC;
        }
        else if (fileFlag < M1RC+1)
        {
            tokens = str_split(buf, ' ');

            /* tokens = str_split(buf, ' ');
             //  printf("%s \n" , tokens[0]);
             for(int i =0; i <M1CC; i++ )
             {
                 *(Mat1 + i*M1CC + dummyCounter) = atoi(tokens[i]);
                 printf("%s " , atoi(tokens[i]);
             }

             dummyCounter++;*/

            int i, j, count = 0;
            for (i = 0; i <  M1RC; i++)
            {
                for (j = 0; j < M1CC; j++)
                {
                    *(Mat1 + i*M1CC + j) = atoi(tokens[j]);
                    //   printf("%s " , tokens[j]);
                }
                printf("\n");
                if(i != M1RC-1)
                {
                    fileFlag++;
                    fgets(buf, sizeof(buf), fp);
                    tokens = str_split(buf, ' ');

                }

            }


        }
        else if (fileFlag > M1RC+1)
        {

            tokens = str_split(buf, ' ');

            /* tokens = str_split(buf, ' ');
             //  printf("%s \n" , tokens[0]);
             for(int i =0; i <M1CC; i++ )
             {
                 *(Mat1 + i*M1CC + dummyCounter) = atoi(tokens[i]);
                 printf("%s " , atoi(tokens[i]);
             }

             dummyCounter++;*/

            int i, j, count = 0;
            for (i = 0; i <  M2RC; i++)
            {
                for (j = 0; j < M2CC; j++)
                {
                    *(Mat2 + i*M2CC + j) = atoi(tokens[j]);
                    //   printf("%s " , tokens[j]);
                }
                printf("\n");
                if(i != M2RC-1)
                {
                    fileFlag++;
                    fgets(buf, sizeof(buf), fp);
                    tokens = str_split(buf, ' ');

                }

            }





        }

        fileFlag++;

    }
    fclose(fp);


    printf("These are dimensions of the arrays Matrix1 %dx%d Matrix2 %dx%d \n", M1RC,M1CC, M2RC,M2CC  ) ;
    if ( M1CC !=M2RC )
        printf("Cannot do the multiplication duo to problem in dimensions \n");



    printf("Mats 1 is \n");
    for (int i = 0; i < M1RC; i++)
    {
        for (int j = 0; j < M1CC; j++)
        {
            // grid[i][j] = '.';
            printf("%d\t", *(Mat1 + i*M1CC + j));
        }
        printf("\n");
    }
    printf("Mats 2 is \n");
    for (int i = 0; i < M2RC; i++)
    {
        for (int j = 0; j < M2CC; j++)
        {
            // grid[i][j] = '.';
            printf("%d\t", *(Mat2 + i*M2CC + j));
        }
        printf("\n");
    }
int thread_id =0;
    threads = malloc( M1RC* M2CC* sizeof(pthread_t) );
     /*for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j)
            for(k=0; k<c1; ++k)*/
struct Pairs *pair;


    for (int i = 0; i < M1RC; i++)
    {
    //printf("asdas asdas");
        for (int j = 0; j < M2CC; j++)
        {
            /*int* p ;
            p =i;*/
/*struct arg_struct *args;
    args->arg1 = i;
    args->arg2 = j;*/


       pair = malloc(sizeof(struct Pairs));
            (*pair).i = i;
            (*pair).j = j;

  //printf("TIF1 %d %d\n" , i , j);
          pthread_create(&threads[thread_id], NULL, MatrixMul, (void*) pair) ;

thread_id++;


        }

    }
    //printf("TIF %d \n" , thread_id);

       for (int i = 0; i < 12; i++)
         pthread_join(threads[i], NULL);


    printf("Mats 3 is \n");
    for (int i = 0; i < M3RC; i++)
    {
        for (int j = 0; j < M3CC; j++)
        {

            // grid[i][j] = '.';
            printf("%d\t", *(Mat3 + i*M3CC + j));
        }
        printf("\n");
    }
        gettimeofday(&t2, NULL);


   gettimeofday(&t2, NULL);

elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
 printf( "Elapsed time %lf ms" ,elapsedTime );
counter(Mat3);

    return 0;
}


