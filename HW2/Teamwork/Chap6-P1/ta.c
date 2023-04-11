#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h> 

#define NUM_SEAT 3 
#define WAIT_MAX_SEC 5 

// 定義 struct 存放 thread 共享資源
typedef struct {
    sem_t sem_stu; 
    sem_t sem_ta;  
    pthread_mutex_t mutex; 
    int* ids;
    int id_count;
    int stu_total;
    int chair[NUM_SEAT]; 
    int waiting_count;
    int next_seat; 
    int next_teach; 
    int ta_awake;
} data_t;

// 函式預宣告
void init_data(data_t* data);
void* stu_behavior(void* stu_id); 
void* ta_behavior(); 
int getUniqueID(data_t* data, int thread_num);
void sit_chair(data_t* data, int id);
void call_student(data_t* data);
void print_chair(const int code, int chair[NUM_SEAT]);
void time_simulation();

// 主程式
int main(){ 
    pthread_t *students; 
    pthread_t ta;  
    int i; 

    srand(time(NULL)); 
    data_t data;
    printf("有幾位學生? "); 
    scanf("%d", &data.stu_total);
    init_data(&data);
    students = (pthread_t*)malloc(sizeof(pthread_t) * data.stu_total); 

    pthread_create(&ta, NULL, ta_behavior, (void*) &data); 
    for(i = 0; i < data.stu_total; i++) { 
        pthread_create(&students[i], NULL, stu_behavior, (void*) &data); 
    }  
    pthread_join(ta, NULL); 
    for(i = 0; i < data.stu_total; i++) {
        pthread_join(students[i], NULL);
    }
    
    free(students);
    free(data.ids);
    return 0; 
} 

// 初始化 thread 共享資源
void init_data(data_t* data) {
    sem_init(&data->sem_stu, 0, 0);
    sem_init(&data->sem_ta, 0, 0);
    pthread_mutex_init(&data->mutex, NULL);
    memset(data->chair, 0, sizeof(data->chair));
    data->waiting_count = 0;
    data->next_seat = 0;
    data->next_teach = 0;
    data->ta_awake = 0;
    data->id_count = 0;
    data->ids = (int*)malloc(data->stu_total * sizeof(int));
}

// 學生行為的 thread
void* stu_behavior(void* arg) { 
    data_t* data = (data_t*)arg;
    pthread_mutex_lock(&data->mutex);
    int id = getUniqueID(data, (int)pthread_self()); 
    pthread_mutex_unlock(&data->mutex);
    printf("[Stu init] %d 號學生到校\n", id);
    while(1) { 
        time_simulation(); 
        pthread_mutex_lock(&data->mutex); 
        if (data->waiting_count < NUM_SEAT) { 
            sit_chair(data, id);
            printf("[Stu] %d 號學生坐在外面等候\n", id); 
            print_chair(1, data->chair);
            if (data->ta_awake == 0) {
                data->ta_awake = 1; 
                printf("( TA 被 %d 號學生叫醒 )\n", id);
            }
            pthread_mutex_unlock(&data->mutex); 
            sem_post(&data->sem_stu); 
            sem_wait(&data->sem_ta); 
        } else { 
            pthread_mutex_unlock(&data->mutex); 
            printf("[Stu] 座位已滿 : %d 號學生待會回來\n", id);	
        } 
    }
} 

// 助教行為的 thread
void* ta_behavior(void* arg) { 
    data_t* data = (data_t*)arg; 
    printf("[TA init] TA 坐在實驗室中\n");
    printf("[TA] 休息片刻\n");
    while(1) { 
        sem_wait(&data->sem_stu);
        pthread_mutex_lock(&data->mutex);
        printf("\t[TA] 正在教導 %d 號學生\n", data->chair[data->next_teach]);
        call_student(data);
        time_simulation(); 
        printf("\t[TA] 結束教導\n");
        print_chair(0, data->chair);
        if (data->waiting_count == 0) {
            data->ta_awake = 0;
            printf("\t[TA] 休息片刻\n");
        }
        pthread_mutex_unlock(&data->mutex); 
        sem_post(&data->sem_ta); 
    }
} 

// 依照 thread 執行序號 轉換為 id : (1 ~ n)
int getUniqueID(data_t* data, int thread_num) {
    int temp_id = abs(thread_num) % data->stu_total;
    while (1) {
        int id_exists = 0;
        for (int i = 0; i < data->id_count; i++) {
            if (data->ids[i] == temp_id + 1) {
                id_exists = 1;
                break;
            }
        }
        if (id_exists) {
            temp_id = (temp_id + 1) % data->stu_total;
        } else {
            data->ids[data->id_count++] = temp_id + 1;
            break;
        }
    }
    return temp_id + 1;
}

// 學生坐在椅子上的共享資源處理
void sit_chair(data_t* data, int id) {
    data->chair[data->next_seat] = id; 
    data->waiting_count++; 
    data->next_seat = (data->next_seat + 1) % NUM_SEAT;
}

// 助教請學生進實驗室的共享資源處理
void call_student(data_t* data) {
    data->chair[data->next_teach] = 0; 
    data->waiting_count--; 
    data->next_teach = (data->next_teach + 1) % NUM_SEAT; 
}

// 輸出目前等候狀況
void print_chair(const int code, int chair[NUM_SEAT]) {
    if (code == 0) printf("\t[TA]");
    else printf("[Stu]");
    printf(" 等候席 : [1] %d [2] %d [3] %d\n", chair[0], chair[1], chair[2]);
}

// 模擬現實事件執行時間
void time_simulation() {
    sleep(rand() % WAIT_MAX_SEC + 1);
}