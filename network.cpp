#include "network.h"

network::network()
{
  size = 0;
}
void network::initialize_network(int s)
//This method produces a variable-size connected graph representing the network. In addition to the number of edges in a minimally
//connected graph, k additional edges are added randomly to the graph. K is a value between 0 and the number of edges in a complete graph
//minus the number of edges in a minimally connected graph. Furthermore, a probability vector representing the chances of a node failiure and
//a probability map representing the chances of a link failiure are produced.
{
  //Resizing vectors
  size = s;
  vector<int> vertex_list(size);
  network_graph.resize(size);  
  link_failiure_matrix.resize(size);
  node_failiure_array.resize(size);
 
  for (int i = 0; i < size; i++) 
  //Initializing vertex_list and node_failiure_array
  {
    vertex_list[i] = i;
    node_failiure_array[i] = (static_cast <float> (rand() % (RAND_MAX/10))/static_cast <float> (RAND_MAX));
  }

  for (int i = 0; i < size; i++) 
  //Resizing network_graph + link_failiure_matrix columns and initializing network_graph
  {
    network_graph[i].resize(size);
    link_failiure_matrix[i].resize(size);
    for (int j = 0; j < size; j++)
    {
      if (i != j)
      {
        network_graph[i][j] = false;
      }
      else
      {
        network_graph[i][j] = true;
      }
    }
  }

  shuffle_list(vertex_list); \
  //Shuffles vertex_list

  for (int i = 1; i < size; i++) 
  //Converting network_graph to a randomly connected graph using shuffled vertex_list
  {
    int j = rand() % i;
    network_graph[vertex_list[i]][vertex_list[j]] = true;
    network_graph[vertex_list[j]][vertex_list[i]] = true;
  }

  int extra_edge_count = rand() % (((size * (size - 1))/2) - (size - 2));
  //value should be between zero and the maximum number of edges in a complete graph minus the number of edges added above

  shuffle_list(vertex_list);
  //shuffles list again so that the k additional edges are added randomly to the graph

  for (int i = 0; i < extra_edge_count; i++)
  // Adds k additional edges to the graph
  {
    for (int j = i + 1; j < size; j++)
    {
      if (network_graph[vertex_list[i]][vertex_list[j]] != true)//checks if there's an edge between two random vertices 
      {
        network_graph[vertex_list[i]][vertex_list[j]] = true;
        network_graph[vertex_list[j]][vertex_list[i]] = true;
        j = size;
      }
    }
  }
  
  for (int i = 0; i < size; i++) //initializes link_failiure_matrix
  {
    for (int j = 0; j < size; j++)
    {
      if (network_graph[i][j] == 0) //If there is no link between the two nodes, the probability of failiure is 0
      {
        link_failiure_matrix[i][j] = 0;
      }
      else
      {
        link_failiure_matrix[i][j] =static_cast <float> (rand() % (RAND_MAX/10))/static_cast <float> (RAND_MAX); //Random value between 0 and .2
      }
    }
  }
}
void network::shuffle_list(vector<int> list)
{
  for (int i = size - 1; i > 0; i--)
  {
    int j = rand() % (i+ 1);
    swap(list[i], list[j]);
  }
} 
void network::print()
{
  cout << "Adjacency Matrix:\n\n";
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      cout << network_graph[i][j] << ' ';
    }
    cout << endl;
  }

  cout << endl << "Link Failiure Matrix:\n\n";

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      cout << link_failiure_matrix[i][j] << ' ';
    }
    cout << endl;
  }

  cout << endl << "Node Failiure Array:\n\n";

  for (int i = 0; i < size; i++)
  {
    cout << node_failiure_array[i] << ' ';
  }

  cout << endl;
}

vector<vector<bool>> network::get_network_graph()
{
  return network_graph;
}

vector<int> network::delete_links_and_nodes()
{
  vector<int> deleted_nodes;
  for (int i = 0; i < size; i++)
  {
    float random = static_cast<float> (rand() % RAND_MAX)/static_cast <float> (RAND_MAX);
    if (random < node_failiure_array[i])
    {
      cout << i << " deleted.\n";
      deleted_nodes.push_back(i);
    }
    else
    {
      for (int j = 0; j < size; j++)
      {
        float random = static_cast <float> (rand() % RAND_MAX)/static_cast <float> (RAND_MAX);
        if (random < link_failiure_matrix[i][j])
        {
          network_graph[i][j] = 0;
          network_graph[j][i] = 0;
          link_failiure_matrix[i][j] = 0;
          link_failiure_matrix[j][i] = 0;
        }
      }
    }
  }
  resize_network(deleted_nodes);
  return deleted_nodes;
}
void network::resize_network(vector<int> deleted_nodes)
{
  if (deleted_nodes.size() <= (size - 2)) 
  // If there are fewer than 2 nodes in the network, continuing the simulation is pointless
  {
    for(int i = 0; i < deleted_nodes.size(); i++)
    {
      network_graph.erase(network_graph.begin() + deleted_nodes[i]);
      link_failiure_matrix.erase(link_failiure_matrix.begin() + deleted_nodes[i]);
      node_failiure_array.erase(node_failiure_array.begin() + deleted_nodes[i]);
      size--;
      for (int j = 0; j < size; j++)
      {
        network_graph[j].erase(network_graph[j].begin() + deleted_nodes[i]);
        link_failiure_matrix[j].erase(link_failiure_matrix[j].begin() + deleted_nodes[i]);
      }
      if (i != (deleted_nodes.size() - 1))
      {
        deleted_nodes[i + 1] -= 1;
      }
    }
  }
  else
  {
    size -= deleted_nodes.size();
  }
}

bool network::is_network_connected()
{
  if (size <= 1)
  // If there are fewer than 2 nodes in the network, continuing the simulation is pointless
  {
    return false;
  }

  vector<bool> visited;

  for (int i = 0; i < size; i++)
  {
    visited.resize(size, false);
    traverse(i, visited);
    if (find(visited.begin(), visited.end(), false) != visited.end())
    {
      return false;
    }
  }
  return true;
}

void network::traverse(int v, vector<bool>& visited)
{
  visited[v] = true;
  for (int i = 0; i < size; i++)
  {
    if (network_graph[i][v] == 1 && i != v && visited[i] == false)
    {
      traverse(i, visited);
    }
  }
}
