#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Map {
    public:
        std::vector<std::string> rows;
        int row_count;
        int col_count;
        int row_position;
        int col_position;
        char orientation;

        Map(std::vector<std::string> rows) {
            this->col_count = 0;
            for (std::string row : rows) {
                if (row.length() > this->col_count) {
                    this->col_count = row.length();
                }
            }
            for (std::string row : rows) {
                std::string temp = row;
                for (int i = row.length(); i < this->col_count; i++) {
                    temp += " ";
                }
                this->rows.push_back(temp);
            }
            this->row_count = rows.size();
            this->row_position = 0;
            this->col_position = -1;
            for (int i = 0; i < col_count; i++) {
                if (rows[0].at(i) == '.') {
                    this->col_position = i;
                    break;
                }
            }
            this->orientation = 'r';
        }

        void turn(char turning_direction) {
            if (turning_direction == 'R') {
                if (this->orientation == 'r') {
                    this->orientation = 'd';
                } else if (this->orientation == 'd') {
                    this->orientation = 'l';
                } else if (this->orientation == 'l') {
                    this->orientation = 'u';
                } else {
                    this->orientation = 'r';
                }
            } else {
                if (this->orientation == 'r') {
                    this->orientation = 'u';
                } else if (this->orientation == 'u') {
                    this->orientation = 'l';
                } else if (this->orientation == 'l') {
                    this->orientation = 'd';
                } else {
                    this->orientation = 'r';
                }
            }
        }

        void print_pos() {
            std::cout << this->row_position << " " << this->col_position << std::endl;
        }

        int position_password() {
            int facing;
            if (this->orientation == 'r') {
                facing = 0;
            } else if (this->orientation == 'd') {
                facing = 1;
            } else if (this->orientation == 'l') {
                facing = 2;
            } else {
                facing = 3;
            }
            return (this->row_position + 1) * 1000 + (this->col_position + 1) * 4 + facing;
        }
};

class FlatMap : public Map {
    public:
        FlatMap(std::vector<std::string> rows) : Map(rows) {
        }

        void walk(int distance) {
            int skip_count = 0;
            if (this->orientation == 'r') {
                int last_accepable_col_position = this->col_position;
                for (int i = 1; i <= distance + skip_count; i++) {
                    if (this->rows[this->row_position].at((this->col_position + i) % this->col_count) == '#') {
                        this->col_position = last_accepable_col_position;
                        return;
                    } else if (this->rows[this->row_position].at((this->col_position + i) % this->col_count) == '.') {
                        last_accepable_col_position = (this->col_position + i) % this->col_count;
                    } else if (this->rows[this->row_position].at((this->col_position + i) % this->col_count) = ' ') {
                        skip_count++;
                    }
                }
                this->col_position = last_accepable_col_position;
            } else if (this->orientation == 'l') {
                int last_accepable_col_position = this->col_position;
                for (int i = -1; i >= -distance - skip_count; i--) {
                    if (this->rows[this->row_position].at((this->col_position + i + this->col_count) % this->col_count) == '#') {
                        this->col_position = last_accepable_col_position;
                        return;
                    } else if (this->rows[this->row_position].at((this->col_position + i + this->col_count) % this->col_count) == '.') {
                        last_accepable_col_position = (this->col_position + i + this->col_count) % this->col_count;
                    } else if (this->rows[this->row_position].at((this->col_position + i + this->col_count) % this->col_count) = ' ') {
                        skip_count++;
                    }
                }
                this->col_position = last_accepable_col_position;
            } else if (this->orientation == 'd') {
                int last_accepable_row_position = this->row_position;
                for (int i = 1; i <= distance + skip_count; i++) {
                    if (this->rows[(this->row_position + i) % this->row_count].at(this->col_position) == '#') {
                        this->row_position = last_accepable_row_position;
                        return;
                    } else if (this->rows[(this->row_position + i) % this->row_count].at(this->col_position) == '.') {
                        last_accepable_row_position = (this->row_position + i) % this->row_count;
                    } else if (this->rows[(this->row_position + i) % this->row_count].at(this->col_position) = ' ') {
                        skip_count++;
                    }
                }
                this->row_position = last_accepable_row_position;
            } else if (this->orientation == 'u') {
                int last_accepable_row_position = this->row_position;
                for (int i = -1; i >= -distance - skip_count; i--) {
                    if (this->rows[(this->row_position + i + this->row_count) % this->row_count].at(this->col_position) == '#') {
                        this->row_position = last_accepable_row_position;
                        return;
                    } else if (this->rows[(this->row_position + i + this->row_count) % this->row_count].at(this->col_position) == '.') {
                        last_accepable_row_position = (this->row_position + i + this->row_count) % this->row_count;
                    } else if (this->rows[(this->row_position + i + this->row_count) % this->row_count].at(this->col_position) = ' ') {
                        skip_count++;
                    }
                }
                this->row_position = last_accepable_row_position;
            }
        }
};

class CubeMap : public Map {
    public:
        CubeMap(std::vector<std::string> rows) : Map(rows) {
        }

        void walk(int distance) {
            int tentative_row_position;
            int tentative_col_position;
            char tentative_orientation;
            for (int i = 1; i <= distance; i++) {
                if (this->orientation == 'r') {
                    if ((this->row_position >= 0 && this->row_position <= 49) && this->col_position == 149) {
                        tentative_row_position = 149 - this->row_position;
                        tentative_col_position = 99;
                        tentative_orientation = 'l';
                    } else if ((this->row_position >= 50 && this->row_position <= 99) && this->col_position == 99) {
                        tentative_row_position = 49;
                        tentative_col_position = this->row_position + 50;
                        tentative_orientation = 'u';
                    } else if ((this->row_position >= 100 && this->row_position <= 149) && this->col_position == 99) {
                        tentative_row_position = 149 - this->row_position;
                        tentative_col_position = 149;
                        tentative_orientation = 'l';
                    } else if ((this->row_position >= 150 && this->row_position <= 199) && this->col_position == 49) {
                        tentative_row_position = 149;
                        tentative_col_position = this->row_position - 100;
                        tentative_orientation = 'u';
                    } else {
                        tentative_row_position = this->row_position;
                        tentative_col_position = this->col_position + 1;
                        tentative_orientation = this->orientation;
                    }
                } else if (this->orientation == 'l') {
                    if ((this->row_position >= 0 && this->row_position <= 49) && this->col_position == 50) {
                        tentative_row_position = 149 - this->row_position;
                        tentative_col_position = 0;
                        tentative_orientation = 'r';
                    } else if ((this->row_position >= 50 && this->row_position <= 99) && this->col_position == 50) {
                        tentative_row_position = 100;
                        tentative_col_position = this->row_position - 50;
                        tentative_orientation = 'd';
                    } else if ((this->row_position >= 100 && this->row_position <= 149) && this->col_position == 0) {
                        tentative_row_position = 149 - this->row_position;
                        tentative_col_position = 50;
                        tentative_orientation = 'r';
                    } else if ((this->row_position >= 150 && this->row_position <= 199) && this->col_position == 0) {
                        tentative_row_position = 0;
                        tentative_col_position = this->row_position - 100;
                        tentative_orientation = 'd';
                    } else {
                        tentative_row_position = this->row_position;
                        tentative_col_position = this->col_position - 1;
                        tentative_orientation = this->orientation;
                    }
                } else if (this->orientation == 'd') {
                    if ((this->col_position >= 0 && this->col_position <= 49) && this->row_position == 199) {
                        tentative_row_position = 0;
                        tentative_col_position = this->col_position + 100;
                        tentative_orientation = 'd';
                    } else if ((this->col_position >= 50 && this->col_position <= 99) && this->row_position == 149) {
                        tentative_row_position = this->col_position + 100;
                        tentative_col_position = 49;
                        tentative_orientation = 'l';
                    } else if ((this->col_position >= 100 && this->col_position <= 149) && this->row_position == 49) {
                        tentative_row_position = this->col_position - 50;
                        tentative_col_position = 99;
                        tentative_orientation = 'l';
                    } else {
                        tentative_row_position = this->row_position + 1;
                        tentative_col_position = this->col_position;
                        tentative_orientation = this->orientation;
                    }
                } else if (this->orientation == 'u') {
                    if ((this->col_position >= 0 && this->col_position <= 49) && this->row_position == 100) {
                        tentative_row_position = this->col_position + 50;
                        tentative_col_position = 50;
                        tentative_orientation = 'r';
                    } else if ((this->col_position >= 50 && this->col_position <= 99) && this->row_position == 0) {
                        tentative_row_position = this->col_position + 100;
                        tentative_col_position = 0;
                        tentative_orientation = 'r';
                    } else if ((this->col_position >= 100 && this->col_position <= 149) && this->row_position == 0) {
                        tentative_row_position = 199;
                        tentative_col_position = this->col_position - 100;
                        tentative_orientation = 'u';
                    } else {
                        tentative_row_position = this->row_position - 1;
                        tentative_col_position = this->col_position;
                        tentative_orientation = this->orientation;
                    }
                }
                if (this->rows[tentative_row_position].at(tentative_col_position) == '#') {
                    break;
                } else {
                    this->row_position = tentative_row_position;
                    this->col_position = tentative_col_position;
                    this->orientation = tentative_orientation;
                }
            }
        }
};

int final_map_password(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> rows;
    while (std::getline(input, line)) {
        if (line.length() == 0) {
            break;
        }
        rows.push_back(line);
    }
    FlatMap map = FlatMap(rows);
    std::getline(input, line);
    std::string temp = "";
    for (int i = 0; i < line.length(); i++) {
        if (line.at(i) == 'R' or line.at(i) == 'L') {
            map.walk(std::stoi(temp));
            temp = "";
            map.turn(line.at(i));
        } else {
            temp += line.at(i);
        }
    }
    map.walk(std::stoi(temp));
    return map.position_password();
}

int final_cube_password(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> rows;
    while (std::getline(input, line)) {
        if (line.length() == 0) {
            break;
        }
        rows.push_back(line);
    }
    CubeMap cube = CubeMap(rows);
    std::getline(input, line);
    std::string temp = "";
    for (int i = 0; i < line.length(); i++) {
        if (line.at(i) == 'R' or line.at(i) == 'L') {
            cube.walk(std::stoi(temp));
            temp = "";
            cube.turn(line.at(i));
        } else {
            temp += line.at(i);
        }
    }
    cube.walk(std::stoi(temp));
    
    return cube.position_password();
}

int main() {
    std::cout << final_map_password("test1.txt") << std::endl;
    std::cout << final_map_password("input.txt") << std::endl;
    std::cout << final_cube_password("input.txt") << std::endl;
}