#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

class Monitor 
{
private:
  pthread_mutex_t mutex;
  pthread_cond_t  x;
  pthread_cond_t  y;

public:
  Monitor()
  {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&x, NULL);
    pthread_cond_init(&y, NULL);
  }

  void methodA()
  {
    cout << "starting methodA..." << endl;
    pthread_mutex_lock(&mutex);
    cout << "LOCK in A executed" << endl;
    pthread_cond_wait(&x, &mutex);
    cout << "COND WAIT in A executed" << endl;
    pthread_cond_signal(&y);
    cout << "COND SIGNAL in A executed" << endl;

    cout << "woke up from methodA..." << endl;
    pthread_mutex_unlock(&mutex); 
  }

  void methodB()
  {
    cout << "starting methodB..." << endl;
    pthread_mutex_lock(&mutex);
    cout << "LOCK in B executed" << endl;
    pthread_cond_wait(&y, &mutex);
    cout << "COND WAIT in B executed" << endl;
    pthread_cond_signal(&x);
    cout << "COND SIGNAL in B executed" << endl;

    cout << "woke up from methodB..." << endl;
    pthread_mutex_unlock(&mutex);
  }

  void methodC()
  {
    cout << "starting methodC..." << endl;
    pthread_mutex_lock(&mutex);
    cout << "signaling X on methodA..." << endl;
    pthread_cond_signal(&x);
    pthread_mutex_unlock(&mutex);
  }
} monitor;

void* thread_func(void* arg)
{
  int id = *(int*)arg;

  if (id == 1) monitor.methodA();
  if (id == 2) monitor.methodB();
}

int main()
{
  pthread_t threadNum[2];
  int       logicalNum[2] = {1, 2};

  cout << "program start..." << endl;

  pthread_create(&threadNum[0], NULL, thread_func, (void*)&logicalNum[0]);
  pthread_create(&threadNum[1], NULL, thread_func, (void*)&logicalNum[1]);

  sleep(3);
  cout << "calling method C..." << endl;
  monitor.methodC();

  pthread_join(threadNum[0], NULL);
  pthread_join(threadNum[1], NULL);

  return 0;
}