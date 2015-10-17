#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 65536
#define MAX_DEPTH 30
#define MAX_LENGTH 65536

#define INC  1
#define DEC  2
#define FWD  3
#define BCK  4
#define PUT  5
#define GET  6
#define JMPF 7
#define JMPB 8
#define END  9

typedef struct {
    unsigned int data[MAX_DEPTH];
    int top;
} Stack;

typedef struct {
    unsigned short opcode;
    unsigned short operand;
} BF;

unsigned int pop(Stack* stack) {
    if (stack->top > 0) {
        return stack->data[--stack->top];
    } else {
        printf("No outer loop. Invalid ']' character\n"); exit(0);
    }
}

void push(Stack* stack, unsigned int open) {
    if (stack->top < MAX_DEPTH) {
        stack->data[stack->top++] = open;
    } else {
        printf("Max loop depth exceeded\n"); exit(0);
    }
}

BF* compile(FILE* f) {
    BF program[MAX_LENGTH];
    char cur;
    unsigned int loc = 0, prevLoc;
    Stack stack;

    stack.top = 0;

    while ((cur = fgetc(f)) != EOF && loc < MAX_LENGTH) {
        switch (cur) {
        case '+': program[loc].opcode = INC; break;
        case '-': program[loc].opcode = DEC; break;
        case '>': program[loc].opcode = FWD; break;
        case '<': program[loc].opcode = BCK; break;
        case '.': program[loc].opcode = PUT; break;
        case ',': program[loc].opcode = GET; break;
        case '[':
            program[loc].opcode = JMPF;
            push(&stack, loc);
            break;
        case ']':
            /* pop the address of the location to jump back to */
            prevLoc = pop(&stack);

            program[loc].opcode = JMPB;
            program[loc].operand = prevLoc;

            /* store the address of the end of the loop */
            program[prevLoc].operand = loc;
            break;
        default:
            loc--;
        }
        loc++;
    }
    program[loc].opcode = END;

    return program;
}

void run(BF* program) {
    unsigned char buffer[MAX_LENGTH], *p = buffer;

    for (int i = 0; i < MAX_LENGTH; i++, p++) {
        *p = 0;
    }

    p = buffer;

    unsigned int pc = 0;
    while (program[pc].opcode != END) {
        switch (program[pc].opcode) {
        case INC: (*p)++; break;
        case DEC: (*p)--; break;
        case FWD: p++; break;
        case BCK: p--; break;
        case PUT: putchar(*p); break;
        case GET: *p = getchar(); break;
        case JMPF:
            if (!*p) {
                pc = program[pc].operand;

            }
            break;
        case JMPB:
            if (*p) {
                pc = program[pc].operand;
            }
            break;
        }
        pc++;
    }
}


int main(int argc, char* argv[]) {
    if (argc > 1) {
        char* path = argv[1];
        FILE* f;
        if (!(f = fopen(path, "r"))) {
            printf("Cannot open file: %s\n", path);
            return 0;
        }

        BF* program = compile(f);
        fclose(f);

        run(program);
        printf("\n");
    }

    return 1;
}
