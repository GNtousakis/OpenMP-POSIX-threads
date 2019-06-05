#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

char * match;
char * mismatch;
char * gap;
int first_m;

int* last_hope;
int cell,traceback_cell;
double time1,time2,time3,time4,time5;
//Pthreads stuff
pthread_t* threads;
pthread_mutex_t mutex,mutex_2,mutex_3,mutex_4,mutex_5,mutex_cv,*mutex_line;
pthread_cond_t *cv,cv_first_time;
pthread_barrier_t mybarrier;

int i,j,max_table,i_1;
int run_threads;
int **table;
char *dline,*qline,*num_threads;
char *output1,*output2;
FILE *ptr,*foutput;

int col,line;

double gettime(void)
{
	struct timeval ttime;
	gettimeofday(&ttime, NULL);
	return ttime.tv_sec+ttime.tv_usec * 0.000001;
}

int checkflags(int argc,char const *argv[])
{
	int check,counter2;
	char  *flags[]={"-name","-input","-match","-mismatch","-gap","-threads"};
	
	for (int counter = 0; counter< 6; counter++)
	{
		check=0;
		counter2=1;
		while(check==0 && counter2<12){
			if ((strcmp(argv[counter2],flags[counter]))==0)
			{
				check=1;
			}
			counter2= counter2+2;	// the flags should be in the 0 2 4 6 8 possition 
		}
		if (check==0)
		{
			return 0;
		}
	}
	return 1;
}

char * findThings(int argc,char const *argv[],char *name)
{
	int check,counter;
	char  *returnvalue;
	

	check=0;
	counter=1;
	while (check==0)
	{

		if ((strcmp(argv[counter],name))==0) // We find the right spot of -input
		{
			returnvalue= malloc(sizeof(argv[counter+1])+5);
			strcpy(returnvalue,argv[counter+1]);
			check=1;
		}
		counter=counter+2;
	}
	return returnvalue;
}

char * get(FILE * fptr,char *string_next,int all_pair,int curr,char * string_to_compare)
{
	char nextChar;
	int counter;

	counter=0;
	nextChar=fgetc(fptr);
	fscanf(fptr,":  ");

	
	if (string_next[0]=='Q')
	{
		do
		{
			string_to_compare[counter]=fgetc(fptr);
			if (string_to_compare[counter]!=0 && string_to_compare[counter]!='\n' && string_to_compare[counter]!='\t')
			{
				counter++;
			}
		} while (string_to_compare[counter-1]!='D');
		string_to_compare[counter-1]='\n';
		fseek(fptr,-sizeof(char),SEEK_CUR);
	}else{
		if (all_pair==curr)
		{
			do
			{
				string_to_compare[counter]=fgetc(fptr);
				//printf("22the next string in string_to_compare is %c\n",string_to_compare[counter] );

				if (string_to_compare[counter]!=0 && string_to_compare[counter]!='\n' && string_to_compare[counter]!='\t')
				{
					counter++;
				}
				
			} while (string_to_compare[counter-1]!=EOF);
			string_to_compare[counter-1]='\0';

		//printf("the string is\n **********************\n %s\n ************************\n ",string_to_compare);
		}else{
			do
			{
				string_to_compare[counter]=fgetc(fptr);
				if (string_to_compare[counter]!=0 && string_to_compare[counter]!='\n' && string_to_compare[counter]!='\t')
				{
					counter++;
				}
			} while (string_to_compare[counter-1]!='Q');
			string_to_compare[counter-1]='\0';
			fseek(fptr,-sizeof(char),SEEK_CUR);
		//printf("the string is\n **********************\n %s\n ************************\n ",string_to_compare);
		}
	}

	return string_to_compare;
}

int max(int max1,int max2,int max3)
{
	if (max1>=max2)
	{
		if (max1>=max3)
		{
			return max1;
		}
		else
		{
			return max3;
		}
	}else
	{
		if (max2>max3)
		{
			return max2;
		}
		else
		{
			return max3;
		}
	}

}


void* fill_table_threads()
{
	int row_1;
		int value;
		int temp8;
		double time2 = gettime();
	pthread_mutex_lock(&mutex);
	while(i<=(line+1))	
   	{ 			
 		
   			row_1=i;
   			i++;  
   		//	printf("the %d \n",row_1 );

   			pthread_mutex_unlock(&mutex);
   	
	
			for(int temp8=1;temp8<=(col+1);++temp8)
     		{
     			int value1;
				int value2;
				int value3;
			
		
       			if(qline[row_1-1]==dline[temp8-1])//check if whe have match with subtrack one cause we look at the table qline
         	 	{ 
         	 		value1=0;
         	 		value2=0;
         	 		value3=0;
          			
         			value1=table[row_1-1][temp8-1]+ atoi(match);	// Atoi to change from char* to int
         		  	value2=table[row_1][temp8-1]- atoi(gap);
         		  	value3=table[row_1-1][temp8]- atoi(gap);	
    	  	     	value= max(value1,value2,value3);
    	   	    	
    	   	    	if (max_table<value)
    	   	    	{
    	   	    		pthread_mutex_lock(&mutex_2);
    	   	    		max_table=value;
    	   	    		pthread_mutex_unlock(&mutex_2);

    	  	     	}
          	 	}
       				else if(qline[row_1-1]!=dline[temp8-1])//check if whe have mismatch
       			{
          	 		value1=table[row_1-1][temp8-1]- atoi(mismatch); // Atoi to change from char* to int
           			value2=table[row_1][temp8-1] - atoi(gap);
       				value3=table[row_1-1][temp8] - atoi(gap);       
    	  	     	value= max(value1,value2,value3);
    	       
       			}
 	    			if(value<0) 
            {value=0;}
            else {cell++;}	// We cant have negative values

 	    			table[row_1][temp8]=value;
       				
       			   			 
     		}
     				pthread_mutex_lock(&mutex);
     				
   	}

   					pthread_mutex_unlock(&mutex);
   					double time3 = gettime();	
}		

void * write_table_threads()
{
				int row;
				double time4 = gettime();	
				pthread_mutex_lock(&mutex);
    	 	while(i<(line+1))
   		   {
   		  	 	row=i;
   				i++;
   				pthread_mutex_unlock(&mutex);

      		   for( int temp7=1;temp7<(col+1);++temp7)
      		     {
					char paula='-';	
					int value1,value2,value3,value;
					if (max_table==table[row][temp7])
					{	
						int tempi,tempj,ttempi,ttempj;	// Initialize temporary variables to print
						int len; 
						tempi=row-1;
						tempj=temp7-1;
						ttempj=temp7;
						ttempi=row;

						strcpy(output1,"D: ");
						strcpy(output2,"Q: ");

						len=strlen(output1);
						//ean exei vrei isotita tote apla pernaei to proto xaraktira poy einai idios sigoura	
						output1[len]=dline[tempj];
						output2[len]=qline[tempi];
				
						//We start to print the info
	
						while(table[ttempi][ttempj]!=0)
						{
						//pthread_mutex_lock(&mutex_2);
						value1=table[ttempi-1][ttempj];
						value2=table[ttempi][ttempj-1];
						value3=table[ttempi-1][ttempj-1];
						//pthread_mutex_unlock(&mutex_2);
						// We check witch of the values is the max
						if (value1>=value2)
						{
							if (value1>=value3)
							{
								len++;
								ttempi=ttempi-1;
								output1[len]=paula;
								output2[len]=qline[ttempi];
								traceback_cell++;
								
							}else
							{
								ttempi=ttempi-1;
								ttempj=ttempj-1;
								len++;
								output1[len]=dline[ttempj];
								output2[len]=qline[ttempi];
								traceback_cell++;
							}
								}else 
									{
								if (value2>=value3)
								{
										len++;
										ttempj=ttempj-1;
										output1[len]=dline[ttempj];
										output2[len]=paula;
										traceback_cell++;
									}
									else
									{

										len++;
										ttempi=ttempi-1;
										ttempj=ttempj-1;
										output1[len]=dline[ttempj];
										output2[len]=qline[ttempi];
										traceback_cell++;

									}
								}
							}
				len++;

				pthread_mutex_lock(&mutex_2);
				first_m++;
				fprintf(foutput, "Match %d[Score:%d,Start:%d,Stop:",first_m,table[row][temp7],tempj);
				fprintf(foutput, "%d]\n",ttempj);
				fprintf(foutput, "%s\n",output1);
				fprintf(foutput, "%s\n\n",output2);
				fscanf(ptr,"\n");
				pthread_mutex_unlock(&mutex_2);
			}

			}
			pthread_mutex_lock(&mutex);
		}
	pthread_mutex_unlock(&mutex);
	double time5 = gettime();	

}


int main(int argc, char const *argv[])
{

	// Declaration of variables
	int test;
	first_m=0;
	char *path,*name;
	// Declaration of variables of the file
	int pairs,Min_table,Max_table,All_pairs;
	double time0 = gettime();

	if (argc!=13)	// We check for the right amount of inputs
	{
		printf("Not the right amount of inputs\n");
		return 0;
	}

	test=checkflags(argc,argv);	
	if (test==0)	// We check with the result of checkflags that the user gave the right flags
	{
		printf("We didnt find the right inputs\n");
		return 0;
	}

	// If we passed the test that means we have the right flags 

	// Find the input path
	path=findThings(argc,argv,"-input");
	printf("The path to the input file is %s\n",path );
	
	// Find the name of the output file
	name=findThings(argc,argv,"-name");
	printf("The name to the output file is %s\n",name);

	// Find the name of the output file
	match=findThings(argc,argv,"-match");
	printf("The name to the output file is %s\n",match);

	// Find the name of the output file
	mismatch=findThings(argc,argv,"-mismatch");
	printf("The name to the output file is %s\n",mismatch);

	// Find the name of the output file
	gap=findThings(argc,argv,"-gap");
	printf("The name to the output file is %s\n",gap);

	//Find the threads that the sytem uses
	num_threads=findThings(argc,argv,"-threads");
	printf("The name to the output file is %s\n",num_threads);

	//We have all the basic info in our variables 

	//So we open the file
	ptr= fopen(path,"r");
	if(ptr == NULL)
  	{
      printf("No file with that name found!");   
      return 0;             
   	}

   	//Thread creation
   	threads=(pthread_t*) malloc(atoi(num_threads)*sizeof(pthread_t));
   
   	
   	//The number of the loops
	fscanf(ptr,"Pairs: %d\n", &pairs);
	fscanf(ptr,"Q_Sz_Min:	%d\n", &Min_table);
	fscanf(ptr,"Q_Sz_Max:	%d\n", &Max_table);
	fscanf(ptr,"D_Sz_All:	%d\n\n", &All_pairs);	// We reach the first Q
	printf("Pairs:%d\n Min_table:%d\n Max_table:%d\n All_pairs:%d\n",pairs,Min_table,Max_table,All_pairs);


	qline= malloc(All_pairs *sizeof(char));
	dline= malloc(All_pairs *sizeof(char));

	table=(int**)malloc((All_pairs+1) * sizeof(int*));//we increase the size of the original line of string by +1
	for(int temp1=0;temp1<(All_pairs+1);++temp1)
	{
		table[temp1]=(int *)malloc((All_pairs+1) * sizeof(int));//we increase the size of the original column of string by +1
	}
		
		//cv= (pthread_cond_t*) malloc((line+2)*sizeof(pthread_cond_t));
    	//mutex_line=(pthread_mutex_t*)malloc((line+2)*sizeof(pthread_mutex_t));
	for (int loop_control = 1; loop_control <= pairs; ++loop_control)
	{

          
		col=0,max_table=-10;
		qline=get(ptr,"Q",loop_control,pairs,qline);
		dline=get(ptr,"D",loop_control,pairs,dline);

		do
		{
			col++;
		} while (dline[col]!='\0');
		printf("clumn %d\n",col);

		line=0;
        do
		{
			line++;
		} while (qline[line]!='\0');
				printf("line %d\n",line);

         for(int temp2=0;temp2<=line;temp2++)
	    {
       	  for(int temp3=0;temp3<=col;temp3++)
      	  {
      		   	table[temp2][temp3]=0;
     	   }
    	}

    	i=1;
   		for (int counter_temp= 0; (counter_temp < atoi(num_threads)) && (counter_temp<(line+1)); ++counter_temp)
   		{
   			pthread_create(&threads[counter_temp],NULL,fill_table_threads,NULL);
   		}

   		for (int join_temp = 0; (join_temp < atoi(num_threads)) && (join_temp<(line+1)); join_temp++)
   		{
   			pthread_join(threads[join_temp],NULL);
   		}
		//printf("THE LIFE IS OVER\n");
  		foutput= fopen(name,"w");	// We will create the file, if the file exists it we will overwritten
		fprintf(foutput, "Q:  %s",qline );
		fprintf(foutput, "D:  %s\n",dline );
		output1=malloc(sizeof(char)*(line+2));
		output2=malloc(sizeof(char)*(col+2));
			
		i=1;
        for (int counter_temp= 0; (counter_temp < atoi(num_threads)) && (counter_temp<(line+1)); ++counter_temp)
   		{
   			int ret=pthread_create(&threads[counter_temp],NULL,write_table_threads,NULL);
   		}
   		for (int join_temp = 0; (join_temp < atoi(num_threads)) && (join_temp<(line+1)); ++join_temp)
   		{
   			pthread_join(threads[join_temp],NULL);
   		}

 		//printf("-----PROBLEM SOLVED222------\n");          		
		//printf("THE FIRST LOOP IS %d\n",loop_control );
		//printf("*************************************************************\n");
	}

	
		free(qline);
		free(dline);
		free(output1);
		//free(output2);
		fclose(foutput);
		double time1 = gettime();
		printf("\n The pairs are :%d",first_m);
		printf("\n The cells are :%d",cell);
		printf("\n The traceback_cells are :%d",traceback_cell);
		printf("\n The execution time of program :%lf\n",time1-time0);
		printf("\n The time of my cells:%lf\n",time3-time2);
		printf("\n The time of my traceback_cells :%lf\n",time5-time4);
		printf("\n The number of my cells:%lf\n",cell/(time1-time0));
		printf("\n The  cups of my life :%lf\n",(time1-time0)/cell);
		return 0;
}
