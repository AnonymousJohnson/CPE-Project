  //Contains adjacency matrix representing network, a probability matrix quantifying each link's chance of failiure, and a probability array quantifying each node's chance of failiure
#ifndef NETWORK_H
#define NETOWRK_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class network
{
  private:
    vector<vector<bool>> network_graph;
    vector<vector<float>> link_failiure_matrix;
    vector<float> node_failiure_array;
    int size;
  public:
    network();
    void initialize_network(int s);
    void shuffle_list(vector<int> list);
    void print();
    vector<vector<bool>> get_network_graph();
    vector<vector<int>> delete_links_and_nodes();
    void resize_network(vector<int> deleted_nodes);
    bool is_network_connected();
    void traverse(int v, vector<bool>& visited);
};

#endif
