#ifndef input_handler_HPP
#define input_handler_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <pair>

class Process {
public:
    int id_, time_, batch_size_;
    std::string name_;
    std::vector<int> resources;
    int getTimeForBatch(int batch_size);
};

class DishNode {
public:
    std::vector<int> parent_nodes_;
    int node_process_;
    std::vector<*DishNode> child_nodes_;
    bool can_start(std::unordered_set<int> process_done);
    void add_parent(DishNode &node);
    void add_child(DishNode &node);
};

extern int cooks, studnets_veg, students_non-veg, studnets_jain;
extern std::unordered_map<int, string> dish_name, resource_name;
extern std::unordered_map<int, int> dish_count;
extern std::unordered_map<int, int> resources;
extern std::unordered_map<int, Process> process_map;
extern std::unordered_map<pair<int, int>, DishNode> node_map;

#endif