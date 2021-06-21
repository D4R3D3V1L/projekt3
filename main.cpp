#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <algorithm>

#define MAX_INT 2147483647

using namespace std;


class TicTacToe{

    int board[10][10];
    int size;
    int winning;
    int alg_depth;

public:


void clear_board(){

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){

                board[i][j]=0;
        }
    }
}
void display(){

        system("cls");

    cout << "   \t";
    for (int i = 0; i < size; i++) {
            cout  <<"  " << i << "\t";
    }
    for (int i= 0; i < size; i++) {

        cout<<endl << "    " << i << " \t";

        for (int j = 0; j < size; j++) {
            switch (board[i][j])
            {
            case 1:
                cout << "| O |\t";
                break;

            case -1:
                cout << "| X |\t";
                break;

            default:
                cout << "|   |\t";
                break;
            }
        }
        cout << endl;
        cout << "   \t";
        for (int i = 0; i < size; i++) {
               cout << " ---\t";
        }
    }
}

bool if_correct(int row, int column){

    if(row < 0 || row >= size){
        cout<<"Niepoprawna wartosc wspolrzednych!";
        return false;
    }
    if(column < 0 || column >= size){
        cout<<"Niepoprawna wartosc wspolrzednych!";
        return false;
    }
    if(board[row][column] != 0){
        cout<<"Niepoprawna wartosc wspolrzednych!";
        return false;
    }
    return true;
}

void player_move(int sign){


    int row, column;

    do{
        cout << endl;
        cout << "podaj wiersz:";
        cin >> row;
        cout << "podaj kolumne:";
        cin >> column;
    }while(!if_correct(row, column));

    board[row][column] = sign;
}


int if_win() {
    int sign[2] = {-1 , 1}; // -1 kołko , 1 krzyżyk  
    int score = 0;
    int score_vert = 0;

    for (int s = 0; s < 2; s++) {
        for (int row = 0; row < size; row++) {
            score = 0;
            score_vert = 0;
            for (int column = 0; column < size; column++) {

                // Sprawdzenie poziomu

                if(board[row][column] == sign[s]){
                    score++;
                    if(score >= winning) {
                        return sign[s];
                    }
                } else {
                    score = 0;
                }

                // Sprawdzenie pionu

                if(board[column][row] == sign[s]){
                    score_vert++;
                    if(score_vert >= winning) {
                        return sign[s];
                    }
                } else {
                    score_vert = 0;
                }
            }
        }

    // Sprawdzenie ukosów

        // ukos od lewej strony

        for(int row = 0; row < size; row++) {
            for(int column = 0; column < size; column++) {
                int now_row = row;
                int now_column = column;
                score = 0;
                while(now_row < size && now_column < size) {
                    if(board[now_row][now_column] == sign[s]) {
                        score++;
                        now_row++;
                        now_column++;
                        if(score >= winning) {
                            return sign[s];
                        }
                    } else {
                        score = 0;
                        now_row++;
                        now_column++;
                    }
                }
            }
        }

        // ukos od prawej

        for(int row = 0; row < size; row++) {
            for(int column = size-1; column >=0; column--) {
                int now_row = row;
                int now_column = column;
                score = 0;
                while(now_row < size && now_column >= 0) {
                    if(board[now_row][now_column] == sign[s]) {
                        score++;
                        now_row++;
                        now_column--;
                        if(score >= winning) {
                            return sign[s];
                        }
                    } else {
                        score = 0;
                        now_row++;
                        now_column--;
                    }
                }
            }
        }            
    }
    return 0; // brak wygranej i przegranej
}


void AI_move(int sign) {
    int score = 0;


    int best_move = -MAX_INT;
    int best_row = 0;
    int best_col= 0;


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                board[i][j] = sign;
                if (if_win() == sign) score = MAX_INT;
                else score = MiniMax( 0, false, -MAX_INT, MAX_INT, sign);
                board[i][j] = 0;
                if (score > best_move) {
                    best_move = score;
                    best_row = i;
                    best_col= j;
                }
            }
            else;
        }
    }
    board[best_row][best_col] = sign;

}

// Algorytm alpha-beta - ulepszony minimax
int MiniMax(int depth, bool minimax, int alfa, int beta,int sign) {
    int score=0;
    int temp = if_win();

    if (temp == -sign) return -1000/ depth;
    if (temp == sign) return 1000/ depth;

    if (depth == alg_depth) return temp;

    if (minimax) {
        int best_score = -MAX_INT;
        // Sekwencja MAX
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                if (board[i][j] == 0) {

                    board[i][j] = sign;

                    score = MiniMax(depth+1 , false, alfa, beta, sign);
                    board[i][j] = 0;
                    best_score= max(score, best_score);
                    alfa = max(alfa, score);
                    if (beta <= alfa) break;
                }
            }
        }
        return best_score;
    }
    else {
        // Sekwencja MIN
        int best_score = MAX_INT;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) {

                    board[i][j] = -sign;

                    score = MiniMax( depth+1 , true, alfa, beta, sign);
                    board[i][j] = 0;
                    best_score = min(score, best_score);
                    beta = min(beta, score);
                    if (beta <= alfa) break;
                }
            }
        }
        return best_score;
    }

}


void play(){
    bool turn = true;
    int player_sign, AI_sign = 0;
    int select = 0;
    int moves = 0;

    cout << "-------------Witaj w grze Kolko i Krzyzyk!!!-------------\n";

    cout << endl << "Podaj rozmiar planszy:\n";
    cin >> size;

    cout<< "Podaj ilosc znakow w rzedzie potrzebnych do wygranej:\n";
    cin >> winning;

    cout << "Podaj glebokosc przeszukiwan algorytmu:\n";
    cin >> alg_depth;

    clear_board();


    do{
        cout << "Wybierz czy gracz ma kolko czy krzyzyk? (Kolko zaczyna)\n";
        cout << "1. Kolko\n";
        cout << "2. Krzyzyk\n";
        cin >> select;

        switch(select){

        case 1:
            player_sign = 1;
            AI_sign = -1;
            
            break;
        case 2:
            player_sign = -1;
            AI_sign = 1;
            break;

        default:
            select = 0;
            cout << "Niepoprawny wybor!\n";
            break;

        }
        }while(select==0);

    if(player_sign==1){

        turn = false;
    }

    while(if_win()==0 && moves < size*size){
        display();
        ++moves;

    if (turn == true) {
        AI_move(AI_sign);
        turn = false;
    }
    else {
        player_move(player_sign);
        turn = true;
    }
    display();
    cout<<endl;
    }

    switch (if_win()) {
    case 1:
        cout << "Zwycieza gracz kolkami";
        break;
    case -1:
        cout << "Zwycieza gracz krzyzykami";
        break;
    case 0:
        cout << "Gra zakonczona remisem";
        break;
    }
}

};

int main(){

    TicTacToe game;

    game.play();

}
