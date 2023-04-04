#include <curses.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <chrono>
#include <string>


using namespace std;
using namespace std::chrono;


double check(WINDOW* win, int x, int y, const char* prompt)
{
    double num;
    bool is_valid = false;

    while (!is_valid)
    {
        char* input_str = new char[256];
        echo();
        wgetstr(win, input_str);
        noecho();
        try
        {
            num = stof(input_str);
            if (num > 0) // Добавляем проверку на значение больше 0.5
            {
                is_valid = true;
            }
            else
            {
                throw invalid_argument(""); // Если значение меньше или равно 0.5, выбрасываем исключение
            }
        }
        catch (const invalid_argument&) // Исправляем тип исключения на const invalid_argument&
        {
            mvwprintw(win, x, y, "                                      ");
            mvwprintw(win, x, y, prompt);
            wrefresh(win);
        }
    }
    return num;
}

double checkit(WINDOW* win, int x, int y, const char* prompt)
{
    double num;
    bool is_valid = false;

    while (!is_valid)
    {
        char* input_str = new char[256];
        echo();
        wgetstr(win, input_str);
        noecho();
        try
        {
            num = stof(input_str);
            if (num > 0.5) // Добавляем проверку на значение больше 0.5
            {
                is_valid = true;
            }
            else
            {
                throw invalid_argument(""); //Если значение меньше или равно 0.5, выбрасываем исключение
            }
        }
        catch (const invalid_argument&) //Исправляем тип исключения на const invalid_argument&
        {
            mvwprintw(win, x, y, "                                      ");
            mvwprintw(win, x, y, prompt);
            wrefresh(win);
        }
    }
    return num;
}

int factorial(int n)//рекурсивно находим факториал
{
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

void findNumbers(WINDOW* win)//находим все числа, у которых сумма факториалов их чисел равна этому числу
{
    for (int i = 100; i <= 999; i++)
    {
        int k = 1;
        int number = i;
        int sum = 0;
        while (number > 0)
        {
            sum += factorial(number % 10);
            number /= 10;
        }
        if (sum == i)
        {
            mvwprintw(win, k, 1, "%d", i);
            k++;
        }
            
    }
    wrefresh(win);
}

double ln_recursive(double x, double e, int n = 1, double result = 0.0)//рекурсивно находим с точностью E
{
    double term = pow((x - 1), n) / (n * pow(x, n));//рекурсивная формула
    result += term;//итоговое значение рекурсивной 
    if (fabs(term) < e)//если точность, какая нужна, то возвращаем итоговое значение
    {
        return result;
    }
    else
    {
        return ln_recursive(x, e, n + 1, result);//если точность не та, то продолжаем цикл
    }
}

double ln_iterative(double x, double e)//итеррационно находим с точностью E
{
    double result = 0;//ответ
    double term = (x - 1) / x;//перввый прогон формулы
    int n = 1;//для степеней
    while (fabs(term) >= e)//пока по модулю точность не будет близка к E
    {
        result += term;//добавляем в конечный ответ точность
        n++;//следующий проход
        term = pow((x - 1) / x, n) / n;//формула
    }
    return result;
}

int main()
{
    initscr();
    //начало curses
    noecho();
    //не отоброжаем ввод пользователя
    WINDOW* menuwin = newwin(5, 20, 7, 40);
    //создаем окно для меню
    box(menuwin, 0, 0);
    //выводим рамки окон
    mvwprintw(menuwin, 0, 1, "Choose:");
    //заголовок окон
    start_color();
    //делаем окна цветными
    /*
        COLOR_BLACK
        COLOR_RED
        COLOR_GREEN
        COLOR_YELLOW
        COLOR_BLUE
        COLOR_MAGENTA
        COLOR_CYAN
        COLOR_WHITE
        */
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    //создаем окно для ввода данных
    WINDOW* inputwin = newwin(13, 30, 12, 10);
    //создаем окно для вывода данных
    WINDOW* outputwin = newwin(13, 60, 12, 40);
    box(inputwin, 0, 0);
    box(outputwin, 0, 0);
    mvwprintw(inputwin, 0, 1, "Input:");
    mvwprintw(outputwin, 0, 1, "Output:");
    wbkgd(inputwin, COLOR_PAIR(1));
    wbkgd(outputwin, COLOR_PAIR(2));
    //делаем данные видимыми
    wrefresh(inputwin);
    wrefresh(outputwin);
    wrefresh(menuwin);
    //начало менюшки
    keypad(menuwin, true);
    //управление стрелочками
    curs_set(0);
    //убираем курсор
    string choices[3] = { "First task", "Second task", "Exit" };
    //выбор в менюшке
    int choice;
    int highlight = 0;
    bool work = 1;
    //переменные для работы с меню
    while (work)
    {
        for (int i = 0; i < 3; i++) 
        {
            if (i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i + 1, 1, choices[i].c_str());
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);
        switch (choice) {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight == 3)
                highlight = 2;
            break;
        default:
            break;
        }
        if (choice == 10 && highlight == 0)
        {
            //очищаем окно от прошлых данных
            wclear(outputwin);
            box(outputwin, 0, 0);
            mvwprintw(outputwin, 0, 1, "Output:");
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            //выводим условия в окно ввода
            mvwprintw(inputwin, 1, 1, "Find all three-digit numbers");
            mvwprintw(inputwin, 2, 1, "representable as sums of ");
            mvwprintw(inputwin, 3, 1, "factorials of their digits.");
            mvwprintw(inputwin, 4, 1, "Use a recursive function to");
            mvwprintw(inputwin, 5, 1, "calculate n!");
            wrefresh(inputwin);
            wrefresh(outputwin);
            //выводим результат
            findNumbers(outputwin);
        }
        if (choice == 10 && highlight == 1)
        {
            wclear(outputwin);
            box(outputwin, 0, 0);
            mvwprintw(outputwin, 0, 1, "Output:");
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");

            wrefresh(inputwin);
            wrefresh(outputwin);
            echo();
            curs_set(1);
            //вводим данные
            double x, e;
            mvwprintw(inputwin, 1, 1, "Enter X: ");
            wrefresh(inputwin);
            x = checkit(inputwin, 1, 1, "Invalid X, try again: ");

            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter X: %f", x);
            wrefresh(inputwin);

            mvwprintw(inputwin, 3, 1, "Enter E: ");
            wrefresh(inputwin);
            e = check(inputwin, 3, 1, "Invalid E, try again: ");

            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter X: %f", x);
            mvwprintw(inputwin, 3, 1, "Enter E: %f", e);
            wrefresh(inputwin);

            //вычислям стандартным способом
            double ln_std = log(x);
            //вычисляем кол-во сложений в рекурсивном алгоритме
            int n = 1;
            double term = pow((x - 1), n) / (n * pow(x, n));
            while (fabs(term) >= e)
            {
                n++;
                term = pow((x - 1), n) / (n * pow(x, n));
            }

            //начинаем вычислять время, затраченное на выполнение рекурсивного алгоритма
            auto start_rec = high_resolution_clock::now();
            //вычисляем с помощью рекурсивного алгоритма 
            double ln_rec = ln_recursive(x, e);
            //заканчиваем вычислять время
            auto end_rec = high_resolution_clock::now();
            //вычислям сколько затратили
            duration<float> duration_rec = end_rec - start_rec;

            //начинаем вычислять время, затраченное на выполнение иттерационного алгоритма
            auto start_iter = high_resolution_clock::now();
            //вычисляем с помощью иттерационного алгоритма  
            double ln_iter = ln_iterative(x, e);
            //заканчиваем вычислять время
            auto end_iter = high_resolution_clock::now();
            //вычислям сколько затратили
            duration<float> duration_iter = end_iter - start_iter;

            //выводим все значения 
            mvwprintw(outputwin, 1, 1, "ln(x) using recursive algorithm: %f", ln_rec);
            mvwprintw(outputwin, 3, 1, "ln(x) using standard library function: %f", ln_std);
            mvwprintw(outputwin, 7, 1, "Number of terms for recursive algorithm: %d", n);
            mvwprintw(outputwin, 5, 1, "ln(x) using iterative algorithm: %f", ln_iter);
            mvwprintw(outputwin, 9, 1, "Execution time for recursive algorithm (ms): %f", duration_rec.count());
            mvwprintw(outputwin, 11, 1, "Execution time for iterative algorithm (ms): %f", duration_iter.count());
            wrefresh(outputwin);
            curs_set(0);   
        }
        if (choice == 10 && highlight == 2)
        {
            wclear(outputwin);
            box(outputwin, 0, 0);
            mvwprintw(outputwin, 0, 1, "Output:");
            mvwprintw(outputwin, 1, 1, "Have a good day!");
            wrefresh(outputwin);
            work = 0;
            break;
        }
    }
    wgetch(menuwin);

    endwin();
    return 0;
}