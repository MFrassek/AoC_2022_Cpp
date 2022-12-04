#include <iostream>
#include <fstream>
#include <string>

int single_round_points(std::string line, int rule_set) {
    int points = 0;
    if (rule_set == 1) {
        char opponent_move = line.at(0);
        char player_move = line.at(2);
        if (player_move == 'X') {
            points += 1;
            if (opponent_move == 'A') {
                points += 3;
            } else if (opponent_move == 'B') {
                points += 0;
            } else if (opponent_move == 'C') {
                points += 6;
            }
        } else if (player_move == 'Y') {
            points += 2;
            if (opponent_move == 'A') {
                points += 6;
            } else if (opponent_move == 'B') {
                points += 3;
            } else if (opponent_move == 'C') {
                points += 0;
            }
        } else if (player_move == 'Z') {
            points += 3;
            if (opponent_move == 'A') {
                points += 0;
            } else if (opponent_move == 'B') {
                points += 6;
            } else if (opponent_move == 'C') {
                points += 3;
            }
        }
    } else {
        char opponent_move = line.at(0);
        char required_game_end = line.at(2);
        if (required_game_end == 'X') {
            points += 0;
            if (opponent_move == 'A') {
                points += 3;
            } else if (opponent_move == 'B') {
                points += 1;
            } else if (opponent_move == 'C') {
                points += 2;
            }
        } else if (required_game_end == 'Y') {
            points += 3;
            if (opponent_move == 'A') {
                points += 1;
            } else if (opponent_move == 'B') {
                points += 2;
            } else if (opponent_move == 'C') {
                points += 3;
            }
        } else if (required_game_end == 'Z') {
            points += 6;
            if (opponent_move == 'A') {
                points += 2;
            } else if (opponent_move == 'B') {
                points += 3;
            } else if (opponent_move == 'C') {
                points += 1;
            }
        }
    }
    return points;
}

int points_gained(std::string file_name, int rule_set) {
    std::ifstream input(file_name);
    std::string line;
    int total_points = 0;
    while (std::getline(input, line)) {
        total_points += single_round_points(line, rule_set);
    }
    return total_points;
}

int main() {   
    std::cout << points_gained("test1.txt", 1) << std::endl;
    std::cout << points_gained("input.txt", 1) << std::endl;
    std::cout << points_gained("test1.txt", 2) << std::endl;
    std::cout << points_gained("input.txt", 2) << std::endl;
}
