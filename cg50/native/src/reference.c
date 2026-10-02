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
    {"calculus", "Limits define continuity/derivatives. FTC links derivatives and integrals.\nCheck theorem hypotheses, interval/domain, and convergence conditions.\nFor series, identify the test and state why it applies."},
    {"linear_algebra", "Ax=b: row reduce or use structure.\nBasis = linearly independent spanning set.\nEigenvalues solve det(A-lambda I)=0; eigenspace = null(A-lambda I).\nRank-nullity: dim(V)=rank(T)+nullity(T)."},
    {"discrete_math", "Proof tools: direct, contrapositive, contradiction, induction.\nCounting: sum/product rules, permutations, combinations, inclusion-exclusion.\nGraphs: track vertices, edges, paths, connectivity, degree."},
    {"differential_equations", "Classify ODE first: order, linear/nonlinear, autonomous, IVP/BVP.\nCommon methods: separable, linear integrating factor, characteristic roots, Laplace.\nVerify solution and initial conditions."},
    {"number_theory", "gcd via Euclidean algorithm.\na congruent b mod n means n divides (a-b).\nUse modular inverses only when gcd(a,n)=1.\nState divisibility/proof steps explicitly."},
    {"real_analysis", "Start from definitions: epsilon-delta, convergence, Cauchy, continuity.\nTrack quantifiers carefully.\nFor proofs, state hypotheses before choosing estimates or constructing N/delta."},
    {"abstract_algebra", "Group: closure, associativity, identity, inverses.\nHomomorphism preserves operation; kernel measures collapse.\nUse subgroup tests, cosets, Lagrange, quotient structures only when hypotheses hold."},
    {"biology", "Connect structure -> mechanism -> function -> evidence.\nTrack levels: molecule, cell, tissue, organism, population.\nDistinguish correlation, mechanism, and evolutionary explanation."},
    {"genetics", "Probability + inheritance: genotype, phenotype, linkage, recombination.\nHardy-Weinberg: p+q=1 and p^2+2pq+q^2=1 under model assumptions.\nGene expression links DNA -> RNA -> protein."},
    {"microbiology", "Compare cell structure, metabolism, genetics, and ecology.\nViruses require host machinery.\nInterpret growth and host-interaction concepts without assuming all microbes are pathogenic."},
    {"anatomy_physiology", "Structure supports function.\nHomeostasis uses sensors, control centers, and effectors.\nTrace flows (blood, air, filtrate, signals) through systems and connect them to regulation."},
    {"chemistry", "Track moles, charge, energy, equilibrium, and units.\nDelta G = Delta H - T Delta S.\nAt equilibrium Q=K; before equilibrium compare Q with K.\nState approximations in acid/base and equilibrium work."},
    {"organic_chemistry", "Think electron flow: nucleophile -> electrophile.\nCompare SN1/SN2/E1/E2 by substrate, nucleophile/base, solvent, kinetics, stereochemistry.\nUse resonance, induction, acidity, and conformations to justify outcomes."},
    {"biochemistry", "Structure drives function.\nEnzyme rate depends on kinetics and regulation.\nTrack carbon, electrons, ATP, and compartment across metabolism.\nRelate Delta G to pathway direction/coupling."},
    {"physics", "Define system + coordinates first.\nMechanics: sum F=ma, energy and momentum with stated conditions.\nFields/waves: use vector direction and boundary/initial conditions.\nCheck dimensions and limiting cases."},
    {"thermodynamics", "Define system, state, process, and sign convention.\nFirst law: Delta U = Q - W (for common physics sign convention).\nSecond law constrains entropy and direction.\nState whether process is isothermal, adiabatic, reversible, etc."},
    {"circuits", "Label node voltages and current directions.\nKCL at nodes; KVL around loops.\nThevenin/Norton simplify linear networks.\nFor RC/RL transients use initial/final values and time constants."},
    {"earth", "Weather = short-term atmosphere; climate = long-term pattern.\nPlate motion drives many earthquakes/volcanoes.\nRock cycle links igneous, sedimentary, metamorphic."},
    {"environmental_science", "Track matter cycles and energy flow.\nPopulation change depends on births, deaths, immigration, emigration.\nEvaluate environmental tradeoffs with evidence."},
    {"ela", "Sentence basics: subject + predicate.\nUse evidence for claims.\nTheme is a broader idea, not just one word.\nTone = author's attitude; mood = reader feeling."},
    {"literature", "Analyze character, conflict, setting, plot, theme, tone, symbolism.\nSupport interpretations with specific text evidence.\nSeparate what the text says from what you infer."},
    {"writing", "Plan: claim/thesis -> evidence -> explanation.\nParagraph: topic sentence, support, analysis.\nRevise for ideas first, then grammar/punctuation.\nConclusion should synthesize, not just repeat."},
    {"history", "Ask: who, what, when, where, why, consequences.\nSeparate primary source from later interpretation.\nTrack chronology and multiple causes; avoid single-cause explanations."},
    {"social_studies", "Connect geography, culture, economics, civics, and history.\nCompare perspectives using evidence.\nWatch for cause/effect versus simple coincidence."},
    {"geography", "Latitude = north/south of Equator. Longitude = east/west of Prime Meridian.\nThink in location, place, region, movement, and human-environment interaction."},
    {"government", "Analyze institutions, constitutional text, precedent, federalism, separation of powers, and civil liberties.\nDistinguish descriptive law/process from political argument."},
    {"political_science", "Separate normative claims from empirical claims.\nCompare institutions, incentives, behavior, and evidence.\nDefine the unit/population studied and attribute contested interpretations."},
    {"economics", "Think at the margin.\nElasticity = percent change in quantity / percent change in price.\nSeparate shifts from movements along curves.\nState market/model assumptions and distinguish positive from normative claims."},
    {"finance", "TVM: PV = FV/(1+r)^n.\nNPV = discounted inflows - outflows.\nBond price moves inversely with yield.\nState timing, compounding, cash-flow, and risk assumptions."},
    {"business", "Marketing mix: product, price, place, promotion.\nProfit = revenue - costs.\nManagement: plan, organize, lead, control.\nKnow customer, value proposition, competition."},
    {"accounting", "Accounting equation: Assets = Liabilities + Equity.\nDebits/credits depend on account type.\nIncome statement tracks revenues/expenses; balance sheet shows position at a date."},
    {"computer_science", "Separate specification, algorithm, implementation, and test.\nState invariants and complexity where relevant.\nUse small counterexamples and edge cases to test reasoning."},
    {"data_structures", "Know invariant + operations + complexity.\nArrays: O(1) indexed access. Hash tables: expected O(1) lookup with assumptions.\nTrees/heaps support ordered/hierarchical operations.\nChoose structure by workload."},
    {"algorithms", "For each algorithm: state input/output, invariant, correctness idea, time/space complexity.\nCompare greedy, divide-and-conquer, dynamic programming, graph methods.\nProve why the chosen strategy works."},
    {"databases", "Relational model: keys + constraints + relations.\nNormalize to reduce anomalies.\nIndexes trade write/storage cost for read speed.\nTransactions: atomicity, consistency, isolation, durability."},
    {"computer_architecture", "Track instruction -> datapath -> memory hierarchy.\nPerformance depends on latency, throughput, CPI, clock, cache behavior.\nUse binary/hex carefully and distinguish ISA from microarchitecture."},
    {"engineering", "Define system, constraints, assumptions, governing model, units, and acceptance criteria.\nSolve symbolically when useful, check dimensions, then validate against physical limits."},
    {"statics_dynamics", "Draw a free-body diagram first.\nStatics: sum F=0 and sum M=0.\nDynamics: sum F=ma and rotational analogs.\nUse consistent axes, signs, geometry, and units."},
    {"materials_science", "Connect processing -> structure -> properties -> performance.\nStress=sigma=F/A; strain=epsilon=Delta L/L.\nElastic slope gives E in linear region.\nUse phase diagrams with composition + temperature."},
    {"cte", "Follow the class safety procedure first.\nMeasure twice, verify specs, document steps.\nTechnical work should match drawings, tolerances, and tool requirements."},
    {"agriculture", "Plant growth depends on light, water, nutrients, temperature, and soil conditions.\nTrack inputs/outputs in production systems.\nUse safe handling procedures for equipment/animals."},
    {"psychology", "Separate theory, operational definition, measurement, design, statistics, and interpretation.\nCorrelation != causation.\nConsider confounds, validity, reliability, effect size, and ethics."},
    {"research_methods", "Question -> hypothesis -> operationalization -> design -> sample -> analysis -> limitations.\nDistinguish internal/external validity.\nNever invent citations, quotes, participants, statistics, or results."},
    {"philosophy_logic", "Argument = premises + conclusion.\nValidity: if premises true, conclusion must follow. Soundness = valid + true premises.\nFormalize connectives/quantifiers when useful and test arguments with countermodels."},
    {"sociology", "Study groups, institutions, culture, norms, roles, and social structures.\nCompare perspectives and distinguish individual examples from population patterns."},
    {"art", "Elements: line, shape, form, value, color, texture, space.\nPrinciples include balance, contrast, emphasis, movement, pattern, rhythm, unity.\nCritique: describe -> analyze -> interpret -> evaluate."},
    {"music", "Rhythm organizes duration; melody is pitch sequence; harmony combines pitches.\nTempo = speed; dynamics = loudness.\nRead key signature, time signature, and note values before playing."},
    {"media", "Preproduction: concept, script, shot list/storyboard.\nProduction: framing, focus, exposure, audio, continuity.\nPost: organize, cut for meaning, mix audio, color-correct, export to spec."},
    {"language", "Study nouns/articles, verb forms, word order, agreement, and context.\nTranslate meaning, not just one word at a time.\nReview high-frequency vocabulary in full sentences."},
    {"health", "Use reliable health sources.\nBalanced nutrition emphasizes variety and nutrient-dense foods.\nHealth class information is educational, not a personal diagnosis."},
    {"physical_education", "Warm up gradually and use safe technique.\nFitness components include cardiovascular endurance, strength, muscular endurance, flexibility, and body composition.\nRules and sportsmanship matter."}
};

char const *qb_reference_text(char const *id)
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
    int count = make_lines(qb_reference_text(subject_id), lines, REF_LINES);
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
