# Linear-Tokenizer
## Introduction
This is a high-efficiency toolkit which can tokenize a given text with linear time complexity.
|Process|Time Complexity|
|:---:|:---:|
|Preprocessing|O(M)|
|Tokenization|O(N)|
- M: Total number of the characters in the vocabulary

- N: Total length of the text
### Tools for Different Language
English:tokenizer-en (brute version)

Chinese:tokenizer-ch (still under development)
## Usage
### File Hierarchy
```
.
├── tokenizer-en.cpp
├── vocab.in
├── text.in
└── result.out(generated after running the tokenizer)
```

### Vocabulary(vocab.in)
Put the vocabulary information in `vocab.in` in the following form:

One single word/word-piece per line (add ## before a word-piece)

For example:
```
apple
orange
play
player
##er
##ing
```
### Text(text.in)
Put the text in `text.in`
### **Process**
Step-1. Put the data in `vocab.in` and `text.in` respectively.

Step-2. Run the command `g++ -o tokenize.exe tokenizer-en.cpp` to compile `tokenizer.cpp`.

Step-3. Run the command `./tokenize.exe` to run the tool.
Step-4. See the generated result in `result.out`.
