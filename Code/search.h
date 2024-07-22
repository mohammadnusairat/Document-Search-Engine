#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

/**
 * Cleans a specific token by:
 * - Removing leading and trailing punctuation
 * - Converting to lowercase
 * If the token has no letters, returns the empty string.
 */
string cleanToken(string s) {
    // edge case: when inputted string contains no letters
    int letterCt = 0;
    for (long unsigned int i = 0; i < s.length(); i++) {
        if (isalpha(s.at(i))) {
            letterCt++;
        }
    }
    if (letterCt == 0) {
        return "";
    }

    // edge case: when string is one character
    if (s.length() == 1) {
        if (ispunct(s.at(0))) { // not a letter
            return "";
        } else { // is a letter
            char c = tolower(s.at(0)); // lowercase the letter
            s = "";
            s += c;
            return s;
        }
    }

    // remove all leading punctuation
    while (ispunct(s.at(0))) {
        // remove first character of string
        s = s.substr(1);
    }

    // remove all trailing punctuation
    int lastIndexOfString = s.length() - 1; // find index of last character of string
    while (ispunct(s.at(lastIndexOfString))) {
        // remove last character of string
        s.erase(lastIndexOfString);
        if (s.length() == 0) { // return empty string if no characters left in string
            return "";
        }

        // reset last character index
        lastIndexOfString = s.length() - 1;
    }

    // if no letters in string, then return empty string
    letterCt = 0;
    for (long unsigned int i = 0; i < s.length(); i++) {
        if (isalpha(s.at(i))) {
            letterCt++;
        }
    }
    if (letterCt == 0) {
        return "";
    }

    // convert the token to lowercase
    string lowerCaseTemp = "";
    for (long unsigned int i = 0; i < s.length(); i++) {
        char c = tolower(s.at(i));
        lowerCaseTemp += c;
    }
    s = lowerCaseTemp;

    return s;
}

/**
 * Returns a set of the cleaned tokens in the given text.
 */
set<string> gatherTokens(string text) {
    // get each word, clean it, then save to a set of strings
    istringstream sstream(text); // call an object of class istringstream
    string word;
    set<string> tokenSet;
    while (sstream >> word) { // use insertion operator to get all words into a string variable
        word = cleanToken(word);
        if (word != "") { // empty words not added to set
            tokenSet.emplace(word);
        }
    }

    return tokenSet;
}

/**
 * Builds the inverted index from the given data file. The existing contents
 * of `index` are left in place.
 */
int buildIndex(string filename, map<string, set<string>>& index) {
    // input the file
    ifstream inputFile(filename);

    // make sure file opened correctly
    if (!inputFile.is_open()) {
        index.clear(); // return by reference empty map
        return 0; // return 0
    }

    // organize data into two maps that have the line number then the string
    string line;
    int lineNumber = 1; // txt files start at 1
    map<int, string> urls; // urls index is (text index - 1). odd indexes.
    set<string> words; // text index is (url index + 1). even indexes.
    int numURL = 0; // number of webpages added to index
    while (getline(inputFile, line)) {
        if (lineNumber % 2 == 0) { // line is currently the text
            // get set of token words
            words = gatherTokens(line);

            // save token words with respective url to invertedIndex map
            for (set<string>::iterator it = words.begin(); it != words.end(); ++it) {
                index[*it].emplace(urls[lineNumber - 1]);
            }
            numURL++;
        } else {
            urls[lineNumber] = line;
        }
        lineNumber++;
    }
    
    // Reset file stream to the beginning of the file
    inputFile.clear();  // Clear any error flags
    inputFile.seekg(0); // Move the read pointer to the beginning

    return numURL;
}

/**
 * Takes in a string, and returns the number of words in the string.
 */
int findNumWordsInString(string line) {
    istringstream iss(line);
    string word;
    int wordCount = 0;

    // Counting words by reading each word from the line
    while (iss >> word) {
        wordCount++;
    }

    return wordCount;
}

/**
 * Runs a search query on the provided index, and returns a set of results.
 *
 * Search terms are processed using set operations (union, intersection, difference).
 */
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    // Assumptions: 
    // 1. The query sentence contains at least one search term.
    // 2. The first search term in the query sentence will never have a modifier.
    // 3. Every search term has at least one letter.
    // 4. There are no leading or trailing spaces.

    // initialize set of token words
    set<string> tokens = gatherTokens(sentence);

    // edge case: set removes duplicates -- count number of words in sentence
    int numWords = findNumWordsInString(sentence);

    set<string> results = {};
    if (numWords == 1) { // one word -- no modifier, at least one letter, no spaces at ends
        if (index.count(*tokens.begin()) == 0) {
            return results;
        } else {
            return results = index[*tokens.begin()];
        }
    } else { // multiple words
        istringstream sstream(sentence); // call an object of class istringstream
        string word;
        string cleanWord;
        set<string> tempResults;
        int ct = 0;
        // use insertion operator to get all words into a string variable, in order of user input
        while (sstream >> word) {
            // make sure valid search term before continuing
            cleanWord = cleanToken(word);
            if (index.count(cleanWord) == 0) { // check if key is invalid
                continue;
            }

            // first word
            if (word.at(0) != '+' && word.at(0) != '-') {
                if (ct == 0) {
                    tempResults = index[cleanWord];
                    results = index[cleanWord];
                    ct++;
                    continue;
                }
            }
            // clear results set
            results.clear();

            // check first character of string
            if (word.at(0) == '+') {
                // edge case: temporary result set is empty
                if (tempResults.empty()) {
                    continue;
                }
                // set intersection
                set_intersection(tempResults.begin(), tempResults.end(), index[cleanWord].begin(), index[cleanWord].end(), inserter(results, results.begin()));
            } else if (word.at(0) == '-') {
                // edge case: temporary result set is empty
                if (tempResults.empty()) {
                    continue;
                }
                // set difference
                set_difference(tempResults.begin(), tempResults.end(), index[cleanWord].begin(), index[cleanWord].end(), inserter(results, results.begin()));
            } else {
                // set union
                set_union(tempResults.begin(), tempResults.end(), index[cleanWord].begin(), index[cleanWord].end(), inserter(results, results.begin()));
            }

            // update temporary result set
            tempResults = results;
            
        }

        return results;

    }

}

/**
 * Runs the main command loop for the search program
 */
void searchEngine(string filename) {
    // check if filename is valid
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Invalid filename." << endl;
    }
    
    // Print how many web pages were processed to build the index and how many distinct words were found across all pages
    cout << "Stand by while building index..." << endl;
    map<string, set<string>> index;
    int webPageCt = buildIndex(filename, index);
    cout << "Indexed " << webPageCt << " pages containing " << index.size() << " unique terms" << endl << endl;

    // Enter the command loop:
    set<string> URLs;
    string command;
    do {
        // Prompt the user to enter a query
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, command);
        if (command != "") {
            // Find the matching pages, and print the URLs (one on each line)
            URLs = findQueryMatches(index, command);
            cout << "Found " << URLs.size() << " matching pages" << endl;
            for (set<string>::iterator it = URLs.begin(); it != URLs.end(); ++it) {
                cout << *it << endl;
            }
        }
    } while (command != ""); // If the query is the empty string, exit the command loop

    // Print a closing message
    cout << "Thank you for searching!" << endl;
}
