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


    qb_offline_answer("biology", "explain", "school", "compare photosynthesis and cellular respiration", out, sizeof(out));
    expect_contains("generic comparison photosynthesis", out, "light energy");
    expect_contains("generic comparison respiration", out, "ATP");
    expect_contains("generic comparison structure", out, "Compare them using the same dimensions");

    qb_offline_answer("economics", "explain", "school", "compare inflation and gross domestic product", out, sizeof(out));
    expect_contains("generic comparison inflation", out, "price level");
    expect_contains("generic comparison gdp", out, "final goods and services");


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


    qb_offline_answer("auto", "explain", "school", "wut is mitosis", out, sizeof(out));
    expect_contains("shorthand wut", out, "daughter cells");

    qb_offline_answer("algebra_1", "explain", "school", "pls explain slope", out, sizeof(out));
    expect_contains("shorthand pls", out, "rate of change");

    qb_offline_answer("auto", "explain", "school", "wsp", out, sizeof(out));
    expect_contains("shorthand wsp", out, "Standalone is up");

    qb_offline_answer("auto", "explain", "school", "wyd", out, sizeof(out));
    expect_contains("shorthand wyd", out, "Running locally");

    qb_offline_answer("biology", "explain", "school", "what is dna", out, sizeof(out));
    expect_contains("wdym base", out, "DNA stores hereditary");

    qb_offline_answer("biology", "explain", "school", "wdym", out, sizeof(out));
    expect_contains("shorthand wdym", out, "I mean this about");


    qb_offline_answer("auto", "answer", "school", "reverse hello", out, sizeof(out));
    expect_contains("text reverse", out, "olleh");

    qb_offline_answer("auto", "answer", "school", "count letters in banana", out, sizeof(out));
    expect_contains("letter count", out, "6");

    qb_offline_answer("auto", "answer", "school", "how many words in one two three", out, sizeof(out));
    expect_contains("word count", out, "3");

    qb_offline_answer("auto", "answer", "school", "is racecar a palindrome", out, sizeof(out));
    expect_contains("palindrome yes", out, "Yes");

    qb_offline_answer("auto", "answer", "school", "binary of 10", out, sizeof(out));
    expect_contains("binary conversion", out, "1010");

    qb_offline_answer("auto", "answer", "school", "hex of 255", out, sizeof(out));
    expect_contains("hex conversion", out, "0xFF");


    qb_offline_answer("auto", "answer", "school", "42", out, sizeof(out));
    expect_contains("raw integer", out, "42 is positive");
    expect_contains("raw integer parity", out, "even");

    qb_offline_answer("auto", "answer", "school", "Q", out, sizeof(out));
    expect_contains("raw letter", out, "letter 17");

    qb_offline_answer("auto", "answer", "school", "0b1010", out, sizeof(out));
    expect_contains("binary literal", out, "10");

    qb_offline_answer("auto", "answer", "school", "0xFF", out, sizeof(out));
    expect_contains("hex literal", out, "255");

    qb_offline_answer("auto", "explain", "school", "name@example.com", out, sizeof(out));
    expect_contains("email shape", out, "looks like an email address");

    qb_offline_answer("auto", "explain", "school", "notes.pdf", out, sizeof(out));
    expect_contains("filename shape", out, "looks like a filename");
    expect_contains("filename extension", out, ".pdf");


    qb_offline_answer("algebra_2", "steps", "school", "x^2 - 5x + 6 = 0", out, sizeof(out));
    expect_contains("quadratic normal root1", out, "x1 = 3");
    expect_contains("quadratic normal root2", out, "x2 = 2");

    qb_offline_answer("algebra_1", "steps", "school", "2/3 = x/12", out, sizeof(out));
    expect_contains("proportion solve", out, "x = 8");

    qb_offline_answer("algebra_1", "steps", "school", "solve system 2x+y=7; x-y=2", out, sizeof(out));
    expect_contains("system x", out, "x = 3");
    expect_contains("system y", out, "y = 1");

    qb_offline_answer("geometry", "answer", "school", "volume of a box 3 4 5", out, sizeof(out));
    expect_contains("box volume", out, "60");

    qb_offline_answer("geometry", "answer", "school", "volume of a sphere radius 3", out, sizeof(out));
    expect_contains("sphere volume", out, "113.097");

    qb_offline_answer("physics", "answer", "school", "kinetic energy mass 2 velocity 3", out, sizeof(out));
    expect_contains("kinetic energy formula", out, "9 J");

    qb_offline_answer("physics", "answer", "school", "momentum mass 4 velocity 5", out, sizeof(out));
    expect_contains("momentum formula", out, "20 kg*m/s");

    qb_offline_answer("chemistry", "answer", "school", "density mass 20 volume 4", out, sizeof(out));
    expect_contains("density formula", out, "5");


    qb_offline_answer("biology", "explain", "school", "what is celluar respiration", out, sizeof(out));
    expect_contains("multiword typo correction", out, "cellular respiration");
    expect_contains("multiword typo answer", out, "ATP");

    qb_offline_answer("statistics", "explain", "school", "define standerd deviation", out, sizeof(out));
    expect_contains("stats typo correction", out, "standard deviation");
    expect_contains("stats typo answer", out, "spread");

    qb_offline_answer("government", "explain", "school", "explain seperation of powers", out, sizeof(out));
    expect_contains("government typo correction", out, "separation of powers");
    expect_contains("government typo answer", out, "branches");


    qb_offline_answer("number_theory", "answer", "college", "gcd 84 126", out, sizeof(out));
    expect_contains("gcd tool", out, "42");

    qb_offline_answer("number_theory", "answer", "college", "lcm 12 18", out, sizeof(out));
    expect_contains("lcm tool", out, "36");

    qb_offline_answer("number_theory", "answer", "college", "factor 360", out, sizeof(out));
    expect_contains("factorization tool", out, "2^3");
    expect_contains("factorization tool 3", out, "3^2");

    qb_offline_answer("math", "answer", "school", "factorial 6", out, sizeof(out));
    expect_contains("factorial tool", out, "720");

    qb_offline_answer("statistics", "answer", "school", "median 1 3 8 10", out, sizeof(out));
    expect_contains("median tool", out, "5.5");

    qb_offline_answer("statistics", "answer", "school", "standard deviation 2 2 4 4", out, sizeof(out));
    expect_contains("stdev tool", out, "1");

    qb_offline_answer("calculus", "derive", "college", "derive 3x^2+2x-5", out, sizeof(out));
    expect_contains("polynomial derivative", out, "6x + 2");

    qb_offline_answer("calculus", "derive", "college", "integrate 6x+4", out, sizeof(out));
    expect_contains("polynomial integral", out, "3x^2 + 4x + C");


    qb_offline_answer("chemistry", "explain", "school", "Fe", out, sizeof(out));
    expect_contains("element symbol lookup", out, "iron");
    expect_contains("element atomic number", out, "26");

    qb_offline_answer("chemistry", "explain", "school", "atomic number 8", out, sizeof(out));
    expect_contains("atomic number lookup", out, "oxygen");
    expect_contains("atomic number symbol", out, "(O)");

    qb_offline_answer("chemistry", "explain", "school", "symbol for sodium", out, sizeof(out));
    expect_contains("element symbol by name", out, "Na");

    qb_offline_answer("biology", "explain", "school", "thermobiology", out, sizeof(out));
    expect_contains("word root thermo", out, "thermo = heat");
    expect_contains("word root bio", out, "bio = life");
    expect_contains("word root caution", out, "not a guaranteed definition");


    qb_offline_answer("auto", "answer", "school", "5 kg to lb", out, sizeof(out));
    expect_contains("kg to lb", out, "11.023");

    qb_offline_answer("auto", "answer", "school", "10 miles to kilometers", out, sizeof(out));
    expect_contains("mi to km", out, "16.09344");

    qb_offline_answer("auto", "answer", "school", "next in sequence 2 5 8 11", out, sizeof(out));
    expect_contains("arithmetic sequence", out, "14");

    qb_offline_answer("auto", "answer", "school", "sequence 3 6 12 24", out, sizeof(out));
    expect_contains("geometric sequence", out, "48");

    qb_offline_answer("auto", "answer", "school", "sequence 1 1 2 3 5", out, sizeof(out));
    expect_contains("fibonacci sequence", out, "8");

    qb_offline_answer("consumer_math", "answer", "school", "20 percent discount on 50", out, sizeof(out));
    expect_contains("discount tool", out, "40");

    qb_offline_answer("consumer_math", "answer", "school", "7 percent sales tax on 100", out, sizeof(out));
    expect_contains("tax tool", out, "107");

    qb_offline_answer("consumer_math", "answer", "school", "18 percent tip on 25", out, sizeof(out));
    expect_contains("tip tool", out, "29.5");

    qb_offline_answer("auto", "answer", "school", "uppercase hello world", out, sizeof(out));
    expect_contains("uppercase tool", out, "HELLO WORLD");

    qb_offline_answer("auto", "answer", "school", "title case the quick brown fox", out, sizeof(out));
    expect_contains("title case tool", out, "The Quick Brown Fox");

    qb_offline_answer("auto", "answer", "school", "count vowels in education", out, sizeof(out));
    expect_contains("vowel count tool", out, "5");

    qb_offline_answer("auto", "answer", "school", "initials of central processing unit", out, sizeof(out));
    expect_contains("initials tool", out, "CPU");

    qb_offline_answer("auto", "answer", "school", "alphabetize pear apple banana", out, sizeof(out));
    expect_contains("alphabetize tool", out, "apple, banana, pear");

    qb_offline_answer("biology", "explain", "school", "what is mitosis", out, sizeof(out));
    expect_contains("and followup base", out, "daughter cells");

    qb_offline_answer("biology", "explain", "school", "and meiosis?", out, sizeof(out));
    expect_contains("and followup new topic", out, "haploid");

    qb_offline_answer("biology", "explain", "school", "same for photosynthesis", out, sizeof(out));
    expect_contains("same for followup", out, "chemical energy");


    qb_offline_answer("math", "answer", "school", "simplify fraction 42/56", out, sizeof(out));
    expect_contains("fraction simplify", out, "3/4");


    qb_offline_answer("math", "answer", "school", "fraction 1/2 + 1/3", out, sizeof(out));
    expect_contains("fraction add exact", out, "5/6");

    qb_offline_answer("math", "answer", "school", "fraction 3/4 * 2/5", out, sizeof(out));
    expect_contains("fraction multiply exact", out, "3/10");

    qb_offline_answer("algebra_1", "answer", "school", "simplify radical 72", out, sizeof(out));
    expect_contains("radical simplify", out, "6*sqrt(2)");

    qb_offline_answer("algebra_2", "answer", "school", "factor x^2-5x+6", out, sizeof(out));
    expect_contains("quadratic factor one", out, "(x - 2)");
    expect_contains("quadratic factor two", out, "(x - 3)");

    qb_offline_answer("algebra_2", "answer", "school", "factor 2x^2+7x+3", out, sizeof(out));
    expect_contains("quadratic factor leading", out, "(2x + 1)");
    expect_contains("quadratic factor second", out, "(x + 3)");

    qb_offline_answer("math", "answer", "school", "scientific notation 123000", out, sizeof(out));
    expect_contains("scientific notation large", out, "1.23 x 10^5");

    qb_offline_answer("math", "answer", "school", "scientific notation 0.00123", out, sizeof(out));
    expect_contains("scientific notation small", out, "1.23 x 10^-3");


    qb_offline_answer("algebra_1", "steps", "school", "solve |x-3|=5", out, sizeof(out));
    expect_contains("absolute value solution one", out, "8");
    expect_contains("absolute value solution two", out, "-2");

    qb_offline_answer("algebra_1", "steps", "school", "solve |2x+1|=7", out, sizeof(out));
    expect_contains("absolute value coefficient one", out, "3");
    expect_contains("absolute value coefficient two", out, "-4");

    qb_offline_answer("algebra_2", "answer", "school", "discriminant x^2-5x+6", out, sizeof(out));
    expect_contains("discriminant expression", out, "1");
    expect_contains("discriminant roots", out, "Two distinct real roots");

    qb_offline_answer("algebra_2", "answer", "school", "discriminant 1 2 5", out, sizeof(out));
    expect_contains("discriminant complex", out, "-16");
    expect_contains("discriminant complex roots", out, "complex conjugate");

    qb_offline_answer("math", "answer", "school", "common denominator 1/4 1/6", out, sizeof(out));
    expect_contains("lcd value", out, "LCD = 12");
    expect_contains("lcd first fraction", out, "3/12");
    expect_contains("lcd second fraction", out, "2/12");


    qb_offline_answer("math", "answer", "school", "simplify ratio 18:24", out, sizeof(out));
    expect_contains("ratio simplify exact", out, "3:4");

    qb_offline_answer("math", "answer", "school", "decimal to fraction 0.375", out, sizeof(out));
    expect_contains("decimal fraction convert", out, "3/8");

    qb_offline_answer("math", "answer", "school", "fraction to decimal 3/8", out, sizeof(out));
    expect_contains("fraction decimal convert", out, "0.375");

    qb_offline_answer("math", "answer", "school", "which is larger 3/4 or 2/3", out, sizeof(out));
    expect_contains("fraction comparison", out, "3/4 is larger");


    qb_offline_answer("auto", "answer", "school", "5 ft 8 in to inches", out, sizeof(out));
    expect_contains("compound feet inches", out, "68");

    qb_offline_answer("auto", "answer", "school", "2 hours 30 minutes to minutes", out, sizeof(out));
    expect_contains("compound time", out, "150");

    qb_offline_answer("auto", "answer", "school", "1 lb 8 oz to ounces", out, sizeof(out));
    expect_contains("compound weight", out, "24");

    qb_offline_answer("consumer_math", "answer", "school", "3 dollars 50 cents to cents", out, sizeof(out));
    expect_contains("compound money", out, "350");


    qb_offline_answer("composition", "answer", "school", "word frequency cat in cat dog cat bird cat", out, sizeof(out));
    expect_contains("word frequency tool", out, "3 times");

    qb_offline_answer("composition", "answer", "school", "contains moon in The moon is bright", out, sizeof(out));
    expect_contains("contains text yes", out, "Yes");

    qb_offline_answer("composition", "answer", "school", "contains sun in The moon is bright", out, sizeof(out));
    expect_contains("contains text no", out, "No");

    qb_offline_answer("composition", "answer", "school", "duplicate words red blue red green blue red", out, sizeof(out));
    expect_contains("duplicate words red", out, "red(3)");
    expect_contains("duplicate words blue", out, "blue(2)");

    qb_offline_answer("composition", "answer", "school", "sentence check Hello hello world.", out, sizeof(out));
    expect_contains("sentence check capital", out, "starts with capital=yes");
    expect_contains("sentence check punctuation", out, "ends with punctuation=yes");
    expect_contains("sentence check repeat", out, "adjacent repeated word=yes");

    qb_offline_answer("consumer_math", "answer", "school", "what percent is 15 of 60", out, sizeof(out));
    expect_contains("reverse percent", out, "25%");

    qb_offline_answer("consumer_math", "answer", "school", "increase 80 by 15 percent", out, sizeof(out));
    expect_contains("percent increase direct", out, "92");

    qb_offline_answer("geometry", "answer", "school", "midpoint between 1 2 5 8", out, sizeof(out));
    expect_contains("midpoint tool", out, "(3, 5)");

    qb_offline_answer("math", "answer", "school", "roman 49", out, sizeof(out));
    expect_contains("roman encode", out, "XLIX");

    qb_offline_answer("math", "answer", "school", "roman to decimal XLIX", out, sizeof(out));
    expect_contains("roman decode", out, "49");

    qb_offline_answer("auto", "answer", "school", "is listen an anagram of silent", out, sizeof(out));
    expect_contains("anagram yes", out, "Yes");

    qb_offline_answer("auto", "answer", "school", "rot13 hello", out, sizeof(out));
    expect_contains("rot13 tool", out, "uryyb");

    qb_offline_answer("auto", "answer", "school", "caesar 3 abc xyz", out, sizeof(out));
    expect_contains("caesar tool", out, "def abc");


    qb_offline_answer("algebra_1", "steps", "school", "solve 2x+3<11", out, sizeof(out));
    expect_contains("linear inequality positive", out, "x < 4");

    qb_offline_answer("algebra_1", "steps", "school", "solve -3x+6>=12", out, sizeof(out));
    expect_contains("linear inequality flip", out, "x <= -2");

    qb_offline_answer("geometry", "answer", "school", "hypotenuse 3 4", out, sizeof(out));
    expect_contains("hypotenuse solver", out, "5");

    qb_offline_answer("trigonometry", "answer", "school", "sin 30 degrees", out, sizeof(out));
    expect_contains("sin degrees", out, "0.5");

    qb_offline_answer("auto", "answer", "school", "180 degrees to radians", out, sizeof(out));
    expect_contains("degrees radians", out, "3.14159");

    qb_offline_answer("chemistry", "answer", "school", "ph 0.001", out, sizeof(out));
    expect_contains("ph solver", out, "3");

    qb_offline_answer("chemistry", "answer", "school", "ideal gas pressure 2 300 10", out, sizeof(out));
    expect_contains("ideal gas solver", out, "4.923");

    qb_offline_answer("physics", "answer", "school", "potential energy mass 2 height 5", out, sizeof(out));
    expect_contains("potential energy solver", out, "98.0665");

    qb_offline_answer("biology", "answer", "school", "cross Aa x Aa", out, sizeof(out));
    expect_contains("punnett AA", out, "AA 25%");
    expect_contains("punnett Aa", out, "Aa 50%");
    expect_contains("punnett aa", out, "aa 25%");
    expect_contains("punnett phenotype", out, "Dominant phenotype 75%");


    qb_offline_answer("biology", "explain", "school", "what is osmosis", out, sizeof(out));
    expect_contains("knowledge osmosis", out, "selectively permeable membrane");

    qb_offline_answer("chemistry", "explain", "school", "what is an isotope", out, sizeof(out));
    expect_contains("knowledge isotope", out, "different neutron counts");

    qb_offline_answer("government", "explain", "school", "what are checks and balances", out, sizeof(out));
    expect_contains("knowledge checks balances", out, "branches of government");

    qb_offline_answer("history", "explain", "school", "what was the industrial revolution", out, sizeof(out));
    expect_contains("knowledge industrial revolution", out, "mechanization");

    qb_offline_answer("economics", "explain", "school", "what is comparative advantage", out, sizeof(out));
    expect_contains("knowledge comparative advantage", out, "opportunity cost");

    qb_offline_answer("computer_science", "explain", "school", "what is an operating system", out, sizeof(out));
    expect_contains("knowledge operating system", out, "memory management");


    qb_offline_answer("auto", "answer", "school", "please calculate 2+2", out, sizeof(out));
    expect_contains("polite calculate", out, "4");

    qb_offline_answer("algebra_1", "steps", "school", "can you solve 3x+7=25", out, sizeof(out));
    expect_contains("polite solve", out, "x = 6");

    qb_offline_answer("biology", "explain", "school", "hey qbai, explain mitosis", out, sizeof(out));
    expect_contains("qbai prefix explain", out, "daughter cells");

    qb_offline_answer("biology", "explain", "school", "what is meiosis", out, sizeof(out));
    expect_contains("polite followup base", out, "haploid");

    qb_offline_answer("biology", "explain", "school", "could you please explain that", out, sizeof(out));
    expect_contains("polite followup", out, "We were talking about");
    expect_contains("polite followup topic", out, "meiosis");


    qb_offline_answer("auto", "explain", "school", "my name is Kenny", out, sizeof(out));
    expect_contains("session name store", out, "Kenny");

    qb_offline_answer("auto", "explain", "school", "what is my name?", out, sizeof(out));
    expect_contains("session name recall", out, "Kenny");

    qb_offline_answer("auto", "explain", "school", "remember that project alpha uses triangles", out, sizeof(out));
    expect_contains("session note store", out, "project alpha uses triangles");

    qb_offline_answer("auto", "explain", "school", "what do you remember?", out, sizeof(out));
    expect_contains("session note recall name", out, "Kenny");
    expect_contains("session note recall note", out, "project alpha uses triangles");

    qb_offline_answer("auto", "explain", "school", "forget everything", out, sizeof(out));
    expect_contains("session memory clear", out, "Cleared");

    qb_offline_answer("auto", "explain", "school", "what do you remember?", out, sizeof(out));
    expect_contains("session memory empty", out, "don't have any");


    qb_offline_answer("linear_algebra", "answer", "college", "det 1 2 3 4", out, sizeof(out));
    expect_contains("matrix determinant", out, "-2");

    qb_offline_answer("linear_algebra", "answer", "college", "inverse 1 2 3 4", out, sizeof(out));
    expect_contains("matrix inverse", out, "-2");
    expect_contains("matrix inverse element", out, "1.5");

    qb_offline_answer("linear_algebra", "answer", "college", "vector magnitude 3 4 12", out, sizeof(out));
    expect_contains("vector magnitude", out, "13");

    qb_offline_answer("linear_algebra", "answer", "college", "dot 1 2 3 ; 4 5 6", out, sizeof(out));
    expect_contains("dot product", out, "32");

    qb_offline_answer("linear_algebra", "answer", "college", "cross 1 0 0 ; 0 1 0", out, sizeof(out));
    expect_contains("cross product", out, "<0, 0, 1>");

    qb_offline_answer("linear_algebra", "answer", "college", "matmul 1 2 3 4 ; 5 6 7 8", out, sizeof(out));
    expect_contains("matrix multiply", out, "19");
    expect_contains("matrix multiply 2", out, "50");

    qb_offline_answer("statistics", "answer", "school", "probability 3 out of 10", out, sizeof(out));
    expect_contains("probability fraction", out, "30%");

    qb_offline_answer("statistics", "answer", "school", "complement 0.3", out, sizeof(out));
    expect_contains("probability complement", out, "70%");

    qb_offline_answer("statistics", "answer", "college", "binomial 10 0.5 3", out, sizeof(out));
    expect_contains("binomial probability", out, "11.71875");

    qb_offline_answer("algebra_2", "answer", "school", "log base 2 of 8", out, sizeof(out));
    expect_contains("log base tool", out, "3");

    qb_offline_answer("algebra_2", "answer", "school", "log 1000", out, sizeof(out));
    expect_contains("log10 tool", out, "3");

    qb_offline_answer("algebra_2", "answer", "school", "nth root 3 of 27", out, sizeof(out));
    expect_contains("nth root tool", out, "3");


    qb_offline_answer("auto", "answer", "school", "leap year 2024", out, sizeof(out));
    expect_contains("leap year", out, "is a leap year");

    qb_offline_answer("auto", "answer", "school", "day of week 2026-10-05", out, sizeof(out));
    expect_contains("weekday tool", out, "Monday");

    qb_offline_answer("auto", "answer", "school", "days in february 2024", out, sizeof(out));
    expect_contains("february days", out, "29");

    qb_offline_answer("auto", "answer", "school", "days between 2026-10-05 and 2026-10-20", out, sizeof(out));
    expect_contains("date difference", out, "15");

    qb_offline_answer("auto", "answer", "school", "morse hello", out, sizeof(out));
    expect_contains("morse encode", out, ".... . .-.. .-.. ---");

    qb_offline_answer("auto", "answer", "school", "decode morse .... . .-.. .-.. ---", out, sizeof(out));
    expect_contains("morse decode", out, "HELLO");

    qb_offline_answer("auto", "answer", "school", "nato cat", out, sizeof(out));
    expect_contains("nato encode", out, "Charlie Alpha Tango");


    qb_offline_answer("chemistry", "answer", "school", "molar mass H2O", out, sizeof(out));
    expect_contains("molar mass water", out, "18.015");

    qb_offline_answer("chemistry", "answer", "school", "moles from 36 g H2O", out, sizeof(out));
    expect_contains("grams to moles", out, "1.998");

    qb_offline_answer("chemistry", "answer", "school", "grams from 2 mol CO2", out, sizeof(out));
    expect_contains("moles to grams", out, "88.018");

    qb_offline_answer("chemistry", "answer", "school", "particles from 2 mol", out, sizeof(out));
    expect_contains("avogadro particles", out, "1.204428152e+24");


    qb_offline_answer("chemistry", "answer", "school", "molar mass Ca(OH)2", out, sizeof(out));
    expect_contains("molar mass calcium hydroxide", out, "74.092");

    qb_offline_answer("chemistry", "answer", "school", "molar mass Al2(SO4)3", out, sizeof(out));
    expect_contains("molar mass aluminum sulfate", out, "342.132");

    qb_offline_answer("chemistry", "answer", "school", "molar mass Mg(NO3)2", out, sizeof(out));
    expect_contains("molar mass magnesium nitrate", out, "148.313");

    qb_offline_answer("computer_science", "answer", "school", "binary and 1010 1100", out, sizeof(out));
    expect_contains("binary and", out, "1000");

    qb_offline_answer("computer_science", "answer", "school", "binary or 1010 1100", out, sizeof(out));
    expect_contains("binary or", out, "1110");

    qb_offline_answer("computer_science", "answer", "school", "binary xor 1010 1100", out, sizeof(out));
    expect_contains("binary xor", out, "0110");

    qb_offline_answer("computer_science", "answer", "school", "binary not 1010", out, sizeof(out));
    expect_contains("binary not", out, "0101");

    qb_offline_answer("computer_science", "answer", "school", "ascii A", out, sizeof(out));
    expect_contains("ascii encode", out, "65");

    qb_offline_answer("computer_science", "answer", "school", "char 65", out, sizeof(out));
    expect_contains("ascii decode", out, "'A'");


    qb_offline_answer("computer_science", "answer", "school", "text to binary Hi", out, sizeof(out));
    expect_contains("text binary H", out, "01001000");
    expect_contains("text binary i", out, "01101001");

    qb_offline_answer("computer_science", "answer", "school", "binary to text 01001000 01101001", out, sizeof(out));
    expect_contains("binary text decode", out, "Hi");

    qb_offline_answer("computer_science", "answer", "school", "text to hex Hi", out, sizeof(out));
    expect_contains("text hex encode", out, "48 69");

    qb_offline_answer("computer_science", "answer", "school", "hex to text 48 69", out, sizeof(out));
    expect_contains("hex text decode", out, "Hi");

    qb_offline_answer("computer_science", "answer", "school", "base 16 FF to base 2", out, sizeof(out));
    expect_contains("base conversion hex binary", out, "11111111");

    qb_offline_answer("computer_science", "answer", "school", "base 2 11111111 to base 16", out, sizeof(out));
    expect_contains("base conversion binary hex", out, "FF");


    qb_offline_answer("geometry", "answer", "school", "third angle 50 60", out, sizeof(out));
    expect_contains("triangle third angle", out, "70");

    qb_offline_answer("geometry", "answer", "school", "interior angle sum 6", out, sizeof(out));
    expect_contains("polygon angle sum", out, "720");

    qb_offline_answer("geometry", "answer", "school", "regular polygon interior angle 6", out, sizeof(out));
    expect_contains("regular polygon angle", out, "120");

    qb_offline_answer("algebra_1", "answer", "school", "line through 1 2 3 6", out, sizeof(out));
    expect_contains("line through points", out, "2x");

    qb_offline_answer("physics", "answer", "school", "final velocity 5 2 3", out, sizeof(out));
    expect_contains("kinematics velocity", out, "11");

    qb_offline_answer("physics", "answer", "school", "kinematic distance 5 2 3", out, sizeof(out));
    expect_contains("kinematics displacement", out, "24");

    qb_offline_answer("physics", "answer", "school", "free fall time height 19.6133", out, sizeof(out));
    expect_contains("free fall time", out, "2");

    qb_offline_answer("business", "answer", "school", "profit revenue 1000 cost 700", out, sizeof(out));
    expect_contains("business profit", out, "300");

    qb_offline_answer("business", "answer", "school", "break even fixed 1000 price 20 variable 12", out, sizeof(out));
    expect_contains("break even", out, "125");

    qb_offline_answer("business", "answer", "school", "roi profit 100 investment 500", out, sizeof(out));
    expect_contains("roi tool", out, "20%");

    qb_offline_answer("consumer_math", "steps", "school", "3 notebooks cost 12 dollars how much do 5 notebooks cost", out, sizeof(out));
    expect_contains("unit rate cost", out, "20");

    qb_offline_answer("physics", "steps", "school", "120 miles in 2 hours how fast", out, sizeof(out));
    expect_contains("natural speed problem", out, "60 mph");


    qb_offline_answer("algebra_2", "answer", "school", "evaluate f(x)=3x^2+2x-5 at x=4", out, sizeof(out));
    expect_contains("function evaluate", out, "51");

    qb_offline_answer("algebra_2", "answer", "school", "vertex y=2x^2-8x+3", out, sizeof(out));
    expect_contains("quadratic vertex", out, "(2, -5)");
    expect_contains("quadratic axis from vertex", out, "x = 2");

    qb_offline_answer("algebra_2", "answer", "school", "axis of symmetry y=x^2-6x+5", out, sizeof(out));
    expect_contains("axis symmetry", out, "x = 3");

    qb_offline_answer("algebra_2", "answer", "school", "zeros x^2-5x+6", out, sizeof(out));
    expect_contains("function zero 1", out, "3");
    expect_contains("function zero 2", out, "2");

    qb_offline_answer("algebra_1", "answer", "school", "y intercept y=4x-7", out, sizeof(out));
    expect_contains("y intercept", out, "(0, -7)");

    qb_offline_answer("statistics", "answer", "school", "mode 1 2 2 3 3 3 4", out, sizeof(out));
    expect_contains("mode tool", out, "3");
    expect_contains("mode frequency", out, "frequency 3");

    qb_offline_answer("statistics", "answer", "school", "quartiles 1 2 3 4 5 6 7 8", out, sizeof(out));
    expect_contains("quartiles q1", out, "Q1 2.5");
    expect_contains("quartiles median", out, "median 4.5");
    expect_contains("quartiles q3", out, "Q3 6.5");
    expect_contains("quartiles iqr", out, "IQR = 4");

    qb_offline_answer("statistics", "answer", "school", "zscore 85 75 5", out, sizeof(out));
    expect_contains("zscore tool", out, "2");


    qb_offline_answer("calculus", "answer", "college", "derivative at x=2 of 3x^2+2x-5", out, sizeof(out));
    expect_contains("derivative at point", out, "14");

    qb_offline_answer("calculus", "answer", "college", "tangent line x^2 at x=3", out, sizeof(out));
    expect_contains("tangent line slope", out, "slope 6");
    expect_contains("tangent line equation", out, "6x -9");

    qb_offline_answer("calculus", "answer", "college", "definite integral x^2 from 0 to 3", out, sizeof(out));
    expect_contains("definite integral", out, "9");

    qb_offline_answer("calculus", "answer", "college", "limit x^2+2x as x->3", out, sizeof(out));
    expect_contains("polynomial limit", out, "15");

    qb_offline_answer("algebra_2", "answer", "school", "arithmetic nth 5 3 10", out, sizeof(out));
    expect_contains("arithmetic nth", out, "32");

    qb_offline_answer("algebra_2", "answer", "school", "geometric nth 2 3 5", out, sizeof(out));
    expect_contains("geometric nth", out, "162");

    qb_offline_answer("algebra_2", "answer", "school", "arithmetic sum first 5 difference 3 terms 10", out, sizeof(out));
    expect_contains("arithmetic sum", out, "185");

    qb_offline_answer("algebra_2", "answer", "school", "geometric sum first 2 ratio 3 terms 4", out, sizeof(out));
    expect_contains("geometric sum", out, "80");


    qb_offline_answer("algebra_1", "steps", "school", "Alex is 5 years older than Sam. Sam is 12. How old is Alex?", out, sizeof(out));
    expect_contains("age older problem", out, "17");

    qb_offline_answer("algebra_1", "steps", "school", "Mia is 3 years younger than Jay. Jay is 14. How old is Mia?", out, sizeof(out));
    expect_contains("age younger problem", out, "11");

    qb_offline_answer("math", "answer", "school", "split 100 in ratio 2:3", out, sizeof(out));
    expect_contains("ratio split first", out, "40");
    expect_contains("ratio split second", out, "60");

    qb_offline_answer("consumer_math", "answer", "school", "grade 45 out of 50", out, sizeof(out));
    expect_contains("grade percentage", out, "90%");

    qb_offline_answer("consumer_math", "answer", "school", "earn 15 dollars per hour for 8 hours", out, sizeof(out));
    expect_contains("hourly pay", out, "120");

    qb_offline_answer("statistics", "answer", "school", "weighted average 90 40 80 60", out, sizeof(out));
    expect_contains("weighted average", out, "84");

    qb_offline_answer("math", "answer", "school", "6 boxes each have 4 pencils how many total", out, sizeof(out));
    expect_contains("equal groups natural", out, "24");

    qb_offline_answer("algebra_1", "answer", "school", "two numbers have sum 20 and difference 4", out, sizeof(out));
    expect_contains("sum difference first", out, "12");
    expect_contains("sum difference second", out, "8");

    puts("Standalone universal responder tests passed");
    return 0;
}
