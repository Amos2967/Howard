/*
** (C) Copyright 2020 Zty. LTD.(WuHan) All Rights Reserved.
** Description：Shu
** Author：Lyx,Sjy
** Date: 
** Modify Record:
*/	
#include"shudu.h"
// using namespace std;
static int num = 0;
static int settle_flag = 0;

void prune(int i, int j, bool point[10]) {
    for (int k = 0; k < 18; k += 2) {
        if (ques_board[i][k] != '0' && k != j)
            point[ques_board[i][k] - '0'] = true;
    }
    for (int k = 0; k < 9; k++) {
        if (ques_board[k][j] != '0' && k != i)
            point[ques_board[k][j] - '0'] = true;
    }
    int m = 0, n = 0;
    if (i / 3 == 0)
        m = 0;
    else if (i / 3 == 1)
        m = 3;
    else if (i / 3 == 2)
        m = 6;

    if (j / 6 == 0)
        n = 0;
    else if (j / 6 == 1)
        n = 6;
    else if (j / 6 == 2)
        n = 12;

    for (int c = m; c < m + 3; c++) {
        for (int d = n; d < n + 6; d += 2) {
            if (c != i && d != j && ques_board[c][d] != '0')
                point[ques_board[c][d] - '0'] = true;
        }
    }
}


void settle(int pos) {
    if (pos == 162) {
        settle_flag = 1;
        return;
    }
    int i, j, k;
    i = pos / 18;
    j = pos % 18;

    bool point[10] = { false };

    if (ques_board[i][j] == '0') {
        prune(i, j, point);

        for (k = 1; k <= 9; k++) {
            if (point[k])
                continue;
            ques_board[i][j] = k + '0';
            settle(pos + 2);
            if (settle_flag) {
                return;
            }
            ques_board[i][j] = '0';
        }
    } else {
        settle(pos + 2);
    }

    if (settle_flag) {
        return;
    }
}



int main(int argc, char** argv) {
    Input inputs(argc, argv);
    inputs.ParameterProcess();
    char type1 = inputs.GetType1();
    char type2 = inputs.GetType2();
    if (type1 == 'c') {
        int number = inputs.GetNum();
        bool w = sudoku_generate(number);
        cout << "The sudoku final generated is in the sudoku.txt\n"
            << "Have a check\n";
    }
    if (type1 == 's') {
        bool abc = settle_ques();
        cout << "The answer you need is in the sudoku.txt\n"
            << "Have a check\n";
    }

    if (type1 == 'n') {
        if (type2 == 'y') {
            int number = inputs.GetNum();
            bool ww = sudoku_generate(number);
            bool abc = ques_generate1(number);
            cout << "The sudoku question generated is in the ques.txt\n"
                << "Have a check\n";
        }
        if (type2 == 'u') {
            int number = inputs.GetNum();
            bool www = sudoku_generate(number * 100);
            ques_generate4(number);
            cout << "The sudoku question generated is in the ques.txt\n"
                << "Have a check\n";
        }
        if (type2 == 'm') {
            int diff = inputs.GetDiff();
            int number = inputs.GetNum();
            bool qaq = sudoku_generate(number);
            ques_generate2(number, diff);
            cout << "The sudoku question generated is in the ques.txt\n"
                << "Have a check\n";
        }
        if (type2 == 'r') {
            int number = inputs.GetNum();
            int number1 = inputs.GetRange1();
            int number2 = inputs.GetRange2();
            bool QAQ = sudoku_generate(number);
            bool cba = ques_generate3(number, number1, number2);
            cout << "The sudoku question generated is in the ques.txt\n"
                << "Have a check\n";
        }

    }
    return 0;
}
