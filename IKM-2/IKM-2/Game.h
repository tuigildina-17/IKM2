#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

class Game 
{
private:
    // ��������� ����� ���� ����������� ������������ ������
    class Node 
    {
    public:
        string name; // ��� �������
        int rating; // ������� �������
        Node* next; // ��������� �� ��������� ���� � ������
        Node* prev; // ��������� �� ���������� ���� � ������
        int order; // ���������� ����� ���� (��� ���������� ����������)

        Node(const string& n, int ord); // ����������� ���� � ������ � ���������� �������
    };

    Node* head; // ��������� �� ������ ����������� ������������ ������
    int size; // ���������� ��������� � ������

    static bool compareNodes(const Node* a, const Node* b); // ����������� ������� ��������� ���� ����� (��� ����������)

public:
    Game(); // ����������� ������ Game
    ~Game(); // ���������� ������ Game

    bool LoadNames(const string& filename); // �������� ��� �� �����
    void Play(int rounds); // ������ ���� �� �������� ����� �������
    void PrintResults(); // ����� ����������� ����

    int readInt(const string& prompt); // ����� ��� ������ ������ �����

private:
    void Clear(); // ������� ������ � ������������ ������
    void PrintListWithArrow(Node* highlight); // ����� ������ � ���������� �� ���������� ����
    void PrintRatings(); // ����� ��������� ���� ��������
};

#endif
