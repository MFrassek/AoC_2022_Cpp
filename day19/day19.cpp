#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <unordered_set>


std::unordered_map<std::string, int> resources_after_production(
        std::unordered_map<std::string, int> available_resources, std::unordered_map<std::string, int> &available_robots) {
    for (auto &robot_name_pair : available_robots) {
        std::string robot_name = robot_name_pair.first;
        int robot_count = robot_name_pair.second;
        available_resources[robot_name] += robot_count;
    }
    return available_resources;
}

std::unordered_map<std::string, int> resources_after_building(
        std::string robot_name,
        std::unordered_map<std::string, std::unordered_map<std::string, int>> &robot_costs,
        std::unordered_map<std::string, int> available_resources) {
    for (auto &material_cost_pair : robot_costs[robot_name]) {
        std::string material_name = material_cost_pair.first;
        int material_amount = material_cost_pair.second;
        available_resources[material_name] -= material_amount;
    }
    return available_resources;
}

std::unordered_map<std::string, int> robots_after_building(std::string robot_name, std::unordered_map<std::string, int> available_robots) {
    available_robots[robot_name] += 1;
    return available_robots;
}

std::unordered_map<std::string, int> maximum_material_costs(std::unordered_map<std::string, std::unordered_map<std::string, int>> &robot_costs) {
    std::unordered_map<std::string, int> maximum_material_required;
    for (auto &robot_cost_pair : robot_costs) {
        std::unordered_map<std::string, int> material_costs = robot_cost_pair.second;
        for (auto &material_cost_pair : material_costs) {
            std::string material_name = material_cost_pair.first;
            int material_amount = material_cost_pair.second;
            if (material_amount > maximum_material_required[material_name]) {
                maximum_material_required[material_name] = material_amount;
            }
        }
    }
    return maximum_material_required;
}

int max_geode_production_capacity(int remaining_time, std::unordered_map<std::string, int> &available_robots) {
    int total = available_robots["geode"] * remaining_time;
    total += (remaining_time * (remaining_time - 1)) / 2;
    return total;
}

int max_geode_production_capacity_based_on_obsidian_production(
        int remaining_time,
        std::unordered_map<std::string, std::unordered_map<std::string, int>> &robot_costs,
        std::unordered_map<std::string, int> available_resources,
        std::unordered_map<std::string, int> &available_robots) {
    // Calculate how many geode robots could be built if obsidian was the only concern and what they could produce
    int geode_robot_obsidian_cost = robot_costs["geode"]["obsidian"];
    int obsidian_count = available_resources["obsidian"];
    int hypothetical_obsidian_robot_count = available_robots["obsidian"];
    int hypothetical_geode_robot_count = available_robots["geode"];
    int total = 0;
    for (int i = 0; i < remaining_time; i++) {
        total += hypothetical_geode_robot_count;
        if (obsidian_count >= geode_robot_obsidian_cost) {
            // Build geode robot
            obsidian_count += hypothetical_obsidian_robot_count;
            obsidian_count -= geode_robot_obsidian_cost;
            hypothetical_geode_robot_count ++;
        } else {
            // Build obsidian robot
            obsidian_count += hypothetical_obsidian_robot_count;
            hypothetical_obsidian_robot_count ++;
        }
    }
    return total;
}

void maximum_geodes(
        int &maximum_geodes_found_so_far,
        std::unordered_map<std::string, std::unordered_map<std::string, int>> &robot_costs,
        int remaining_time,
        std::unordered_map<std::string, int> &maximum_material_required,
        std::unordered_map<std::string, int> available_resources,
        std::unordered_map<std::string, int> available_robots) {
    std::unordered_set<std::string> affordable_robots;
    if (remaining_time == 0) {
        if (available_resources["geode"] > maximum_geodes_found_so_far) {
            maximum_geodes_found_so_far = available_resources["geode"];
        }
        return;
    }
    // if (available_resources["geode"] + max_geode_production_capacity(remaining_time, available_robots) <= maximum_geodes_found_so_far) {
    if (available_resources["geode"] + max_geode_production_capacity_based_on_obsidian_production(
            remaining_time, robot_costs, available_resources, available_robots) <= maximum_geodes_found_so_far) {
        return;
    }
    for (auto &robot_cost_pair : robot_costs) {
        std::string robot_name = robot_cost_pair.first;
        std::unordered_map<std::string, int> material_costs = robot_cost_pair.second;
        bool is_affordable = true;
        for (auto &material_cost_pair : material_costs) {
            std::string material_name = material_cost_pair.first;
            int material_amount = material_cost_pair.second;
            if (material_amount > available_resources[material_name]) {
                is_affordable = false;
            }
        }
        if (is_affordable) {
            affordable_robots.insert(robot_name);
            // std::cout << robot_name << "\n";
        }
    }
    if (affordable_robots.find("geode") != affordable_robots.end()) {
        maximum_geodes(maximum_geodes_found_so_far, robot_costs, remaining_time - 1, maximum_material_required,
            resources_after_production(resources_after_building("geode", robot_costs, available_resources), available_robots), 
            robots_after_building("geode", available_robots));
    }
    else {
        for (std::string affordable_robot_name : affordable_robots) {
            if (maximum_material_required[affordable_robot_name] > available_robots[affordable_robot_name]) {
                maximum_geodes(maximum_geodes_found_so_far, robot_costs, remaining_time - 1, maximum_material_required,
                    resources_after_production(resources_after_building(affordable_robot_name, robot_costs, available_resources), available_robots), 
                   robots_after_building(affordable_robot_name, available_robots));
            }
        }
        maximum_geodes(maximum_geodes_found_so_far, robot_costs, remaining_time - 1, maximum_material_required,
                resources_after_production(available_resources, available_robots), available_robots);
    }        
    return;
}

int maximum_geodes_of_line(std::string blueprint, int initial_time) {
    int current_maximum = 0;
    int ore_robot_ore_cost_start = blueprint.find("costs ") + 6;
    int ore_robot_ore_cost_end = blueprint.find(" ", ore_robot_ore_cost_start);
    int ore_robot_ore_cost = std::stoi(blueprint.substr(ore_robot_ore_cost_start, ore_robot_ore_cost_end - ore_robot_ore_cost_start));
    int clay_robot_ore_cost_start = blueprint.find("costs ", ore_robot_ore_cost_end) + 6;
    int clay_robot_ore_cost_end = blueprint.find(" ", clay_robot_ore_cost_start);
    int clay_robot_ore_cost = std::stoi(blueprint.substr(clay_robot_ore_cost_start, clay_robot_ore_cost_end - clay_robot_ore_cost_start));
    int obsidian_robot_ore_cost_start = blueprint.find("costs ", clay_robot_ore_cost_end) + 6;
    int obsidian_robot_ore_cost_end = blueprint.find(" ", obsidian_robot_ore_cost_start);
    int obsidian_robot_ore_cost = std::stoi(blueprint.substr(obsidian_robot_ore_cost_start, obsidian_robot_ore_cost_end - obsidian_robot_ore_cost_start));
    int obsidian_robot_clay_cost_start = blueprint.find("and ", obsidian_robot_ore_cost_end) + 4;
    int obsidian_robot_clay_cost_end = blueprint.find(" ", obsidian_robot_clay_cost_start);
    int obsidian_robot_clay_cost = std::stoi(blueprint.substr(obsidian_robot_clay_cost_start, obsidian_robot_clay_cost_end - obsidian_robot_clay_cost_start));
    int geode_robot_ore_cost_start = blueprint.find("costs ", obsidian_robot_clay_cost_end) + 6;
    int geode_robot_ore_cost_end = blueprint.find(" ", geode_robot_ore_cost_start);
    int geode_robot_ore_cost = std::stoi(blueprint.substr(geode_robot_ore_cost_start, geode_robot_ore_cost_end - geode_robot_ore_cost_start));
    int geode_robot_obsidian_cost_start = blueprint.find("and ", geode_robot_ore_cost_end) + 4;
    int geode_robot_obsidian_cost_end = blueprint.find(" ", geode_robot_obsidian_cost_start);
    int geode_robot_obsidian_cost = std::stoi(blueprint.substr(geode_robot_obsidian_cost_start, geode_robot_obsidian_cost_end - geode_robot_obsidian_cost_start));
    int maximum_geodes_found_so_far = 0;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> robot_costs = {
        {"ore", {{"ore", ore_robot_ore_cost}}},
        {"clay", {{"ore", clay_robot_ore_cost}}},
        {"obsidian", {{"ore", obsidian_robot_ore_cost}, {"clay", obsidian_robot_clay_cost}}},
        {"geode", {{"ore", geode_robot_ore_cost}, {"obsidian", geode_robot_obsidian_cost}}},
        };
    std::unordered_map<std::string, int> available_resources = {{"ore", 0}, {"clay", 0}, {"obsidian", 0}, {"geode", 0}};
    std::unordered_map<std::string, int> available_robots = {{"ore", 1}, {"clay", 0}, {"obsidian", 0}, {"geode", 0}};
    std::unordered_map<std::string, int> maximum_material_required = maximum_material_costs(robot_costs);
    maximum_geodes(maximum_geodes_found_so_far, robot_costs, initial_time, maximum_material_required, available_resources, available_robots);
    return maximum_geodes_found_so_far;
}

int quality_level(std::string file_name, bool is_short) {
    std::ifstream input(file_name);
    std::string line;
    if (is_short) {
        int total = 0;
        int id = 1;
        while (std::getline(input, line)) {
            total += id * maximum_geodes_of_line(line, 24);
            id++;
        }
        return total;
    } else {
        int total = 1;
        int count = 0;
        while (std::getline(input, line) && count < 3) {
            total *= maximum_geodes_of_line(line, 32);
            count ++;
        }
        return total;
    }
}


int main() {
    std::cout << quality_level("test1.txt", true) << "\n";
    std::cout << quality_level("input.txt", true) << "\n";
    std::cout << quality_level("test1.txt", false) << "\n";
    std::cout << quality_level("input.txt", false) << "\n";
    // 24 minutes
    // 31740
}