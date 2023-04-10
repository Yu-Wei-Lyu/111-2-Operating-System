#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> // memset 
#include <pthread.h> // pthread_t, pthread_create, pthread_join 
#include <semaphore.h> // sem_init, sem_wait, sem_post 
#include <time.h> 

#define NUM_SEAT 3 
#define SLEEP_MAX 5 

//sematphores 
sem_t sem_stu; 
sem_t sem_ta; 

//sem_t mutex; 
pthread_mutex_t mutex; 

int chair[NUM_SEAT]; 
int waiting_count = 0; //number of waiting students 
int next_seat = 0; 
int next_teach = 0; 

void* stu_programming(void* stu_id); 
void* ta_teaching(); 
void rand_sleep(void);

int main(){ 
    pthread_t *students; 
    pthread_t ta; 
    int* student_ids; 
    int student_num; 
    int i; 
    srand(time(NULL)); 
    printf("有幾位學生? "); 
    scanf("%d", &student_num); 

    //initialize 
    students = (pthread_t*)malloc(sizeof(pthread_t) * student_num); 
    student_ids = (int*)malloc(sizeof(int) * student_num); 
    memset(student_ids, 0, student_num); 
    sem_init(&sem_stu, 0, 0); 
    sem_init(&sem_ta, 0, 1); 
    pthread_mutex_init(&mutex, NULL); 

    //create threads 
    pthread_create(&ta, NULL, ta_teaching, NULL); 
    for(i = 0; i < student_num; i++) { 
        student_ids[i] = i+1; 
        pthread_create(&students[i], NULL, stu_programming, (void*) &student_ids[i]); 
    }  
    pthread_join(ta, NULL); 
    for(i = 0; i < student_num; i++) 
        pthread_join(students[i], NULL);
    return 0; 
} 

void* stu_programming(void* stu_id) { 
    int id = *(int*)stu_id; 
    printf("[-] %d 號學生從TA辦公室離開\n", id);
    while(1) { 
        rand_sleep(); 
        pthread_mutex_lock(&mutex); 
        if (waiting_count < NUM_SEAT) { 
            chair[next_seat] = id; 
            waiting_count++; 
            printf("[+] %d 號學生坐在外面等候\n", id); 
            printf("等候席 : [1] %d [2] %d [3] %d\n", chair[0], chair[1], chair[2]); 
            next_seat = (next_seat + 1) % NUM_SEAT;
            pthread_mutex_unlock(&mutex); 
            sem_post(&sem_stu); 
            sem_wait(&sem_ta); 
        } else { 
            pthread_mutex_unlock(&mutex); 
            printf("[!] 座位已滿 : %d 號學生待會回來\n", id);	
        } 
    }
} 

void* ta_teaching() { 
    while(1) { 
        sem_wait(&sem_stu);	
        printf("[!] TA 被學生叫醒了\n"); 
        while(1) {
            pthread_mutex_lock(&mutex); 
            printf("等候席 : [1] %d [2] %d [3] %d\n",chair[0],chair[1],chair[2]);
            if (waiting_count == 0) {
                printf("[!] 沒有學生 所以 TA 趴下睡覺\n");
                pthread_mutex_unlock(&mutex); 
                sem_post(&sem_ta);
                break;
            }
            printf("[TA] 正在教導 %d 號學生\n",chair[next_teach]);
            chair[next_teach]=0; 
            waiting_count--; 
            next_teach = (next_teach + 1) % NUM_SEAT; 
            rand_sleep(); 
            printf("[TA] 結束教導\n");

            pthread_mutex_unlock(&mutex); 
            sem_post(&sem_ta); 

        }

    }
} 

void rand_sleep(void){ 
    int time = rand() % SLEEP_MAX + 1; 
    sleep(time); 
}