#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct ListNode {
    int data;
    ListNode* next;
    ListNode(int d) : data(d), next(nullptr) {}
};

struct MyList {
    ListNode* head;
    MyList() : head(nullptr) {}

    static ListNode* create_list(const int data) {
        auto* newNode = new ListNode(data);
        newNode->next = newNode;
        return newNode;
    }

    void push_back(const int data) {
        if (head == nullptr) {
            head = create_list(data);
        } else {
            auto* newNode = new ListNode(data);
            ListNode* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->next = head;
        }
    }

    void print_list() const {
        if (head == nullptr) {
            return;
        }
        ListNode* temp = head;
        do {
            cout << temp->data << " ";
            temp = temp->next;
        } while (temp != head);
        // cout << "\n";
    }

    int count_list() const {
        if (head == nullptr) {
            return 0;
        }
        int count = 1;
        ListNode* temp = head;
        while (temp->next != head) {
            temp = temp->next;
            count++;
        }
        return count;
    }

    void delete_by_value(const int key) {
        if (head == nullptr) {
            return;
        }
        else if (head->next == head && head->data == key) {
            delete head;
            head = nullptr;
            return;
        }
        ListNode* temp = head;
        ListNode* prev = nullptr;
        int count = count_list();
        while (temp->data == key) {
            prev = temp;
            temp = temp->next;
            delete prev;
            count--;
        }
        if (count == 0) {
            head = nullptr;
            return;
        }
        head = temp;
        for (int i = 1; i < count; i++) {
            if (temp->next->data == key) {
                prev = temp->next;
                temp->next = prev->next;
                delete prev;
            } else {
                temp = temp->next;
            }
        }
        temp->next = head;
    }
};

struct total_info {
    string name;
    string surname;
    string patronymic;
    int pass_series;
    int pass_num;

    bool operator<(const total_info& key) const {
        if (name != key.name)
            return name < key.name;
        if (surname != key.surname)
            return surname < key.surname;
        if (patronymic != key.patronymic)
            return patronymic < key.patronymic;
        if (pass_series != key.pass_series)
            return pass_series < key.pass_series;
        if (pass_num != key.pass_num)
            return pass_num < key.pass_num;
        return false;
    }

    bool operator==(const total_info& key) const {
        if (name == key.name && surname == key.surname && patronymic == key.patronymic && pass_series == key.pass_series && pass_num == key.pass_num)
            return true;
        return false;
    }
};

struct Node {
    total_info key;
    int balance_factor;
    MyList curr_list;
    Node* right;
    Node* left;
    Node(total_info info, int value) : key(info), balance_factor(0), right(nullptr), left(nullptr) {
        curr_list.push_back(value);
    }
};

void balanceL(Node*& root, bool& h) {
    Node* p1;
    Node* p2;

    if (root->balance_factor == -1) {
        root->balance_factor = 0;
    } else if (root->balance_factor == 0) {
        root->balance_factor = 1;
        h = false;
    } else {
        p1 = root->right;
        if (p1->balance_factor >= 0) {
            root->right = p1->left;
            p1->left = root;
            if (p1->balance_factor == 0) {
                root->balance_factor = 1;
                p1->balance_factor = -1;
                h = false;

            } else {
                root->balance_factor = 0;
                p1->balance_factor = 0;
            }
            root = p1;
        } else {
            p2 = p1->left;
            p1->left = p2->right;
            p2->right = p1;
            root->right = p2->left;
            p2->left = root;

            if (p2->balance_factor == 1) {
                root->balance_factor = -1;
            } else {
                root->balance_factor = 0;
            }

            if (p2->balance_factor == -1) {
                p1->balance_factor = 1;
            } else {
                p1->balance_factor = 0;
            }

            root = p2;
            p2->balance_factor = 0;
        }
    }
}

void balanceR(Node*& root, bool& h) {
    Node* p1;
    Node* p2;

    if (root->balance_factor == 1) {
        root->balance_factor = 0;
    } else if (root->balance_factor == 0) {
        root->balance_factor = -1;
        h = false;
    } else {
        p1 = root->left;
        if (p1->balance_factor <= 0) {
            root->left = p1->right;
            p1->right = root;
            if (p1->balance_factor == 0) {
                root->balance_factor = -1;
                p1->balance_factor = 1;
                h = false;
            } else {
                root->balance_factor = 0;
                p1->balance_factor = 0;
            }
            root = p1;
        } else {
            p2 = p1->right;
            p1->right = p2->left;
            p2->left = p1;
            root->left = p2->right;
            p2->right = root;

            if (p2->balance_factor == -1) {
                root->balance_factor = 1;
            } else {
                root->balance_factor = 0;
            }

            if (p2->balance_factor == 1) {
                p1->balance_factor = -1;
            } else {
                p1->balance_factor = 0;
            }

            root = p2;
            p2->balance_factor = 0;
        }
    }
}

void insert(Node*& root, const total_info& key, int& value, bool& h) {
    Node* p1; Node* p2;
    if (root == nullptr) {
        value++;
        root = new Node(key, value);
        root->balance_factor = 0;
        h = true;
        return;
    } else if (key < root->key) {
        insert(root->left, key, value, h);
        if (h) {
            if (root->balance_factor == 1) {
                root->balance_factor = 0;
                h = false;
            } else if (root->balance_factor == 0)
                root->balance_factor = -1;
            else if (root->balance_factor == -1) {
                p1 = root->left;
                if (p1->balance_factor == -1) {
                    root->left = p1->right;
                    p1->right = root;
                    root->balance_factor = 0;
                    root = p1;
                } else {
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left = p1;
                    root->left = p2->right;
                    p2->right = root;
                    if (p2->balance_factor == -1)
                        root->balance_factor = 1;
                    else root->balance_factor = 0;
                    if (p2->balance_factor == 1)
                        p1->balance_factor = -1;
                    else
                        p1->balance_factor = 0;
                    root = p2;
                }
                root->balance_factor = 0;
                h = false;
            }
        }
    }
    else if (root->key < key) {
        insert(root->right, key, value, h);
        if (h) {
            if (root->balance_factor == -1) {
                root->balance_factor = 0;
                h = false;
            } else if (root->balance_factor == 0)
                root->balance_factor = 1;
            else if (root->balance_factor == 1) {
                p1 = root->right;
                if (p1->balance_factor == 1) {
                    root->right = p1->left;
                    p1->left = root;
                    root->balance_factor = 0;
                    root = p1;
                } else {
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right = p1;
                    root->right = p2->left;
                    p2->left = root;
                    if (p2->balance_factor == 1)
                        root->balance_factor = -1;
                    else root->balance_factor = 0;
                    if (p2->balance_factor == -1)
                        p1->balance_factor = 1;
                    else
                        p1->balance_factor = 0;
                    root = p2;
                }
                root->balance_factor = 0;
                h = false;
            }
        }
    } else {
        root->curr_list.push_back(++value);
        h = false;
    }
}


void search(Node *&root, const total_info& key) {
    if (root == nullptr) {
        cout << "The node WASN'T found in the tree.\n";
        return;
    }
    else if (key == root->key) {
        cout << "The node WAS found in the tree.\n";
        return;
    }
    if (key < root->key)
        search(root->left, key);
    else if (root->key < key)
        search(root->right, key);
}

Node* del(Node*& root, bool& h) {
    Node* q;
    if (root->left == nullptr) {
        q = root;
        root = root->right;
        h = true;
        return q;
    } else {
        Node* deleted = del(root->left, h);
        if (h) balanceL(root, h);
        return deleted;
    }
}

void delete_node(Node*& root, const total_info& key, int num, bool& h) {
    if (root == nullptr)
        return;

    if (key < root->key) {
        delete_node(root->left, key, num, h);
        if (h) balanceL(root, h);
    } else if (root->key < key) {
        delete_node(root->right, key, num, h);
        if (h) balanceR(root, h);
    } else {
        if (root->curr_list.count_list() > 1) {
            root->curr_list.delete_by_value(num);
            h = false;
            return;
        }
        Node* q = root;
        if (root->left == nullptr) {
            root = root->right;
            h = true;
        } else if (root->right == nullptr) {
            root = root->left;
            h = true;
        } else {
            q = del(root->right, h);
            root->key = q->key;
            root->curr_list = q->curr_list;
            if (h) balanceR(root, h);
        }
        delete q;
    }
}

void right_left_print(Node*& root) {
    if (root == nullptr)
        return;
    right_left_print(root->right);
    int size_this_node = root->key.name.size() + root->key.surname.size() + root->key.patronymic.size() + 2;
    cout << string(size_this_node / 2, ' ') << "↓↓↓\n";
    cout << "┌" << string(size_this_node, '-') << "┐\n";
    cout << "|" << root->key.surname << " " << root->key.name << " " << root->key.patronymic << "|\n";
    cout << "|" << root->key.pass_series << " " << root->key.pass_num << string(size_this_node - 11, ' ') << "|   "; cout << "[string number: "; root->curr_list.print_list(); cout << "]\n";
    cout << "└"  << string(size_this_node, '-') << "┘\n";
    right_left_print(root->left);
}

void print_tree(Node* root, const string& prefix = "", bool isLeft = true) {
    string new_string;
    if (root == nullptr)
        return;
    cout << prefix;
    if (isLeft)
        cout << "├── ";
    else
        cout << "└── ";
    root->curr_list.print_list();
    cout << root->key.name << " " << root->key.surname << " " << root->key.patronymic << " " << root->key.pass_series << " " << root->key.pass_num << " " << root->balance_factor; cout << endl;
    if (isLeft)
        new_string = prefix + "|    ";
    else
        new_string = prefix + "    ";
    print_tree(root->left, new_string, true);
    print_tree(root->right, new_string, false);
}

void delete_tree(Node *&root, int& value) {
    if (root == nullptr)
        return;
    delete_tree(root->right, value);
    delete_tree(root->left, value);
    delete root;
    root = nullptr;
    value = 0;
}

bool fio_checker(total_info& current_name) {
    if (current_name.name.size() <= 1 || current_name.surname.size() <= 1 || current_name.patronymic.size() <= 1)
        return false;
    string full_name = current_name.name + current_name.surname + current_name.patronymic;
    for (auto i : full_name) {
        if (!isalpha(i))
            return false;
    }
    return true;
}

bool passport_checker(total_info& current_number) {
    if (1000 <= current_number.pass_series && current_number.pass_series <= 9999 && 100000 <= current_number.pass_num && current_number.pass_num <= 999999)
        return true;
    return false;
}

void files_parser(Node*& root, fstream& file_input, int& value) {
    if (!file_input.is_open())
        return;
    string curr;
    while (getline(file_input, curr)) {
        istringstream this_line(curr);
        total_info info;
        this_line >> info.name >> info.surname >> info.patronymic >> info.pass_series >> info.pass_num;
        if (fio_checker(info) && passport_checker(info)) {
            bool h;
            insert(root, info, value, h);
        } else
            value++;
    }
}

void files_output(Node*& root, ofstream& file_output) {
    if (root == nullptr)
        return;
    files_output(root->right, file_output);
    ListNode* temp = root->curr_list.head;
    do {
        file_output << temp->data << " ";
        file_output << root->key.surname << " " << root->key.name << " " << root->key.patronymic << " " << root->key.pass_series << " " << root->key.pass_num << endl;
        temp = temp->next;
    } while (temp != root->curr_list.head);
    files_output(root->left, file_output);
}

bool error_cin_processing() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(100, '\n');
        return false;
    }
    return true;
}

total_info answer_checker() {
    total_info new_node; bool flag = true;
    while (flag) {
        cout << "Type your data [Name + Surname + Patronymic + Passport series + Passport number]: ";
        cin >> new_node.name;
        cin >> new_node.surname;
        cin >> new_node.patronymic;
        if (fio_checker(new_node))
            flag = false;
        else
            cout << "Incorrect ID. Name & surname & patronymic contains ONLY characters\n";
    }
    flag = true;

    while (flag) {
        error_cin_processing();
        cin >> new_node.pass_series;
        cin >> new_node.pass_num;
        if (passport_checker(new_node))
            flag = false;
        else
            cout << "Incorrect ID. Passport series ONLY 4 symbols and passport series ONLY 6 symbols.\n";
    }
    return new_node;
}

int main() {
    int answer = 0;
    Node *root;
    fstream file_input; int value = 0; bool h;

    cout << "Type '1' - to add your element to the tree\n"
            "Type '2' - to print all tree (right-left)\n"
            "Type '3' - to print all tree\n"
            "Type '4' - to delete all the tree\n"
            "Type '5' - to find 1 element by key\n"
            "Type '6' - to delete 1 element by key and a string number\n"
            "Type '7' - to import all data from the file\n"
            "Type '8' - to export all data from the tree to the file\n"
            "Type '-1' - to leave from the program\n";
    while (cout << "Your choice: " && cin >> answer && answer != -1) {
        total_info new_node;
        switch (answer) {
            case 1: {
                insert(root, answer_checker(), value, h);
                break;
            }
            case 2: {
                cout << "Your total tree: \n";
                print_tree(root);
                break;
            }
            case 3: {
                cout << "Your total tree: \n";
                cout << endl;
                right_left_print(root);
                break;
            }
            case 4: {
                cout << "Successfully! Your bin tree was deleted.\n";
                delete_tree(root, value);
                value = 0;
                break;
            }
            case 5: {
                search(root, answer_checker());
                break;
            }
            case 6: {
                cout << "Enter your string number: "; cin >> answer; cout << "\n";
                delete_node(root, answer_checker(), answer, h);
                break;
            }
            case 7: {
                file_input.open("file.txt");
                cout << "Successfully. All files was imported to the bin tree.\n";
                files_parser(root, file_input, value);
                file_input.close();
                break;
            }
            case 8: {
                cout << "Successfully. All files was exported to the file.\n";
                ofstream file_output;
                file_output.open("file_output.txt", std::ofstream::out | std::ofstream::trunc);
                files_output(root, file_output);
                file_output.close();
                break;
            }
            case -1: {
                cout << "The program is closing...\n";
                break;
            }
            default: {
                cout << "Incorrect answer! Try again.\n";
                break;
            }
        }
    }
}