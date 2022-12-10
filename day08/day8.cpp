#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>

std::vector<std::vector<int>> tree_grid(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::vector<int>> tree_grid;
    while (std::getline(input, line)) {
        std::vector<int> tree_line;
        for (char &c : line) {
            tree_line.push_back(int(c)-48);
        }
        tree_grid.push_back(tree_line);
    }
    return tree_grid;
}


int visible_tree_count(std::string file_name) {
    std::vector<std::vector<int>> trees = tree_grid(file_name);
    std::set<std::tuple<int, int>> visible_trees;
    int row_count = trees.size();
    int col_count = trees[0].size();
    for (int r = 0; r < row_count; r ++) {
        int highest_tree_left = -1;
        for (int c = 0; c < col_count; c ++) {
            if (trees[r][c] > highest_tree_left) {
                highest_tree_left = trees[r][c];
                visible_trees.insert({r, c});
            }
        }
        int highest_tree_right = -1;
        for (int c = col_count - 1; c >= 0; c --) {
            if (trees[r][c] > highest_tree_right) {
                highest_tree_right = trees[r][c];
                visible_trees.insert({r, c});
            }
        }
    }
    for (int c = 0; c < col_count; c ++) {
        int highest_tree_top = -1;
        for (int r = 0; r < row_count; r ++) {
            if (trees[r][c] > highest_tree_top) {
                highest_tree_top = trees[r][c];
                visible_trees.insert({r, c});
            }
        }
        int highest_tree_bottom = -1;
        for (int r = row_count - 1; r >= 0; r --) {
            if (trees[r][c] > highest_tree_bottom) {
                highest_tree_bottom = trees[r][c];
                visible_trees.insert({r, c});
            }
        }
    }
    return visible_trees.size();

}

int scenic_score(std::vector<std::vector<int>> trees, int target_row, int target_col) {
    int row_count = trees.size();
    int col_count = trees[0].size();
    int score = 1;
    int tree_height = trees[target_row][target_col];
    int top_visible = 0;
    for (int r = target_row - 1; r >= 0; r --) {
        top_visible ++;
        if (trees[r][target_col] >= tree_height) {
            break;
        }
    }
    int bottom_visible = 0;
    for (int r = target_row + 1; r < row_count; r ++) {
        bottom_visible ++;
        if (trees[r][target_col] >= tree_height) {
            break;
        }
    }
    int left_visible = 0;
    for (int c = target_col - 1; c >= 0; c --) {
        left_visible ++;
        if (trees[target_row][c] >= tree_height) {
            break;
        }
    }
    int right_visible = 0;
    for (int c = target_col + 1; c < col_count; c ++) {
        right_visible ++;
        if (trees[target_row][c] >= tree_height) {
            break;
        }
    }

    return top_visible * bottom_visible * left_visible * right_visible;
}

int best_scenic_view(std::string file_name) {
    std::vector<std::vector<int>> trees = tree_grid(file_name);
    int row_count = trees.size();
    int col_count = trees[0].size();
    int best_score = 0;
    for (int r = 0; r < row_count; r ++) {
        for (int c = 0; c < col_count; c ++) {
            int score = scenic_score(trees, r, c);
            if (score > best_score) {
                best_score = score;
            }
        }
    }
    return best_score;
}

int main() {
    std::cout << visible_tree_count("test1.txt") << std::endl;
    std::cout << visible_tree_count("input.txt") << std::endl;
    std::cout << best_scenic_view("test1.txt") << std::endl;
    std::cout << best_scenic_view("input.txt") << std::endl;
}