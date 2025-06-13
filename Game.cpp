#include "Game.h"

// �������������� ���, �������, ��������� �� nullptr � ���������� �����
Game::Node::Node(const string& n, int ord)
    : name(n), rating(0), next(nullptr), prev(nullptr), order(ord)
{
}

// �������������� ������ ������, ������ � ��������� ��������� ��������� �����
Game::Game() : head(nullptr), size(0) 
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// �������� ����� Clear ��� ������������ ������ ��� ����������� �������
Game::~Game() 
{
    Clear();
}

// ����������� ��� ������, ������� ������ ������, ������� ������ � ���������� ������ � 0
void Game::Clear()
{
    if (!head) return; // ���� ������ ����, ������ ������ �� �����
    Node* cur = head->next;
    // �������� �� ���� �����, ������� �� ���������� �� �������, � ������� ��
    while (cur != head)
    {
        Node* temp = cur;
        cur = cur->next;
        delete temp;
    }
    delete head; // ������� ������ ������
    head = nullptr; // ���������� ��������� �� ������
    size = 0; // ������ ������ ������ 0
}

// ��������� ����� �������� �� ���������� ����� 
// � ������ ���������� ����������� ������ �� ��������� ��� � ����������� ��������
bool Game::LoadNames(const string& filename) 
{
    Clear(); // ������� ������� ������ ����� ��������� ����� ������

    ifstream fin(filename); // ��������� ���� ��� ������
    if (!fin.is_open()) // ��������� ���������� �������� �����
    {
        cerr << "�� ������� ������� ���� " << filename << endl;
        return false;
    }

    string line;
    int order = 0; // ���������� ����� ��� ������� ������ ����

    while (getline(fin, line)) // ��������� ���� ���������
    {
        if (line.empty()) continue; // ���������� ������ ������

        // ������ ����� ���� � ������ �� ������ � ���������� �������
        Node* newNode = new Node(line, order++);
        if (!head) // ���� ������ ����, ����� ���� ���������� ������� � �������� ������ ��� �� ����
        {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else // ����� ��������� ����� ���� ����� ������� (� ����� ������)
        {
            Node* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        size++; // ����������� ������ ������
    }
    fin.close(); // ��������� ����

    if (size == 0) // ���� ���� ��� ���� ��� �� �������� ���
    {
        cerr << "���� ���� ��� �� �������� ���." << endl;
        return false;
    }
    return true; // ������� ���������
}

// ������� � ������� ������ �������� � �������� � ����������� ���� highlight
void Game::PrintListWithArrow(Node* highlight)
{
    if (!head) return;

    Node* cur = head;

    do
    {
        cout << cur->name << " :";
        if (cur == highlight) // ���� ������� ���� ��������� � ����������
            cout << " <-"; // ������� �������
        cout << endl;

        cur = cur->next; // ��������� � ���������� ����
    } while (cur != head); // ���������, ���� �� ������� � ������
}

// ������� � ������� ����� �������� � �� ������� �������� � ������� ������ ������
void Game::PrintRatings()
{
    if (!head) return;

    Node* cur = head;

    do
    {
        cout << cur->name << " : " << cur->rating << endl;
        cur = cur->next;
    } while (cur != head);
}

// ��������� ���� �� ��������
// ��� ��������� �������� ��������� �� ����������� ������
bool Game::compareNodes(const Node* a, const Node* b)
{
    if (a->rating != b->rating)
        return a->rating > b->rating; // ������� ������� ���� � ������
    return a->order < b->order; // ��� ��������� �������� - ����� ������ �� ������� ����
}

// ��������� ���� �� �������� ����� �������
// � ������ ������ ������������ ��������� ����� �� -10 �� 10
// �� ����� ���������� ����������� �� ������ ������ - "�� ������� �������"  (���� ����� �������������)
// ��� ����� - "������ ������� �������" (���� �������������)
// ���������� ��������������� ������ � ��� ������� ������������� �� 1
void Game::Play(int rounds)
{
    if (rounds <= 0 || !head) return; // ������ ������, ���� ������� <=0 ��� ������ ����

    Node* current = head; // ������� ��������� ��� ����������� �� ������

    for (int round = 1; round <= rounds; ++round)
    {
        int number = rand() % 21 - 10; // ��������� ����� �� -10 �� 10

        cout << "����� " << round << ":" << endl;
        cout << "������ �����: " << number << endl;

        cout << "������ �������� (� ��������, �� ��� ����������� ����):" << endl;

        if (number == 0)
        {
            PrintListWithArrow(nullptr);
            cout << "\n������ ���� ����� �� ������" << endl;
            cout << "\n�������� ��������:" << endl;
            PrintRatings();
            cout << endl;
            continue; // ��������� � ���������� ������ ��� ��������� �������� � ��������� current
        }

        int steps = abs(number); // ��������� ���������� ����� ��� �����������
        // ������������ ������ ��� ����� �� ������������ ������ �� ����������� ����� �����
        if (number > 0)
        {
            for (int i = 0; i < steps; ++i)
                current = current->next;
        }
        else
        {
            for (int i = 0; i < steps; ++i)
                current = current->prev;
        }

        PrintListWithArrow(current); // ������� ������ � �������� �� ������� �������

        current->rating += 1; // ����������� ������� ���������� ������� �� 1
        cout << "\n������ ���� ������(�) " << current->name << "" << endl;
        cout << "���(�) ������� ������ " << current->rating << endl;

        cout << "\n�������� ��������:" << endl;
        PrintRatings();

        cout << endl;

        // ���������� current �� ��������� ��� ���������� ������� ��� ���������� ������
        if (number > 0)
            current = current->next;
        else
            current = current->prev;
    }
}

// ������� �������� ������� ���� ��������, �������� �� ������� �� �������� ��������
// � ��� ��������� � �� ��������� �������
void Game::PrintResults()
{
    if (!head) return;

    vector<Node*> vec; // ������ ������ ���������� �� ����

    Node* cur = head; // �������� ����� ������ � ������
    do
    {
        vec.push_back(cur); // ��������� ��������� �� ������� ���� � ������
        cur = cur->next;
    } while (cur != head);

    stable_sort(vec.begin(), vec.end(), compareNodes); // ��������� ������ � ������� ������� ���������

    cout << "�������� ������� ��������:" << endl;
    for (const auto& node : vec)
    {
        cout << node->name << " : " << node->rating << endl;
    }
}

// ��������� ��������� ����� ����� �� �����
// ��������� ���� �� ��� ���, ���� ������������ �� ����� ���������� ����� ����� ��� ������ ��������
int Game::readInt(const string& prompt)
{
    int val;
    string line;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
        {
            cout << "������ �����. ���������.\n";
            continue; // ���� ���� ���������, ����������� �����
        }
        stringstream ss(line);
        if (ss >> val)
        {
            char c;
            if (!(ss >> c)) // ���������, ��� ����� ����� ��� ������ ��������
            {
                return val; // ���������� ��������� �����
            }
        }
        cout << "�������� ����. ������� ����� ����� ��� ������ ��������.\n";
    }
}