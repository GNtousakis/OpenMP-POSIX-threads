#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int cell,traceback_cell;
double time1,time2,time3,time4,time5;
double gettime(void)
{
	struct timeval ttime;
	gettimeofday(&ttime, NULL);
	return ttime.tv_sec+ttime.tv_usec * 0.000001;
}

int checkflags(int argc,char const *argv[])
{
	int check,counter2;
	char  *flags[]={"-name","-input","-match","-mismatch","-gap"};
	
	for (int counter = 0; counter< 5; counter++)
	{
		check=0;
		counter2=1;
		while(check==0 && counter2<10){
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

				if (string_to_compare[counter]!=0 && string_to_compare[counter]!='\n' && string_to_compare[counter]!='\t')
				{
					counter++;
				}
				
			} while (string_to_compare[counter-1]!=EOF);
			string_to_compare[counter-1]='\0';

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

int fill_table(int **table,int i,int j,char * qline,char* dline,char* match,char* mismatch,char* gap,int max_table)
{
	 	double time2 = gettime();
		int value1,value2,value3,value;
        if(qline[i-1]==dline[j-1])//check if whe have match with subtrack one cause we look at the table qline
          	{ 
          		value1=0;
          		value2=0;
          		value3=0;
          			
         		value1=table[i-1][j-1]+ atoi(match);	// Atoi to change from char* to int
         	  	value2=table[i][j-1]- atoi(gap);
         	  	value3=table[i-1][j]- atoi(gap);	
    	       	value= max(value1,value2,value3);	
    	       	if (max_table<value)
    	       	{
    	       		max_table=value;
    	       	}
    	       	
           	}
            else if(qline[i-1]!=dline[j-1])//check if whe have mismatch
           	{
           		//double time2 = gettime();
            	value1=table[i-1][j-1]- atoi(mismatch); // Atoi to change from char* to int
            	value2=table[i][j-1] - atoi(gap);
           		value3=table[i-1][j] - atoi(gap);
            	value= max(value1,value2,value3);
            	
           	}
            if(value<0) 
            {value=0;}
            else {cell++;}	// We cant have negative values
           	table[i][j]=value;
           	return max_table;
           	double time3 = gettime();	
}

int write_table(int first_m,int **table,int i,int j,char * qline,char* dline,int line,int col,int max_table,FILE *ptr,FILE *foutput)
{
	char *output1,*output2;
	char paula='-';	
	int value1,value2,value3,value;

	double time4 = gettime();	
	if (max_table==table[i][j])
		{	
			int tempi,tempj,ttempi,ttempj;	// Initialize temporary variables to print
			int len; 
				
			first_m++;
			tempi=i-1;
			tempj=j-1;
			ttempj=j;
			ttempi=i;

			output1=malloc(sizeof(char)*(line+2));
			output2=malloc(sizeof(char)*(col+2));

			strcpy(output1,"D: ");
			strcpy(output2,"Q: ");

			len=strlen(output1);
			//ean exei vrei isotita tote apla pernaei to proto xaraktira poy einai idios sigoura	
			output1[len]=dline[tempj];
			output2[len]=qline[tempi];

			
			//We start to print the info
			fprintf(foutput, "Match %d[Score:%d,Start:%d,Stop:",first_m,table[i][j],tempj);
			//printf("go die %dn",first_m);

			while(table[ttempi][ttempj]!=0)
			{
				value1=table[ttempi-1][ttempj];
				value2=table[ttempi][ttempj-1];
				value3=table[ttempi-1][ttempj-1];


				// We check witch of the values is the max
				if (value1>=value2)
					{
						if (value1>=value3)
						{
							len++;
							ttempi=ttempi-1;
							output1[len]=paula;
                         	traceback_cell++;
							output2[len]=qline[ttempi];

						}else
						{
							ttempi=ttempi-1;
							ttempj=ttempj-1;
							len++;
							traceback_cell++;
							output1[len]=dline[ttempj];
							output2[len]=qline[ttempi];

						}
					}else 
					{
						if (value2>=value3)
						{

							len++;
							ttempj=ttempj-1;
							traceback_cell++;
							output1[len]=dline[ttempj];
							output2[len]=paula;


						}
						else
						{

							len++;
							ttempi=ttempi-1;
							ttempj=ttempj-1;
							traceback_cell++;
							output1[len]=dline[ttempj];
							output2[len]=qline[ttempi];
														

						}
					}

				}
				len++;
				double time5 = gettime();
				fprintf(foutput, "%d]\n",ttempj);
				fprintf(foutput, "%s\n",output1);

				fprintf(foutput, "%s\n\n",output2);
				fscanf(ptr,"\n");
				free(output1);
				free(output2);
			}

			return first_m;
		}



int main(int argc, char const *argv[])
{
	// Declaration of variables
	int test,first_m=0;
	char *path,*name,*match,*mismatch,*gap;
	// Declaration of variables of the file
	FILE *ptr,*foutput;
	int pairs,Min_table,Max_table,All_pairs;
	char *qline,*dline;
	int **table;
	double time0 = gettime();


	if (argc!=11)	// We check for the right amount of inputs
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

	//We have all the basic info in our variables 

	//So we open the file
	ptr= fopen(path,"r");
	if(ptr == NULL)
  	{
      printf("No file with that name found!");   
      return 0;             
   	}

   	//The number of the loops
	fscanf(ptr,"Pairs: %d\n", &pairs);
	fscanf(ptr,"Q_Sz_Min:	%d\n", &Min_table);
	fscanf(ptr,"Q_Sz_Max:	%d\n", &Max_table);
	fscanf(ptr,"D_Sz_All:	%d\n\n", &All_pairs);	// We reach the first Q
	printf("Pairs:%d\n Min_table:%d\n Max_table:%d\n All_pairs:%d\n",pairs,Min_table,Max_table,All_pairs);


	qline= malloc(All_pairs *sizeof(char));
	dline= malloc(All_pairs *sizeof(char));

	table=(int**)malloc((All_pairs+1) * sizeof(int*));//we increase the size of the original line of string by +1
	for(int i=0;i<(All_pairs+1);++i)
	{
		table[i]=(int *)malloc((All_pairs+1) * sizeof(int));//we increase the size of the original column of string by +1
	}
		
	for (int loop_control = 1; loop_control <= pairs; ++loop_control)
	{

          
		int col=0,max_table=-10,i,j;
		qline=get(ptr,"Q",loop_control,pairs,qline);
		dline=get(ptr,"D",loop_control,pairs,dline);

		do
		{
			col++;
		} while (dline[col]!='\0');

		int line=0;
        do
		{
			line++;
		} while (qline[line]!='\0');
	
         for(int i=0;i<=line;i++)
	    {
       	  for(int j=0;j<=col;j++)
      	  {
      		   	table[i][j]=0;
     	   }
    	}

    int value=0;
    int value1,value2,value3;

   		for (i=1;i<(line+1);++i)
   		{
      		for(j=1;j<(col+1);++j)
     		{
     			int temp_max=fill_table(table,i,j,qline,dline,match,mismatch,gap,max_table);
     			max_table=temp_max;
     		}
   		}
  
// printf("-----PROBLEM SOLVED------\n");          		
  	foutput= fopen(name,"w");	// We will create the file, if the file exists it we will overwritten
	
	fprintf(foutput, "Q:  %s",qline );
	fprintf(foutput, "D:  %s\n",dline );
			

   		for (int i=1;i<=(line+1);++i)
   		{
      		for(int j=1;j<=(col+1);++j)
      		{
      			first_m=write_table(first_m,table,i,j,qline,dline,All_pairs,All_pairs,max_table,ptr,foutput);	//We return the first_m to increase each time we have a pair
			}
		}

 		//printf("-----PROBLEM SOLVED222------\n");          		
		printf("THE FIRST LOOP IS %d\n",loop_control );
		//printf("*************************************************************\n");
	}

	//free(table);
	free(qline);
	free(dline);
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
