#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

void input_matrix(int matrix[25][80], int height, int width);
void update_matrix(int matrix1[25][80], int matrix2[25][80], int height, int width);
int count_neighbors(int matrix1[25][80], int i, int j, int height, int width);
int decision(int neighbors, int condition);
void replace(int matrix2[25][80], int matrix1[25][80], int height, int width);
int check(int matrix1[25][80], int matrix2[25][80], int height, int width);
int change_speed(char control_button, int *flag, int time_mili_sec);
int count(int matr[25][80], int height, int width);

int main() {
    int height = 25;
    int width = 80;

    int matr1[25][80];
    int matr2[25][80];
    int time_mili_sec = 500;
    int stop = 0;
    input_matrix(matr1, height, width);
    if (freopen("/dev/tty", "r", stdin)) initscr();  // Инициализация ncurses
    nodelay(stdscr, true);
    while (stop != 1) {
        char control_button = getch();
        if (count(matr1, height, width) == 0) {
            stop = 1;
        }
        time_mili_sec = change_speed(control_button, &stop, time_mili_sec);
        usleep(time_mili_sec * 1000);
        clear();
        update_matrix(matr1, matr2, height, width);
        if (check(matr1, matr2, height, width) == 2000) {
            stop = 1;
        }
        replace(matr2, matr1, height, width);
    }
    endwin();
    return 0;
}

void input_matrix(int matr[25][80], int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &matr[i][j]);
        }
    }
}

void update_matrix(int matr1[25][80], int matr2[25][80], int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matr2[i][j] = decision(count_neighbors(matr1, i, j, height, width), matr1[i][j]);
            if (matr2[i][j] == 1)
                printw("0");
            else
                printw("-");
        }
        printw("\n");
    }
}

int change_speed(char control_button, int *flag, int time_mili_sec) {
    if (control_button == '1')
        time_mili_sec = 900;
    else if (control_button == '2')
        time_mili_sec = 400;
    else if (control_button == '3')
        time_mili_sec = 70;
    else if (control_button == 'q')
        *flag = 1;
    return time_mili_sec;
}

int count(int matr[25][80], int height, int width) {
    int sum = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            sum += matr[i][j];
        }
    }
    return sum;
}

void replace(int matrix2[25][80], int matrix1[25][80], int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix1[i][j] = matrix2[i][j];
        }
    }
}

int check(int matrix1[25][80], int matrix2[25][80], int height, int width) {
    int ans = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (matrix1[i][j] == matrix2[i][j]) ans++;
        }
    }
    return ans;
}

int count_neighbors(int matrix1[25][80], int i, int j, int height, int width) {
    int sum = 0;
    int i_minus = i - 1, j_minus = j - 1, i_plus = i + 1, j_plus = j + 1;
    if (i_minus < 0) i_minus = height - 1;
    if (j_minus < 0) j_minus = width - 1;
    if (i_plus > height - 1) i_plus = i_plus % height;
    if (j_plus > width - 1) j_plus = j_plus % width;
    sum += matrix1[i_minus][j_minus];
    sum += matrix1[i_minus][j];
    sum += matrix1[i_minus][j_plus];
    sum += matrix1[i][j_plus];
    sum += matrix1[i_plus][j_plus];
    sum += matrix1[i_plus][j];
    sum += matrix1[i_plus][j_minus];
    sum += matrix1[i][j_minus];
    return sum;
}

int decision(int neighbors, int condition) {
    if ((neighbors == 2 || neighbors == 3) && condition == 1) {
        return 1;
    } else if (neighbors == 3 && condition == 0) {
        return 1;
    } else {
        return 0;
    }
}