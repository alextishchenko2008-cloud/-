
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 30

char board[MAX][MAX];
int size;
int win_length;

void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int read_int(void) {
    int x;

#ifdef _MSC_VER
    while (scanf_s("%d", &x) != 1) {
#else
    while (scanf("%d", &x) != 1) {
#endif
        printf("Enter a valid number: ");
        clear_input();
    }

    return x;
    }

void init_board(void) {
    int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = '.';
        }
    }
}

void print_board(void) {
    int i, j;

    printf("\n    ");

    for (i = 0; i < size; i++) {
        printf("%3d", i + 1);
    }

    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%3d ", i + 1);

        for (j = 0; j < size; j++) {
            printf("%3c", board[i][j]);
        }

        printf("\n");
    }
}

int check_win(char symbol) {
    int i, j, k, d;
    int r, c;

    int dr[4] = { 0, 1, 1, 1 };
    int dc[4] = { 1, 0, 1, -1 };

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] != symbol) {
                continue;
            }

            for (d = 0; d < 4; d++) {
                for (k = 0; k < win_length; k++) {
                    r = i + dr[d] * k;
                    c = j + dc[d] * k;

                    if (r < 0 || r >= size ||
                        c < 0 || c >= size ||
                        board[r][c] != symbol) {
                        break;
                    }
                }

                if (k == win_length) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int board_full(void) {
    int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == '.') {
                return 0;
            }
        }
    }

    return 1;
}

void player_move(void) {
    int row, col;

    while (1) {
        printf("\nEnter row (1-%d): ", size);
        row = read_int();

        printf("Enter column (1-%d): ", size);
        col = read_int();

        if (row < 1 || row > size ||
            col < 1 || col > size) {
            printf("Outside the board!\n");
            continue;
        }

        row--;
        col--;

        if (board[row][col] != '.') {
            printf("Cell is occupied!\n");
            continue;
        }

        board[row][col] = 'X';
        return;
    }
}

void bot_move(void) {
    int i, j;
    int count, target;

    /* Try to win */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == '.') {
                board[i][j] = 'O';

                if (check_win('O')) {
                    printf("\nBot: %d %d\n", i + 1, j + 1);
                    return;
                }

                board[i][j] = '.';
            }
        }
    }

    /* Block player */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == '.') {
                board[i][j] = 'X';

                if (check_win('X')) {
                    board[i][j] = 'O';
                    printf("\nBot: %d %d\n", i + 1, j + 1);
                    return;
                }

                board[i][j] = '.';
            }
        }
    }

    /* Random free cell */
    count = 0;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == '.') {
                count++;
            }
        }
    }

    if (count == 0) {
        return;
    }

    target = rand() % count;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == '.') {
                if (target == 0) {
                    board[i][j] = 'O';
                    printf("\nBot: %d %d\n", i + 1, j + 1);
                    return;
                }

                target--;
            }
        }
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    printf("TIC TAC TOE\n");
    printf("Enter board size (3-%d): ", MAX);

    size = read_int();

    if (size < 3 || size > MAX) {
        printf("Invalid board size!\n");
        return 1;
    }

    if (size <= 5) {
        win_length = size;
    }
    else {
        win_length = 5;
    }

    init_board();

    printf("\nYou are X, bot is O\n");
    printf("To win, get %d in a row.\n", win_length);

    while (1) {
        print_board();
        player_move();

        if (check_win('X')) {
            print_board();
            printf("\nYOU WIN!\n");
            break;
        }

        if (board_full()) {
            print_board();
            printf("\nDRAW!\n");
            break;
        }

        bot_move();

        if (check_win('O')) {
            print_board();
            printf("\nBOT WINS!\n");
            break;
        }

        if (board_full()) {
            print_board();
            printf("\nDRAW!\n");
            break;
        }
    }

    return 0;
}
