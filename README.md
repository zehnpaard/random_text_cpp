# random_text_cpp
Random Text Generator using Markov Chains in C++

Input
- file: text file name
- n: n-gram size
- length: total number of words to be generated

Generate random text through the following steps:
1. Read a text file
2. Choose a random (n-1)-gram from the text
3. Generate the following word based on frequencies of following words in the source text
4. Repeat steps 2 and 3 until *length* words have been chosen
5. Print the words

Considerations:
- If an (n-1)-gram from the end of the file is chosen, it is possible that there is no corresponding word that will follow.
- (Solution) Wrap the first n-1 words to the end of the file so that the ngrams form a loop
