# C-sar
A simple Caesar cipher encrypter/decrypter for POSIX systems.
Encrypts a string or file and prints cipher to stdout.

C-sar features:
- En/decryption of strings and files.
- Option for showing all 26 possible encryptions ``(-a)``.

# Installation
Make it (using make) with ``make clean`` from the project root. This uses gcc, so that is also required.

# Usage
``c-sar [key] [string] [-i inputfile] [-a] [-h] [-v]``

Example usage:
- ``c-sar -h`` will print some (hopefully) helpful text.
- ``c-sar 2 hello`` will encrypt the string "hello" with the key 2.
- ``c-sar -a hello`` will generate all possible encryptions of the string "hello"
- ``c-sar 42 "forty two"`` will encrypt the string "forty two" with the key 42.
- ``c-sar 5 -i plaintext.txt`` will encrypt the contents of file ``plaintext.txt`` with the key 5.

# Known issues
- Currently only supports English letters (a-z) (other characters are simply copied to the cipher as they are). *Support for custom alphabets is planned for the future.*
- Currently only works on POSIX systems. This is due to how the program reads files. *Windows support is planned for the future.*