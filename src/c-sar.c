#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *parseOptions(int argc, char *argv[], char *filepath, bool *tryAllKeys);
void printUsageExit(char *argv[]);
char *encrypt(char *input, int key, char *alphabet, char *output);
int findCharIndex(char c, char *alphabet);

char *PROGRAM_VERSION = "1.0";

/* A simple caesar cipher program written by Oliver Razpotnik.
 * Supports encrypting command-line-supplied strings or files.
 * 
 * Version: 1.0 (First public version)
 * Currently only supports encryption of lowercase characters.
 *
 * TODO uppercase characters, custom alphabets, negative keys.
 */
int main(int argc, char *argv[])
{
    bool tryAllKeys = false;
    char *filepath = "";
    char *inputString;

    filepath = parseOptions(argc, argv, filepath, &tryAllKeys);

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

    char *decodedString = calloc(strlen(inputString), sizeof(char));
    
    if (tryAllKeys)
        {
            for (int i = 0; i < strlen(alphabet); i++)
                {
                    fprintf(stdout, "%d:    %s\n", i,
                            encrypt(inputString, i, alphabet, decodedString));
                }
            return 0;
        }
    else
        {
            int key = strtol(argv[optind], NULL, 10);
            fprintf(stdout, "%s\n", encrypt(inputString, key,
                                            alphabet, decodedString));
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

/* Parse command-line options, setting option variables.
 *
 */
char *parseOptions(int argc, char *argv[], char *input, bool *tryAllKeys)
{

    char *opts = "vi:ah";
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
                    fprintf(stderr, "c-sar: a simple Caesar cipher en-/de-crypter\n")
                    fprintf(stderr, "Usage: %s [key] [string] [-i inputfile] [-a]\n\n",
                            argv[0]);
                    fprintf(stderr, "   key: Specify a key (positive integer)\n");
                    fprintf(stderr, "string: String to encrypt\n");
                    fprintf(stderr, "    -v: Print the program version\n");
                    fprintf(stderr, "    -i: Specify a file to encrypt\n");
                    fprintf(stderr, "    -a: Show all permutations\n");
                    exit(EXIT_SUCCESS);
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
