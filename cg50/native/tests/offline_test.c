#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/offline.h"

char const *qb_reference_text(char const *subject_id)
{
    (void)subject_id;
    return "Reference fallback works.";
}

static void expect_contains(char const *label, char const *answer, char const *needle)
{
    if(!strstr(answer, needle)) {
        fprintf(stderr, "FAIL %s: expected [%s] in [%s]\n", label, needle, answer);
        exit(1);
    }
}

int main(void)
{
    char out[2048];

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

    qb_offline_answer("history", "explain", "school", "random unmatched question", out, sizeof(out));
    expect_contains("fallback", out, "Reference fallback works.");

    puts("Standalone offline core tests passed");
    return 0;
}
