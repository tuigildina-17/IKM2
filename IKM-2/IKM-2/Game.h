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
    // Вложенный класс узла двусвязного циклического списка
    class Node 
    {
    public:
        string name; // Имя ученика
        int rating; // Рейтинг ученика
        Node* next; // Указатель на следующий узел в списке
        Node* prev; // Указатель на предыдущий узел в списке
        int order; // Порядковый номер узла (для стабильной сортировки)

        Node(const string& n, int ord); // Конструктор узла с именем и порядковым номером
    };

    Node* head; // Указатель на голову двусвязного циклического списка
    int size; // Количество элементов в списке

    static bool compareNodes(const Node* a, const Node* b); // Статическая функция сравнения двух узлов (для сортировки)

public:
    Game(); // Конструктор класса Game
    ~Game(); // Деструктор класса Game

    bool LoadNames(const string& filename); // Загрузка имён из файла
    void Play(int rounds); // Запуск игры на заданное число раундов
    void PrintResults(); // Вывод результатов игры

    int readInt(const string& prompt); // Метод для чтения целого числа

private:
    void Clear(); // Очистка списка и освобождение памяти
    void PrintListWithArrow(Node* highlight); // Вывод списка с указателем на выделенный узел
    void PrintRatings(); // Вывод рейтингов всех учеников
};

#endif
