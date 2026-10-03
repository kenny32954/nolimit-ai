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
    expect_contains("unmatched ref", out, "History reference");
    expect_not_contains("no old canned prefix", out, "STANDALONE CORE\nQuick method");

    puts("Standalone universal responder tests passed");
    return 0;
}
