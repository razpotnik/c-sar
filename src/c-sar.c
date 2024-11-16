#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

char *parseOptions(int argc, char *argv[], char *filepath,
                   bool *tryAllKeys, int *key);
void printUsageExit(char *argv[]);
char *encrypt(char *input, int key, char *alphabet, char *output);
int findCharIndex(char c, char *alphabet);
bool *checkCapitalization(char *s, bool *cap);
char *capitalize(char *s, bool *cap);

char *PROGRAM_VERSION = "1.1";

/* A simple caesar cipher program written by Oliver Razpotnik.
 * Supports encrypting command-line-supplied strings or files.
 * 
 * Version: 1.1(Capitalization support)
 * Now supports capitalization!
 *
 * TODO custom alphabets, negative keys.
 */
int main(int argc, char *argv[])
{
    bool tryAllKeys = false;
    char *filepath = "";
    char *inputString;
    int key = 0;

    filepath = parseOptions(argc, argv, filepath, &tryAllKeys, &key);

    if (argc < 3) printUsageExit(argv);
    
    char *alphabet = "abcdefghijklmnopqrstuvwxyz";

    if (filepath == "") inputString = argv[2];
    else
        {
            FILE *fp = fopen(filepath, "rb");
            size_t len = 0;
            ssize_t bytes_read = getdelim(&inputString, &len, '\0', fp);
            if (bytes_read == -1)
                {
                    fprintf(stderr, "Error while parsing file.");
                    return 1;
                }
        }

    int inputlen = strlen(inputString);
    char *decodedString = calloc(inputlen, sizeof(char));
    bool *cap = calloc(inputlen, sizeof(bool));

    cap = checkCapitalization(inputString, cap);

    for (int i = 0; i < inputlen; i++)
        {
            inputString[i] = (char) tolower(inputString[i]);
        }
    
    if (tryAllKeys)
        {
            for (int i = 0; i < strlen(alphabet); i++)
                {
                    decodedString = encrypt(inputString, i, alphabet, decodedString);
                    decodedString = capitalize(decodedString, cap);
                    fprintf(stdout, "%d:    %s\n", i, decodedString);
                }
            return 0;
        }
    else
        {
            if (key == 0)
                {
                    key = strtol(argv[optind], NULL, 10);
                }
            decodedString = encrypt(inputString, key, alphabet, decodedString);
            decodedString = capitalize(decodedString, cap);
            fprintf(stdout, "%s\n", decodedString);
            return 0;
        }

    free(decodedString);
    return 1;
}

/* encrypt
 * Encrypt a string with a key over an alphabet, saving to a buffer.
 * If a character is not in the alphabet, the original character is used.
 * input: the string to encrypt
 * key: the key
 * alphabet: the alphabet
 * output: the buffer to save encrypted result in
 */
char *encrypt(char *input, int key, char *alphabet, char *output)
{    
    int inputLength = strlen(input);
    for (int i = 0; i < inputLength; i++)
        {
            char currentChar = input[i];
            int currentCharIndex = findCharIndex(currentChar, alphabet);
            if (currentCharIndex != -1) {
                output[i] = alphabet[(currentCharIndex + key) % strlen(alphabet)];
            } else output[i] = input[i];
        
        }
    return output;
}

/* findCharIndex
 * Finds the index of a char in an alphabet and returns it.
 * If the character is not found, returns -1.
 * c: the character to look for
 * alphabet: the alphabet
 */
int findCharIndex(char c, char *alphabet)
{
    for (int i = 0; i < strlen(alphabet); i++)
        {
            if (c == alphabet[i]) return i;
        }
    return -1;
}


/* checkCapitalization
 * Checks capitalization of chars in char array and constructs bool array.
 * If the character at index i is uppercase, puts true at index i in cap.
 * s: the char array to check
 * cap: the bool array for capitalization
 * returns: the bool array
 */
bool *checkCapitalization(char *s, bool *cap)
{
    for (int i = 0; i < strlen(s); i++)
        {
            cap[i] = isupper(s[i]);
        }
    return cap;
}

/* capitalize
 * Capitalizes characters in char array based on capitalization array.
 * If the bool at index i in capitalization array is true,
 * capitalize corresponding character in char array.
 * s: the char array to capitalize
 * cap: the bool array for capitalization
 * returns: the capitalized char array
 */
char *capitalize(char *s, bool *cap)
{
    for (int i = 0; i < strlen(s); i++)
        {
            if (cap[i])
                {
                    s[i] = (char) toupper(s[i]);
                }
        }
    return s;
}

/* Parse command-line options, setting option variables.
 *
 */
char *parseOptions(int argc, char *argv[], char *input,
                   bool *tryAllKeys, int *key)
{

    char *opts = "vi:ah1234567890";
    int opt;

    while((opt=getopt(argc, argv,opts)) != -1)
        {
            switch(opt)
                {
                case 'v': // Print program version
                    fprintf(stderr, "c-sar version %s\n", PROGRAM_VERSION);
                    exit(EXIT_SUCCESS);
                case 'i': // Specify input file
                    input = optarg;
                    break;
                case 'a': // Try all keys (1-25) - in case key is unknown
                    *tryAllKeys = true;
                    break;
                case 'h': // Show help
                    fprintf(stderr, "c-sar: a simple Caesar cipher en-/de-crypter\n");
                    fprintf(stderr, "Usage: %s [key] [string] [-i inputfile] [-a]\n\n",
                            argv[0]);
                    fprintf(stderr, "   key: Specify a key (positive integer)\n");
                    fprintf(stderr, "string: String to encrypt\n");
                    fprintf(stderr, "    -v: Print the program version\n");
                    fprintf(stderr, "    -i: Specify a file to encrypt\n");
                    fprintf(stderr, "    -a: Show all permutations\n");
                    exit(EXIT_SUCCESS);
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                    *key = atoi(argv[1]);
                    break;
                default: // Incorrect usage
                    printUsageExit(argv);
                }
        }
    return(input);
}

/* Print the program usage string and exit with an error code.
 */
void printUsageExit(char *argv[])
{
    fprintf(stderr, "Incorrect usage! Usage: %s key [string] [-i inputfile] [-a]\n", argv[0]);
    exit(EXIT_FAILURE);
}
