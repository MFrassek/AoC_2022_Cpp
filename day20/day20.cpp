#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Node {
    public:
        long long int value;
        Node* previous;
        Node* next;

        Node(long long int value) {
            this->value = value;
        }

        void connect_to_previous(Node* previous_node) {
            this->previous = previous_node;
            previous_node->next = this;
        }

        void connect_to_next(Node* next_node) {
            this->next = next_node;
            next_node->previous = this;
        }

        void move(long long int list_length_modulo) {
            if (this->value == 0) {
                return;
            }
            Node* original_previous = this->previous;
            Node* original_next = this->next;
            original_previous->connect_to_next(original_next);
            if (this->value > 0) {
                Node* new_next = this->next;
                for (long long int i = 0; i < this->value % (list_length_modulo-1); i++) {
                    new_next = new_next->next;
                }
                this->connect_to_previous(new_next->previous);
                this->connect_to_next(new_next);
            } else {
                Node* new_previous = this->previous;
                for (long long int i = 0; i > this->value % (list_length_modulo-1); i--) {
                    new_previous = new_previous->previous;
                }
                this->connect_to_next(new_previous->next);
                this->connect_to_previous(new_previous);
            }
        }
};

class NodeSequence {
    public:
        std::vector<Node*> nodes;

        NodeSequence(std::vector<long long int> node_values, int multiplier) {
            for (long long int value : node_values) {
                Node* node = new Node(value * multiplier);
                if (nodes.size() != 0) {
                    nodes[nodes.size() - 1]->connect_to_next(node);
                }
                nodes.push_back(node);
            }
            nodes[nodes.size() - 1]->connect_to_next(nodes[0]);
        }

        void mix_nodes() {
            for (Node* node : nodes) {
                node->move(nodes.size());
            }
        }
        
        long long int nth_value_from_zero(int n) {
            for (Node* node : nodes) {
                if (node->value == 0) {
                    Node* current_node = node;
                    for (int i = 0; i < n; i++) {
                        current_node = current_node->next;
                    }
                    return current_node->value;
                }
            }
            return -1;
        }


};
long long int groove_coordinate_sum(std::string file_name, int multiplier, long long int mixing_count) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<long long int> node_values;
    while (std::getline(input, line)) {
        node_values.push_back(std::stoi(line));
    }
    NodeSequence node_sequence = NodeSequence(node_values, multiplier);
    for (int i = 0; i < mixing_count; i++) {
        node_sequence.mix_nodes();
    }
    return node_sequence.nth_value_from_zero(1000) + node_sequence.nth_value_from_zero(2000) +node_sequence.nth_value_from_zero(3000);
}

int main() {
    std::cout << groove_coordinate_sum("test1.txt", 1, 1) << std::endl;
    std::cout << groove_coordinate_sum("input.txt", 1, 1) << std::endl;
    std::cout << groove_coordinate_sum("test1.txt", 811589153, 10) << std::endl;
    std::cout << groove_coordinate_sum("input.txt", 811589153, 10) << std::endl;
}
