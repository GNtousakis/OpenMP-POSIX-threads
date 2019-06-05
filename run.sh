gcc ./lab1/main.c -o ./lab1/ektelesimo
gcc -fopenmp ./lab1_omp/main.c -o ./lab1_omp/ektelesimo
gcc -pthread ./lab1_pthreads/main.c -o ./lab1_pthreads/ektelesimo

sleep_time=3

echo -e "\n\n"
echo -e "---------------------- We will run the 4 first dataset -------------------------"
echo -e "------------------------- to demonstrate that everything works ---------------------------"

echo -e "\n\n"
echo -e "---------------------- We ran the first dataset -------------------------"
echo -e "------------------------- for all the 3 cases ---------------------------"
echo -e "-----------------------------------------------------------------------\n"
./lab1/ektelesimo -name ./lab1/Report_ID.txt -input ./data/D1.txt -match 3 -mismatch 1 -gap 1  
./lab1_omp/ektelesimo -name ./lab1_omp/Report_ID_OMP_INT4.txt -input ./data/D1.txt -match 3 -mismatch 1 -gap 1 -threads 5 
./lab1_pthreads/ektelesimo -name ./lab1_pthreads/Report_ID_PTH_INT4.txt -input ./data/D1.txt -match 3 -mismatch 1 -gap 1 -threads 5 
sleep ${sleep_time}


echo -e "\n\n"
echo -e "---------------------- We ran the second dataset -------------------------"
echo -e "------------------------- for all the 3 cases ---------------------------"
echo -e "-----------------------------------------------------------------------\n"
./lab1/ektelesimo -name ./lab1/Report_ID.txt -input ./data/D2.txt -match 3 -mismatch 1 -gap 1  
./lab1_omp/ektelesimo -name ./lab1_omp/Report_ID_OMP_INT4.txt -input ./data/D2.txt -match 3 -mismatch 1 -gap 1 -threads 5 
./lab1_pthreads/ektelesimo -name ./lab1_pthreads/Report_ID_PTH_INT4.txt -input ./data/D2.txt -match 3 -mismatch 1 -gap 1 -threads 5 
sleep ${sleep_time}


echo -e "\n\n"
echo -e "---------------------- We ran the third dataset -------------------------"
echo -e "------------------------- for all the 3 cases ---------------------------"
echo -e "-----------------------------------------------------------------------\n"
./lab1/ektelesimo -name ./lab1/Report_ID.txt -input ./data/D3.txt -match 3 -mismatch 1 -gap 1  
./lab1_omp/ektelesimo -name ./lab1_omp/Report_ID_OMP_INT4.txt -input ./data/D3.txt -match 3 -mismatch 1 -gap 1 -threads 5 
./lab1_pthreads/ektelesimo -name ./lab1_pthreads/Report_ID_PTH_INT4.txt -input ./data/D3.txt -match 3 -mismatch 1 -gap 1 -threads 5
sleep ${sleep_time}



