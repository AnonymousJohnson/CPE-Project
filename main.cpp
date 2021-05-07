#include "network.h"

vector<bool> translate_deleted_nodes(vector<int>& deleted_nodes, vector<bool>& translate_deleted);
vector<int> unmap_cantor(int val);
void translate_link(vector<int>& link, vector<bool>& translate_deleted);
void pass_messages(vector<vector<int>> master_table, vector<vector<bool>> master_graph, vector<int> nodes_in_graph, int rows, int current_index);
bool table_is_all_twos(vector<vector<int>> master_table);

int main()
{
  srand((unsigned int)time(NULL));

  int size = (rand() % 18) + 2; 
  network master_network;
  vector<bool> translate_deleted(size, false);
  vector<vector<int>> affected_nodes;
  bool is_connected;

  cout << "Size:\n";
  cout << size << endl;
  master_network.initialize_network(size);
  vector<vector<bool>> master_graph = master_network.get_network_graph();

  for(;;)
  {
    vector<vector<bool>> network_graph = master_network.get_network_graph();

    affected_nodes = master_network.delete_links_and_nodes();
    is_connected = master_network.is_network_connected();

    if (!is_connected)
    {
      cout << "Network disconnected.\n";
      break;
    }
    
    vector<vector<int>> master_table;
    vector<int> nodes_in_graph;
    vector<int> deleted_nodes = affected_nodes[0];
    vector<bool> previous_deleted = translate_deleted_nodes(deleted_nodes, translate_deleted);
    nodes_in_graph.push_back(0);

    for (int i = 0; i < translate_deleted.size(); i++)
    {
      while(i < translate_deleted.size() && translate_deleted[i])
      {
        i++;
      }
      if (i < translate_deleted.size() && !translate_deleted[i])
      {
        nodes_in_graph.push_back(i);
      }
    }

    master_table.push_back(nodes_in_graph);

    for (int i = 0; i < deleted_nodes.size(); i++)
    {
      vector<int> v(nodes_in_graph.size(), 0);
      v[0] = deleted_nodes[i];
      for (int k = 1; k < nodes_in_graph.size(); k++)
      {
        if (!master_graph[deleted_nodes[i]][nodes_in_graph[k]])
        {
          v[k] = 0;
        }
        else if (master_graph[deleted_nodes[i]][nodes_in_graph[k]])
        {
          v[k] = 1;
        }
      }
      master_table.push_back(v);
    }

    for (int i = 1; i < affected_nodes.size(); i++)
    {
      vector<int> v(nodes_in_graph.size(), 0);
      vector<int> link = unmap_cantor(affected_nodes[i][0]);
      translate_link(link, previous_deleted);
      v[0] = link[0];

      int j = 1;
      int k = 0;
      while(j < nodes_in_graph.size())
      {
        if (k < affected_nodes[i].size())
        {
          link = unmap_cantor(affected_nodes[i][k]);
          translate_link(link, previous_deleted);
          k++;
          while (nodes_in_graph[j] != link[1])
          {
            v[j] = 0;
            j++;
          }
          v[j] = 1;
          master_graph[link[0]][link[1]] = 0;
        }
        else
        {
          v[j] = 0;
        }
        j++;
      }
      master_table.push_back(v);
    }
    int former_table_size = master_table.size();
    for (int i = deleted_nodes.size() + 1; i < former_table_size; i++)
    //RIP anyone trying to understand this
    {
      for (int j = 0; j < master_table[i].size(); j++)
      {
        if (master_table[i][j] == 1)
        {

          bool found_it = false;

          for (int k = i + 1; k < former_table_size; k++)
          {
            if (master_table[k][0] == nodes_in_graph[j])
            {
              int l = 1;
              while (nodes_in_graph[l] != master_table[i][0])
              {
                l++;
              }
              master_table[k][l] = 1;
              found_it = true;
            }
          }
          if (!found_it)
          {
            vector<int> v(master_table[i].size(), 0);
            v[0] = nodes_in_graph[j];
            int k = 1;
            
            while(nodes_in_graph[k] != master_table[i][0])
            {
              k++;
            }

            v[k] = 1;
            master_table.push_back(v);
          }
        }
      }
    }
    for (int i = 0; i < master_table.size(); i++)
    {
      for (int j = 0; j < master_table[i].size(); j++)
      {
        cout << master_table[i][j] << ' ';
      }
      cout << endl;
    }
    cout << endl;
    pass_messages(master_table, network_graph, nodes_in_graph, deleted_nodes.size() + affected_nodes.size() - 1, 0);
  }

  return 0;
}

vector<bool> translate_deleted_nodes(vector<int>& deleted_nodes, vector<bool>& translate_deleted)
{
  int i, j, k;
  vector<bool> previous_deleted = translate_deleted;
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
  return previous_deleted;
}

vector<int> unmap_cantor(int val)
{
  float z = .5*(float)val;
  int w = (int)((-1+sqrt(1+8*z))/2);
  vector<int> v;
  v.push_back(z-((w*(w+1))/2));
  v.push_back(((w*(w+3))/2) - z);
  return v;
} 

void translate_link(vector<int>& link, vector<bool>& translate_deleted)
{
  int i, j, k;
  for (i = 0, j = 0, k = 0; i < translate_deleted.size() && k < link.size(); i++, j++)
  {
    if (translate_deleted[i])
    {
      j--;
    }
    if (!translate_deleted[i] && j == link[k])
    {
      link[k] = i;
      k++;
    }
  }
}

void pass_messages(vector<vector<int>> master_table, vector<vector<bool>> master_graph, vector<int> nodes_in_graph, int rows, int current_index)
{
  if (!table_is_all_twos(master_table))
  {
    for (int i = current_index; i < nodes_in_graph.size(); i++)
    {
      for (int j = 1; j < rows; j++)
      {
        if (master_table[j][i] == 1)
        {
          master_table[j][i] = 2;
          for (int k = 0; k < nodes_in_graph.size(); k++)
          {
            if (master_graph[master_table[0][j]][nodes_in_graph[k]])
            {
              pass_messages(master_table, master_graph, nodes_in_graph, rows, nodes_in_graph[k]);
            }
          }
        }
      }
    }
  }
}

bool table_is_all_twos(vector<vector<int>> master_table)
{
  for (int i = 0; i < master_table.size(); i++)
  {
    for (int j = 0; j < master_table[i].size(); j++)
    {
      if (master_table[i][j] != 2)
      {
        return false;
      }
    }
  }
  return true;
}
