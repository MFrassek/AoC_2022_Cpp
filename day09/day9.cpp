#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <tuple>
#include <vector>

class Node {
    public:
        int x = 0;
        int y = 0;
};

class Rope {
    public:
        Node head = Node();
        std::vector<Node> tails;

        Rope(int tail_count) {
            for (int i = 0; i < tail_count; i ++) {
                tails.push_back(Node());
            }
        }

        void follow(Node leader, Node& follower) {
            if (leader.x == follower.x & leader.y == follower.y + 2) {
                follower.y ++;
            } else if (leader.x == follower.x & leader.y == follower.y - 2) {
                follower.y --;
            } else if (leader.x == follower.x + 2 & leader.y == follower.y) {
                follower.x ++;
            } else if (leader.x == follower.x - 2 & leader.y == follower.y) {
                follower.x --;
            } else if (leader.x >= follower.x + 2 & leader.y >= follower.y + 1) {
                follower.x ++;
                follower.y ++;
            } else if (leader.x >= follower.x + 2 & leader.y <= follower.y - 1) {
                follower.x ++;
                follower.y --;
            } else if (leader.x <= follower.x - 2 & leader.y >= follower.y + 1) {
                follower.x --;
                follower.y ++;
            } else if (leader.x <= follower.x - 2 & leader.y <= follower.y - 1) {
                follower.x --;
                follower.y --;
            } else if (leader.x >= follower.x + 1 & leader.y >= follower.y + 2) {
                follower.x ++;
                follower.y ++;
            } else if (leader.x <= follower.x - 1 & leader.y >= follower.y + 2) {
                follower.x --;
                follower.y ++;
            } else if (leader.x >= follower.x + 1 & leader.y <= follower.y - 2) {
                follower.x ++;
                follower.y --;
            } else if (leader.x <= follower.x - 1 & leader.y <= follower.y - 2) {
                follower.x --;
                follower.y --;
            }
        }
        void move_up() {
            head.y ++;
            follow(head, tails[0]);
            for (int i= 0; i < tails.size() - 1; i ++){
                follow(tails[i], tails[i+1]);
            }
        }
        void move_down() {
            head.y --;
            follow(head, tails[0]);
            for (int i= 0; i < tails.size() - 1; i ++){
                follow(tails[i], tails[i+1]);
            }
        }
        void move_right() {
            head.x ++;
            follow(head, tails[0]);
            for (int i= 0; i < tails.size() - 1; i ++){
                follow(tails[i], tails[i+1]);
            }
        }
        void move_left() {
            head.x --;
            follow(head, tails[0]);
            for (int i= 0; i < tails.size() - 1; i ++){
                follow(tails[i], tails[i+1]);
            }
        }
        std::tuple<int, int> tail_position() {
            return {tails[tails.size()-1].x, tails[tails.size()-1].y};
        }

};

int tail_position_count(std::string file_name, int tail_count) {
    std::ifstream input(file_name);
    std::string line;
    std::set<std::tuple<int, int>> visited_by_tail;
    Rope rope = Rope(tail_count);
    while (std::getline(input, line)) {
        std::string direction = line.substr(0, 1);
        int steps = std::stoi(line.substr(2, line.length() - 2));
        for (int i = 0; i < steps; i ++) {
            if (direction == "U") {
                rope.move_up();
            } else if (direction == "D") {
                rope.move_down();
            } else if (direction == "L") {
                rope.move_left();
            } else if (direction == "R") {
                rope.move_right();
            }
            visited_by_tail.insert(rope.tail_position());
        }
    }
    return visited_by_tail.size();
}

int main() {
    std::cout << tail_position_count("test1.txt", 1) << std::endl;
    std::cout << tail_position_count("test1.txt", 9) << std::endl;
    std::cout << tail_position_count("test2.txt", 9) << std::endl;
    std::cout << tail_position_count("input.txt", 1) << std::endl;
    std::cout << tail_position_count("input.txt", 9) << std::endl;
}