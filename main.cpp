#include "network.h"

bool main_thread_started = true;
bool is_table_full = false;
bool is_connected = true;
bool deleted_nodes_exited = false;
vector<int> deleted_nodes;
vector<bool> translate_deleted;
network master_network;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;

struct arguments
{
  network n;
  int id;
};

void translate_deleted_nodes(vector<int>& deleted_nodes);

void* node_worker (void* arg);

int main()
{
  srand((unsigned int)time(NULL));

  int size;
  cout << "Please enter size of the network for the simulation: ";
  cin >> size;
  cout << endl;
  
  master_network.initialize_network(size);
  arguments* arg = new arguments[size];
  translate_deleted.resize(size, false);
  
  pthread_t* threads = new pthread_t[size];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  for (int i = 0; i < size; i++)
  {
    (arg + i)->n = master_network;
    (arg + i)->id = i;
    pthread_create(&threads[i], 0, node_worker, (void*)(arg + i));
  }
  
  for(;;)
  {
    pthread_mutex_lock(&lock1);
    deleted_nodes = master_network.delete_links_and_nodes();
    is_connected = master_network.is_network_connected();

    if (!is_connected)
    {
      main_thread_started = false;
      pthread_cond_signal(&cond1);
      pthread_mutex_unlock(&lock1);
      break;
    }
    cout << "Deleted nodes before:\n";
    for (int i = 0; i < deleted_nodes.size(); i++)
    {
      cout << deleted_nodes[i] << ' ';
    }
    cout << endl;

    translate_deleted_nodes(deleted_nodes);
    cout << "Deleted nodes after:\n";
    for (int i = 0; i < deleted_nodes.size(); i++)
    {
      cout << deleted_nodes[i] << ' ';
    }
    cout << endl;

    main_thread_started = false;
    deleted_nodes_exited = false;
    is_table_full = false;
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&lock1);

    pthread_mutex_lock(&lock2);
    while (!is_table_full)
    {
      pthread_cond_wait(&cond2, &lock2);
    }
    pthread_mutex_unlock(&lock2);
  }

  for (int i = 0; i < size; i++)
  {
    pthread_join(threads[i], 0);
  }

  return 0;
}

void* node_worker (void* arg)
{
  int id = ((arguments*)arg)->id;
  for(;;)
  {
    pthread_mutex_lock(&lock1);
    while (main_thread_started)
    {
      pthread_cond_wait(&cond1, &lock1);
    }
    if (!is_connected)
    {
      pthread_cond_signal(&cond1);
      pthread_mutex_unlock(&lock1);
      return 0;
    }
    if (deleted_nodes_exited)
    {
      pthread_mutex_lock(&lock2);
      int gambit = rand() % 10;
      if (gambit == 0)
      {
        is_table_full = true;
        main_thread_started = true;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&lock2);
      }
      else
      {
        pthread_mutex_unlock(&lock2);
      }
    }
    else if (deleted_nodes.empty())
    {
        deleted_nodes_exited = true;
    }
    else
    {
      vector<int>::iterator it = find(deleted_nodes.begin(), deleted_nodes.end(), id);
      if (it != deleted_nodes.end())
      {
        cout << id << " was deleted.\n";
        deleted_nodes.erase(it);
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock1);
        return 0;
      }
    }
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&lock1);
  }
}

void translate_deleted_nodes(vector<int>& deleted_nodes)
{
  int i, j, k;
  for (i = 0, j = 0, k = 0; i < translate_deleted.size() && k < deleted_nodes.size(); i++, j++)
  {
    if (translate_deleted[i])
    {
      j--;
    }
    if (!translate_deleted[i] && j == deleted_nodes[k])
    {
      translate_deleted[i] = true;
      deleted_nodes[k] = i;
      k++;
    }
  }
}
