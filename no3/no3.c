#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_CUSTOMERS 20

typedef struct {
	int ticket[BUFFER_SIZE];
    	int totalTickets;
	int in, out;
	pthread_mutex_t mutex;
	pthread_cond_t full;
	pthread_cond_t empty;
} ticket_buffer_t;

ticket_buffer_t ticketQueue = {{0}, 0, 0, 0,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER};



int issue_ticket() {
	int ticketNumber = rand() % 1000 + 1;
	printf("손님 %d번이 번호표를 발급받았습니다.\n", ticketNumber);
	sleep(1);
	return ticketNumber;

}

int enqueue_ticket(int ticketNumber) {
	int status;
	status = pthread_mutex_lock(&ticketQueue.mutex);
	if (status != 0)
		return status;
	while (ticketQueue.totalTickets >= BUFFER_SIZE && status == NULL)
		status = pthread_cond_wait(&ticketQueue.empty, &ticketQueue.mutex);
	if (status != 0) {
		pthread_mutex_unlock(&ticketQueue.mutex);
		return status;
	}
	ticketQueue.ticket[ticketQueue.in] = ticketNumber;
	ticketQueue.in = (ticketQueue.in + 1) % BUFFER_SIZE;
	ticketQueue.totalTickets++;
	if (status = pthread_cond_signal(&ticketQueue.full)) {
		pthread_mutex_unlock(&ticketQueue.mutex);
		return status;
	}
	return pthread_mutex_unlock(&ticketQueue.mutex);

}


void *customer(void *arg) {
	while (1) {
		int ticketNumber = issue_ticket();
		enqueue_ticket(ticketNumber);

	}
}



int dequeue_ticket(int *ticketNumber) {
	int status;
	status = pthread_mutex_lock(&ticketQueue.mutex);
	if (status != 0)
		return status;
	while (ticketQueue.totalTickets <= 0 && status == NULL)
		status = pthread_cond_wait(&ticketQueue.full, &ticketQueue.mutex);
	if (status != 0) {
		pthread_mutex_unlock(&ticketQueue.mutex);
		return status;

	}
	*ticketNumber = ticketQueue.ticket[ticketQueue.out];
	ticketQueue.out = (ticketQueue.out + 1) % BUFFER_SIZE;
	ticketQueue.totalTickets--;
	if (status = pthread_cond_signal(&ticketQueue.empty)) {
		pthread_mutex_unlock(&ticketQueue.mutex);
		return status;
	}
	return pthread_mutex_unlock(&ticketQueue.mutex);
}



void *waiter(void *arg) {
	int ticketNumber;
	while (1) {
		dequeue_ticket(&ticketNumber);
		printf("번호표 %d번 손님의 주문을 받습니다.\n", ticketNumber);
		sleep(1);

	}
}

int main() {
	int status;
	pthread_t customer_tid, waiter_tid;	
	/* 쓰레드 생성 */
	status = pthread_create(&customer_tid, NULL, customer, NULL);
	if (status != 0)
		perror("Create customer thread");
	status = pthread_create(&waiter_tid, NULL, waiter, NULL);
	if (status != 0)
		perror("Create waiter thread");


	
	/* 쓰레드가 종료될 때까지 대기 */
	status = pthread_join(customer_tid, NULL);
	if (status != 0)
		perror("Join customer thread");

	status = pthread_join(waiter_tid, NULL);
	if (status != 0)
		perror("Join waiter thread");
	
	return 0;

}
