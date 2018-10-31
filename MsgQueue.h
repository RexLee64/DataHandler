#ifndef MSGQUEUE_H
#define MSGQUEUE_H
#endif

#include <pthread.h>
#define SIZE 4096                                        // 接收缓冲区大小
#define QUEUE_SIZE 60000                                 // 消息队列大小
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; // 队列锁
pthread_mutex_t error_mutex = PTHREAD_MUTEX_INITIALIZER; // 错误文件锁

// 缓存数据队列
typedef struct MsgQueue
{
    int head;
    int rear;
    char buff[QUEUE_SIZE][SIZE];
    int recvLen[QUEUE_SIZE];
} msgQueue;

// 队列初始化
int QueueInit(msgQueue *q)
{
    if (!q)
    {
        printf("Invalid Queue!\n");
        return -1;
    }

    q->head = 0;
    q->rear = 0;

    return 0;
}

// 进队
int EnQueue(msgQueue *q, char *buff, int len)
{
    if (q->head == (q->rear + 1) % QUEUE_SIZE)
    {
        pthread_mutex_lock(&error_mutex);

        FILE *fp = fopen("error.txt", "a+");
        if (fp != NULL)
        {
            fprintf(fp, "Full Queue!\n");
        }

        fclose(fp);

        pthread_mutex_unlock(&error_mutex);

        printf("Full Queue!\n");

        return -1;
    }

    pthread_mutex_lock(&queue_mutex);

    memcpy(&(q->buff[q->rear]), buff, SIZE);
    q->recvLen[q->rear] = len;
    q->rear = (q->rear + 1) % QUEUE_SIZE;

    pthread_mutex_unlock(&queue_mutex);

    return 0;
}

// 出队
int DeQueue(msgQueue *q, char *buff, int *len)
{

    if (!q)
    {
        printf("invalid Queue!\n");
        return -1;
    }

    if (q->head == q->rear)
    {
        return -1;
    }

    pthread_mutex_lock(&queue_mutex);

    memcpy(buff, &(q->buff[q->head]), SIZE);
    *len = q->recvLen[q->head];
    memset(&q->buff[q->head], 0, sizeof(q->buff[q->head]));
    q->head = (q->head + 1) % QUEUE_SIZE;

    pthread_mutex_unlock(&queue_mutex);

    return 0;
}
