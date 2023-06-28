/*
** Author：Xsj,Xyr
** Date: 2023/6/28
*/
#include"shudu.h"

Input::Input(int a, char** b) {
    this->argc = a; //参数个数
    this->argv = b; //参数数组指针
    type1 = 'y';//参数1类型
    type2 = 'y';//参数2类型,y表示没有
    this->num = 0;//数独盘数
    this->diff = 0;//难度
    this->range1 = 0;//挖空范围下限
    this->range2 = 0;//挖空范围上限
}

void Input::ParameterProcess() {
    if (this->argc == 3) {
        //-c 需要的数独终盘数
        if (!strcmp(this->argv[1], "-c")) {
            int len = strlen(this->argv[2]);
            for (int i = 0; i < len; i++) {
                if (this->argv[2][i] > '9' || this->argv[2][i] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The second param should be a positive integer\n";
                    return ;
                }
            }

            int number = 0;
            for (int i = 0; i < len; i++) {
                number = number * 10 + this->argv[2][i] - '0';
            }
            if (number < 1 || number>1000000) {
                printf("Please input 1 to 1000000֮\n");
                return ;
            } else {
                this->num = number;
                this->type1 = 'c';
            }
        }
        //-s 指定路径，默认为ques.txt
        if (!strcmp(this->argv[1], "-s")) {
            this->type1 = 's';
            strncpy(AbsolutePath, this->argv[2], 30);  
        }
        //-n 数独游戏盘数
        if (!strcmp(this->argv[1], "-n")) {
            int len = strlen(this->argv[2]);
            for (int i = 0; i < len; i++) {
                if (this->argv[2][i] > '9' || this->argv[2][i] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The second param should be a positive integer\n";
                    return ;
                }
            }

            int number = 0;
            for (int i = 0; i < len; i++) {
                number = number * 10 + this->argv[2][i] - '0';
            }
            if (number < 1 || number>1000000) {
                printf("Please input 1 to 1000000֮\n");
                return ;
            } else {
                this->num = number;
                this->type1 = 'n';
            }
        }
    } else if (argc == 4) {
        //-n num -u 数独唯一解
        if (strcmp(this->argv[1], "-n") && strcmp(this->argv[3], "-u"))    {
            cout << "Illegal paramater\n"
                << "The first param should be -n, the third should be -u\n";
            return ;
        }

        if (!strcmp(this->argv[1], "-n")) {
            int len = strlen(this->argv[2]);
            for (int i = 0; i < len; i++) {
                if (this->argv[2][i] > '9' || this->argv[2][i] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The second param should be a positive integer\n";
                    return ;
                }
            }

            int number = 0;
            for (int i = 0; i < len; i++) {
                number = number * 10 + this->argv[2][i] - '0';
            }
            if (number < 1 || number>1000000) {
                printf("Please input 1 to 1000000֮\n");
                return ;
            } else {
                this->num = number;
                this->type1 = 'n';
                this->type2 = 'u';
            }
        }
    } else if (argc == 5) {
        if (strcmp(this->argv[1], "-n") && (strcmp(this->argv[3], "-m")
        || strcmp(this->argv[3], "-r"))) {
            cout << "Illegal paramater\n"
                << "The first param should be -n, the third -r or -m\n";
            return ;
        }

        if (!strcmp(this->argv[1], "-n")) {
            int len = strlen(this->argv[2]);
            for (int i = 0; i < len; i++) {
                if (this->argv[2][i] > '9' || this->argv[2][i] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The second param should be a positive int\n";
                    return ;
                }
            }

            int number = 0;
            for (int i = 0; i < len; i++) {
                number = number * 10 + this->argv[2][i] - '0';
            }
            if (number < 1 || number>1000000) {
                printf("Please input 1 to 1000000֮\n");
                return ;
            } else {
                this->num = number;
                this->type1 = 'n';
            }
            //-n -m 指示难度等级
            if (!strcmp(this->argv[3], "-m")) {
                if (this->argv[4][0] > '3' || this->argv[4][0] < '1') {
                    cout << "Illegal paramater\n";
                    cout << "The forth param should be a positive int(1-3)\n";
                    return ;
                }

                int diff = 0;
                diff = this->argv[4][0] - '0';
                this->type2 = 'm';
                this->diff = diff;
            } else {
            //-n -r 指示挖空范围
                int len = strlen(this->argv[4]);
                if (this->argv[4][0] > '9' || this->argv[4][0] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The forth param should be a positive integer\n";
                    return ;
                }
                if (this->argv[4][1] > '9' || this->argv[4][1] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The forth param should be a positive integer\n";
                    return ;
                }
                /*if (argv[4][2] != '~' || argv[4][2] != '-')
                {
                    cout << "Illegal paramater\n";
                    cout << "The split symbol should be ~ or -\n";
                    return 1;
                }*/
                if (this->argv[4][3] > '9' || this->argv[4][3] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The forth param should be a positive integer\n";
                    return ;
                }
                if (this->argv[4][4] > '9' || this->argv[4][4] < '0') {
                    cout << "Illegal paramater\n";
                    cout << "The forth param should be a positive integer\n";
                    return ;
                }
                int number1 = 0;
                int number2 = 0;
                int temp1 = (this->argv[4][0] - '0') * 10;
                int temp2 = (this->argv[4][3] - '0') * 10;
                number1 = temp1 + this->argv[4][1] - '0';
                number2 = temp2 + this->argv[4][4] - '0';
                this->type2 = 'r';
                this->range1 = number1;
                this->range2 = number2;
            }
        }
    } else {
        cout << "Illegal paramater number\n"
            << "Usage:\n"
            << "      sudoku.exe -c number --> "
            << "generate n sudoku finals. \n"
            << "      sudoku.exe -s path --> Read sudoku from"
            << " file in the given path,and solve them.\n"
            << "      sudoku.exe -n number --> produce sudoku problem"
            << " into the given path(prepare for the sudoku game)\n"
            << "      The charactre should be itself:"
            << "such as C is not equal to c.\n";
        return ;
    }
}

char Input::GetType1() {
    char c;
    c = this->type1;
    return c;
}

char Input::GetType2() {
    char c;
    c = this->type2;
    return c;
}

int Input::GetNum() {
    int x;
    x = this->num;
    return x;
}

int Input::GetDiff() {
    int x;
    x = this->diff;
    return x;
}

int Input::GetRange1() {
    int x;
    x = this->range1;
    return x;
}

int Input::GetRange2() {
    int x;
    x = this->range2;
    return x;
}
