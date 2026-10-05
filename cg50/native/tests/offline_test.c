#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/offline.h"

char const *qb_reference_text(char const *subject_id)
{
    if(strcmp(subject_id, "history") == 0) {
        return "History reference: chronology, evidence, context, cause and effect.";
    }
    if(strcmp(subject_id, "algebra_1") == 0) {
        return "Algebra reference: equations, functions, slope, systems.";
    }
    return "Subject-specific reference material.";
}

static void expect_contains(char const *label, char const *answer, char const *needle)
{
    if(!strstr(answer, needle)) {
        fprintf(stderr, "FAIL %s: expected [%s] in [%s]\n", label, needle, answer);
        exit(1);
    }
}

static void expect_not_contains(char const *label, char const *answer, char const *needle)
{
    if(strstr(answer, needle)) {
        fprintf(stderr, "FAIL %s: did not expect [%s] in [%s]\n", label, needle, answer);
        exit(1);
    }
}

int main(void)
{
    char out[4096];

    if(!qb_offline_answer("algebra_1", "steps", "school", "calc (2+3)*4", out, sizeof(out))) {
        return 1;
    }
    expect_contains("calc", out, "20");

    qb_offline_answer("geometry", "answer", "school", "slope 1 2 5 10", out, sizeof(out));
    expect_contains("slope", out, "2");

    qb_offline_answer("algebra_2", "steps", "school", "quad 1 -5 6", out, sizeof(out));
    expect_contains("quad", out, "3");

    qb_offline_answer("physics", "answer", "school", "force 2 3", out, sizeof(out));
    expect_contains("force", out, "6");

    qb_offline_answer("biology", "explain", "school", "what is photosynthesis", out, sizeof(out));
    expect_contains("photosynthesis", out, "chemical energy");

    qb_offline_answer("auto", "explain", "school", "hi", out, sizeof(out));
    expect_contains("greeting", out, "Hey!");
    expect_contains("greeting mode", out, "Standalone");

    qb_offline_answer("auto", "explain", "school", "hey bro", out, sizeof(out));
    expect_contains("casual greeting", out, "Hey!");

    qb_offline_answer("physics", "explain", "school", "how are you", out, sizeof(out));
    expect_contains("how are you", out, "running fine");

    qb_offline_answer("auto", "explain", "school", "who are you", out, sizeof(out));
    expect_contains("identity", out, "Quantum Breaks AI Standalone");

    qb_offline_answer("physics", "explain", "school", "what can you do", out, sizeof(out));
    expect_contains("capabilities", out, "Physics");

    qb_offline_answer("history", "explain", "school", "thanks", out, sizeof(out));
    expect_contains("thanks", out, "welcome");

    qb_offline_answer("history", "explain", "school", "define thermopylae", out, sizeof(out));
    expect_contains("unknown definition topic", out, "thermopylae");
    expect_contains("unknown definition subject", out, "History");

    qb_offline_answer("history", "explain", "school", "what's thermopylae", out, sizeof(out));
    expect_contains("whats topic", out, "thermopylae");

    qb_offline_answer("history", "explain", "school", "tell me about thermopylae", out, sizeof(out));
    expect_contains("tell me topic", out, "thermopylae");

    qb_offline_answer("history", "explain", "school", "what does thermopylae mean?", out, sizeof(out));
    expect_contains("what does topic", out, "thermopylae");
    expect_not_contains("what does strips mean", out, "thermopylae mean");

    qb_offline_answer("biology", "explain", "school", "difference between frobules and glarps", out, sizeof(out));
    expect_contains("difference topic", out, "frobules and glarps");
    expect_contains("difference intent", out, "comparison");

    qb_offline_answer("biology", "explain", "school", "why florpules reproduce", out, sizeof(out));
    expect_contains("why topic", out, "florpules reproduce");
    expect_contains("why intent", out, "WHY");

    qb_offline_answer("computer_science", "steps", "school", "how frobnicator works", out, sizeof(out));
    expect_contains("how topic", out, "frobnicator works");
    expect_contains("how subject", out, "Computer Science");

    qb_offline_answer("history", "explain", "school", "who Zargle McTestface", out, sizeof(out));
    expect_contains("who topic", out, "Zargle McTestface");
    expect_contains("who honesty", out, "won't invent");

    qb_offline_answer("history", "explain", "school", "when flarble war", out, sizeof(out));
    expect_contains("when topic", out, "flarble war");
    expect_contains("when honesty", out, "won't guess");

    qb_offline_answer("geography", "explain", "school", "where blorptown", out, sizeof(out));
    expect_contains("where topic", out, "blorptown");
    expect_contains("where honesty", out, "won't make one up");

    qb_offline_answer("algebra_1", "explain", "school", "qxzvbnmpt", out, sizeof(out));
    expect_contains("gibberish exact", out, "qxzvbnmpt");
    expect_contains("gibberish recognized", out, "don't recognize");

    qb_offline_answer("algebra_1", "steps", "school", "bananas", out, sizeof(out));
    expect_contains("single word exact", out, "bananas");
    expect_contains("single word subject", out, "Algebra 1");

    qb_offline_answer("history", "explain", "school", "random unmatched question about purple clocks", out, sizeof(out));
    expect_contains("unmatched exact", out, "random unmatched question about purple clocks");
    expect_contains("unmatched subject", out, "History");
    expect_contains("unmatched statement", out, "statement rather than a direct question");
    expect_contains("unmatched strategy", out, "causes, effects, evidence");
    expect_not_contains("no old canned prefix", out, "STANDALONE CORE\nQuick method");


    qb_offline_answer("auto", "answer", "school", "what is 18 plus 7", out, sizeof(out));
    expect_contains("natural plus", out, "25");

    qb_offline_answer("auto", "answer", "school", "square root of 81", out, sizeof(out));
    expect_contains("natural sqrt", out, "9");

    qb_offline_answer("auto", "answer", "school", "20 percent of 50", out, sizeof(out));
    expect_contains("natural percent", out, "10");

    qb_offline_answer("auto", "answer", "school", "area of a circle radius 3", out, sizeof(out));
    expect_contains("circle area", out, "28.274");

    qb_offline_answer("auto", "explain", "school", "what is photosythesis", out, sizeof(out));
    expect_contains("typo suggestion", out, "photosynthesis");

    qb_offline_answer("auto", "explain", "school", "what is mitosis", out, sizeof(out));
    expect_contains("auto subject fact", out, "daughter cells");

    qb_offline_answer("auto", "explain", "school", "why?", out, sizeof(out));
    expect_contains("followup why topic", out, "mitosis");
    expect_contains("followup why layer", out, "WHY layer");

    qb_offline_answer("auto", "explain", "school", "simpler", out, sizeof(out));
    expect_contains("followup simpler", out, "Simpler version");
    expect_contains("followup simpler topic", out, "mitosis");

    qb_offline_answer("auto", "explain", "school", "repeat that", out, sizeof(out));
    expect_contains("followup repeat", out, "Simpler version");

    qb_offline_answer("auto", "explain", "school", "what is voltage", out, sizeof(out));
    expect_contains("auto physics", out, "potential difference");

    qb_offline_answer("auto", "explain", "school", "give me an example", out, sizeof(out));
    expect_contains("followup example", out, "Example for");


    qb_offline_answer("auto", "steps", "school", "solve 3x + 7 = 25", out, sizeof(out));
    expect_contains("linear solve", out, "x = 6");

    qb_offline_answer("auto", "steps", "school", "solve 2x - 4 = x + 9", out, sizeof(out));
    expect_contains("linear both sides", out, "x = 13");

    qb_offline_answer("auto", "answer", "school", "120 inches to feet", out, sizeof(out));
    expect_contains("unit inches feet", out, "10");

    qb_offline_answer("auto", "answer", "school", "5 kilometers to meters", out, sizeof(out));
    expect_contains("unit km m", out, "5000");

    qb_offline_answer("auto", "answer", "school", "72 fahrenheit to celsius", out, sizeof(out));
    expect_contains("temperature conversion", out, "22.222");

    qb_offline_answer("auto", "explain", "school", "lol", out, sizeof(out));
    expect_contains("casual lol", out, "Fair enough");

    qb_offline_answer("auto", "explain", "school", "idk", out, sizeof(out));
    expect_contains("casual idk", out, "Uncertain is fine");
    expect_contains("casual idk context", out, "lol");

    qb_offline_answer("auto", "explain", "school", "???", out, sizeof(out));
    expect_contains("punctuation input", out, "only spaces/punctuation/symbols");


    qb_offline_answer("auto", "answer", "school", "what is 2+2?", out, sizeof(out));
    expect_contains("prefixed expression", out, "4");

    qb_offline_answer("auto", "answer", "school", "calculate (7-2)*6", out, sizeof(out));
    expect_contains("calculate expression", out, "30");

    qb_offline_answer("auto", "steps", "school", "3x + 7 = 25", out, sizeof(out));
    expect_contains("bare linear equation", out, "x = 6");

    qb_offline_answer("auto", "explain", "school", "mitosis vs meiosis", out, sizeof(out));
    expect_contains("known comparison mitosis", out, "Mitosis");
    expect_contains("known comparison meiosis", out, "meiosis");

    qb_offline_answer("auto", "explain", "school", "difference between ionic bond and covalent bond", out, sizeof(out));
    expect_contains("bond comparison", out, "electron transfer");


    qb_offline_answer("biology", "summary", "school", "summarize photosynthesis", out, sizeof(out));
    expect_contains("summary request", out, "Summary - photosynthesis");

    qb_offline_answer("algebra_1", "flashcards", "school", "flashcards on slope", out, sizeof(out));
    expect_contains("flashcard request", out, "FLASHCARD");
    expect_contains("flashcard topic", out, "slope");

    qb_offline_answer("biology", "quiz", "school", "quiz me on mitosis", out, sizeof(out));
    expect_contains("quiz request", out, "QUIZ");
    expect_contains("quiz reveal instruction", out, "show answer");

    qb_offline_answer("biology", "quiz", "school", "show answer", out, sizeof(out));
    expect_contains("quiz revealed", out, "ANSWER - mitosis");
    expect_contains("quiz answer content", out, "daughter cells");

    qb_offline_answer("computer_science", "explain", "school", "https://example.com/page", out, sizeof(out));
    expect_contains("url recognition", out, "looks like a URL");

    qb_offline_answer("computer_science", "explain", "school", "print(x)", out, sizeof(out));
    expect_contains("code recognition", out, "source code");

    qb_offline_answer("auto", "explain", "school", "ZXQ", out, sizeof(out));
    expect_contains("acronym recognition", out, "acronym or initialism");


    qb_offline_answer("history", "explain", "school", "I think the policy changed society", out, sizeof(out));
    expect_contains("claim intent", out, "claim/answer");

    qb_offline_answer("composition", "explain", "school", "write an essay about courage", out, sizeof(out));
    expect_contains("writing intent", out, "Writing plan");
    expect_contains("writing topic", out, "courage");

    qb_offline_answer("algebra_1", "check", "school", "is this correct 3x+2=11 so x=3", out, sizeof(out));
    expect_contains("check intent", out, "CHECK mode");

    qb_offline_answer("biology", "explain", "school", "give me reasons why cells divide", out, sizeof(out));
    expect_contains("reasons intent", out, "separate causes from effects");

    qb_offline_answer("auto", "explain", "school", "what do you think about homework", out, sizeof(out));
    expect_contains("opinion intent", out, "don't have personal opinions");


    qb_offline_answer("biology", "explain", "school", "what is photosynthesis", out, sizeof(out));
    expect_contains("follow base", out, "chemical energy");

    qb_offline_answer("biology", "explain", "school", "what does that mean?", out, sizeof(out));
    expect_contains("follow meaning", out, "We were talking about");
    expect_contains("follow meaning topic", out, "photosynthesis");

    qb_offline_answer("biology", "explain", "school", "show me steps", out, sizeof(out));
    expect_contains("follow steps", out, "Steps for");
    expect_contains("follow steps topic", out, "photosynthesis");

    qb_offline_answer("biology", "explain", "school", "what about meiosis?", out, sizeof(out));
    expect_contains("what about new topic", out, "haploid");

    qb_offline_answer("biology", "explain", "school", "what about that?", out, sizeof(out));
    expect_contains("what about that", out, "meiosis");

    qb_offline_answer("algebra_1", "steps", "school", "solve x**2 = 9", out, sizeof(out));
    expect_contains("malformed equation", out, "couldn't parse");
    expect_contains("malformed exact", out, "x**2");


    qb_offline_answer("biology", "explain", "school", "what is mitosis", out, sizeof(out));
    expect_contains("continuation base", out, "daughter cells");

    qb_offline_answer("biology", "explain", "school", "yes", out, sizeof(out));
    expect_contains("yes continuation", out, "still on");
    expect_contains("yes continuation topic", out, "mitosis");

    qb_offline_answer("biology", "explain", "school", "no", out, sizeof(out));
    expect_contains("no continuation", out, "which part seems wrong");

    qb_offline_answer("history", "explain", "school", "The empire expanded quickly", out, sizeof(out));
    expect_contains("statement classification", out, "statement rather than a direct question");
    expect_contains("statement exact", out, "empire expanded quickly");


    qb_offline_answer("auto", "answer", "school", "twenty five plus seven", out, sizeof(out));
    expect_contains("number words plus", out, "32");

    qb_offline_answer("auto", "answer", "school", "one hundred divided by four", out, sizeof(out));
    expect_contains("number words divide", out, "25");

    qb_offline_answer("auto", "answer", "school", "what is forty two minus nineteen", out, sizeof(out));
    expect_contains("number words subtract", out, "23");

    qb_offline_answer("auto", "steps", "school", "sam has 8 apples and gets 5 more", out, sizeof(out));
    expect_contains("word problem add", out, "13");

    qb_offline_answer("auto", "steps", "school", "sam has 12 apples and gives 4 away", out, sizeof(out));
    expect_contains("word problem subtract", out, "8");

    qb_offline_answer("physics", "steps", "school", "a car travels 60 miles per hour for 2 hours", out, sizeof(out));
    expect_contains("rate time word problem", out, "120 miles");

    qb_offline_answer("consumer_math", "steps", "school", "percent increase from 50 to 75", out, sizeof(out));
    expect_contains("percent increase word problem", out, "50%");


    qb_offline_answer("auto", "explain", "school", "DNA", out, sizeof(out));
    expect_contains("known acronym dna", out, "deoxyribonucleic acid");

    qb_offline_answer("auto", "explain", "school", "what does CPU stand for?", out, sizeof(out));
    expect_contains("known acronym cpu", out, "central processing unit");

    qb_offline_answer("auto", "answer", "school", "is 2+2=4?", out, sizeof(out));
    expect_contains("numeric claim true", out, "TRUE");

    qb_offline_answer("auto", "answer", "school", "is 2+2=5?", out, sizeof(out));
    expect_contains("numeric claim false", out, "FALSE");

    qb_offline_answer("auto", "answer", "school", "10/2 >= 5", out, sizeof(out));
    expect_contains("numeric inequality", out, "TRUE");

    puts("Standalone universal responder tests passed");
    return 0;
}
