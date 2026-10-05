#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/offline.h"

char const *qb_reference_text(char const *subject_id)
{
    (void)subject_id;
    return "General reference: identify the concept, rule, evidence, inputs, and requested result.";
}

static void fail(char const *label, char const *input, char const *output)
{
    fprintf(stderr, "FUZZ FAIL %s\nINPUT: [%s]\nOUTPUT: [%s]\n",
            label, input ? input : "(null)", output ? output : "(null)");
    exit(1);
}

int main(void)
{
    static char const *inputs[] = {
        " ",
        "???",
        "@@@###",
        "hi",
        "wsp",
        "idk",
        "lol",
        "42",
        "-17",
        "3.14159",
        "Q",
        "ZXQ",
        "0b101101",
        "0xBEEF",
        "notes.pdf",
        "name@example.com",
        "https://example.com/test",
        "print(x)",
        "what is photosythesis",
        "celluar respiration",
        "standerd deviation",
        "seperation of powers",
        "what is mitosis",
        "what is osmosis",
        "what is an isotope",
        "what is comparative advantage",
        "what is an operating system",
        "random unmatched question about purple clocks",
        "frobnicator",
        "thermobiology",
        "why do cells divide",
        "how does a computer loop work",
        "who Zargle McTestface",
        "where blorptown",
        "when flarble war",
        "2+2",
        "what is 18 plus 7",
        "twenty five plus seven",
        "calculate (7-2)*6",
        "sqrt(81)",
        "square root of 81",
        "20 percent of 50",
        "what percent is 15 of 60",
        "increase 80 by 15 percent",
        "percent increase from 50 to 75",
        "simplify fraction 42/56",
        "gcd 84 126",
        "lcm 12 18",
        "factor 360",
        "factorial 6",
        "median 1 3 8 10",
        "standard deviation 2 2 4 4",
        "next in sequence 2 5 8 11",
        "sequence 3 6 12 24",
        "sequence 1 1 2 3 5",
        "3x+7=25",
        "solve 2x-4=x+9",
        "x^2-5x+6=0",
        "solve 2x+3<11",
        "solve -3x+6>=12",
        "2/3=x/12",
        "solve system 2x+y=7; x-y=2",
        "derive 3x^2+2x-5",
        "integrate 6x+4",
        "distance between 1 2 5 8",
        "midpoint between 1 2 5 8",
        "hypotenuse 3 4",
        "sin 30 degrees",
        "180 degrees to radians",
        "5 kg to lb",
        "10 miles to kilometers",
        "120 inches to feet",
        "72 fahrenheit to celsius",
        "kinetic energy mass 2 velocity 3",
        "momentum mass 4 velocity 5",
        "potential energy mass 2 height 5",
        "ph 0.001",
        "ideal gas pressure 2 300 10",
        "cross Aa x Aa",
        "Fe",
        "atomic number 8",
        "symbol for sodium",
        "20 percent discount on 50",
        "7 percent sales tax on 100",
        "18 percent tip on 25",
        "uppercase hello world",
        "title case the quick brown fox",
        "count vowels in education",
        "initials of central processing unit",
        "alphabetize pear apple banana",
        "reverse hello",
        "is racecar a palindrome",
        "is listen an anagram of silent",
        "rot13 hello",
        "caesar 3 abc xyz",
        "roman 49",
        "roman to decimal XLIX",
        "The empire expanded quickly",
        "I think the policy changed society",
        "write an essay about courage",
        "check this answer",
        "what do you think about homework"
    };
    char out[4096];
    size_t i;

    for(i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++) {
        int ok = qb_offline_answer("auto", "explain", "school",
                                   inputs[i], out, sizeof(out));
        if(!ok) fail("returned false", inputs[i], out);
        if(out[0] == '\0') fail("empty answer", inputs[i], out);
        if(strstr(out, "STANDALONE CORE\nQuick method"))
            fail("legacy generic fallback leaked", inputs[i], out);
    }

    {
        char long_input[650];
        size_t j;
        strcpy(long_input, "explain ");
        for(j = strlen(long_input); j + 2 < sizeof(long_input); j++) {
            long_input[j] = (j % 7 == 0) ? ' ' : (char)('a' + (j % 26));
        }
        long_input[sizeof(long_input)-1] = '\0';

        if(!qb_offline_answer("auto", "explain", "school",
                              long_input, out, sizeof(out)))
            fail("long input returned false", long_input, out);
        if(out[0] == '\0') fail("long input empty answer", long_input, out);
    }

    printf("Standalone fuzz corpus passed: %zu fixed inputs + long-input case\n",
           sizeof(inputs)/sizeof(inputs[0]));
    return 0;
}
