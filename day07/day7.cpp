#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <optional>

class File {
    public:
        std::string name;
        int size;

        File(std::string name, int size) {
            this->name = name;
            this->size = size;
        }
};

class Directory {
    public:
        std::string name;
        std::vector<Directory*> subdirectories;
        std::vector<File*> files;
        Directory* parent;

        Directory(std::string name) {
            this->name = name;
            subdirectories = {};
            files = {};

        }
        void add_subdirectory(Directory* subdirectory) {
            subdirectories.push_back(subdirectory);
            subdirectory->parent = this;
            //std::cout << (*this).name << std::endl;
            //std::cout << (subdirectory).name << std::endl;
        }
        void add_file(File* file) {
            files.push_back(file);
        }
        auto child(std::string name) {
            for (Directory* subdirectory : subdirectories) {
                if (subdirectory->name == name) {
                    return subdirectory;
                }
            }
            return subdirectories[0];
        }
        int size() {
            int total = 0;
            for (auto &subdirectory : subdirectories) {
                total += subdirectory->size();
            }
            for (auto &file : files) {
                total += file->size;
            }
            return total;
        }
};


int directory_size(std::string file_name, bool small_dirs) {
    std::ifstream input(file_name);
    std::string line;
    std::getline(input, line);
    Directory root = Directory("/");
    std::string current_directory_name = "/";
    Directory* current_directory = &root;
    while (std::getline(input, line)) {
        if (line == "$ ls") {
            while (std::getline(input, line)) {
                if (line.substr(0, 3) == "dir") {
                    std::string dir_name = current_directory_name + "/" + line.substr(4, line.length() - 4);
                    Directory* dir = new Directory(dir_name);
                    current_directory->add_subdirectory(dir);
                }  else {
                    int space_index = line.find(" ");
                    int size = std::stoi(line.substr(0, space_index));
                    std::string file_name = line.substr(space_index + 1, line.length() - space_index - 1);
                    current_directory->add_file(new File(file_name, size));
                }
                if (input.peek() == '$') {
                    break;
                }
            } 
        } else if (line.substr(0, 7) == "$ cd ..") {
            current_directory_name = current_directory->parent->name;
            current_directory = current_directory->parent;
        } else if (line.substr(0, 4) == "$ cd") {
            std::string child_directory_name = line.substr(5, line.length() - 5);
            current_directory_name += "/" + child_directory_name;
            current_directory = current_directory->child(current_directory_name);
        }
    }
    std::vector<Directory*> queue;
    queue.push_back(&root);
    if (small_dirs) {
        int total = 0;
        while (queue.size() > 0) {
            Directory* dir = queue[queue.size() - 1];
            int size = dir->size();
            if (size <= 100000) {
                total += size;
            }
            queue.pop_back();
            for (Directory* child_dir : dir->subdirectories) {
                queue.push_back(child_dir);
            }
        }
        return total;
    } else {
        int root_size = root.size();
        int remaining_space = 70000000 - root_size;
        int needed_space = 30000000 - remaining_space;
        int smallest_valid_for_delete = root_size;
        while (queue.size() > 0) {
            Directory* dir = queue[queue.size() - 1];
            int size = dir->size();
            if (size >= needed_space & size < smallest_valid_for_delete) {
                smallest_valid_for_delete = size;
            }
            queue.pop_back();
            for (Directory* child_dir : dir->subdirectories) {
                queue.push_back(child_dir);
            }
        }
        return smallest_valid_for_delete;
    }
}

int main() {
    std::cout << directory_size("test1.txt", true) << std::endl;
    std::cout << directory_size("test1.txt", false) << std::endl;
    std::cout << directory_size("input.txt", true) << std::endl;
    std::cout << directory_size("input.txt", false) << std::endl;
}