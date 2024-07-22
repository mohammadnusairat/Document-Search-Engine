#include <gtest/gtest.h>

#include "search.h"
#include "tests/build_index.h"
#include "tests/clean_token.h"
#include "tests/find_query_matches.h"
#include "tests/gather_tokens.h"

// TODO_STUDENT: write your tests here!

// Tokens with punctuation at both the beginning and end
TEST(CleanToken, BegAndEndPunc) {
    map<string, set<string>> tests;

    tests["hello"].insert(".!?hello?!.");
    tests["hello"].insert(".!?hello.!?");
    tests["hello"].insert(".\"!?hello,");

    tests["timesheet"].insert(";timesheet;");
    tests["timesheet"].insert(";.!timesheet!.;");
    tests["timesheet"].insert(".,.!?timesheet?!.,.");

    testCleanToken(tests);
}

// Tokens with punctuation in the middle, as well as possibly at the ends
TEST(CleanToken, MiddlePuncAndEndPunc) {
    map<string, set<string>> tests;

    tests["he..llo"].insert("?!.he..llo");
    tests["he.llo"].insert("he.llo?!..");
    tests["he?llo"].insert("he?llo");

    tests["time,,sheet"].insert(";time,,sheet;");
    tests["time;sheet"].insert(";.!time;sheet!.;");
    tests["t.imesheet"].insert("t.imesheet?!.,.");

    testCleanToken(tests);
}

// Tokens with numbers at an end
TEST(CleanToken, NumbersAtEnd) {
    map<string, set<string>> tests;

    tests["he..llo123"].insert("?!.he..llo123");
    tests["123he.llo"].insert("123he.llo?!..");
    tests["123he?llo123"].insert("123he?llo123");

    tests["time12sheet"].insert(";time12sheet;");

    testCleanToken(tests);
}

// Tokens that are 1 letter long, like “x”
TEST(CleanToken, OneLetterToken) {
    map<string, set<string>> tests;

    tests[""].insert("!");
    tests[""].insert("1");
    tests["x"].insert("x");

    testCleanToken(tests);
}

// Text with one leading space
TEST(GatherTokens, OneLeadingSpace) {
    string text = " to be ora not to be";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// Text with more than one leading space
TEST(GatherTokens, MultipleLeadingSpaces) {
    string text = "   to be ora not to be";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// Text with one trailing space
TEST(GatherTokens, OneTrailingSpace) {
    string text = "to be ora not to be ";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// Text with more than one trailing space
TEST(GatherTokens, MultipleTrailingSpaces) {
    string text = "to be ora not to be   ";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// Text with multiple spaces between words
TEST(GatherTokens, MultipleSpacesBetweenWords) {
    string text = "to  be  ora  not   to be";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// Text with multiple spaces on both ends
TEST(GatherTokens, MultipleSpacesOnBothEnds) {
    string text = "   to be ora not to be   ";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// Text with multiple spaces on both ends
TEST(GatherTokens, MultipleSpacesOnBothEndsAndBetweenWords) {
    string text = "   to   be  ora not  to   be   ";
    set<string> expected = {"to", "be", "ora", "not"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// test tiny.txt
TEST(BuildIndex, TinyTxt) {
    string filename = "tiny.txt";
    map<string, set<string>> expected = {
        {"eggs", {"www.shoppinglist.com"}},
        {"milk", {"www.shoppinglist.com"}},
        {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
        {"bread", {"www.shoppinglist.com"}},
        {"cheese", {"www.shoppinglist.com"}},
        {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
        {"green", {"www.rainbow.org"}},
        {"orange", {"www.rainbow.org"}},
        {"yellow", {"www.rainbow.org"}},
        {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
        {"indigo", {"www.rainbow.org"}},
        {"violet", {"www.rainbow.org"}},
        {"one", {"www.dr.seuss.net"}},
        {"two", {"www.dr.seuss.net"}},
        {"blue", {"www.rainbow.org"}},
        {"i'm", {"www.bigbadwolf.com"}},
        {"not", {"www.bigbadwolf.com"}},
        {"trying", {"www.bigbadwolf.com"}},
        {"to", {"www.bigbadwolf.com"}},
        {"eat", {"www.bigbadwolf.com"}},
        {"you", {"www.bigbadwolf.com"}},
    };
    map<string, set<string>> studentIndex;
    int studentNumProcesed = buildIndex(filename, studentIndex);

    string indexTestFeedback = "buildIndex(\"" + filename + "\", ...) index incorrect\n";
    EXPECT_EQ(expected, studentIndex) << indexTestFeedback;

    string retTestFeedback = "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
    EXPECT_EQ(4, studentNumProcesed) << retTestFeedback;
}

// test no file found
TEST(BuildIndex, FileNotFound) {
    string filename = "notfound.txt";
    map<string, set<string>> expected;
    map<string, set<string>> studentIndex;
    int studentNumProcesed = buildIndex(filename, studentIndex);

    string indexTestFeedback = "buildIndex(\"" + filename + "\", ...) index incorrect\n";
    EXPECT_EQ(expected, studentIndex) << indexTestFeedback;

    string retTestFeedback = "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
    EXPECT_EQ(0, studentNumProcesed) << retTestFeedback;
}

// The first query term does not appear in the index
TEST(FindQueryMatches, FirstTermNotInIndex) {
    set<string> expected;

    expected = {};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "mohammad"));
    EXPECT_EQ(expected, findQueryMatches(INDEX, "nusairat mohammad"));
    
    expected = {"example.com", "uic.edu"};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "nusairat hello"));
    
    expected = {};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "nusairat +hello"));
    EXPECT_EQ(expected, findQueryMatches(INDEX, "nusairat -hello"));
}

// A later query term, possibly with a modifier, does not appear in the index
TEST(FindQueryMatches, LaterTermNotInIndex) {
    set<string> expected;

    expected = {"example.com"};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "there mohammad laws aviation -to +hello"));

    expected = {};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "all -mohammad -all +all"));

    expected = {"example.com", "uic.edu", "random.org"};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "all -all +mohammad +all all"));
}