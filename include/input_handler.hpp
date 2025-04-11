#ifndef input_handler_HPP
#define input_handler_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>

class DishNode
{
public:
    std::vector<std::reference_wrapper<DishNode>> parent_nodes_;
    int node_id, node_process_id_, instances_required_, instances_done;
    bool is_done;
    std::vector<std::reference_wrapper<DishNode>> child_nodes_;
    bool can_start();
    void add_parent(DishNode &node);
    void add_child(DishNode &node);
    void instance_complete();
};

class Dish
{
public:
    int id_, student_count_;
    std::string name_;
    std::unordered_map<int, DishNode> graph_;
    std::vector<std::reference_wrapper<DishNode>> starting_nodes_;
    std::vector<std::reference_wrapper<DishNode>> ending_nodes_;
};

class Resource
{
public:
    int id_, count_;
    std::string name_;
};

class Process
{
public:
    int id_, time_, batch_size_;
    std::string name_;
    std::vector<int> resources_;
    int getTimeForBatch(int batch_size);
};

namespace inputs
{
    extern int cooks, studnets_veg, students_non_veg, studnets_jain;
    extern std::unordered_map<int, Dish> dish_map;
    extern std::unordered_map<int, Resource> resource_map;
    extern std::unordered_map<int, Process> process_map;

    void handle_inputfile(std::string filename);
}

#endif