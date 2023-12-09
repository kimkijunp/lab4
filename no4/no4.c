#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
int clientCount = 0;
void *clientThread(void *arg) {
	int clientId = *((int *)arg);
	printf("클라이언트 %d: 메시지 전송 요청 받음\n", clientId);

	pthread_mutex_lock(&mutex);
	clientCount++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	while (clientCount < MAX_CLIENTS) {
		pthread_cond_wait(&condVar, &mutex);
	}
	pthread_mutex_unlock(&mutex);
	
	printf("서버: 모든 클라이언트에게 메시지 방송!\n");
	
	return NULL;
}

int main() {
	pthread_t clients[MAX_CLIENTS];
	int clientIds[MAX_CLIENTS];


	for (int i = 0; i < MAX_CLIENTS; ++i) {
		clientIds[i] = i + 1;
		pthread_create(&clients[i], NULL, clientThread, &clientIds[i]);

	}
	sleep(1);
	
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&condVar);
	pthread_mutex_unlock(&mutex);




	
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		pthread_join(clients[i], NULL);

	}
	return 0;
}
