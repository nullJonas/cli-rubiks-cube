#include <stdio.h>
#include <stdlib.h>
#include "turns.h"

#define BUFFER_SIZE 256

/*
Create a cube in the solved state
*/
struct cube *init_cube(){
    int i, j, k;
    struct cube *cube = malloc(sizeof(struct cube));
    char colors[6] = "WOGRBY";

    for (i = 0; i < 6; i++)
        for (j = 0; j < 3; j++)
            for (k = 0; k < 3; k++)
                cube->stickers[i][j][k] = colors[i];        

    return cube;
}

/*
Print a colored character in the terminal
*/
void show_sticker(char c){
    switch (c){
        case 'W':
            printf("\e[48;2;255;255;255m  "); // White
            break;
        case 'O':
            printf("\e[48;2;255;165;0m  "); // Orange
            break;
        case 'G':
            printf("\e[48;2;0;255;0m  "); // Green
            break;
        case 'R':
            printf("\e[48;2;255;0;0m  "); // Red
            break;
        case 'B':
            printf("\e[48;2;0;0;255m  "); // Blue
            break;
        case 'Y':
            printf("\e[48;2;255;255;0m  "); // Yellow
            break;
        default:
            break;
    }
    printf("\e[0m");
}

void show_cube(struct cube *cube){
    int i, j, k;
    // U side
    printf("\n");
    for (i = 0; i < 3; i++){
        printf("          ");
        for (j = 0; j < 3; j++)
            show_sticker(cube->stickers[0][i][j]);
        printf("\n");
    }
    // F, R, B and L sides
    printf("\n");
    for (i = 0; i < 3; i++){
        for (k = 1; k < 5; k++){
            printf("  ");
            for (j = 0; j < 3; j++)
                show_sticker(cube->stickers[k][i][j]);
        }
        printf("\n");
    }
    printf("\n");
    // D side
    for (i = 0; i < 3; i++){
        printf("          ");
        for (j = 0; j < 3; j++)
            show_sticker(cube->stickers[5][i][j]);
        printf("\n");
    }
    printf("\n\n");
}

void process_input(struct cube *cube, char buffer[]){
    int i, code1, code2, code3;
    unsigned char useCode2, useCode3, isWide;

    for (i = 0; buffer[i] != '\0'; i++){
        useCode2 = useCode3 = isWide = 0;
        // Check for lowercase wide turns
        if (buffer[i] > 'Z'){
            isWide = 1;
            buffer[i] -= 32;
        }
        // Normal turns
        switch (buffer[i]){
            case 'R':
                code1 = 0;
                break;
            case 'U':
                code1 = 1;
                break;
            case 'F':
                code1 = 2;
                break;
            case 'L':
                code1 = 3;
                break;
            case 'D':
                code1 = 4;
                break;
            case 'B':
                code1 = 5;
                break;
            case 'M':
                code1 = 6;
                isWide = 0;
                break;
            case 'E':
                code1 = 7;
                isWide = 0;
                break;
            case 'S':
                code1 = 8;
                isWide = 0;
                break;

        // Cube rotations
            case 'X':
                code1 = 0;
                code3 = 12;
                useCode3 = 1;
                isWide = 1;
                break;
            case 'Y':
                code1 = 1;
                code3 = 13;
                useCode3 = 1;
                isWide = 1;
                break;
            case 'Z':
                code1 = 2;
                code3 = 14;
                useCode3 = 1;
                isWide = 1;
                break;
            default:
                continue;
        }
        // Wide turns
        if (isWide || buffer[i + 1] == 'w'){
            useCode2 = 1;
            if (code1 == 0)
                code2 = 15;
            else if (code1 == 1)
                code2 = 16;
            else if (code1 == 2)
                code2 = 8;
            else if (code1 == 3)
                code2 = 6;
            else if (code1 == 4)
                code2 = 7;
            else if (code1 == 5)
                code2 = 17;
            if (!isWide)
                i++;
        }
        // Reverse turns
        if (buffer[i + 1] == '\''){
            code1 = (code1 + 9) % 18;
            code2 = (code2 + 9) % 18;
            code3 = (code3 + 9) % 18;
            i++;
        }
        // Double turns
        if (buffer[i + 1] == '2'){
            turn(cube, code1);
            if (useCode2){
                turn(cube, code2);
                if (useCode3)
                    turn(cube, code3);
            }
            i++;
        }
        // Applying the turns
        turn(cube, code1);
        if (useCode2){
            turn(cube, code2);
            if (useCode3)
                turn(cube, code3);
        }
    }
}

int main(){
    char buffer[BUFFER_SIZE];
    struct cube *cube = init_cube();

    show_cube(cube);
    while (scanf(" %s", buffer) != EOF){
        process_input(cube, buffer);
        show_cube(cube);
    }
    
    free(cube);
}