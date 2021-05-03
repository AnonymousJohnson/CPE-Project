#ifndef WORKER_IDENTIFIER_H
#define WORKER_IDENTIFIER_H

#include <iostream>
using namespace std;

class worker_identifier
{
  private:
    static int object_count;
    int id;
  public:
    worker_identifier()
    {
      id = object_count;
      object_count++;
    }
    int get_id()
    {
      return id;
    }
};



#endif
