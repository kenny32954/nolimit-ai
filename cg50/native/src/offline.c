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
    {"latitude", "Latitude measures north/south position from the Equator; longitude measures east/west position from the Prime Meridian."},

    {"linear equation", "A linear equation has variables only to the first power and graphs as a straight line. Solve by applying inverse operations equally to both sides."},
    {"exponent", "An exponent tells how many times a base is multiplied by itself. Key rules include a^m*a^n=a^(m+n), (a^m)^n=a^(mn), and a^0=1 for a nonzero."},
    {"logarithm", "A logarithm reverses exponentiation: log_b(x)=y means b^y=x. Log rules turn products into sums, quotients into differences, and powers into coefficients."},
    {"domain", "The domain of a function is the set of allowed input values. Restrictions often come from division by zero, even roots of negatives in real numbers, or context."},
    {"range", "The range of a function is the set of possible output values produced by inputs in the domain."},
    {"vertex", "For a parabola, the vertex is its turning point. In y=a(x-h)^2+k, the vertex is (h,k)."},
    {"parabola", "A parabola is the graph of a quadratic function. Its direction and width depend on the leading coefficient, and its axis of symmetry passes through the vertex."},
    {"circle equation", "A circle centered at (h,k) with radius r has equation (x-h)^2+(y-k)^2=r^2."},
    {"sine", "In a right triangle, sin(theta)=opposite/hypotenuse. On the unit circle, sine is the y-coordinate."},
    {"cosine", "In a right triangle, cos(theta)=adjacent/hypotenuse. On the unit circle, cosine is the x-coordinate."},
    {"tangent", "In a right triangle, tan(theta)=opposite/adjacent, and tan(theta)=sin(theta)/cos(theta) where cosine is nonzero."},
    {"unit circle", "The unit circle has radius 1. A point at angle theta has coordinates (cos theta, sin theta), linking geometry with trigonometric functions."},
    {"probability", "Probability ranges from 0 to 1. For equally likely outcomes, P(event)=favorable outcomes/total outcomes. Complement: P(not A)=1-P(A)."},
    {"permutation", "A permutation counts arrangements where order matters. For n distinct objects taken r at a time: nPr=n!/(n-r)!."},
    {"combination", "A combination counts selections where order does not matter. nCr=n!/[r!(n-r)!]."},
    {"median", "The median is the middle value of ordered data. If there are two middle values, average them."},
    {"variance", "Variance measures average squared distance from the mean. Standard deviation is the square root of variance."},
    {"limit", "A limit describes the value a function approaches as the input approaches a point. A function can have a limit even if its value at that point differs or is undefined."},
    {"chain rule", "Chain rule: d/dx f(g(x)) = f'(g(x))*g'(x). It differentiates compositions of functions."},
    {"product rule", "Product rule: d/dx[f(x)g(x)] = f'(x)g(x)+f(x)g'(x)."},
    {"quotient rule", "Quotient rule: d/dx[f/g] = (f'g-fg')/g^2 where g is nonzero."},
    {"fundamental theorem", "The Fundamental Theorem of Calculus connects differentiation and integration: derivatives recover integrands from accumulation functions, and definite integrals can be evaluated with antiderivatives."},

    {"cell membrane", "The cell membrane is a selectively permeable phospholipid bilayer with proteins that control transport, signaling, and interactions with the environment."},
    {"nucleus", "In eukaryotic cells, the nucleus stores most DNA and is the main site of transcription. A nuclear envelope separates it from the cytoplasm."},
    {"ribosome", "Ribosomes build proteins by translating messenger RNA. They consist of RNA and proteins and can be free in cytoplasm or attached to rough ER."},
    {"protein synthesis", "Protein synthesis includes transcription of DNA into RNA and translation of messenger RNA into an amino-acid chain at a ribosome."},
    {"enzyme", "An enzyme is a biological catalyst that lowers activation energy without being consumed. Activity depends on factors such as temperature, pH, concentration, and inhibitors."},
    {"ecosystem", "An ecosystem includes organisms plus their physical environment and the flows of energy and matter connecting them."},
    {"food web", "A food web shows interconnected feeding relationships. Arrows usually represent energy transfer from the organism being eaten toward the consumer."},
    {"carrying capacity", "Carrying capacity is the population size an environment can sustain over time given resources, competition, disease, and other limiting factors."},
    {"genotype", "Genotype is an organism's genetic makeup for a trait or locus; phenotype is the observable outcome influenced by genotype and environment."},
    {"phenotype", "Phenotype is an observable characteristic resulting from genetic information interacting with environmental factors."},
    {"dominant", "A dominant allele can affect the phenotype when one copy is present in a simple Mendelian model; a recessive phenotype typically requires two recessive copies."},
    {"recessive", "A recessive allele is masked by a dominant allele in a simple heterozygous Mendelian genotype but can appear when no dominant copy is present."},

    {"periodic table", "The periodic table organizes elements by atomic number. Rows are periods; columns are groups whose elements often share valence-electron patterns and chemical properties."},
    {"ionic bond", "An ionic bond is electrostatic attraction between oppositely charged ions, commonly formed after electron transfer between atoms."},
    {"covalent bond", "A covalent bond forms when atoms share electron pairs. Bond polarity depends on differences in electronegativity."},
    {"electronegativity", "Electronegativity describes an atom's tendency to attract shared electrons in a bond. Differences help predict bond polarity."},
    {"chemical equilibrium", "At chemical equilibrium, forward and reverse reaction rates are equal, so macroscopic concentrations remain constant even though reactions continue microscopically."},
    {"le chatelier", "Le Chatelier's principle predicts that an equilibrium system responds to a disturbance in a direction that partially opposes the change."},
    {"oxidation", "Oxidation is loss of electrons and reduction is gain of electrons. In redox reactions, oxidation and reduction always occur together."},
    {"reduction", "Reduction is gain of electrons; oxidation is loss. Track oxidation numbers or electron transfer to identify each process."},
    {"ideal gas", "Ideal gas law: PV=nRT. It relates pressure, volume, amount of gas, and absolute temperature under the ideal-gas model."},
    {"density", "Density is mass per unit volume: rho=m/V. Keep units consistent when solving."},

    {"velocity", "Velocity is the rate of change of position and includes direction. Average velocity is displacement divided by elapsed time."},
    {"acceleration", "Acceleration is the rate of change of velocity. Average acceleration is Delta v / Delta t."},
    {"gravity", "Near Earth's surface, freely falling objects accelerate downward at about 9.8 m/s^2 when air resistance is neglected."},
    {"work", "Mechanical work is energy transferred by a force acting through displacement: W=F d cos(theta)."},
    {"power", "Power is the rate of energy transfer or work: P=W/t. In circuits, P=VI."},
    {"wave", "A wave transfers energy and information through a disturbance. Wave speed satisfies v=f lambda."},
    {"frequency", "Frequency is the number of cycles per second, measured in hertz. It is related to period by f=1/T."},
    {"wavelength", "Wavelength is the spatial length of one wave cycle. Wave speed obeys v=f lambda."},
    {"electric field", "An electric field gives force per unit positive test charge: E=F/q. For a point charge, magnitude is k|Q|/r^2."},
    {"electric current", "Electric current is charge flow rate: I=Delta Q/Delta t. Conventional current direction is the direction positive charge would move."},
    {"voltage", "Voltage is electric potential difference, or energy transferred per unit charge. One volt equals one joule per coulomb."},
    {"resistance", "Resistance measures opposition to electric current. For an ohmic component, R=V/I."},
    {"energy conservation", "Energy can change form or move between systems, but total energy in an isolated system is conserved."},
    {"entropy", "Entropy is a thermodynamic state function associated with energy dispersal and the number of microscopic arrangements. In an isolated system, total entropy does not spontaneously decrease."},

    {"greenhouse effect", "The greenhouse effect occurs when gases such as water vapor, carbon dioxide, and methane absorb and re-emit infrared radiation, warming Earth's lower atmosphere and surface."},
    {"climate change", "Climate change is a long-term shift in climate statistics. Current global warming is primarily linked to increased greenhouse-gas concentrations from human activities, alongside natural variability."},
    {"moon phases", "Moon phases result from seeing different fractions of the Moon's sunlit half as the Moon orbits Earth; they are not caused by Earth's shadow except during a lunar eclipse."},
    {"solar system", "The Solar System includes the Sun, eight planets, dwarf planets, moons, asteroids, comets, and smaller bodies bound mainly by the Sun's gravity."},

    {"simile", "A simile compares unlike things using words such as 'like' or 'as' to create an image or relationship."},
    {"personification", "Personification gives human qualities or actions to nonhuman things, ideas, or forces."},
    {"irony", "Irony involves a contrast between expectation and reality. Common forms include verbal, situational, and dramatic irony."},
    {"tone", "Tone is the author's or speaker's attitude toward a subject, revealed through diction, syntax, imagery, and other choices."},
    {"mood", "Mood is the emotional atmosphere experienced by the audience or reader, created through setting, imagery, diction, pacing, and other techniques."},
    {"symbolism", "Symbolism uses an object, character, setting, color, or action to represent meanings beyond its literal role."},
    {"motif", "A motif is a recurring element, image, phrase, situation, or idea that helps develop a theme."},
    {"alliteration", "Alliteration is repetition of initial consonant sounds in nearby words."},
    {"topic sentence", "A topic sentence states the controlling idea of a paragraph and connects that paragraph to the larger thesis or purpose."},
    {"citation", "A citation identifies the source of information, evidence, or ideas so readers can locate it and distinguish borrowed material from your own work."},
    {"plagiarism", "Plagiarism is presenting another person's words or ideas as your own without proper acknowledgment. Quote, paraphrase accurately, and cite the source."},
    {"rhetoric", "Rhetoric is the strategic use of language and communication to influence understanding or response. Analyze audience, purpose, context, appeals, and choices."},
    {"ethos", "Ethos is a rhetorical appeal based on credibility or character. Pathos appeals to emotion, and logos appeals to reasoning and evidence."},
    {"pathos", "Pathos is a rhetorical appeal to emotion. Effective analysis explains how the emotional appeal supports the speaker's purpose rather than merely naming it."},
    {"logos", "Logos is a rhetorical appeal based on reasoning, evidence, examples, and logical relationships."},
    {"claim evidence reasoning", "Claim-Evidence-Reasoning organizes an explanation: make a claim, support it with relevant evidence, then explain why that evidence supports the claim."},

    {"renaissance", "The Renaissance was a period of major cultural, artistic, intellectual, and economic change in Europe, beginning in Italian city-states and later spreading more broadly."},
    {"reformation", "The Protestant Reformation was a 16th-century movement that challenged aspects of Western Christianity and contributed to new Christian denominations and major political changes."},
    {"enlightenment", "The Enlightenment emphasized reason, natural rights, scientific inquiry, and debate about government and society, strongly influencing modern political thought."},
    {"industrial revolution", "The Industrial Revolution transformed production through mechanization, factories, fossil-fuel energy, transportation, urbanization, and new labor systems."},
    {"imperialism", "Imperialism is the extension of political, economic, or military control or influence by one power over other territories or peoples."},
    {"world war i", "World War I lasted from 1914 to 1918. Its causes included alliance systems, militarism, imperial competition, nationalism, and the crisis following the assassination of Archduke Franz Ferdinand."},
    {"world war ii", "World War II lasted from 1939 to 1945 in Europe and involved global conflict among the Axis and Allied powers, shaped by expansionism, unresolved tensions, and total war."},
    {"cold war", "The Cold War was a prolonged geopolitical rivalry after World War II, especially between the United States and Soviet Union, involving alliances, nuclear competition, proxy conflicts, and ideological competition."},
    {"civil rights movement", "The U.S. Civil Rights Movement challenged racial segregation and discrimination through litigation, organizing, protest, legislation, and other forms of civic action, especially during the 1950s and 1960s."},
    {"constitution", "A constitution establishes fundamental rules and institutions of government. The U.S. Constitution defines federal structures, powers, limits, and amendment procedures."},
    {"checks and balances", "Checks and balances are mechanisms allowing branches of government to limit or review one another's actions, reducing concentration of power."},
    {"judicial review", "Judicial review is the power of courts to evaluate whether laws or government actions are consistent with a constitution or other higher law."},
    {"bill of rights", "The U.S. Bill of Rights is the first ten amendments to the Constitution, protecting specific liberties and placing limits on government power."},
    {"democracy", "Democracy is a system in which political authority ultimately derives from the people, exercised directly or through elected representatives under defined institutions and rules."},
    {"republic", "A republic is a political system without a hereditary monarch in which public authority is exercised through institutions and representatives under law."},

    {"gdp", "Gross domestic product is the market value of final goods and services produced within a country over a period. It measures production, not overall well-being by itself."},
    {"unemployment", "The unemployment rate is the percentage of the labor force that is without a job and actively seeking work under the survey definition being used."},
    {"fiscal policy", "Fiscal policy uses government spending and taxation to influence economic activity, subject to legislative and institutional constraints."},
    {"monetary policy", "Monetary policy is conducted by a central bank using tools that influence interest rates, credit conditions, and the money/financial system to pursue policy goals."},
    {"market economy", "A market economy allocates many resources through decentralized exchange and prices, though real economies usually combine markets with laws, public institutions, and regulation."},

    {"revenue", "Revenue is income generated from selling goods or services before subtracting expenses."},
    {"profit", "Profit equals revenue minus costs. Accounting profit uses explicit recorded costs; economic profit also considers opportunity costs."},
    {"fixed cost", "A fixed cost does not change directly with output over the relevant range, while a variable cost changes as activity or production changes."},
    {"variable cost", "A variable cost changes with the level of production or activity, such as some materials or transaction-based expenses."},
    {"swot", "SWOT analysis organizes Strengths, Weaknesses, Opportunities, and Threats to help structure strategic thinking."},
    {"target market", "A target market is the customer segment a business chooses to serve with a tailored value proposition and marketing strategy."},
    {"segmentation", "Market segmentation divides a broader market into groups with shared needs or characteristics so strategies can be tailored more effectively."},
    {"branding", "Branding shapes how an organization, product, or service is identified and perceived through names, design, experiences, messages, and reputation."},

    {"array", "An array stores elements in indexed positions, typically contiguously. Indexed access is usually O(1), while insertion/removal costs depend on location and implementation."},
    {"stack", "A stack is a last-in, first-out data structure. Core operations are push, pop, and peek/top."},
    {"queue", "A queue is a first-in, first-out data structure. Core operations enqueue items at one end and dequeue them from the other."},
    {"binary tree", "A binary tree is a hierarchical structure where each node has at most two children. Specialized forms include binary search trees and heaps."},
    {"hash table", "A hash table maps keys to array locations using a hash function. Average lookup can be near O(1) with suitable hashing and load management."},
    {"binary search", "Binary search repeatedly halves a sorted search interval. Its time complexity is O(log n)."},
    {"recursion", "Recursion occurs when a function solves a problem by calling itself on smaller instances. A correct recursive solution needs a base case and progress toward it."},
    {"object oriented", "Object-oriented programming organizes software around objects that combine state and behavior. Common ideas include encapsulation, inheritance, interfaces, and polymorphism."},
    {"sql", "SQL is a language for working with relational databases. Common operations include SELECT, INSERT, UPDATE, DELETE, joins, grouping, and constraints."},
    {"database", "A database stores structured information for reliable querying and updates. Relational databases organize data into tables connected by keys and constraints."},
    {"ip address", "An IP address identifies a network interface within Internet Protocol networking. IPv4 uses 32-bit addresses; IPv6 uses 128-bit addresses."},
    {"network", "A computer network connects devices so they can exchange data using agreed protocols. Key ideas include addressing, routing, switching, transport, and application protocols."},
    {"encryption", "Encryption transforms readable data into ciphertext using a key so unauthorized parties cannot easily interpret it. Decryption reverses the process with the appropriate key."},
    {"cybersecurity", "Cybersecurity focuses on protecting systems, networks, and data through prevention, detection, response, access control, updates, backups, and safe user practices."},
    {"machine learning", "Machine learning uses data and optimization to fit models that make predictions or decisions. Performance must be evaluated on suitable data, not just training examples."},
    {"artificial intelligence", "Artificial intelligence is a broad field focused on systems that perform tasks associated with reasoning, perception, language, planning, learning, or decision-making."},

    {"composition", "In visual art, composition is the arrangement of elements within a work to control balance, emphasis, movement, hierarchy, and viewer attention."},
    {"contrast", "Contrast is difference between elements, such as light/dark, large/small, rough/smooth, or complementary colors, used to create emphasis and visual interest."},
    {"balance", "Balance is the distribution of visual weight. It may be symmetrical, asymmetrical, or radial."},
    {"harmony", "In music, harmony concerns how notes sound together and how chords progress over time."},
    {"melody", "Melody is an organized sequence of pitches perceived as a musical line."},
    {"tempo", "Tempo is the speed of a musical pulse, often measured in beats per minute."},
    {"dynamics", "Dynamics describe relative loudness and changes in loudness, using markings such as piano, forte, crescendo, and diminuendo."},
    {"key signature", "A key signature indicates which pitch classes are normally sharp or flat in a piece and helps establish its tonal center."},
    {"time signature", "A time signature shows how beats are grouped in a measure and what note value represents the beat unit in standard notation."},
    {"exposure", "In photography, exposure is controlled mainly by aperture, shutter speed, and sensor sensitivity/ISO. Changing one affects brightness and often motion blur, depth of field, or noise."},

    {"noun", "A noun names a person, place, thing, or idea. Nouns can function as subjects, objects, complements, and more."},
    {"verb", "A verb expresses an action, occurrence, or state. Verb forms can communicate tense, aspect, mood, voice, number, and person."},
    {"adjective", "An adjective modifies a noun or pronoun by describing or specifying it."},
    {"subject verb agreement", "Subject-verb agreement means the verb form matches the grammatical number/person of its subject, with special rules for compound or intervening structures."},
    {"conjugation", "Conjugation changes a verb's form to express features such as tense, person, number, mood, or aspect."},
    {"cognate", "Cognates are words in different languages that share a historical origin and often similar form or meaning; false cognates can look similar but mean different things."},

    {"carbohydrate", "Carbohydrates are a major energy-providing nutrient category that includes sugars, starches, and fiber. Different carbohydrate foods vary greatly in nutrient density and fiber."},
    {"dietary protein", "Dietary protein supplies amino acids used to build and maintain body proteins and other molecules. Needs vary with age, growth, and overall diet."},
    {"dietary fat", "Dietary fats provide energy, essential fatty acids, and help absorb fat-soluble vitamins. Health effects depend partly on fat type and overall dietary pattern."},
    {"vitamin", "Vitamins are organic micronutrients needed in small amounts for normal biological functions. Different vitamins have distinct roles and deficiency effects."},
    {"mineral", "Minerals are inorganic nutrients such as calcium, iron, potassium, and zinc that support structural and regulatory functions in the body."},
    {"calorie", "A food Calorie is a kilocalorie, a unit of energy. Nutrition quality depends on more than calorie count alone."},
    {"aerobic exercise", "Aerobic activity relies mainly on oxygen-supported energy pathways during sustained activity. Examples include walking, cycling, and swimming at appropriate intensity."},
    {"anaerobic exercise", "Anaerobic activity relies more heavily on short-duration energy systems for brief higher-intensity efforts. Training should be age-appropriate and safely supervised."},
    {"sleep", "Sleep supports learning, memory, mood, physical recovery, and health. Regular schedules and adequate duration are important, especially during adolescence."}
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

static bool phrase_match(char const *text, char const *phrase)
{
    size_t n = strlen(phrase);
    char const *p = text;

    while((p = strstr(p, phrase)) != NULL) {
        unsigned char before = (p == text) ? 0 : (unsigned char)p[-1];
        unsigned char after = (unsigned char)p[n];

        if((p == text || !isalnum(before)) &&
           (after == '\0' || !isalnum(after))) {
            return true;
        }

        p++;
    }

    return false;
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

static bool word_prefix(char const *text, char const *word)
{
    size_t n = strlen(word);
    unsigned char next;

    if(strncmp(text, word, n) != 0) return false;
    next = (unsigned char)text[n];
    return next == '\0' || isspace(next) || next == '?' || next == '!' ||
           next == ',' || next == '.' || next == ':';
}

static bool exact_or_punct(char const *text, char const *word)
{
    size_t n = strlen(word);
    if(strncmp(text, word, n) != 0) return false;

    while(text[n] && isspace((unsigned char)text[n])) n++;
    if(text[n] == '?' || text[n] == '!' || text[n] == '.') n++;
    while(text[n] && isspace((unsigned char)text[n])) n++;
    return text[n] == '\0';
}

static void friendly_subject(char const *subject, char *out, size_t out_size)
{
    size_t i = 0;
    bool cap = true;

    if(!subject || !*subject || strcmp(subject, "auto") == 0) {
        snprintf(out, out_size, "General");
        return;
    }

    while(*subject && i + 1 < out_size) {
        char c = *subject++;

        if(c == '_') {
            out[i++] = ' ';
            cap = true;
            continue;
        }

        if(cap && c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        out[i++] = c;
        cap = false;
    }

    out[i] = '\0';
}

static char const *subject_strategy(char const *subject)
{
    if(!subject) return "identify the important terms, what is known, and exactly what the question is asking";

    if(strstr(subject, "algebra") || strstr(subject, "math") ||
       strstr(subject, "geometry") || strstr(subject, "calculus") ||
       strstr(subject, "statistics") || strstr(subject, "trigonometry") ||
       strstr(subject, "precalculus") || strstr(subject, "differential") ||
       strstr(subject, "number_theory") || strstr(subject, "linear_algebra")) {
        return "identify the givens and unknowns, choose the matching rule/equation, solve symbolically when possible, then check the result";
    }

    if(strstr(subject, "physics") || strstr(subject, "chemistry") ||
       strstr(subject, "biology") || strstr(subject, "science") ||
       strstr(subject, "astronomy") || strstr(subject, "environment") ||
       strstr(subject, "anatomy") || strstr(subject, "genetics") ||
       strstr(subject, "microbiology")) {
        return "identify the system, variables, mechanism, evidence, and any units or conservation laws involved";
    }

    if(strstr(subject, "literature") || strstr(subject, "writing") ||
       strstr(subject, "ela") || strstr(subject, "composition") ||
       strstr(subject, "journalism") || strstr(subject, "speech")) {
        return "identify the claim or meaning, then connect specific words/evidence to an explanation";
    }

    if(strstr(subject, "history") || strstr(subject, "government") ||
       strstr(subject, "civics") || strstr(subject, "social") ||
       strstr(subject, "geography") || strstr(subject, "economics") ||
       strstr(subject, "political") || strstr(subject, "sociology") ||
       strstr(subject, "psychology") || strstr(subject, "anthropology")) {
        return "separate factual claims from interpretation, establish context, and trace causes, effects, evidence, or competing explanations";
    }

    if(strstr(subject, "computer") || strstr(subject, "algorithm") ||
       strstr(subject, "database") || strstr(subject, "web_") ||
       strstr(subject, "cyber") || strstr(subject, "engineering") ||
       strstr(subject, "robotics") || strstr(subject, "electronics")) {
        return "define the input, desired output, constraints, and failure cases, then work through the logic step by step";
    }

    if(strstr(subject, "spanish") || strstr(subject, "french") ||
       strstr(subject, "german") || strstr(subject, "latin") ||
       strstr(subject, "language") || strstr(subject, "asl")) {
        return "identify vocabulary, grammar, word order, and context, then translate or explain the meaning rather than matching words blindly";
    }

    if(strstr(subject, "art") || strstr(subject, "music") ||
       strstr(subject, "media") || strstr(subject, "film") ||
       strstr(subject, "photography") || strstr(subject, "theater")) {
        return "identify the technique, formal elements, purpose, context, and evidence visible or audible in the work";
    }

    return "identify the important terms, the relationship between them, and the evidence or rule needed to answer";
}

static void copy_topic(char const *prompt, char *topic, size_t topic_size)
{
    char const *p = prompt;
    size_t len;

    while(*p && isspace((unsigned char)*p)) p++;

    if(starts_with(lowerbuf, "what is ")) p = prompt + 8;
    else if(starts_with(lowerbuf, "what are ")) p = prompt + 9;
    else if(starts_with(lowerbuf, "what's ")) p = prompt + 7;
    else if(starts_with(lowerbuf, "whats ")) p = prompt + 6;
    else if(starts_with(lowerbuf, "what does ") && strstr(lowerbuf, " mean")) p = prompt + 10;
    else if(starts_with(lowerbuf, "define ")) p = prompt + 7;
    else if(starts_with(lowerbuf, "explain ")) p = prompt + 8;
    else if(starts_with(lowerbuf, "tell me about ")) p = prompt + 14;
    else if(starts_with(lowerbuf, "why ")) p = prompt + 4;
    else if(starts_with(lowerbuf, "how ")) p = prompt + 4;
    else if(starts_with(lowerbuf, "who ")) p = prompt + 4;
    else if(starts_with(lowerbuf, "when ")) p = prompt + 5;
    else if(starts_with(lowerbuf, "where ")) p = prompt + 6;
    else if(starts_with(lowerbuf, "compare ")) p = prompt + 8;

    while(*p && isspace((unsigned char)*p)) p++;
    snprintf(topic, topic_size, "%s", p);

    len = strlen(topic);
    while(len > 0 && (isspace((unsigned char)topic[len - 1]) ||
          topic[len - 1] == '?' || topic[len - 1] == '!' ||
          topic[len - 1] == '.')) {
        topic[--len] = '\0';
    }

    if(topic[0] == '\0') snprintf(topic, topic_size, "%s", prompt);
}

static int alpha_word_count(char const *text)
{
    int words = 0;
    bool in_word = false;

    while(*text) {
        bool alpha = isalpha((unsigned char)*text) != 0;

        if(alpha && !in_word) words++;
        in_word = alpha;
        text++;
    }

    return words;
}

static bool looks_like_unknown_token(char const *text)
{
    int letters = 0;
    int vowels = 0;
    int spaces = 0;

    while(*text) {
        char c = (char)tolower((unsigned char)*text);
        if(isalpha((unsigned char)c)) {
            letters++;
            if(c == 'a' || c == 'e' || c == 'i' || c == 'o' ||
               c == 'u' || c == 'y') vowels++;
        }
        else if(isspace((unsigned char)c)) spaces++;
        text++;
    }

    return spaces == 0 && letters >= 7 && vowels == 0;
}

static bool write_conversation(char const *subject, char const *prompt,
                               char *out, size_t out_size)
{
    char subject_name[64];

    friendly_subject(subject, subject_name, sizeof(subject_name));

    if(word_prefix(lowerbuf, "hi") || word_prefix(lowerbuf, "hello") ||
       word_prefix(lowerbuf, "hey") || word_prefix(lowerbuf, "yo") ||
       word_prefix(lowerbuf, "sup") || word_prefix(lowerbuf, "wassup") ||
       word_prefix(lowerbuf, "good morning") || word_prefix(lowerbuf, "good afternoon") ||
       word_prefix(lowerbuf, "good evening")) {
        snprintf(out, out_size,
            "Hey! QBAI Standalone is running entirely on your calculator. "
            "You're currently in %s. Ask me a question, type a calculation, "
            "or switch subjects with F1.", subject_name);
        return true;
    }

    if(strstr(lowerbuf, "how are you")) {
        snprintf(out, out_size,
            "I'm running fine in Standalone mode. No network needed. "
            "You're in %s right now—throw me a question.", subject_name);
        return true;
    }

    if(strstr(lowerbuf, "who are you") || strstr(lowerbuf, "what are you")) {
        snprintf(out, out_size,
            "I'm Quantum Breaks AI Standalone, the calculator-only academic engine. "
            "I work with no internet or other device. In %s mode I can answer stored concepts, "
            "solve supported calculations, and reason about the exact question you type.",
            subject_name);
        return true;
    }

    if(strstr(lowerbuf, "what can you do") || exact_or_punct(lowerbuf, "help") ||
       strstr(lowerbuf, "how do i use you")) {
        snprintf(out, out_size,
            "In %s I can explain stored concepts, answer subject questions, do local math, "
            "and interpret unknown questions instead of returning a canned error. "
            "Try: 'what is slope', 'why does mitosis happen', 'quad 1 -5 6', "
            "'compare mitosis and meiosis', or any sentence you want.",
            subject_name);
        return true;
    }

    if(word_prefix(lowerbuf, "thanks") || word_prefix(lowerbuf, "thank you") ||
       exact_or_punct(lowerbuf, "thx")) {
        snprintf(out, out_size,
            "You're welcome. I'm still in %s Standalone mode, so you can keep asking without connecting anything.",
            subject_name);
        return true;
    }

    if(exact_or_punct(lowerbuf, "bye") || word_prefix(lowerbuf, "goodbye") ||
       word_prefix(lowerbuf, "see you")) {
        snprintf(out, out_size,
            "See you. QBAI Standalone will be here on the calculator whenever you open it again.");
        return true;
    }

    (void)prompt;
    return false;
}

static void write_contextual_fallback(char const *subject, char const *mode,
                                      char const *level, char const *prompt,
                                      char *out, size_t out_size)
{
    char topic[180];
    char subject_name[64];
    char const *strategy;
    char const *ref;
    int words;

    copy_topic(prompt, topic, sizeof(topic));
    friendly_subject(subject, subject_name, sizeof(subject_name));
    strategy = subject_strategy(subject);
    ref = qb_reference_text(subject);
    words = alpha_word_count(prompt);

    if(starts_with(lowerbuf, "what is ") || starts_with(lowerbuf, "what are ") ||
       starts_with(lowerbuf, "what's ") || starts_with(lowerbuf, "whats ") ||
       (starts_with(lowerbuf, "what does ") && strstr(lowerbuf, " mean")) ||
       starts_with(lowerbuf, "define ") || starts_with(lowerbuf, "explain ") ||
       starts_with(lowerbuf, "tell me about ")) {
        snprintf(out, out_size,
            "You're asking for an explanation of \"%s\". I don't have a stored definition for that exact term yet. "
            "In %s mode, I'd analyze it by trying to %s. "
            "Related %s reference: %.180s",
            topic, subject_name, strategy, subject_name, ref);
        return;
    }

    if(starts_with(lowerbuf, "why ")) {
        snprintf(out, out_size,
            "You're asking WHY about \"%s\". I don't have a direct stored explanation for that exact wording, "
            "so in %s mode I'd answer by looking for the cause or mechanism: %s.",
            topic, subject_name, strategy);
        return;
    }

    if(starts_with(lowerbuf, "how ")) {
        snprintf(out, out_size,
            "You're asking HOW \"%s\" works or is done. In %s mode, I don't have a memorized exact answer for that phrase, "
            "but I can still structure it: %s.",
            topic, subject_name, strategy);
        return;
    }

    if(starts_with(lowerbuf, "compare ") || starts_with(lowerbuf, "difference between ") ||
       strstr(lowerbuf, " vs ") || strstr(lowerbuf, " versus ")) {
        snprintf(out, out_size,
            "You want a comparison involving \"%s\". For %s, compare them across the same dimensions, "
            "then state one similarity, one difference, and why that difference matters. "
            "Use this subject rule: %s.",
            topic, subject_name, strategy);
        return;
    }

    if(starts_with(lowerbuf, "who ")) {
        snprintf(out, out_size,
            "You're asking WHO \"%s\" refers to. I don't have a verified stored biography for that exact name, "
            "so I won't invent one. In %s, give me one extra clue (time period, work, class topic, etc.) "
            "and I can narrow down how to analyze it.",
            topic, subject_name);
        return;
    }

    if(starts_with(lowerbuf, "when ")) {
        snprintf(out, out_size,
            "You're asking WHEN about \"%s\". I don't have a stored date tied to that exact phrase, "
            "so I won't guess. In %s, the useful next step is to place it on a timeline and connect the date "
            "to what happened immediately before and after.",
            topic, subject_name);
        return;
    }

    if(starts_with(lowerbuf, "where ")) {
        snprintf(out, out_size,
            "You're asking WHERE \"%s\" is or occurred. I don't have a stored location for that exact phrase, "
            "so I won't make one up. In %s, identify the place, region, and why location matters to the question.",
            topic, subject_name);
        return;
    }

    if(word_prefix(lowerbuf, "is") || word_prefix(lowerbuf, "are") ||
       word_prefix(lowerbuf, "do") || word_prefix(lowerbuf, "does") ||
       word_prefix(lowerbuf, "did") || word_prefix(lowerbuf, "can") ||
       word_prefix(lowerbuf, "could") || word_prefix(lowerbuf, "should")) {
        snprintf(out, out_size,
            "I read \"%s\" as a yes/no or evaluation question in %s. I don't have enough stored facts to honestly choose yes or no, "
            "so I'd test the claim by trying to %s.",
            topic, subject_name, strategy);
        return;
    }

    if(words == 1) {
        if(looks_like_unknown_token(lowerbuf)) {
            snprintf(out, out_size,
                "I don't recognize \"%s\" as a stored word or concept. It may be a name, abbreviation, invented term, or typo. "
                "In %s mode, add a little context and I'll treat that exact term as the topic instead of replacing it with a generic answer.",
                topic, subject_name);
        }
        else {
            snprintf(out, out_size,
                "You entered the single term \"%s\". I don't have a stored definition for it yet. "
                "In %s mode, I can still use it as the topic; try 'define %s', 'explain %s', or 'why %s'.",
                topic, subject_name, topic, topic, topic);
        }
        return;
    }

    snprintf(out, out_size,
        "I read your exact question as: \"%s\"\n"
        "Selected subject: %s | mode: %s | level: %s.\n"
        "I don't have a memorized answer for that exact wording, so I won't fake one. "
        "My standalone approach for this question is to %s. "
        "Relevant %s reference: %.150s",
        topic, subject_name, mode, level, strategy, subject_name, ref);
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

    if(write_conversation(subject, prompt, out, out_size)) {
        return true;
    }

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
        if(phrase_match(lowerbuf, facts[i].needle)) {
            snprintf(out, out_size, "%s", facts[i].answer);
            return true;
        }
    }

    write_contextual_fallback(subject, mode, level, prompt, out, out_size);
    return true;
}
