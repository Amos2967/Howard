/*
** Author：Xsj,Xyr
** Date: 2023/6/28
*/
#include"shudu.h"
static int num = 0;
static int settle_flag = 0;
static char buf[MAX];
int total = 0;

void prune(int i, int j, bool point[10]) {
    for (int k = 0; k < 18; k += 2) {
        if (ques_board[i][k] != '$' && k != j)
            point[ques_board[i][k] - '0'] = true;
    }
    for (int k = 0; k < 9; k++) {
        if (ques_board[k][j] != '$' && k != i)
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
            if (c != i && d != j && ques_board[c][d] != '$')
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

    if (ques_board[i][j] == '$') {
        prune(i, j, point);

        for (k = 1; k <= 9; k++) {
            if (point[k])
                continue;
            ques_board[i][j] = k + '0';
            settle(pos + 2);
            if (settle_flag) {
                return;
            }
            ques_board[i][j] = '$';
        }
    }
    else {
        settle(pos + 2);
    }

    if (settle_flag) {
        return;
    }
}

// 从指定路径中读取数独文件，并将题解写入sudoku.txt
bool settle_ques() {
    int begin = 0, end = 0;
    FILE* fpQues, * fpSolution;
    char strSolution[200];
    int n = 0;
    if (AbsolutePath[0] == 0)
        fpQues = fopen(QUESPATH, "r");
    else
        fpQues = fopen(AbsolutePath, "r");

    fpSolution = fopen(SUDOKUPATH, "w");
    int flag = 0;
    while (1) {
        n = 0;
        flag = 0;
        for (int i = 0; i < 9; i++) {
            fgets(ques_board[i], 20, fpQues);
        }
        flag = fgetc(fpQues);

        begin = clock();
        settle_flag = 0;
        settle(0);
        end = clock();
        total += end - begin;
        strSolution[0] = '\0';
        for (int i = 0; i < 9; i++) {
            strncat(strSolution, ques_board[i], 20);
        }
        if (flag == -1) {
            strSolution[161] = '\0';
        }
        else {
            strSolution[161] = '\n';
            strSolution[162] = '\n';
            strSolution[163] = '\0';
        }
        fputs(strSolution, fpSolution);
        if (flag == -1)
            break;
    }
    fclose(fpQues);
    fclose(fpSolution);
    return true;
}

// 产生n个数独终局
bool sudoku_generate(int n) {
    int cot = n;
    buf[0] = '\0';
    int bit = 0;
    char line[9] = { '5', '1', '2', '3', '4', '6', '7', '8', '9' };
    char line1[19] = { '5', ' ', '1', ' ', '2', ' ',
    '3', ' ', '4', ' ', '6', ' ', '7',
    ' ', '8', ' ', '9', '\n', '\0' };
    int shift[9] = { 0, 6, 12, 2, 8, 14, 4, 10, 16 };

    int pos1[6][3] = { { 3, 4, 5 },
    { 3, 5, 4 },
    { 4, 5, 3 },
    { 4, 3, 5 },
    { 5, 4, 3 },
    { 5, 3, 4 } };
    int pos2[6][3] = { { 6, 7, 8 }, { 6, 8, 7 }, { 7, 6, 8 },
    { 7, 8, 6 }, { 8, 6, 7 }, { 8, 7, 6 } };

    char final[10][19];
    int i, j, k;
    int flag = 0;
    char str[200];

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 17; j++) {
            final[i][j] = ' ';
        }
        final[i][17] = '\n';
        final[i][18] = '\0';
    }
    final[9][0] = '\n';
    final[9][1] = '\0';
    FILE* fp = fopen(SUDOKUPATH, "w");

    do {
        for (int i = 0; i < 9; i++) {
            line1[2 * i] = line[i];
        }
        memcpy(final[0], line1, sizeof(line1));
        for (i = 1; i < 9; i++) {
            for (j = 0; j < 18; j += 2) {
                final[i][j] = line1[(j + shift[i]) % 18];
            }
        }

        for (i = 0; i < 6; i++) {
            for (j = 0; j < 6; j++) {
                str[0] = '\0';
                flag++;
                for (k = 0; k < 3; k++) {
                    strncpy(buf + bit, final[k], 19);
                    bit += 18;
                }

                for (k = 0; k < 3; k++) {
                    strncpy(buf + bit, final[pos1[i][k]], 19);
                    bit += 18;
                }
                for (k = 0; k < 3; k++) {
                    strncpy(buf + bit, final[pos2[j][k]], 19);
                    bit += 18;
                }
                strncpy(buf + bit, "\n", 1);
                bit++;
                if (n == 1) {
                    buf[163 * (cot - 1) + 161] = '\0';
                    fputs(buf, fp);
                }

                n--;
                if (!n) {
                    fclose(fp);
                    return true;
                }
            }
        }
    } while (next_permutation(line + 1, line + 9));
    return false;
}

// 生成n个数独游戏
bool ques_generate1(int ques_num) {
    FILE* fpQues1;
    FILE* fpBase1;
    char str[200];

    fpBase1 = fopen(SUDOKUPATH, "r");
    fpQues1 = fopen(QUESPATH, "w");
    ques_board[9][0] = '\n';
    ques_board[9][1] = '\0';
    while (ques_num--) {
        str[0] = '\0';
        for (int i = 0; i < 9; i++) {
            fgets(ques_board[i], 20, fpBase1);
        }
        fgetc(fpBase1);
        int base[9] = { 0, 6, 12, 54, 60, 66, 108, 114, 120 };
        int plus[9] = { 0, 2, 4, 18, 20, 22, 36, 38, 40 };
        // unsigned int seed = 123;
        srand(ques_num);
        for (int k = 0; k < 9; k++) {
            int i, j,
                hole[2];
            hole[0] = (rand()- ques_num) % 9;
            hole[1] = (rand() - ques_num) % 9;
            while (hole[0] == hole[1]) {
                hole[1] = (rand() - ques_num) % 9;
            }
            for (int t = 0; t < 2; t++) {
                int dot;
                dot = base[k] + plus[hole[t]];
                i = dot / 18;
                j = dot % 18;
                ques_board[i][j] = '$';
            }
        }

        int others;
        others = 12 + (rand() - ques_num) % 31;
        while (others--) {
            int k = (rand() - ques_num) % 81;
            int i = k / 9;
            int j = k % 9;
            j *= 2;
            if (ques_board[i][j] != '$')
                ques_board[i][j] = '$';
            else
                others++;
        }

        for (int i = 0; i < 10; i++) {
            strncat(str, ques_board[i], 20);
        }
        if (!ques_num) {
            str[161] = '\0';

        }

        fputs(str, fpQues1);
    }
    fclose(fpBase1);
    fclose(fpQues1);
    return true;
}

// 生成n个数独游戏，diff指定生成难度
bool ques_generate2(int ques_num, int diff) {
    FILE* fpQues1;
    FILE* fpBase1;
    char str[200];

    fpBase1 = fopen(SUDOKUPATH, "r");
    fpQues1 = fopen(QUESPATH, "w");
    ques_board[9][0] = '\n';
    ques_board[9][1] = '\0';
    while (ques_num--) {
        str[0] = '\0';
        for (int i = 0; i < 9; i++) {
            fgets(ques_board[i], 20, fpBase1);
        }
        fgetc(fpBase1);
        int base[9] = { 0, 6, 12, 54, 60, 66, 108, 114, 120 };
        int plus[9] = { 0, 2, 4, 18, 20, 22, 36, 38, 40 };
        srand(ques_num);
        for (int k = 0; k < 9; k++) {
            int i, j,
                hole[2];
            hole[0] = (rand() - ques_num) % 9;
            hole[1] = (rand() - ques_num) % 9;
            while (hole[0] == hole[1]) {
                hole[1] = (rand() - ques_num) % 9;
            }
            for (int t = 0; t < 2; t++) {
                int dot;
                dot = base[k] + plus[hole[t]];
                i = dot / 18;
                j = dot % 18;
                ques_board[i][j] = '$';
            }
        }

        int others = 0;  // 2-42
        // 2-22
        if (diff == 1) {
            others = 2 + (rand() - ques_num) % 21;
        }
        // 23-37
        if (diff == 2)
            others = 23 + (rand() - ques_num) % 15;
        // 38-42
        if (diff == 3)
            others = 38 + (rand() - ques_num) % 5;
        while (others--) {
            int k = (rand() - ques_num) % 81;
            int i = k / 9;
            int j = k % 9;
            j *= 2;
            if (ques_board[i][j] != '$')
                ques_board[i][j] = '$';
            else
                others++;
        }

        for (int i = 0; i < 10; i++) {
            strncat(str, ques_board[i], 20);
        }
        if (!ques_num) {
            str[161] = '\0';

        }

        fputs(str, fpQues1);
    }
    fclose(fpBase1);
    fclose(fpQues1);
    return true;
}

// 生成n个数独游戏，限制挖空的数在一定的范围之中
bool ques_generate3(int ques_num, int space_num1, int space_num2) {
    FILE* fpQues1;
    FILE* fpBase1;
    char str[200];
    fpBase1 = fopen(SUDOKUPATH, "r");
    fpQues1 = fopen(QUESPATH, "w");
    ques_board[9][0] = '\n';
    ques_board[9][1] = '\0';
    while (ques_num--) {
        str[0] = '\0';
        for (int i = 0; i < 9; i++) {
            fgets(ques_board[i], 20, fpBase1);
        }
        fgetc(fpBase1);
        int base[9] = { 0, 6, 12, 54, 60, 66, 108, 114, 120 };
        int plus[9] = { 0, 2, 4, 18, 20, 22, 36, 38, 40 };
        srand(ques_num);
        for (int k = 0; k < 9; k++) {
            int i, j,
                hole[2];
            hole[0] = (rand() - ques_num) % 9;
            hole[1] = (rand() - ques_num) % 9;
            while (hole[0] == hole[1]) {
                hole[1] = (rand() - ques_num) % 9;
            }
            for (int t = 0; t < 2; t++) {
                int dot;
                dot = base[k] + plus[hole[t]];
                i = dot / 18;
                j = dot % 18;
                ques_board[i][j] = '$';
            }
        }

        int others;
        int temp = (rand() - ques_num) % (space_num2 - space_num1 + 1);
        others = space_num1 - 18 + temp;
        while (others--) {
            int k = (rand() - ques_num) % 81;
            int i = k / 9;
            int j = k % 9;
            j *= 2;
            if (ques_board[i][j] != '$')
                ques_board[i][j] = '$';
            else
                others++;
        }

        // freopen(QUESPATH, "w", stdout);
        for (int i = 0; i < 10; i++) {
            strncat(str, ques_board[i], 20);
        }
        if (!ques_num) {
            str[161] = '\0';

        }

        fputs(str, fpQues1);
    }
    fclose(fpBase1);
    fclose(fpQues1);
    return true;
}

// 生成n个数独游戏，要求解唯一
bool ques_generate4(int ques_num) {
    FILE* fpQues1;
    FILE* fpBase1;
    char str[200];
    fpBase1 = fopen(SUDOKUPATH, "r");
    fpQues1 = fopen(QUESPATH, "w");
    ques_board[9][0] = '\n';
    ques_board[9][1] = '\0';
    // while (ques_num--)
    while (ques_num) {
        str[0] = '\0';
        for (int i = 0; i < 9; i++) {
            fgets(ques_board[i], 20, fpBase1);
        }
        fgetc(fpBase1);

        int base[9] = { 0, 6, 12, 54, 60, 66, 108, 114, 120 };
        int plus[9] = { 0, 2, 4, 18, 20, 22, 36, 38, 40 };
        srand(ques_num);
        for (int k = 0; k < 9; k++) {
            int i, j,
                hole[2];
            hole[0] = (rand() - ques_num) % 9;
            hole[1] = (rand() - ques_num) % 9;
            while (hole[0] == hole[1]) {
                hole[1] = (rand() - ques_num) % 9;
            }
            for (int t = 0; t < 2; t++) {
                int dot;
                dot = base[k] + plus[hole[t]];
                i = dot / 18;
                j = dot % 18;
                ques_board[i][j] = '$';
            }
        }
        int others;
        others = 2 + (rand() - ques_num) % 11;
        while (others--) {
            int k = (rand() - ques_num) % 81;
            int i = k / 9;
            int j = k % 9;
            j *= 2;
            if (ques_board[i][j] != '$')
                ques_board[i][j] = '$';
            else
                others++;
        }

        int temp[9][9];
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                temp[i][j] = ques_board[i][2 * j];
            }
        }
        if (checkUniqueness(temp)) {
            ques_num--;
            // freopen(QUESPATH, "w", stdout);
            for (int i = 0; i < 10; i++) {
                strncat(str, ques_board[i], 20);
            }
            if (!ques_num) {
                str[161] = '\0';

            }

            fputs(str, fpQues1);
        }
        else {
            continue;
        }
    }
    fclose(fpBase1);
    fclose(fpQues1);
    return true;
}

bool checkUniqueness(int puzzle[UN][UN]) {
    // get the number of missing spots
    int missing = 0;

    for (int i = 0; i < UN; i++) {
        for (int j = 0; j < UN; j++) {
            if (puzzle[i][j] == 0) {
                missing++;
            }
        }
    }

    // get the rows and cols for each missing one
    int* row = new int[missing];
    int* col = new int[missing];

    int counter = 0;

    for (int i = 0; i < UN; i++) {
        for (int j = 0; j < UN; j++) {
            if (puzzle[i][j] == 0) {
                row[counter] = i;
                col[counter] = j;
                counter++;
            }
        }
    }

    if (missing == 1 || missing == 0) {  // acounts for corner cases
        return true;
    }

    // get a completely solved puzzle as an example
    int solved[UN][UN];

    copy_grid(solved, puzzle);
    if (!solveSudoku(solved)) {
        return false;
    }

    // search for another solution
    for (int i = 0; i < missing; i++) {  // for each missing spot
        for (int j = 0; j < UN; j++) {  // try each potential number
            if (j != solved[row[i]][col[i]]) {
                int test[UN][UN];
                copy_grid(test, puzzle);
                // try solving with new number
                if (isValidInsert(test, row[i], col[i], j)) {
                    test[row[i]][col[i]] = j;
                    if (solveSudoku(test)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void copy_grid(int puzzleCopy[UN][UN], int puzzle[UN][UN]) {
    for (int row = 0; row < UN; row++) {
        for (int column = 0; column < UN; column++)
            puzzleCopy[row][column] = puzzle[row][column];
    }
}

bool solveSudoku(int grid[UN][UN]) {
    int row, col;

    // if no empty space is found, we solved it
    if (!findEmptySpace(grid, &row, &col)) {
        return true;
    }

    // attempt to insert numbers 1 - 9
    for (int i = 1; i <= UN; i++) {
        // check if inserting tht number is allowed
        if (isValidInsert(grid, row, col, i)) {
            grid[row][col] = i;   // if allowed, insert it
            // if that number leads to a solution, great, stop.
            if (solveSudoku(grid)) {
                return true;
            }

            grid[row][col] = 0;
        }
    }
    // if it gets here, there is no solution
    return false;
}

bool findEmptySpace(int grid[UN][UN], int* row, int* col) {
    for (int i = 0; i < UN; i++) {
        for (int j = 0; j < UN; j++) {
            // if it is zero, it is an empty space
            if (grid[i][j] == 0) {
                // save the row and column of the empty space
                *row = i; *col = j;
                return true;
            }
        }
    }
    return false;
}

bool isValidInsert(int grid[UN][UN], int row, int col, int num) {
    // check if the number exists elsewhere in the same row
    for (int i = 0; i < UN; i++) {
        if (i != col && grid[row][i] == num) {
            return false;
        }
    }

    // check if the number exists elsewhere in the same column
    for (int i = 0; i < UN; i++) {
        if (i != row && grid[i][col] == num) {
            return false;
        }
    }

    // check if the number exists elsewhere in the 3 x 3 grid

    int startRow = row - (row % 3);  // starting row of the 3 x 3 grid
    int startCol = col - (col % 3);  // starting column of the 3 x 3 grid

    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (i != row && j != col && grid[i][j] == num) {
                return false;
            }
        }
    }
    return true;
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
