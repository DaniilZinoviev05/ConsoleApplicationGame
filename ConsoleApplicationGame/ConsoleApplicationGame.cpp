#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>

using namespace std;

string Bot1function(int bot1Level, const char* str) {
    string answer = str;
    srand(time(NULL));
    int randNum = rand() % 100;
    if (bot1Level == 1 && randNum < 40) {
        return answer;
    }
    else if (bot1Level == 2 && randNum < 60) {
        return answer;
    }
    else if (bot1Level == 3 && randNum < 90) {
        return answer;
    }
    else {
        return "I don't know...";
    }
}

string Bot2function(int bot2Level, const char* str) {
    string answer = str;
    srand(time(NULL));
    int randNum = rand() % 100;
    if (bot2Level == 1 && randNum < 40) {
        return answer;
    }
    else if (bot2Level == 2 && randNum < 60) {
        return answer;
    }
    else if (bot2Level == 3 && randNum < 90) {
        return answer;
    }
    else {
        return "I don't know...";
    }
}

int TopicHandlingFunction(const char* Str, int Num, int c, int* scores, string* names, string* Topics, bool(*used)[5], int bot1Level, int bot2Level) {
    fstream fout;
    string question;
    string NT = Str;
    string answer;
    string answer_by_player;
    int index = -1;
    for (int i = 0; i < 5; i++) {
        if (NT == Topics[i]) {
            index = i;
            break;
        }
    }
    if (index != -1 && !used[index][Num / 100 - 1]) {
        fout.open("questions/" + to_string(index + 1) + "/" + to_string(Num / 100) + ".txt");
        if (fout.is_open()) {
            getline(fout, question);
            cout << question << endl;
        }
        getline(fout, answer);
        fout.close();
        cout << "answer: ";
        if (names[c % 3] == "Bot1") {
            answer_by_player = Bot1function(bot1Level, answer.c_str());
        }
        else if (names[c % 3] == "Bot2") {
            answer_by_player = Bot2function(bot2Level, answer.c_str());
        }
        else {
            cin.ignore();
            getline(cin, answer_by_player);
        }
        if (answer_by_player == answer) {
            scores[c % 3] += Num;
        }
        else {
            scores[c % 3] -= Num;
        }
        used[index][Num / 100 - 1] = true;
        return (c + 1);
    }
    else {
        return c;
    }
}

void ScoreBoard(string* Topics, int* number, bool(*used)[5], int* scores, string* names, int c) {
    cout << c << endl;
    ifstream fin("questions/Topics.txt");
    if (fin.is_open()) {
        for (int i = 0; i < 5; i++) {
            getline(fin, Topics[i]);
        }
        fin.close();
    }
    else {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }
    cout << setw(22) << "Игрок" << setw(28) << "Корзина" << endl;
    for (int i = 0; i < 3; i++) {
        cout << setw(22) << names[i] << setw(28) << scores[i] << endl;
    }
    cout << endl;
    for (int i = 0; i < 5; i++) {
        cout << setw(30) << Topics[i];
        for (int j = 0; j < 5; j++) {
            if (used[i][j]) {
                cout << "    ";
            }
            else {
                cout << " " << number[j];
            }
        }
        cout << endl;
    }
}

void print_win(int* scores, string* names, int bot1Level, int bot2Level) {
    int maxScore = -1000;
    int numWinners = 0;
    string winner;
    for (int i = 0; i < 3; i++) {
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            winner = names[i];
            numWinners = 1;
        }
        else if (scores[i] == maxScore) {
            numWinners++;
        }
    }
    if (numWinners == 1) {
        cout << "=====================================";
        cout << endl << "Победитель: " << winner << " с результатом " << maxScore << "!" << endl;
        cout << "=====================================";
    }
    else {
        system("cls");
        cout << endl << "Игроки набрали одинаковое количество очков. Переходим к ставкам!"<< endl;
        int* bets = new int[numWinners];
        string* betNames = new string[numWinners];
        int betSum = 0;
        int j = 0;
        for (int i = 0; i < 3; i++) {
            if (scores[i] == maxScore) {
                cout << names[i] << ", введите свою ставку (не больше " << scores[i] << "): ";
                if (names[i] == "Bot1") {
                    int botBet = rand() % (scores[i] + 1);
                    while (botBet == bets[0]) {
                        botBet = rand() % (scores[i] + 1);
                    }
                    bets[j] = botBet;
                    cout << botBet << endl;;
                }
                else if (names[i] == "Bot2") {
                    int botBet = rand() % (scores[i] + 1);
                    while (botBet == bets[0] || botBet == bets[1]) {
                        botBet = rand() % (scores[i] + 1);
                    }
                    bets[j] = botBet;
                    cout << botBet << endl;
                }
                else {
                    cin >> bets[j];
                    for (int k = 0; k < j; k++) {
                        while (bets[j] == bets[k]) {
                            cout << "Ставки не могут быть одинаковыми. Попробуйте еще раз: " << endl;
                            cin >> bets[j];
                        }
                    }
                    while (bets[j] > scores[i]) {
                        cout << "Ставка не может быть больше, чем количество очков в вашей корзине. Попробуйте еще раз: " << endl;
                        cin >> bets[j];
                    }
                }
                betNames[j] = names[i];
                betSum += bets[j];
                j++;
            }
        }
        const int numQuestions = 10;
        string questions[numQuestions];
        string answers[numQuestions];
        ifstream infile("AdditionalQuestion.txt");
        for (int i = 0; i < numQuestions; i++) {
            getline(infile, questions[i]);
            getline(infile, answers[i]);
        }
        infile.close();
        for (int i = 0; i < numWinners; i++) {
            string response;
            int randIndex = rand() % numQuestions;
            cout << endl << betNames[i] << ", вот ваш вопрос: " << questions[randIndex] << endl;
            if (betNames[i] != "Bot2" && betNames[i] != "Bot1") {
                cin >> response;
            }
            else if (betNames[i] == "Bot1") {
                response = Bot1function(bot1Level, answers[randIndex].c_str());
                cout << response << endl;
            }
            else if (betNames[i] == "Bot2") {
                response = Bot2function(bot2Level, answers[randIndex].c_str());
                cout << response << endl;
            }
            if (response == answers[randIndex]) {
                cout << "Правильно!" << endl;
                scores[i] += bets[i];
            }
            else {
                cout << "Неправильно!" << endl;
                scores[i] -= bets[i];
            }
            for (int j = randIndex; j < numQuestions - 1; j++) {
                questions[j] = questions[j + 1];
                answers[j] = answers[j + 1];
            }
        }
        return print_win(scores, names, bot1Level, bot2Level);
    }
}

int main()
{
    system("color 1F");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string Topics[5];
    int number[5] = { 100,200,300,400,500 };
    bool used[5][5] = { false };
    int scores[3] = { 0 };
    string names[3];
    int gameMode;
    int bot1Level = 0;
    int bot2Level = 0;
    cout << "Выберите режим игры:\n1 - Игра втроём\n2 - Игра с ботами\n3 - Игра вдвоём с ботом\n";
    cin >> gameMode;
    cin.ignore();
    if (gameMode == 1) {
        cout << "решите любым способом, кто из вас будет ходить первым, вторым и третьем. В порядке, в котором вы ходите записывайте свои имена" << endl;
        for (int i = 0; i < 3; i++) {
            cout << "Введите имя игрока " << i + 1 << ": ";
            getline(cin, names[i]);
        }
    }
    else if (gameMode == 2) {
        cout << "Введите имя игрока" << ": ";
        getline(cin, names[0]);
        names[1] = "Bot1";
        names[2] = "Bot2";
        cout << "Выберите уровень сложности для Bot1:\n1 - Легкий\n2 - Средний\n3 - Сложный\n";
        cin >> bot1Level;
        cout << "Выберите уровень сложности для Bot2:\n1 - Легкий\n2 - Средний\n3 - Сложный\n";
        cin >> bot2Level;
    }
    else if (gameMode == 3) {
        cout << "решите любым способом, кто из вас будет ходить первым и вторым. В порядке, в котором вы ходите записывайте свои имена" << endl;
        for (int i = 0; i < 2; i++) {
            cout << "Введите имя игрока " << i + 1 << ": ";
            getline(cin, names[i]);
        }
        names[2] = "Bot1";
        cout << "Выберите уровень сложности для Bot1:\n1 - Легкий\n2 - Средний\n3 - Сложный\n";
        cin >> bot1Level;
    }
    else {
        cout << "Ошибка выбора режима игры!";
        return 0;
    }
    system("cls");
    string NT;
    int Num;
    int c = 0;
    srand(time(NULL));
    while (c < 25) {
        ScoreBoard(Topics, number, used, scores, names, c);
        cout << endl << "Ходит игрок " << names[c % 3] << endl;
        if (names[c % 3] == "Bot1" || names[c % 3] == "Bot2") {
            vector<int> unusedTopics;
            for (int i = 0; i < 5; i++) {
                bool topicUsed = true;
                for (int j = 0; j < 5; j++) {
                    if (!used[i][j]) {
                        topicUsed = false;
                        break;
                    }
                }
                if (!topicUsed) {
                    unusedTopics.push_back(i);
                }
            }
            int index = unusedTopics[rand() % unusedTopics.size()];
            int numIndex = rand() % 5;
            NT = Topics[index];
            Num = number[numIndex];
        }
        else {
            cout << "Выбирайте тему: ";
            getline(cin, NT);
            int index = -1;
            for (int i = 0; i < 5; i++) {
                if (NT == Topics[i]) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                system("cls");
                continue;
            }
            while (true) {
                cout << "Выберите сумму: ";
                string input;
                cin >> input;

                try {
                    Num = stoi(input);
                }
                catch (const std::exception&) {
                    cout << "Вы ввели не число. Попробуйте еще раз." << endl;
                    continue;
                }

                if (Num != 100 && Num != 200 && Num != 300 && Num != 400 && Num != 500) {
                    cout << "Вы выбрали неправильную сумму. Попробуйте еще раз." << endl;
                    continue;
                }
                break;
            }
        }
        c = TopicHandlingFunction(NT.c_str(), Num, c, scores, names, Topics, used, bot1Level, bot2Level);
        system("cls");
    }
    ScoreBoard(Topics, number, used, scores, names, c);
    print_win(scores, names, bot1Level, bot2Level);
    return 0;
}