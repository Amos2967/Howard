#include"shudu.h"
static char buf[MAX];
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

    for (int i = 0; i < 9; i++)    {
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
        for (int i = 0; i < 9; i++)    {
            line1[2 * i] = line[i];
        }
        memcpy(final[0], line1, sizeof(line1));
        for (i = 1; i < 9; i++)    {
            for (j = 0; j < 18; j += 2)    {
                final[i][j] = line1[(j + shift[i]) % 18];
            }
        }

        for (i = 0; i < 6; i++)    {
            for (j = 0; j < 6; j++)    {
                str[0] = '\0';
                flag++;
                for (k = 0; k < 3; k++)    {
                    strncpy(buf + bit, final[k], 19);
                    bit += 18;
                }

                for (k = 0; k < 3; k++)    {
                    strncpy(buf + bit, final[pos1[i][k]], 19);
                    bit += 18;
                }
                for (k = 0; k < 3; k++)    {
                    strncpy(buf + bit, final[pos2[j][k]], 19);
                    bit += 18;
                }
                strncpy(buf + bit, "\n", 1);
                bit++;
                if (n == 1)    {
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
    }
    while (next_permutation(line + 1, line + 9));
}
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
        for (int i = 0; i < 9; i++)    {
            fgets(ques_board[i], 20, fpBase1);
        }
        fgetc(fpBase1);
        int base[9] = { 0, 6, 12, 54, 60, 66, 108, 114, 120 };
        int plus[9] = { 0, 2, 4, 18, 20, 22, 36, 38, 40 };
        // unsigned int seed = 123;
        for (int k = 0; k < 9; k++)    {
            int i, j,
            hole[2];
            hole[0] = rand() % 9;
            hole[1] = rand() % 9;
            while (hole[0] == hole[1]) {
                hole[1] = rand() % 9;
            }
            for (int t = 0; t < 2; t++)    {
                int dot;
                dot = base[k] + plus[hole[t]];
                i = dot / 18;
                j = dot % 18;
                ques_board[i][j] = '0';
            }
        }

        int others;
        others = 12 + rand() % 31;
        while (others--) {
            int k = rand() % 81;
            int i = k / 9;
            int j = k % 9;
            j *= 2;
            if (ques_board[i][j] != '0')
                ques_board[i][j] = '0';
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

        for (int k = 0; k < 9; k++)    {
            int i, j,
            hole[2];
            hole[0] = rand() % 9;
            hole[1] = rand() % 9;
            while (hole[0] == hole[1]) {
                hole[1] = rand() % 9;
            }
            for (int t = 0; t < 2; t++)    {
                int dot;
                dot = base[k] + plus[hole[t]];
                i = dot / 18;
                j = dot % 18;
                ques_board[i][j] = '0';
            }
        }

        int others = 0;  // 2-42
        // 2-22
        if (diff == 1) {
            others = 2 + rand() % 21;
        }
        // 23-37
        if (diff == 2)
            others = 23 + rand() % 15;
        // 38-42
        if (diff == 3)
            others = 38 + rand() % 5;
        while (others--) {
            int k = rand() % 81;
            int i = k / 9;
            int j = k % 9;
            j *= 2;
            if (ques_board[i][j] != '0')
                ques_board[i][j] = '0';
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
