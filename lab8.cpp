#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>
#include <chrono>

using namespace std;
using Node = pair<double, double>;

// Структура графа
struct Graph {
    map<Node, list<pair<Node, double>>> nodes_list;

    // Добавление ребра в граф
    void add_edge(double lon_parent, double lat_parent, double lon_child, double lat_child, double weight) {
        nodes_list[{lon_parent, lat_parent}].push_back({ {lon_child, lat_child}, weight });
        nodes_list[{lon_child, lat_child}].push_back({ {lon_parent, lat_parent}, weight });
    }

    // Нахождение ближайшей вершины
    Node find_closest_node(double lon, double lat) {
        Node node_founded; 
        double min_distance = pow(10, 9);

        // Перебираем все вершины графа
        for (const auto& node : nodes_list) {
            double distance = sqrt(pow(node.first.first - lon, 2) + pow(node.first.second - lat, 2));
            if (distance < min_distance) { 
                min_distance = distance; 
                node_founded = node.first; 
            }
        }
        return node_founded;
    }

    // Получение данных из файла
    void get_file_data(const string& filename) {
        ifstream file(filename); 
        string line; 

        while (getline(file, line)) { 
            stringstream line_stream(line);
            string node_info;  
            getline(line_stream, node_info, ':'); 

            stringstream node_stream(node_info);
            string node_coords; 

            getline(node_stream, node_coords, ','); 
            double lon1 = stod(node_coords);
            getline(node_stream, node_coords);
            double lat1 = stod(node_coords); 

            string edges_info;
            while (getline(line_stream, edges_info, ';')) { 
                stringstream edge_stream(edges_info);  
                string child_info;  

                getline(edge_stream, child_info, ','); 
                double lon2 = stod(child_info); 

                getline(edge_stream, child_info, ','); 
                double lat2 = stod(child_info); 

                getline(edge_stream, child_info, ','); 
                double weight = stod(child_info); 

                add_edge(lon1, lat1, lon2, lat2, weight); 
            }
        }
        file.close(); 
    }

    // Вывод пути
    void print_path(vector<Node> path) {
        if (path.empty()) {
            cout << "The path was not found." << endl; 
            return; 
        }
        cout << "Your path: ";  
        for (int i = 0; i < path.size() - 1; i++) {
            cout << "(" << path[i].first << ";" << path[i].second << "), "; 
        }
        cout << "(" << path[path.size() - 1].first << ";" << path[path.size() - 1].second << ")" << endl; 
    }
};

// ----- Алгоритм BFS -----
vector<Node> bfs(Graph& graph, Node start, Node goal) {
    auto start_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    queue<Node> q; // Память: O(V), V - кол-во вершин
    map<Node, Node> parents; // Память: O(V), V - кол-во вершин
    map<Node, bool> visited;  // Память: O(V), V - кол-во вершин

    q.push(start); // Память: O(1), Сложность: O(1)
    visited[start] = true;  // Память: O(1), Сложность: O(1)
    parents[start] = start; // Память: O(1), Сложность: O(1)

    while (!q.empty()) { // Сложность: O(V + E), V - вершины, E - ребра
        Node current = q.front(); // Память: O(1), Сложность: O(1)
        q.pop();  // Память: O(1), Сложность: O(1)

        if (current == goal) { // Память: O(1), Сложность: O(1)
            auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Память: O(1), Сложность: O(1)
            cout << "BFS Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
            cout << "BFS Memory: " << sizeof(q) + sizeof(parents) + sizeof(visited) << " bytes" << endl;  // Память: O(1), Сложность: O(1)
            vector<Node> path; // Память: O(1), Сложность: O(1)
            Node node = goal; // Память: O(1), Сложность: O(1)
            while (node != start) { // Сложность: O(P), P - длина пути
                path.push_back(node); // Память: O(1), Сложность: O(1)
                node = parents[node]; // Память: O(1), Сложность: O(1)
            }
            path.push_back(start); // Память: O(1), Сложность: O(1)
            reverse(path.begin(), path.end()); // Сложность: O(P), P - длина пути
            return path; // Память: O(1), Сложность: O(1)
        }

        for (const auto& edge : graph.nodes_list[current]) {  // Сложность: O(E), E - кол-во ребер
            Node neighbor = edge.first; // Память: O(1), Сложность: O(1)
            if (!visited[neighbor]) { // Память: O(1), Сложность: O(1)
                visited[neighbor] = true; // Память: O(1), Сложность: O(1)
                parents[neighbor] = current; // Память: O(1), Сложность: O(1)
                q.push(neighbor); // Память: O(1), Сложность: O(1)
            }
        }
    }
    auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);  // Память: O(1), Сложность: O(1)
    cout << "BFS Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
    cout << "BFS Memory: " << sizeof(q) + sizeof(parents) + sizeof(visited) << " bytes" << endl; // Память: O(1), Сложность: O(1)
    return {}; // Память: O(1), Сложность: O(1)
}
/*
    Сложность по времени: O(V + E), где V - количество вершин, E - количество рёбер.
    Сложность по памяти: O(V), где V - количество вершин (для очереди и карт).
*/

// ----- Алгоритм DFS -----
vector<Node> dfs(Graph& graph, Node start, Node goal) {
    auto start_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    stack<Node> s; // Память: O(V), V - кол-во вершин
    map<Node, Node> parents; // Память: O(V), V - кол-во вершин
    map<Node, bool> visited;  // Память: O(V), V - кол-во вершин

    s.push(start); // Память: O(1), Сложность: O(1)
    visited[start] = true;  // Память: O(1), Сложность: O(1)
    parents[start] = start;  // Память: O(1), Сложность: O(1)

    while (!s.empty()) {  // Сложность: O(V + E), V - вершины, E - ребра
        Node current = s.top(); // Память: O(1), Сложность: O(1)
        s.pop(); // Память: O(1), Сложность: O(1)

        if (current == goal) {  // Память: O(1), Сложность: O(1)
            auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Память: O(1), Сложность: O(1)
            cout << "DFS Time: " << duration.count() << " microseconds" << endl;  // Память: O(1), Сложность: O(1)
            cout << "DFS Memory: " << sizeof(s) + sizeof(parents) + sizeof(visited) << " bytes" << endl;  // Память: O(1), Сложность: O(1)

            vector<Node> path;  // Память: O(1), Сложность: O(1)
            Node node = goal;  // Память: O(1), Сложность: O(1)
            while (node != start) {  // Сложность: O(P), P - длина пути
                path.push_back(node);  // Память: O(1), Сложность: O(1)
                node = parents[node];  // Память: O(1), Сложность: O(1)
            }
            path.push_back(start);  // Память: O(1), Сложность: O(1)
            reverse(path.begin(), path.end()); // Сложность: O(P), P - длина пути
            return path; // Память: O(1), Сложность: O(1)
        }

        for (const auto& edge : graph.nodes_list[current]) {  // Сложность: O(E), E - кол-во ребер
            Node neighbor = edge.first; // Память: O(1), Сложность: O(1)
            if (!visited[neighbor]) { // Память: O(1), Сложность: O(1)
                visited[neighbor] = true; // Память: O(1), Сложность: O(1)
                parents[neighbor] = current; // Память: O(1), Сложность: O(1)
                s.push(neighbor); // Память: O(1), Сложность: O(1)
            }
        }
    }
    auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);  // Память: O(1), Сложность: O(1)
    cout << "DFS Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
    cout << "DFS Memory: " << sizeof(s) + sizeof(parents) + sizeof(visited) << " bytes" << endl; // Память: O(1), Сложность: O(1)
    return {}; // Память: O(1), Сложность: O(1)
}
/*
    Сложность по времени: O(V + E), где V - количество вершин, E - количество рёбер.
    Сложность по памяти: O(V), где V - количество вершин (для стека и карт).
*/

// ----- Алгоритм Dijkstra -----
struct NodeInfo { // Память: O(1), Сложность: O(1)
    Node node; // Память: O(1), Сложность: O(1)
    double distance; // Память: O(1), Сложность: O(1)
    bool operator>(const NodeInfo& other) const { // Память: O(1), Сложность: O(1)
        return distance > other.distance; // Память: O(1), Сложность: O(1)
    }
};

vector<Node> dijkstra(Graph& graph, Node start, Node goal) {
    auto start_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    priority_queue<NodeInfo, vector<NodeInfo>, greater<NodeInfo>> pq;  // Память: O(V), V - кол-во вершин
    map<Node, double> dist; // Память: O(V), V - кол-во вершин
    map<Node, Node> parents; // Память: O(V), V - кол-во вершин

    for (const auto& node : graph.nodes_list) { // Сложность: O(V), V - кол-во вершин
        dist[node.first] = numeric_limits<double>::max(); // Память: O(1), Сложность: O(1)
    }

    dist[start] = 0; // Память: O(1), Сложность: O(1)
    pq.push({ start, 0 }); // Память: O(1), Сложность: O(logV)

    while (!pq.empty()) { // Сложность: O(E log V), E - ребра, V - вершины
        NodeInfo currentInfo = pq.top(); // Память: O(1), Сложность: O(logV)
        pq.pop(); // Память: O(1), Сложность: O(logV)
        Node current = currentInfo.node; // Память: O(1), Сложность: O(1)
        double currentDist = currentInfo.distance; // Память: O(1), Сложность: O(1)

        if (current == goal) { // Память: O(1), Сложность: O(1)
            auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Память: O(1), Сложность: O(1)
            cout << "Dijkstra Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
            cout << "Dijkstra Memory:" << sizeof(pq) + sizeof(dist) + sizeof(parents) << " bytes" << endl; // Память: O(1), Сложность: O(1)

            vector<Node> path; // Память: O(1), Сложность: O(1)
            Node node = goal; // Память: O(1), Сложность: O(1)
            while (node != start) { // Сложность: O(P), P - длина пути
                path.push_back(node);  // Память: O(1), Сложность: O(1)
                node = parents[node]; // Память: O(1), Сложность: O(1)
            }
            path.push_back(start); // Память: O(1), Сложность: O(1)
            reverse(path.begin(), path.end()); // Сложность: O(P), P - длина пути
            return path; // Память: O(1), Сложность: O(1)
        }

        if (currentDist > dist[current]) { // Память: O(1), Сложность: O(1)
            continue;  // Память: O(1), Сложность: O(1)
        }

        for (const auto& edge : graph.nodes_list[current]) { // Сложность: O(E), E - кол-во ребер
            Node neighbor = edge.first; // Память: O(1), Сложность: O(1)
            double weight = edge.second; // Память: O(1), Сложность: O(1)
            if (dist[current] + weight < dist[neighbor]) {  // Память: O(1), Сложность: O(1)
                dist[neighbor] = dist[current] + weight; // Память: O(1), Сложность: O(1)
                parents[neighbor] = current; // Память: O(1), Сложность: O(1)
                pq.push({ neighbor, dist[neighbor] }); // Память: O(1), Сложность: O(logV)
            }
        }
    }
    auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Память: O(1), Сложность: O(1)
    cout << "Dijkstra Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
    cout << "Dijkstra Memory:" << sizeof(pq) + sizeof(dist) + sizeof(parents) << " bytes" << endl; // Память: O(1), Сложность: O(1)
    return {}; // Память: O(1), Сложность: O(1)
}
/*
    Сложность по времени: O(E log V), где V - количество вершин, E - количество рёбер.
    Сложность по памяти: O(V), где V - количество вершин (для приоритетной очереди и карт).
*/

// ----- Алгоритм A* -----
double heuristic(Node current, Node goal) { // Память: O(1), Сложность: O(1)
    return sqrt(pow(current.first - goal.first, 2) + pow(current.second - goal.second, 2)); // Память: O(1), Сложность: O(1)
}

struct AStarNode { // Память: O(1), Сложность: O(1)
    Node node; // Память: O(1), Сложность: O(1)
    double fScore;  // Память: O(1), Сложность: O(1)

    bool operator>(const AStarNode& other) const { // Память: O(1), Сложность: O(1)
        return fScore > other.fScore; // Память: O(1), Сложность: O(1)
    }
};

vector<Node> astar(Graph& graph, Node start, Node goal) {
    auto start_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> openSet;  // Память: O(V), V - кол-во вершин
    map<Node, Node> cameFrom; // Память: O(V), V - кол-во вершин
    map<Node, double> gScore; // Память: O(V), V - кол-во вершин
    map<Node, double> fScore; // Память: O(V), V - кол-во вершин

    for (const auto& node : graph.nodes_list) { // Сложность: O(V), V - кол-во вершин
        gScore[node.first] = numeric_limits<double>::max(); // Память: O(1), Сложность: O(1)
        fScore[node.first] = numeric_limits<double>::max(); // Память: O(1), Сложность: O(1)
    }

    gScore[start] = 0;  // Память: O(1), Сложность: O(1)
    fScore[start] = heuristic(start, goal);  // Память: O(1), Сложность: O(1)
    openSet.push({ start, fScore[start] }); // Память: O(1), Сложность: O(logV)

    while (!openSet.empty()) { // Сложность: O(E logV), E - ребра, V - вершины
        AStarNode currentInfo = openSet.top(); // Память: O(1), Сложность: O(logV)
        openSet.pop(); // Память: O(1), Сложность: O(logV)
        Node current = currentInfo.node; // Память: O(1), Сложность: O(1)

        if (current == goal) { // Память: O(1), Сложность: O(1)
            auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Память: O(1), Сложность: O(1)
            cout << "A* Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
            cout << "A* Memory:" << sizeof(openSet) + sizeof(gScore) + sizeof(fScore) + sizeof(cameFrom) << " bytes" << endl; // Память: O(1), Сложность: O(1)

            vector<Node> path; // Память: O(1), Сложность: O(1)
            Node node = goal; // Память: O(1), Сложность: O(1)
            while (node != start) { // Сложность: O(P), P - длина пути
                path.push_back(node); // Память: O(1), Сложность: O(1)
                node = cameFrom[node]; // Память: O(1), Сложность: O(1)
            }
            path.push_back(start);  // Память: O(1), Сложность: O(1)
            reverse(path.begin(), path.end()); // Сложность: O(P), P - длина пути
            return path; // Память: O(1), Сложность: O(1)
        }

        for (const auto& edge : graph.nodes_list[current]) { // Сложность: O(E), E - кол-во ребер
            Node neighbor = edge.first; // Память: O(1), Сложность: O(1)
            double weight = edge.second; // Память: O(1), Сложность: O(1)
            double tentative_gScore = gScore[current] + weight; // Память: O(1), Сложность: O(1)
            if (tentative_gScore < gScore[neighbor]) { // Память: O(1), Сложность: O(1)
                cameFrom[neighbor] = current;  // Память: O(1), Сложность: O(1)
                gScore[neighbor] = tentative_gScore; // Память: O(1), Сложность: O(1)
                fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal); // Память: O(1), Сложность: O(1)
                openSet.push({ neighbor, fScore[neighbor] }); // Память: O(1), Сложность: O(logV)
            }
        }
    }
    auto end_time = chrono::high_resolution_clock::now(); // Память: O(1), Сложность: O(1)
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Память: O(1), Сложность: O(1)
    cout << "A* Time: " << duration.count() << " microseconds" << endl; // Память: O(1), Сложность: O(1)
    cout << "A* Memory:" << sizeof(openSet) + sizeof(gScore) + sizeof(fScore) + sizeof(cameFrom) << " bytes" << endl; // Память: O(1), Сложность: O(1)
    return {}; // Память: O(1), Сложность: O(1)
}
/*
    Сложность по времени: O(E log V) в худшем случае, хотя на практике A* часто работает быстрее.
    Сложность по памяти: O(V), где V - количество вершин (для приоритетной очереди и карт).
*/

int main() {
    Graph graph;
    graph.get_file_data("spb_graph.txt");

    double lonUserStart = 30.229684;
    double latUserStart = 60.034552;
    double lonUserEnd = 30.309514;
    double latUserEnd = 59.956184;

    Node start = graph.find_closest_node(lonUserStart, latUserStart);
    Node end = graph.find_closest_node(lonUserEnd, latUserEnd);

    cout << "Start node: (" << start.first << "; " << start.second << ")" << endl;
    cout << "End node: (" << end.first << "; " << end.second << ")" << endl;

    cout << "\nBFS path" << endl;
    vector<Node> bfsPath = bfs(graph, start, end);
    graph.print_path(bfsPath);

    cout << "\nDFS path" << endl;
    vector<Node> dfsPath = dfs(graph, start, end);
    graph.print_path(dfsPath);

    cout << "\nDijkstra path" << endl;
    vector<Node> dijkstraPath = dijkstra(graph, start, end);
    graph.print_path(dijkstraPath);

    cout << "\nA* path" << endl;
    vector<Node> astarPath = astar(graph, start, end);
    graph.print_path(astarPath);

    Graph custom_graph;
    custom_graph.add_edge(0, 0, 1, 1, 1);
    custom_graph.add_edge(0, 0, 1, -1, 2);
    custom_graph.add_edge(1, 1, 2, 2, 1);
    custom_graph.add_edge(1, -1, 2, -2, 1);
    custom_graph.add_edge(2, 2, 3, 3, 1);
    custom_graph.add_edge(2, -2, 3, -3, 1);

    Node custom_start = { 0, 0 };
    Node custom_end = { 3, 3 };

    cout << "\nCustom graph tests:" << endl;
    cout << "\nCustom BFS path" << endl;
    vector<Node> customBfsPath = bfs(custom_graph, custom_start, custom_end);
    custom_graph.print_path(customBfsPath);

    cout << "\nCustom DFS path" << endl;
    vector<Node> customDfsPath = dfs(custom_graph, custom_start, custom_end);
    custom_graph.print_path(customDfsPath);

    cout << "\nCustom Dijkstra path" << endl;
    vector<Node> customDijkstraPath = dijkstra(custom_graph, custom_start, custom_end);
    custom_graph.print_path(customDijkstraPath);

    cout << "\nCustom A* path" << endl;
    vector<Node> customAstarPath = astar(custom_graph, custom_start, custom_end);
    custom_graph.print_path(customAstarPath);
    return 0;
}