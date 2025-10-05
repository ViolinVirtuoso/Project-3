Samuel Strong
005831181
https://github.com/ViolinVirtuoso/Project-3

Collaboration & Sources:
I got help from you, professor, to get my project up and running. Then, with very explicit instructions not to give me the solution, I prompted Claude AI to help explain what I was seeing in the code you provided. With that, I was able to write the Scanner constructor, the readWord function, and the first tokenize function. However, I was slightly confused with the overloaded tokenize function, so I asked Claude to give me a hint. Unfortunately, it straight up gave me the solution instead. So I took time to understand its answer and reworded it to make it my own code.

Implementation Details:
I have 5 files. main.cpp, Scanner.hpp, Scanner.cpp, utils.hpp, and utils.cpp.
main.cpp is the tester.
Scanner.hpp and Scanner.cpp define a class that can scan an input txt file and "tokenize" it into a .tokens file.
utils.hpp and utils.cpp define a class that is used in main and in Scanner to throw various errors if things go wrong.
You can find comments to help you along in your reading of my code within my code.

Testing & Status:
Everything works.
Please note, though, that the input file you gave us contains a curly apostrophe, instead of a normal ASCII one.
I could not find a clean way to handle it, so I changed it to the ASCII version and it worked fine.
Be aware that my program does not detect apostrophes that are not ASCII.
Here is the output:

"C:\Users\samue\CLionProjects\Project 3\cmake-build-debug\Project_3.exe" input_output/TheBells.txt

Process finished with exit code 0
