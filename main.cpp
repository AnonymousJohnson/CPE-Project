#include "network.h"

void translate_affected_nodes(vector<vector<int>>& affected_nodes, vector<bool>& translate_deleted);
int main()
{
  srand((unsigned int)time(NULL));

  int size = (rand() % 18) + 2; 
  network master_network;
  vector<bool> translate_deleted(size, false);
  vector<vector<int> affected_nodes_table;
  bool is_connected;

  cout << "Size:\n";
  cout << size << endl << endl;
  master_network.initialize_network(size);

  do
  {
    vector<vector<bool>> network_graph = master_network.get_network_graph();

    affected_nodes_table = master_network.delete_links_and_nodes();
    is_connected = master_network.is_network_connected();

    translate_affected_nodes(affected_nodes_table, translate_deleted);
    vector<int> nodes_in_graph;
    nodes_in_graph.push_back(-1);

    for (int i = 0; i < translate_deleted.size(); i++)
    {
      while(translate_deleted[i])
      {
        i++;
      }
      if (i < translate_deleted.size())
      {
        nodes_in_graph.push_back(i);
      }
    }
    master_table.push_back(nodes_in_graph);

    for (int i = 0; i < affected_nodes_table[1].size(); i++)
    {
      affected_nodes_table[0].push_back(affected_nodes[1][i]);
    }
    vector<int> affected_nodes = affected_nodes_table[0];

    for (int i = 0; i < affected_nodes.size(); i++)
    {
      vector<int> v(nodes_in_graph.size() + 1, 0)
      v.push_back(affected_nodes[i]);
      for (int j = 0; j < nodes_in_graph.size(); j++)
      {
        if (translate_deleted[i] && i == affected_nodes[i])
        {



  } while (is_connected);

  return 0;
}

void translate_affected_nodes(vector<vector<int>>& affected_nodes_table, vector<bool>& translate_deleted)
{
  int i, j, k;
  for (i = 0, j = 0, k = 0; i < translate_deleted.size() && k < affected_nodes_table[0].size(); i++, j++)
  {
    if (translate_deleted[i])
    {
      j--;
    }
    if (!translate_deleted[i] && j == affected_nodes_table[0][k])
    {
      translate_deleted[i] = true;
      affected_nodes_table[0][k] = i;
      k++;
    }
  }

  for (i = 0, j = 0, k = 0; i < translate_deleted.size() && k < affected_nodes_table[1].size(); i++, j++)
  {
    if (translated_deleted[i])
    {
      j--;
    }
    if (!translated_deleted[i] && j == affected_nodes_table[1][k])
    {
      affected_nodes_table[1][k] = i;
      k++;
    }
  }
}
