#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int distance_to_closing_bracket(std::string line) {
    int nested = 1;
    for (int i = 1; i < line.length(); i++) {
        if (line.at(i) == '[') {
            nested ++;
        } else if (line.at(i) == ']') {
            nested --;
        }
        if (nested == 0) {
            return i;
        }
    }
    return -1;
}

int have_right_order(std::string line_l, std::string line_r) {
    int nest_l = 0;
    int nest_r = 0;
    int ordered = 0;
    int l;
    int r;
    for (l = 1, r = 1; l < line_l.length() && r < line_r.length(); l ++, r ++) {
        if (std::isdigit(line_l.at(l)) && std::isdigit(line_r.at(r))) {
            int advance_l = std::min(line_l.find(']', l), line_l.find(',', l)) - l;
            int advance_r = std::min(line_r.find(']', r), line_r.find(',', r)) - r;
            int int_l = std::stoi(line_l.substr(l, advance_l));
            int int_r = std::stoi(line_r.substr(r, advance_r));
            if (int_l == int_r) {
                l += advance_l;
                r += advance_r;
                continue;
            } else if (int_l < int_r) {
                return 1;
            } else if (int_l > int_r) {
                return -1;
            }
        } else if (line_l.at(l) == '[' && line_r.at(r) == '[') {
            int advance_l = distance_to_closing_bracket(line_l.substr(l)) + 1;
            std::string subline_l = line_l.substr(l, advance_l);
            int advance_r = distance_to_closing_bracket(line_r.substr(r)) + 1;
            std::string subline_r = line_r.substr(r, advance_r);
            ordered = have_right_order(subline_l, subline_r);
            if (ordered != 0) {
                return ordered;
            }
            l += advance_l;
            r += advance_r;
        } else if (std::isdigit(line_l.at(l)) && line_r.at(r) == '[') {
            int advance_l = std::min(line_l.find(']', l), line_l.find(',', l)) - l;
            std::string subline_l = "[" + line_l.substr(l, advance_l) + "]";
            int advance_r = distance_to_closing_bracket(line_r.substr(r)) + 1;
            std::string subline_r = line_r.substr(r, advance_r);
            ordered = have_right_order(subline_l, subline_r);
            if (ordered != 0) {
                return ordered;
            }
            l += advance_l;
            r += advance_r;
        } else if (line_l.at(l) == '[' && std::isdigit(line_r.at(r))) {
            int advance_l = distance_to_closing_bracket(line_l.substr(l)) + 1;
            std::string subline_l = line_l.substr(l, advance_l);
            int advance_r = std::min(line_r.find(']', r), line_r.find(',', r))- r;
            std::string subline_r = "[" + line_r.substr(r, advance_r) + "]";
            ordered = have_right_order(subline_l, subline_r);
            if (ordered != 0) {
                return ordered;
            }
            l += advance_l;
            r += advance_r;
        } else if (line_l.at(l) == ']' && l+1 == line_l.length() && r+1 != line_r.length()) {
            return 1;
        } else if (line_r.at(r) == ']' && r+1 == line_r.length() && l+1 != line_l.length()) {
            return -1;
        } else if (line_l.at(l) == ']' && l+1 == line_l.length() && line_r.at(r) == ']' & r+1) {
            continue;
        }
    }
    if (l == line_l.length() && r < line_r.length()) {
        return 1;
    } else if (r == line_r.length() && l < line_l.length()) {
        return -1;
    }
    return ordered;
}

int right_order_count(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    int index_sum = 0;
    for (int i = 0; i < lines.size(); i += 3) {
        std::string line_1 = lines[i];
        std::string line_2 = lines[i+1];
        if (have_right_order(line_1, line_2) == 1) {
            index_sum += (i / 3) + 1;
        }
    }
    return index_sum;
}


int decoder_key(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(input, line)) {
        if (line.length() > 0) {
            lines.push_back(line);
        }
    }
    lines.push_back("[[2]]");
    lines.push_back("[[6]]");
    std::sort(lines.begin(), lines.end(), [](std::string &a, std::string &b){return (have_right_order(a, b) == 1) ? true : false;});
    int divider_packet_1_index = std::find(lines.begin(), lines.end(), "[[2]]") - lines.begin() + 1;
    int divider_packet_2_index = std::find(lines.begin(), lines.end(), "[[6]]") - lines.begin() + 1;
    return divider_packet_1_index * divider_packet_2_index;
}

int main() {
    std::cout << right_order_count("test1.txt") << std::endl;
    std::cout << right_order_count("input.txt") << std::endl;
    std::cout << decoder_key("test1.txt") << std::endl;
    std::cout << decoder_key("input.txt") << std::endl;
}