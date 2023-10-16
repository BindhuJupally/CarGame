#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int game[10][60];
int last[10][60];
int carPosition=0;
int score=0;
int gameover=0;

void clearScreen()
{
    system("cls");
}

void *handleInput(void *arg)
{
    while(1){
        char move=getch();

        if(move=='W'|| move=='w')
        {
            if(carPosition>0) --carPosition;
        }
        else if(move=='S'||move=='s')
        {
            if(carPosition<9) ++carPosition;

        }
        if(game[carPosition][0]==1){
            for(int i=0; i<10; i++){
                for(int j=0; j<60; j++){
                    last[i][j]=game[i][j];
                }
            }
            gameover=1;
            pthread_exit(NULL);
        }
    }
}

void displayTrack(int game[10][60])
{
    clearScreen();

    for(int i=0;i<10; i++){
        for(int j=0; j<60; j++){
            if(j==0&& carPosition==i){
                if(game[i][j]==0){
                    printf("o>");
                }
                else{
                    printf("X ");
                }
            }
            else if(game[i][j]==1){
                    printf("()");
            }
            else{
                printf("  ");
            }
        }
        printf("\n");
    }
}

void move(){
    printf("Score: %d\n",score);
    for(int i=0; i<10; i++){
        for(int j=1; j<60; j++){
            game[i][j-1]=game[i][j];
        }
    }
    for(int i=0; i<10; i++){
        game[i][59]=0;
    }

    struct timeval t;
    gettimeofday(&t,NULL);

    long long now=t.tv_sec*1000+t.tv_usec/1000;

    srand(now);

    if(rand()%4!=0){
        return;
    }

    for(int i=0; i<3; i++){
        int obs=rand()%10;
        game[obs][59]=1;
    }
}

void* renderGame(void *arg)
{
    int raceDistance = 50;

    while (!gameover)
    {
        move(); //Generates obstacles and moves car by 1 step forward.
        displayTrack(game);

        if(game[carPosition][0]==1){
            for(int i=0; i<10; i++){
                for(int j=0; j<60; j++){
                    last[i][j]=game[i][j];
                }
            }
            gameover=1;
            pthread_exit(NULL);
        }

        usleep(100000);
        score++;
    }
}

int main()
{
    for(int i=0; i<10; i++){
        for(int j=0; j<60; j++){
            game[i][j]=0;
        }
    }
    pthread_t render_thread, input_thread;

    if (pthread_create(&render_thread, NULL, renderGame, NULL) != 0) {
        perror("Failed to create render thread");
        return 1;
    }

    if (pthread_create(&input_thread, NULL, handleInput, NULL) != 0) {
        perror("Failed to create input thread");
        return 1;
    }

    while(gameover != 1){

    }
    pthread_cancel(render_thread);
    pthread_cancel(input_thread);
    clearScreen();
    displayTrack(last);
    printf("\nGAME OVER!\nYour Score is %d\n",score);

    //pthread_join(render_thread, NULL);
    //pthread_join(input_thread, NULL);


    printf("\n");
    return 0;
}
