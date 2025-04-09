#include "input_handler.hpp"

int cooks, studnets_veg, students_non-veg, studnets_jain;
std::unordered_map<int, string> dish_name, resource_name;
std::unordered_map<int, int> dish_count;
std::unordered_map<int, int> resources;
std::unordered_map<int, Process> process_map;
std::unordered_map<pair<int, int>, DishNode> node_map;

int Process::getTimeForBatch(int batch_size) {
    return time_*ceil(batch_size/batch_size_);
}

bool DishNode::can_start(std::unordered_set<int> process_done) {
    for(auto parent: parent_nodes_) {
        if(process_done.find(parent) == process_done.end()) {
            return false;
        }
    }
    return true;
}