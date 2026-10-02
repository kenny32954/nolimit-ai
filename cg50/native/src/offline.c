#include "offline.h"
#include "reference.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char const *needle;
    char const *answer;
} fact_t;

static fact_t const facts[] = {
    {"photosynthesis", "Photosynthesis converts light energy into chemical energy. In simplified form: CO2 + H2O + light -> glucose + O2. Chloroplasts carry out the process; light reactions make ATP/NADPH and the Calvin cycle fixes carbon."},
    {"cellular respiration", "Cellular respiration transfers energy from glucose into ATP. Major stages are glycolysis, the citric-acid cycle, and oxidative phosphorylation. Aerobic respiration ultimately uses oxygen as the final electron acceptor."},
    {"mitosis", "Mitosis produces two genetically similar daughter cells for growth and repair. Main phases: prophase, metaphase, anaphase, telophase, followed by cytokinesis."},
    {"meiosis", "Meiosis produces haploid cells and increases genetic variation. It has two divisions after one DNA replication and includes crossing over and independent assortment."},
    {"dna", "DNA stores hereditary information in nucleotide sequences. Genes are DNA regions used to make functional RNA or proteins; base pairing is A-T and C-G in DNA."},
    {"natural selection", "Natural selection occurs when heritable variation affects survival or reproduction. Traits that increase reproductive success can become more common across generations."},
    {"homeostasis", "Homeostasis is regulation that keeps internal conditions within workable ranges. Feedback loops use sensors, control centers, and effectors; negative feedback is the common stabilizing pattern."},
    {"atom", "An atom has a nucleus containing protons and neutrons, with electrons in quantized energy states around it. Atomic number equals proton count; isotopes differ in neutron count."},
    {"mole", "A mole is 6.022 x 10^23 entities. Convert mass to moles with moles = mass / molar mass, then use mole ratios from a balanced chemical equation."},
    {"stoichiometry", "Stoichiometry uses coefficients in a balanced chemical equation as mole ratios. Standard path: given quantity -> moles -> mole ratio -> requested quantity."},
    {"acid", "Acids donate H+ in the Bronsted-Lowry model; bases accept H+. At 25 C, pH below 7 is acidic, 7 neutral, and above 7 basic."},
    {"newton", "Newton's laws: (1) motion stays constant unless a net force acts; (2) net force equals mass times acceleration, F=ma; (3) interaction forces occur in equal-and-opposite pairs on different objects."},
    {"kinetic energy", "Translational kinetic energy is K = 1/2 m v^2. Work done by the net force changes kinetic energy: W_net = Delta K."},
    {"momentum", "Linear momentum is p = mv. In an isolated system total momentum is conserved. Impulse J = F_avg Delta t = Delta p."},
    {"ohm", "Ohm's law is V = I R for an ohmic element. Electrical power can be written P=VI, P=I^2 R, or P=V^2/R."},
    {"pythagorean", "For a right triangle with legs a,b and hypotenuse c: a^2+b^2=c^2. The hypotenuse is always opposite the right angle."},
    {"quadratic", "A quadratic has form ax^2+bx+c=0 with a nonzero. Quadratic formula: x=(-b +/- sqrt(b^2-4ac))/(2a). The discriminant b^2-4ac determines the root type."},
    {"slope", "Slope measures rate of change: m=(y2-y1)/(x2-x1). Slope-intercept form is y=mx+b."},
    {"system of equations", "A system asks for values satisfying all equations at once. Common methods are substitution, elimination, graphing, and matrix row reduction."},
    {"inequality", "Solve inequalities like equations, but reverse the inequality sign when multiplying or dividing both sides by a negative number."},
    {"derivative", "A derivative is an instantaneous rate of change. Geometrically it is tangent-line slope. Power rule: d/dx x^n = n x^(n-1)."},
    {"integral", "A definite integral accumulates signed area/change. The Fundamental Theorem of Calculus connects antiderivatives with definite integrals."},
    {"eigenvalue", "For matrix A, lambda is an eigenvalue when det(A-lambda I)=0. The corresponding eigenspace is the null space of A-lambda I."},
    {"induction", "Mathematical induction proves statements over integers: prove a base case, assume the claim for k, then use that assumption to prove the k+1 case."},
    {"mean", "Mean = sum of values / number of values. Median is the middle ordered value. Mode is the most frequent value. Range = max-min."},
    {"standard deviation", "Standard deviation measures typical spread around the mean. A larger standard deviation means values are more dispersed."},
    {"correlation", "Correlation measures association, not causation. A causal claim needs a justified design and control of alternative explanations."},
    {"thesis", "A thesis is the central claim an essay will support. A strong thesis is specific, arguable, and matched to the assignment and available evidence."},
    {"theme", "Theme is a broader idea a text develops about people, society, or life. Support a theme claim with specific events, patterns, symbols, or language from the text."},
    {"metaphor", "A metaphor directly describes one thing as another to create meaning or comparison without using 'like' or 'as'."},
    {"dramatic irony", "Dramatic irony occurs when the audience knows important information that one or more characters do not, creating tension, humor, or anticipation."},
    {"primary source", "A primary source comes directly from the period/event being studied, such as a letter, law, diary, speech, artifact, photograph, or original dataset."},
    {"federalism", "Federalism divides governing authority between national and subnational governments. In the U.S., powers are allocated among federal and state governments under the Constitution."},
    {"separation of powers", "Separation of powers divides government functions among branches; checks and balances give branches tools to limit one another."},
    {"supply", "Supply describes quantities producers are willing and able to sell at different prices. Demand describes quantities consumers are willing and able to buy. Their interaction helps determine market price and quantity."},
    {"opportunity cost", "Opportunity cost is the value of the next-best alternative given up when a choice is made."},
    {"inflation", "Inflation is a sustained rise in the general price level, which reduces purchasing power of a unit of currency if income does not rise similarly."},
    {"marketing mix", "The traditional marketing mix is Product, Price, Place, and Promotion. It organizes decisions about what is offered, what it costs, how it reaches customers, and how it is communicated."},
    {"accounting equation", "Accounting equation: Assets = Liabilities + Equity. Transactions must keep this equation balanced."},
    {"debit", "Debits and credits are accounting entry directions, not simply plus/minus. Assets normally increase with debits; liabilities and equity normally increase with credits."},
    {"algorithm", "An algorithm is a finite, well-defined procedure for solving a problem. Evaluate correctness plus time and space complexity."},
    {"variable", "A variable names a stored value. Its meaning depends on scope, type, lifetime, and how the program updates or reads it."},
    {"loop", "A loop repeats code while a condition or iteration rule applies. Common forms are for-loops and while-loops; always consider termination and edge cases."},
    {"function", "A function packages behavior behind inputs and outputs. Good functions have clear responsibilities, predictable contracts, and limited side effects."},
    {"html", "HTML gives a web page semantic structure; CSS controls presentation; JavaScript adds behavior and interaction."},
    {"hypothesis", "A research hypothesis is a testable prediction. Good studies define variables operationally, select an appropriate design, analyze uncertainty, and discuss limitations."},
    {"validity", "Internal validity asks whether a study supports its causal/measurement conclusions; external validity asks how well results generalize beyond the study context."},
    {"plate tectonics", "Earth's lithosphere is divided into moving plates. Divergent boundaries separate, convergent boundaries collide/subduct, and transform boundaries slide past."},
    {"rock cycle", "The rock cycle connects igneous, sedimentary, and metamorphic rocks through melting, cooling, weathering, deposition, burial, heat, and pressure."},
    {"weather", "Weather describes short-term atmospheric conditions; climate describes long-term statistical patterns over a region."},
    {"photosphere", "The photosphere is the Sun's visible surface layer. Above it are the chromosphere and corona; energy is generated much deeper in the core by fusion."},
    {"perspective", "Linear perspective creates depth by making parallel lines appear to converge toward vanishing points on a horizon line."},
    {"color theory", "Color theory studies relationships among hue, value, and saturation. Complementary colors lie opposite each other on a color wheel and create strong contrast."},
    {"rhythm", "In music, rhythm organizes durations and accents over time. Meter groups beats; tempo describes speed."},
    {"chord", "A chord combines pitches sounding together. In tonal music, triads are commonly built from a root, third, and fifth."},
    {"aperture", "In photography, aperture controls the lens opening. A lower f-number means a wider opening, more light, and usually shallower depth of field."},
    {"shutter speed", "Shutter speed controls exposure time. Faster speeds reduce motion blur; slower speeds admit more light and can blur movement."},
    {"nutrition", "Nutrition focuses on energy and nutrients needed for growth and health. A balanced pattern emphasizes variety and nutrient-dense foods rather than extreme restriction."},
    {"credit score", "A credit score estimates credit risk from credit-report information. Common influences include payment history, amounts owed/utilization, account age, credit mix, and new credit activity."},
    {"compound interest", "Compound interest earns interest on principal plus accumulated interest. A common formula is A=P(1+r/n)^(nt)."},
    {"latitude", "Latitude measures north/south position from the Equator; longitude measures east/west position from the Prime Meridian."}
};

static char lowerbuf[700];

static void lowercase(char const *src)
{
    size_t i;
    for(i = 0; i + 1 < sizeof(lowerbuf) && src[i]; i++) {
        lowerbuf[i] = (char)tolower((unsigned char)src[i]);
    }
    lowerbuf[i] = '\0';
}

static bool starts_with(char const *text, char const *prefix)
{
    return strncmp(text, prefix, strlen(prefix)) == 0;
}

static void skip_spaces(char const **p)
{
    while(**p && isspace((unsigned char)**p)) (*p)++;
}

typedef struct {
    char const *p;
    bool error;
} parser_t;

static double parse_expr(parser_t *ps);

static double parse_primary(parser_t *ps)
{
    double value;
    char *end;

    skip_spaces(&ps->p);

    if(*ps->p == '(') {
        ps->p++;
        value = parse_expr(ps);
        skip_spaces(&ps->p);
        if(*ps->p != ')') {
            ps->error = true;
            return 0.0;
        }
        ps->p++;
        return value;
    }

    if(starts_with(ps->p, "sqrt")) {
        ps->p += 4;
        skip_spaces(&ps->p);
        if(*ps->p != '(') {
            ps->error = true;
            return 0.0;
        }
        ps->p++;
        value = parse_expr(ps);
        skip_spaces(&ps->p);
        if(*ps->p != ')' || value < 0.0) {
            ps->error = true;
            return 0.0;
        }
        ps->p++;
        return sqrt(value);
    }

    value = strtod(ps->p, &end);
    if(end == ps->p) {
        ps->error = true;
        return 0.0;
    }

    ps->p = end;
    return value;
}

static double parse_unary(parser_t *ps)
{
    skip_spaces(&ps->p);

    if(*ps->p == '+') {
        ps->p++;
        return parse_unary(ps);
    }
    if(*ps->p == '-') {
        ps->p++;
        return -parse_unary(ps);
    }

    return parse_primary(ps);
}

static double parse_power(parser_t *ps)
{
    double left = parse_unary(ps);
    skip_spaces(&ps->p);

    if(*ps->p == '^') {
        double right;
        ps->p++;
        right = parse_power(ps);
        return pow(left, right);
    }

    return left;
}

static double parse_term(parser_t *ps)
{
    double value = parse_power(ps);

    while(!ps->error) {
        char op;
        double rhs;
        skip_spaces(&ps->p);
        op = *ps->p;

        if(op != '*' && op != '/') break;
        ps->p++;
        rhs = parse_power(ps);

        if(op == '*') value *= rhs;
        else {
            if(rhs == 0.0) {
                ps->error = true;
                return 0.0;
            }
            value /= rhs;
        }
    }

    return value;
}

static double parse_expr(parser_t *ps)
{
    double value = parse_term(ps);

    while(!ps->error) {
        char op;
        double rhs;
        skip_spaces(&ps->p);
        op = *ps->p;

        if(op != '+' && op != '-') break;
        ps->p++;
        rhs = parse_term(ps);

        if(op == '+') value += rhs;
        else value -= rhs;
    }

    return value;
}

static bool eval_expression(char const *text, double *value)
{
    parser_t ps;
    ps.p = text;
    ps.error = false;

    *value = parse_expr(&ps);
    skip_spaces(&ps.p);

    return !ps.error && *ps.p == '\0';
}

static bool looks_like_expression(char const *text)
{
    bool has_digit = false;
    size_t i;

    for(i = 0; text[i]; i++) {
        unsigned char c = (unsigned char)text[i];

        if(isdigit(c)) {
            has_digit = true;
            continue;
        }

        if(isspace(c) || c == '.' || c == '+' || c == '-' ||
           c == '*' || c == '/' || c == '^' || c == '(' || c == ')') {
            continue;
        }

        return false;
    }

    return has_digit;
}

static int parse_list(char const *text, double *values, int cap)
{
    int count = 0;
    char *end;

    while(*text && count < cap) {
        while(*text && (isspace((unsigned char)*text) || *text == ',')) text++;
        if(!*text) break;

        values[count] = strtod(text, &end);
        if(end == text) break;

        count++;
        text = end;
    }

    return count;
}

static void write_reference(char const *subject, char *out, size_t out_size)
{
    char const *ref = qb_reference_text(subject);
    snprintf(
        out,
        out_size,
        "STANDALONE CORE\n%s\n\nTip: for calculations use commands like calc, slope, quad, mean, force, ohm, moles, molarity, percent, pyth, or compound.",
        ref
    );
}

bool qb_offline_answer(
    char const *subject,
    char const *mode,
    char const *level,
    char const *prompt,
    char *out,
    size_t out_size
)
{
    double v[32];
    double result;
    int n;
    unsigned int i;
    (void)mode;
    (void)level;

    if(!out || out_size == 0 || !prompt || !*prompt) return false;
    out[0] = '\0';
    lowercase(prompt);

    if(starts_with(lowerbuf, "calc ")) {
        if(eval_expression(prompt + 5, &result)) {
            snprintf(out, out_size, "= %.12g", result);
            return true;
        }
        snprintf(out, out_size, "Could not parse that expression. Offline calc supports + - * / ^ parentheses and sqrt(...).");
        return true;
    }

    if(looks_like_expression(lowerbuf) && eval_expression(prompt, &result)) {
        snprintf(out, out_size, "= %.12g", result);
        return true;
    }

    if(starts_with(lowerbuf, "slope ")) {
        n = parse_list(prompt + 6, v, 4);
        if(n == 4 && v[2] != v[0]) {
            result = (v[3] - v[1]) / (v[2] - v[0]);
            snprintf(out, out_size, "Slope m=(y2-y1)/(x2-x1) = %.12g", result);
        }
        else snprintf(out, out_size, "Use: slope x1 y1 x2 y2. A vertical line has undefined slope.");
        return true;
    }

    if(starts_with(lowerbuf, "pyth ")) {
        n = parse_list(prompt + 5, v, 2);
        if(n == 2 && v[0] >= 0.0 && v[1] >= 0.0) {
            result = sqrt(v[0]*v[0] + v[1]*v[1]);
            snprintf(out, out_size, "c=sqrt(a^2+b^2)=%.12g", result);
        }
        else snprintf(out, out_size, "Use: pyth a b  (two nonnegative leg lengths)");
        return true;
    }

    if(starts_with(lowerbuf, "quad ")) {
        n = parse_list(prompt + 5, v, 3);
        if(n == 3 && v[0] != 0.0) {
            double d = v[1]*v[1] - 4.0*v[0]*v[2];
            if(d >= 0.0) {
                double r1 = (-v[1] + sqrt(d)) / (2.0*v[0]);
                double r2 = (-v[1] - sqrt(d)) / (2.0*v[0]);
                snprintf(out, out_size, "Discriminant=%.12g\nx1=%.12g\nx2=%.12g", d, r1, r2);
            }
            else {
                double real = -v[1] / (2.0*v[0]);
                double imag = sqrt(-d) / fabs(2.0*v[0]);
                snprintf(out, out_size, "Discriminant=%.12g\nRoots: %.12g +/- %.12gi", d, real, imag);
            }
        }
        else snprintf(out, out_size, "Use: quad a b c  for ax^2+bx+c=0");
        return true;
    }

    if(starts_with(lowerbuf, "mean ")) {
        n = parse_list(prompt + 5, v, 32);
        if(n > 0) {
            int j;
            result = 0.0;
            for(j = 0; j < n; j++) result += v[j];
            result /= n;
            snprintf(out, out_size, "n=%d\nMean=%.12g", n, result);
        }
        else snprintf(out, out_size, "Use: mean 4 7 9 10  (spaces or commas)");
        return true;
    }

    if(starts_with(lowerbuf, "percent ")) {
        n = parse_list(prompt + 8, v, 2);
        if(n == 2 && v[1] != 0.0) {
            snprintf(out, out_size, "%.12g is %.12g%% of %.12g", v[0], 100.0*v[0]/v[1], v[1]);
        }
        else snprintf(out, out_size, "Use: percent part whole");
        return true;
    }

    if(starts_with(lowerbuf, "force ")) {
        n = parse_list(prompt + 6, v, 2);
        if(n == 2) snprintf(out, out_size, "F=ma = %.12g N", v[0]*v[1]);
        else snprintf(out, out_size, "Use: force mass_kg acceleration_m_s2");
        return true;
    }

    if(starts_with(lowerbuf, "ohm ")) {
        n = parse_list(prompt + 4, v, 2);
        if(n == 2 && v[1] != 0.0) snprintf(out, out_size, "I=V/R = %.12g A", v[0]/v[1]);
        else snprintf(out, out_size, "Use: ohm voltage resistance");
        return true;
    }

    if(starts_with(lowerbuf, "moles ")) {
        n = parse_list(prompt + 6, v, 2);
        if(n == 2 && v[1] != 0.0) snprintf(out, out_size, "moles=mass/molar_mass = %.12g mol", v[0]/v[1]);
        else snprintf(out, out_size, "Use: moles mass_g molar_mass_g_per_mol");
        return true;
    }

    if(starts_with(lowerbuf, "molarity ")) {
        n = parse_list(prompt + 9, v, 2);
        if(n == 2 && v[1] != 0.0) snprintf(out, out_size, "M=moles/liters = %.12g mol/L", v[0]/v[1]);
        else snprintf(out, out_size, "Use: molarity moles liters");
        return true;
    }

    if(starts_with(lowerbuf, "compound ")) {
        n = parse_list(prompt + 9, v, 4);
        if(n == 4 && v[2] > 0.0) {
            result = v[0] * pow(1.0 + v[1]/v[2], v[2]*v[3]);
            snprintf(out, out_size, "A=P(1+r/n)^(nt) = %.12g", result);
        }
        else snprintf(out, out_size, "Use: compound P r n t  (r as decimal)");
        return true;
    }

    for(i = 0; i < sizeof(facts)/sizeof(facts[0]); i++) {
        if(strstr(lowerbuf, facts[i].needle)) {
            snprintf(out, out_size, "%s", facts[i].answer);
            return true;
        }
    }

    write_reference(subject, out, out_size);
    return true;
}
