#include "Game.h"

// Инициализирует имя, рейтинг, указатели на nullptr и порядковый номер
Game::Node::Node(const string& n, int ord)
    : name(n), rating(0), next(nullptr), prev(nullptr), order(ord)
{
}

// Инициализирует пустой список, размер и запускает генератор случайных чисел
Game::Game() : head(nullptr), size(0) 
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Вызывает метод Clear для освобождения памяти при уничтожении объекта
Game::~Game() 
{
    Clear();
}

// Освобождает всю память, занятую узлами списка, очищает список и сбрасывает размер в 0
void Game::Clear()
{
    if (!head) return; // Если список пуст, ничего делать не нужно
    Node* cur = head->next;
    // Проходим по всем узлам, начиная со следующего за головой, и удаляем их
    while (cur != head)
    {
        Node* temp = cur;
        cur = cur->next;
        delete temp;
    }
    delete head; // Удаляем голову списка
    head = nullptr; // Сбрасываем указатель на голову
    size = 0; // Размер списка теперь 0
}

// Загружает имена учеников из текстового файла 
// и создаёт двусвязный циклический список из считанных имён с порядковыми номерами
bool Game::LoadNames(const string& filename) 
{
    Clear(); // Очищаем текущий список перед загрузкой новых данных

    ifstream fin(filename); // Открываем файл для чтения
    if (!fin.is_open()) // Проверяем успешность открытия файла
    {
        cerr << "Не удалось открыть файл " << filename << endl;
        return false;
    }

    string line;
    int order = 0; // Порядковый номер для каждого нового узла

    while (getline(fin, line)) // Считываем файл построчно
    {
        if (line.empty()) continue; // Пропускаем пустые строки

        // Создаём новый узел с именем из строки и порядковым номером
        Node* newNode = new Node(line, order++);
        if (!head) // Если список пуст, новый узел становится головой и замыкает список сам на себя
        {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else // Иначе вставляем новый узел перед головой (в конец списка)
        {
            Node* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        size++; // Увеличиваем размер списка
    }
    fin.close(); // Закрываем файл

    if (size == 0) // Если файл был пуст или не содержал имён
    {
        cerr << "Файл пуст или не содержит имён." << endl;
        return false;
    }
    return true; // Успешно загружено
}

// Выводит в консоль список учеников с пометкой у выделенного узла highlight
void Game::PrintListWithArrow(Node* highlight)
{
    if (!head) return;

    Node* cur = head;

    do
    {
        cout << cur->name << " :";
        if (cur == highlight) // Если текущий узел совпадает с выделенным
            cout << " <-"; // Выводим стрелку
        cout << endl;

        cur = cur->next; // Переходим к следующему узлу
    } while (cur != head); // Повторяем, пока не вернёмся к голове
}

// Выводит в консоль имена учеников и их текущие рейтинги в порядке обхода списка
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

// Сортирует узлы по рейтингу
// при равенстве рейтинга сортирует по порядковому номеру
bool Game::compareNodes(const Node* a, const Node* b)
{
    if (a->rating != b->rating)
        return a->rating > b->rating; // Больший рейтинг выше в списке
    return a->order < b->order; // При равенстве рейтинга - более ранний по порядку выше
}

// Запускает игру на заданное число раундов
// В каждом раунде генерируется случайное число от -10 до 10
// По числу происходит перемещение по списку вперед - "по часовой стрелке"  (если число положительное)
// или назад - "против часовой стрелки" (если отрицательное)
// Выделяется соответствующий ученик и его рейтинг увеличивается на 1
void Game::Play(int rounds)
{
    if (rounds <= 0 || !head) return; // Нечего играть, если раундов <=0 или список пуст

    Node* current = head; // Текущий указатель для перемещения по списку

    for (int round = 1; round <= rounds; ++round)
    {
        int number = rand() % 21 - 10; // Случайное число от -10 до 10

        cout << "Раунд " << round << ":" << endl;
        cout << "Выпало число: " << number << endl;

        cout << "Список учеников (с пометкой, на ком остановился счёт):" << endl;

        if (number == 0)
        {
            PrintListWithArrow(nullptr);
            cout << "\nДоброе дело никто не сделал" << endl;
            cout << "\nРейтинги учеников:" << endl;
            PrintRatings();
            cout << endl;
            continue; // Переходим к следующему раунду без изменений рейтинга и указателя current
        }

        int steps = abs(number); // Вычисляем количество шагов для перемещения
        // Перемещаемся вперед или назад по циклическому списку на определённое число шагов
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

        PrintListWithArrow(current); // Выводим список с пометкой на текущем ученике

        current->rating += 1; // Увеличиваем рейтинг выбранного ученика на 1
        cout << "\nДоброе дело сделал(а) " << current->name << "" << endl;
        cout << "Его(её) рейтинг теперь " << current->rating << endl;

        cout << "\nРейтинги учеников:" << endl;
        PrintRatings();

        cout << endl;

        // Перемещаем current на следующий или предыдущий элемент для следующего раунда
        if (number > 0)
            current = current->next;
        else
            current = current->prev;
    }
}

// Выводит итоговый рейтинг всех учеников, сортируя их сначала по убыванию рейтинга
// а при равенстве — по исходному порядку
void Game::PrintResults()
{
    if (!head) return;

    vector<Node*> vec; // Создаём вектор указателей на узлы

    Node* cur = head; // Начинаем обход списка с головы
    do
    {
        vec.push_back(cur); // Добавляем указатель на текущий узел в вектор
        cur = cur->next;
    } while (cur != head);

    stable_sort(vec.begin(), vec.end(), compareNodes); // Сортируем вектор с помощью функции сравнения

    cout << "Итоговый рейтинг учеников:" << endl;
    for (const auto& node : vec)
    {
        cout << node->name << " : " << node->rating << endl;
    }
}

// Безопасно считывает целое число из ввода
// Повторяет ввод до тех пор, пока пользователь не введёт корректное целое число без лишних символов
int Game::readInt(const string& prompt)
{
    int val;
    string line;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
        {
            cout << "Ошибка ввода. Повторите.\n";
            continue; // Если ввод прервался, запрашиваем снова
        }
        stringstream ss(line);
        if (ss >> val)
        {
            char c;
            if (!(ss >> c)) // Проверяем, что после числа нет лишних символов
            {
                return val; // Возвращаем считанное число
            }
        }
        cout << "Неверный ввод. Введите целое число без лишних символов.\n";
    }
}