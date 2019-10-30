# words
Searches given word (string) in provided directory and subdirectories.
The program has graphical interface based on the Qt Widgets library.
## Features
- Progress bar for indexing and searching
- Opens file with highlighted found words
- Iterates over found words
- Listener on files changes
- Indexing and searching runs on separate thread to not to hang application
- If file is large then indexing/searching computed multithreaded
- Cancels task if needed
## Algorithm
Indexing:
1. for each file:
2. make sure that file is non binary (using magic numbers)
3. read file and add all trigrams to set

Searching:

1. compute trigrams of the given word (string)
2. for each file:
3. check that file trigrams contain all word trigrams
4. find words using regex
