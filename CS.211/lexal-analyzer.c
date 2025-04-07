#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;           // Stores the character class of the current character
char lexeme[100];        // Array to store the current lexeme
char nextChar;           // The current character being processed
int lexLen;              // Length of the current lexeme
int token;               // Current token
int nextToken;           // Next token to be processed
char inputBuffer[100];   // Buffer to store the current line of input
int inputPos = 0;        // Position in the input buffer

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define EOF -1

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20  // Added assignment operator token code
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/******************************************************/
/* main driver */
int main() {
    printf("Enter input: ");
    while (fgets(inputBuffer, sizeof(inputBuffer), stdin)) {  // Read a line of input from standard input
        inputPos = 0;            // Reset input position for new line
        getChar();               // Initialize by getting the first character
        do {
            lex();               // Perform lexical analysis
        } while (nextToken != EOF);
        printf("Enter input: "); // Prompt for the next line of input
    }
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=':                  // Added case for '=' operator.
            addChar();
            nextToken = ASSIGN_OP;
            break;
        default:
            addChar();
            nextToken = UNKNOWN;   // Use UNKNOWN rather than EOF for unrecognized symbols.
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {           // Ensure lexeme array doesn't overflow
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';    // Null-terminate the string
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its character class */
void getChar() {
    if (inputBuffer[inputPos] != '\0' && inputBuffer[inputPos] != '\n') {  // Check if end of buffer or newline
        nextChar = inputBuffer[inputPos++];  // Get next character from buffer
        if (isalpha(nextChar))
            charClass = LETTER;   // Character is a letter
        else if (isdigit(nextChar))
            charClass = DIGIT;    // Character is a digit
        else
            charClass = UNKNOWN;  // Character is neither letter nor digit
    } else {
        charClass = EOF;          // End of input
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /* EOF */
        case EOF:
            nextToken = EOF;
            strcpy(lexeme, "EOF");
            break;
    }
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
