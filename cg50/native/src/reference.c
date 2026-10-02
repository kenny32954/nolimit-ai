#include "reference.h"

#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>

#define REF_LINES 36
#define REF_COLS 54
#define REF_VISIBLE 8

typedef struct {
    char const *id;
    char const *text;
} ref_entry_t;

static ref_entry_t const refs[] = {
    {"math", "ORDER: parentheses, exponents, multiply/divide, add/subtract.\nPercent = part / whole x 100.\nSlope = (y2-y1)/(x2-x1).\nDistance = rate x time."},
    {"algebra", "Linear: y=mx+b.\nSlope m=(y2-y1)/(x2-x1).\nQuadratic: x=(-b +/- sqrt(b^2-4ac))/(2a).\nCheck solutions in the original equation."},
    {"geometry", "Triangle angle sum = 180 deg.\nRectangle A=lw. Triangle A=bh/2. Circle A=pi r^2.\nCircumference=2 pi r.\nPythagorean: a^2+b^2=c^2."},
    {"statistics", "Mean=sum/n. Median=middle.\nRange=max-min.\nProbability=favorable/total.\nCorrelation does not automatically prove causation."},
    {"calculus", "Derivative = instantaneous rate of change.\nPower rule: d/dx x^n = n x^(n-1).\nIntegral = accumulated change/area.\nAlways check constants and domain."},
    {"biology", "Cell membrane controls exchange.\nDNA stores genetic information.\nMitosis: growth/repair. Meiosis: gametes.\nPhotosynthesis stores light energy in glucose."},
    {"chemistry", "Atoms conserve in reactions: balance equations.\nMoles = mass / molar mass.\nMolarity = moles / liters.\npH<7 acidic, 7 neutral, >7 basic."},
    {"physics", "v=d/t. a=change in v / t.\nF=ma. Weight=mg.\nWork=F d (parallel force).\nOhm: V=IR. Power: P=VI."},
    {"earth", "Weather = short-term atmosphere; climate = long-term pattern.\nPlate motion drives many earthquakes/volcanoes.\nRock cycle links igneous, sedimentary, metamorphic."},
    {"environmental_science", "Track matter cycles and energy flow.\nPopulation change depends on births, deaths, immigration, emigration.\nEvaluate environmental tradeoffs with evidence."},
    {"ela", "Sentence basics: subject + predicate.\nUse evidence for claims.\nTheme is a broader idea, not just one word.\nTone = author's attitude; mood = reader feeling."},
    {"literature", "Analyze character, conflict, setting, plot, theme, tone, symbolism.\nSupport interpretations with specific text evidence.\nSeparate what the text says from what you infer."},
    {"writing", "Plan: claim/thesis -> evidence -> explanation.\nParagraph: topic sentence, support, analysis.\nRevise for ideas first, then grammar/punctuation.\nConclusion should synthesize, not just repeat."},
    {"history", "Ask: who, what, when, where, why, consequences.\nSeparate primary source from later interpretation.\nTrack chronology and multiple causes; avoid single-cause explanations."},
    {"social_studies", "Connect geography, culture, economics, civics, and history.\nCompare perspectives using evidence.\nWatch for cause/effect versus simple coincidence."},
    {"geography", "Latitude = north/south of Equator. Longitude = east/west of Prime Meridian.\nThink in location, place, region, movement, and human-environment interaction."},
    {"government", "US federal branches: legislative makes laws; executive enforces; judicial interprets.\nFederalism divides power between national and state governments."},
    {"economics", "Scarcity forces choices. Opportunity cost = next-best alternative.\nDemand usually falls as price rises; supply usually rises.\nEquilibrium is where supply and demand meet."},
    {"business", "Marketing mix: product, price, place, promotion.\nProfit = revenue - costs.\nManagement: plan, organize, lead, control.\nKnow customer, value proposition, competition."},
    {"accounting", "Accounting equation: Assets = Liabilities + Equity.\nDebits/credits depend on account type.\nIncome statement tracks revenues/expenses; balance sheet shows position at a date."},
    {"computer_science", "Algorithm = ordered procedure.\nVariable stores a value. Condition chooses. Loop repeats. Function packages behavior.\nTest edge cases and read the actual error message."},
    {"engineering", "Design cycle: define -> research -> brainstorm -> prototype -> test -> improve.\nRecord constraints and criteria.\nUse units and tolerances consistently."},
    {"cte", "Follow the class safety procedure first.\nMeasure twice, verify specs, document steps.\nTechnical work should match drawings, tolerances, and tool requirements."},
    {"agriculture", "Plant growth depends on light, water, nutrients, temperature, and soil conditions.\nTrack inputs/outputs in production systems.\nUse safe handling procedures for equipment/animals."},
    {"psychology", "Distinguish correlation from causation.\nIndependent variable is manipulated; dependent variable is measured.\nBehavior and cognition are studied with evidence, not diagnosis-by-guess."},
    {"sociology", "Study groups, institutions, culture, norms, roles, and social structures.\nCompare perspectives and distinguish individual examples from population patterns."},
    {"art", "Elements: line, shape, form, value, color, texture, space.\nPrinciples include balance, contrast, emphasis, movement, pattern, rhythm, unity.\nCritique: describe -> analyze -> interpret -> evaluate."},
    {"music", "Rhythm organizes duration; melody is pitch sequence; harmony combines pitches.\nTempo = speed; dynamics = loudness.\nRead key signature, time signature, and note values before playing."},
    {"media", "Preproduction: concept, script, shot list/storyboard.\nProduction: framing, focus, exposure, audio, continuity.\nPost: organize, cut for meaning, mix audio, color-correct, export to spec."},
    {"language", "Study nouns/articles, verb forms, word order, agreement, and context.\nTranslate meaning, not just one word at a time.\nReview high-frequency vocabulary in full sentences."},
    {"health", "Use reliable health sources.\nBalanced nutrition emphasizes variety and nutrient-dense foods.\nHealth class information is educational, not a personal diagnosis."},
    {"physical_education", "Warm up gradually and use safe technique.\nFitness components include cardiovascular endurance, strength, muscular endurance, flexibility, and body composition.\nRules and sportsmanship matter."}
};

static char const *lookup(char const *id)
{
    unsigned int i;
    for(i = 0; i < sizeof(refs) / sizeof(refs[0]); i++) {
        if(strcmp(refs[i].id, id) == 0) return refs[i].text;
    }

    return "Quick method:\n1. Identify what the assignment asks.\n2. List given information.\n3. Choose the rule, concept, or evidence.\n4. Work carefully.\n5. Check the result against the question.";
}

static int make_lines(char const *text, char lines[][REF_COLS], int max_lines)
{
    int count = 0;
    int pos = 0;
    int len = (int)strlen(text);

    while(pos < len && count < max_lines) {
        int remaining = len - pos;
        int take = remaining < REF_COLS - 1 ? remaining : REF_COLS - 1;
        int i;
        int newline = -1;

        for(i = 0; i < take; i++) {
            if(text[pos + i] == '\n') {
                newline = i;
                break;
            }
        }

        if(newline >= 0) {
            take = newline;
        }
        else if(take < remaining) {
            for(i = take; i > 0; i--) {
                if(text[pos + i] == ' ') {
                    take = i;
                    break;
                }
            }
        }

        if(take < 0) take = 0;
        memcpy(lines[count], text + pos, (size_t)take);
        lines[count][take] = '\0';
        count++;

        pos += take;
        if(pos < len && text[pos] == '\n') pos++;
        while(pos < len && text[pos] == ' ') pos++;
    }

    return count;
}

void qb_reference_show(char const *subject_id, char const *subject_label)
{
    char lines[REF_LINES][REF_COLS];
    int count = make_lines(lookup(subject_id), lines, REF_LINES);
    int scroll = 0;

    while(1) {
        color_t bg = C_RGB(3, 3, 4);
        color_t panel = C_RGB(6, 6, 8);
        color_t text = C_RGB(29, 29, 30);
        color_t muted = C_RGB(18, 18, 20);
        color_t accent = C_RGB(28, 11, 8);
        key_event_t ev;
        int i;

        dclear(bg);
        drect(0, 0, DWIDTH - 1, 31, panel);
        dtext(10, 8, text, "QUICK REFERENCE");
        dtext(184, 8, accent, subject_label);

        for(i = 0; i < REF_VISIBLE && scroll + i < count; i++) {
            dtext(12, 43 + i * 19, text, lines[scroll + i]);
        }

        drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
        dtext(8, DHEIGHT - 23, muted, "UP/DOWN SCROLL");
        dtext(276, DHEIGHT - 23, muted, "EXIT BACK");
        dupdate();

        ev = getkey();
        if(ev.key == KEY_EXIT) return;
        if(ev.key == KEY_UP && scroll > 0) scroll--;
        if(ev.key == KEY_DOWN && scroll + REF_VISIBLE < count) scroll++;
    }
}
