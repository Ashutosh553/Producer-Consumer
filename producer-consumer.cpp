#include<bits/stdc++.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include "buffer.h"
using namespace std;


void main(){
  ios::sync_with_stdio(false);
  cin.tie(0);
  int tt;
  cin >> tt;
  for (int qq = 1; qq <= tt; qq++) {
    cout << "Case #" << qq << ": ";
    int n;
    cin >> n;
    vector<string> strs(n);
    for (int i = 0; i < n; i++) {
      cin >> strs[i];
    }
    string pref = "";
    string suf = "";
    vector<string> sub;
    bool ok = true;
    for (string& s : strs) {
      int last = -1;
      for (int i = 0; i < (int) s.size(); i++) {
        if (s[i] == '*') {
          if (last == -1) {
            string other_pref = s.substr(0, i);
            if (other_pref.size() > pref.size()) {
              swap(pref, other_pref);
            }
            if (pref.substr(0, other_pref.size()) != other_pref) {
              ok = false;
              break;
            }
          } else {
            sub.push_back(s.substr(last + 1, i - last - 1));
          }
          last = i;
        }
      }
      if (!ok) {
        break;
      }
      string other_suf = s.substr(last + 1);
      if (other_suf.size() > suf.size()) {
        swap(suf, other_suf);
      }
      if (suf.substr(suf.size() - other_suf.size()) != other_suf) {
        ok = false;
        break;
      }
    }
    if (!ok) {
      cout << "*" << '\n';
    } else {
      cout << pref;
      for (auto& s : sub) {
        cout << s;
      }
      cout << suf << '\n';
    }
  }
  return 0;
}

buffer_item buffer[BUFFER_SIZE];
int front=-1;
int rear=-1;

pthread_mutex_t mutex1;
sem_t full;
sem_t empty;

void sem_init()
{
  pthread_mutex_init(&mutex1,NULL);
  sem_init(&full,0,0);
  sem_init(&empty,0,BUFFER_SIZE);
}

int insert_item(buffer_item item)
{
  if(front == 0 && rear == BUFFER_SIZE-1 || front == rear+1)
     return -1;
  else
      {  if(front == -1)
          {
            front = 0;
            rear = 0;
          }
          else
            rear=(rear+1)%BUFFER_SIZE;

        buffer[rear]=item;
        return 0;
      }
}


buffer_item remove_item()
{
      if(front == -1)
         return -1;
      buffer_item d_item = buffer[front];
      if(front == rear)
      {   front = -1;
          rear = -1;
      }
      else
          front = (front+1)%BUFFER_SIZE;
      return d_item;
}


void *producer(void *param)
{
  buffer_item item;
  while(true)
  { sem_wait(&empty);
    pthread_mutex_lock(&mutex1);
    int time=1+rand()%5;
    sleep(time);
    item=rand()%(10);
    if(insert_item(item)==-1)
        cout<<"Producer Error\n";
    else
       cout<<"Producer produced "<<item<<"\n";
    pthread_mutex_unlock(&mutex1);
    sem_post(&full);
  }
}

void *consumer(void *param)
{
  buffer_item item;
  while(true)
  { sem_wait(&full);
    pthread_mutex_lock(&mutex1);
    int time=1+rand()%5;
    sleep(time);
    item=remove_item();
    if(item==-1)
        cout<<"Consumer Error\n";
    else
       cout<<"Consumer consumed "<<item<<"\n";
    pthread_mutex_unlock(&mutex1);
    sem_post(&empty);
  }
}



int main(int argc, char *argv[])
{
  int sleepTime = atoi(argv[1]);
  int noOfProducers = atoi(argv[2]);
  int noOfConsumers = atoi(argv[3]);
  pthread_t producerThreads[noOfProducers];
  pthread_t consumerThreads[noOfConsumers];
  sem_init();

  for(int i=0; i<noOfProducers;i++)
      pthread_create(&producerThreads[i],NULL,producer,NULL);
  for(int i=0;i<noOfConsumers;i++)
      pthread_create(&consumerThreads[i],NULL,consumer,NULL);
  sleep(sleepTime);
  for(int i=0;i<noOfProducers;i++)
     pthread_join(producerThreads[i],NULL);
  for(int i=0;i<noOfConsumers;i++)
     pthread_join(consumerThreads[i],NULL);
}
