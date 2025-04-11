#include "input_handler.hpp"
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <iostream>

int Process::getTimeForBatch(int batch_size)
{
    return time_ * ceil(batch_size / batch_size_);
}

bool DishNode::can_start()
{
    for (auto &parent : parent_nodes_)
    {
        if (!parent.get().is_done)
        {
            return false;
        }
    }
    return true;
}

void DishNode::add_parent(DishNode &node)
{
    parent_nodes_.push_back(node);
}

void DishNode::add_child(DishNode &node)
{
    child_nodes_.push_back(node);
}

namespace inputs
{
    int cooks, studnets_veg, students_non_veg, studnets_jain;
    std::unordered_map<int, Dish> dish_map;
    std::unordered_map<int, Resource> resource_map;
    std::unordered_map<int, Process> process_map;

    void handle_inputfile(std::string filename)
    {
        std::ifstream input_file(filename);
        if (!input_file.is_open())
        {
            std::cout << "File not found";
            return;
        }
        std::stringstream ss;
        ss << input_file.rdbuf();
        input_file.close();
        int count;
        ss.ignore(std::numeric_limits<int>::max(), 'M');
        ss >> cooks;
        ss.ignore(std::numeric_limits<int>::max(), 'S');
        ss >> studnets_veg >> students_non_veg >> studnets_jain;
        ss.ignore(std::numeric_limits<int>::max(), 'C');
        ss >> count;
        resource_map.reserve(count);
        while (count-- > 0)
        {
            Resource temp_resource;
            ss >> temp_resource.id_ >> temp_resource.name_ >> temp_resource.count_;
        }
        ss.ignore(std::numeric_limits<int>::max(), 'C');
        ss >> count;
        process_map.reserve(count);
        while (count-- > 0)
        {
            Process pr;
            ss >> pr.id_ >> pr.name_ >> pr.time_ >> pr.batch_size_;
            int num_resources;
            ss >> num_resources;
            pr.resources_.reserve(num_resources);
            while (num_resources-- > 0)
            {
                int temp;
                ss >> temp;
                pr.resources_.push_back(temp);
            }
        }
        ss.ignore(std::numeric_limits<int>::max(), 'C');
        ss >> count;
        dish_map.reserve(count);
        while (count-- > 0)
        {
            int node_count;
            bool veg, non_veg, jain;
            Dish temp_dish;
            ss >> temp_dish.id_ >> temp_dish.name_ >> veg >> non_veg >> jain >> node_count;
            temp_dish.student_count_ += veg ? studnets_veg : 0;
            temp_dish.student_count_ += non_veg ? students_non_veg : 0;
            temp_dish.student_count_ += jain ? studnets_jain : 0;
            temp_dish.graph_.reserve(node_count);
            while (node_count-- > 0)
            {
                DishNode node;
                ss >> node.node_id >> node.node_process_id_;
                temp_dish.graph_[node.node_id] = node;
            }
            ss >> node_count;
            while (node_count-- > 0)
            {
                int start, end;
                ss >> start >> end;
                temp_dish.graph_[start].child_nodes_.push_back(temp_dish.graph_[end]);
                temp_dish.graph_[end].parent_nodes_.push_back(temp_dish.graph_[start]);
            }
        }
    }
}
