#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01f;
    start_y *= 0.01f;
    end_x *= 0.01f;
    end_y *= 0.01f;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue above to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
   current_node->FindNeighbors();
   for(auto neighbor : current_node->neighbors){
      	neighbor->parent = current_node;
    	neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
      	neighbor->h_value = CalculateHValue(neighbor);
      	open_list.push_back(neighbor);
      	neighbor->visited = true;
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool compare(const RouteModel::Node* a, const RouteModel::Node* b){
	return (a->g_value + a->h_value) > (b->g_value + b->h_value);
}

RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), compare);
    RouteModel::Node* temp = open_list.back();
    open_list.pop_back();
  	return temp;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    std::vector<RouteModel::Node> path_found;
    // TODO: Implement your solution here.
    distance = 0.0f;
  	RouteModel::Node parent_node;
    while(current_node->parent != nullptr){
        path_found.push_back(*current_node);
      	parent_node = *(current_node->parent);
      	distance += current_node->distance(parent_node);
      	std::cout << distance << std::endl;
      	current_node = current_node->parent;
    }
  	path_found.push_back(*current_node);
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    //std::cout << distance << std::endl;
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    //std::cout << "start at " << start_node->x << ' ' <<  start_node->y << std::endl;
  	//std::cout << "end   at " << end_node->x << ' ' << end_node->y << std::endl;
    // initialise open_list vector with starting node
    start_node->visited = true;
    open_list.push_back(start_node);
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
    // Use a heuristic methode to prioritize what node to open and expand to end node
    while(open_list.size() > 0){
        current_node = NextNode();
        // check if the current node is the end node
        if( current_node->distance(*end_node) == 0){
        	m_Model.path = ConstructFinalPath(current_node);
          	return;
        } else {
        AddNeighbors(current_node);
        }
    }    

}