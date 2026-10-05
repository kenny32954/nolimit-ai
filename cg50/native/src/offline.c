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
    {"sleep", "Sleep supports learning, memory, mood, physical recovery, and health. Regular schedules and adequate duration are important, especially during adolescence."},

    {"osmosis", "Osmosis is the net movement of water across a selectively permeable membrane toward the side with lower free-water concentration, often described as toward higher solute concentration."},
    {"diffusion", "Diffusion is net movement of particles from regions of higher concentration toward lower concentration because of random molecular motion."},
    {"chromosome", "A chromosome is a DNA-protein structure carrying genetic information. Human body cells normally contain homologous chromosome pairs, while gametes contain one set."},
    {"gene expression", "Gene expression is the process by which information in DNA is used to produce functional RNA or protein, regulated at multiple stages."},
    {"ecological succession", "Ecological succession is change in community composition over time. Primary succession begins without established soil; secondary succession follows disturbance where soil remains."},
    {"trophic level", "A trophic level describes an organism's feeding position in an energy-transfer system, such as producer, primary consumer, or higher-level consumer."},

    {"isotope", "Isotopes are atoms of the same element with the same proton count but different neutron counts, giving different mass numbers."},
    {"valence electron", "Valence electrons occupy an atom's outer occupied energy levels and strongly influence bonding and chemical reactivity."},
    {"molar mass", "Molar mass is the mass of one mole of a substance, usually expressed in grams per mole. It connects particle-scale formulas to measurable mass."},
    {"endothermic", "An endothermic process absorbs energy from the surroundings; in thermochemical equations its enthalpy change is positive under the usual sign convention."},
    {"exothermic", "An exothermic process releases energy to the surroundings; its enthalpy change is negative under the usual sign convention."},

    {"projectile motion", "Projectile motion can be analyzed by treating horizontal and vertical motion separately when air resistance is neglected: horizontal velocity is constant while vertical acceleration is approximately -9.8 m/s^2 near Earth."},
    {"centripetal force", "Centripetal force is the net inward force required for circular motion. Its magnitude is F=mv^2/r; it is not a separate new type of force."},
    {"gravitational potential energy", "Near Earth's surface, gravitational potential energy can be approximated as U=mgh relative to a chosen zero height."},

    {"absolute value", "Absolute value gives distance from zero on the number line. |x|=a with a>0 has solutions x=a and x=-a; |x| cannot be negative."},
    {"exponential function", "An exponential function has the variable in the exponent, commonly y=a*b^x. Growth has b>1; decay has 0<b<1."},
    {"rational function", "A rational function is a ratio of polynomials. Domain restrictions occur where the denominator equals zero, and those values can create holes or vertical asymptotes."},
    {"asymptote", "An asymptote is a line or curve that a graph approaches according to a limiting relationship. Common types include vertical, horizontal, and oblique asymptotes."},
    {"inverse function", "An inverse function reverses a one-to-one function: f^-1(f(x))=x on the appropriate domain. Graphs of inverse functions reflect across y=x."},

    {"judicial review", "Judicial review is the power of courts to evaluate whether laws or government actions conflict with a constitution. In the U.S. federal system it became firmly established through Supreme Court practice."},
    {"checks and balances", "Checks and balances are constitutional mechanisms that let branches of government limit or influence one another so power is not concentrated in a single branch."},
    {"rule of law", "Rule of law means public power is exercised through established laws that apply to officials and citizens, with procedures for interpretation and enforcement."},

    {"industrial revolution", "The Industrial Revolution was a long transformation toward mechanized production, factories, new energy sources, urbanization, and major economic and social change beginning in the late eighteenth century."},
    {"enlightenment", "The Enlightenment emphasized reason, inquiry, natural rights, and debate about government and society, influencing political thought and reform movements."},
    {"cold war", "The Cold War was a prolonged geopolitical rivalry centered on the United States and Soviet Union after World War II, involving alliances, nuclear competition, proxy conflicts, ideology, and diplomacy."},
    {"great depression", "The Great Depression was a severe worldwide economic downturn beginning in 1929, marked by major declines in output, banking failures, unemployment, and social hardship."},

    {"scarcity", "Scarcity means resources are limited relative to wants, so individuals and societies must make choices and face opportunity costs."},
    {"comparative advantage", "Comparative advantage exists when a producer has a lower opportunity cost for producing a good or service. It explains potential gains from specialization and trade."},
    {"gross domestic product", "Gross domestic product measures the market value of final goods and services produced within a country's borders during a specified period."},

    {"rhetorical situation", "A rhetorical situation includes the communicator, audience, purpose, message, context, and constraints shaping a piece of communication."},
    {"counterclaim", "A counterclaim is a competing or opposing claim. Strong argument writing represents it fairly and then responds with reasoning and evidence."},
    {"context clue", "A context clue is information around an unfamiliar word or idea that helps infer meaning, such as definitions, examples, contrasts, or cause-effect relationships."},

    {"operating system", "An operating system manages hardware resources and provides services such as process scheduling, memory management, files, device access, security, and user interfaces."},
    {"compiler", "A compiler translates source code into another representation, often machine code or intermediate code, while checking syntax and performing transformations or optimizations."},
    {"boolean", "A Boolean value represents one of two logical states, commonly true or false, and is fundamental to conditions, comparisons, and digital logic."}
};

typedef struct {
    char const *a;
    char const *b;
    char const *answer;
} compare_t;

static compare_t const comparisons[] = {
    {"mitosis", "meiosis", "Mitosis makes two genetically similar cells and usually supports growth/repair; meiosis makes haploid cells for sexual reproduction, uses two divisions, and increases genetic variation."},
    {"ionic bond", "covalent bond", "Ionic bonding involves attraction between oppositely charged ions, commonly after electron transfer; covalent bonding involves atoms sharing electron pairs."},
    {"genotype", "phenotype", "Genotype is genetic information; phenotype is an observable characteristic produced by genotype interacting with environment."},
    {"weather", "climate", "Weather is short-term atmospheric conditions; climate is the long-term statistical pattern of weather in a region."},
    {"speed", "velocity", "Speed is a scalar rate of distance traveled; velocity is a vector rate of displacement and includes direction."},
    {"mass", "weight", "Mass measures amount/inertia and is measured in kilograms; weight is the gravitational force on that mass, W=mg, measured in newtons."},
    {"democracy", "republic", "Democracy describes political authority deriving from the people; a republic is a system without hereditary monarchy in which public power is exercised through institutions and representatives under law. A country can be both."},
    {"supply", "demand", "Supply describes seller willingness/ability at different prices; demand describes buyer willingness/ability. Their interaction helps determine market equilibrium."},
    {"debit", "credit", "Debit and credit are opposite accounting entry directions. Assets normally increase with debits, while liabilities and equity normally increase with credits."},
    {"hardware", "software", "Hardware is the physical computer equipment; software is the instructions and data executed or used by that hardware."},
    {"ram", "storage", "RAM is fast working memory used while programs run and is usually volatile; storage keeps files/programs longer-term and is nonvolatile."},
    {"primary source", "secondary source", "A primary source comes directly from the period, event, participant, or original data; a secondary source analyzes, interprets, or synthesizes primary and other sources."},
    {"mean", "median", "Mean is the arithmetic average; median is the middle ordered value. Mean is more affected by extreme values."},
    {"acid", "base", "In the Bronsted-Lowry model, acids donate H+ and bases accept H+. Their behavior depends on the chemical system and equilibrium."},
    {"series circuit", "parallel circuit", "A series circuit has one main current path; a parallel circuit has multiple branches sharing the same two nodes. Current/voltage relationships differ accordingly."},
    {"prokaryote", "eukaryote", "Prokaryotic cells lack a membrane-bound nucleus; eukaryotic cells have a nucleus and other membrane-bound organelles."}
};

static char lowerbuf[700];

static void lowercase_into(char const *src, char *dst, size_t dst_size)
{
    size_t i;

    if(dst_size == 0) return;

    for(i = 0; i + 1 < dst_size && src[i]; i++) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

static void lowercase(char const *src)
{
    lowercase_into(src, lowerbuf, sizeof(lowerbuf));
}

static bool starts_with(char const *text, char const *prefix)
{
    return strncmp(text, prefix, strlen(prefix)) == 0;
}

static void friendly_subject(char const *subject, char *out, size_t out_size);
static void copy_topic(char const *prompt, char *topic, size_t topic_size);
static int alpha_word_count(char const *text);
static bool eval_expression(char const *text, double *value);
static bool approx_equal(double a, double b);
static int extract_flexible_numbers(char const *text, double *values, int cap);
static void trim_text(char *text);
static fact_t const *suggest_fact_entry(char const *topic);

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


static fact_t const *find_fact_match(char const *text)
{
    unsigned int i;

    for(i = 0; i < sizeof(facts)/sizeof(facts[0]); i++) {
        if(phrase_match(text, facts[i].needle)) return &facts[i];
    }

    return NULL;
}

static bool is_all_upper_word(char const *text)
{
    int letters = 0;

    while(*text) {
        if(isalpha((unsigned char)*text)) {
            letters++;
            if(islower((unsigned char)*text)) return false;
        }
        else if(!isspace((unsigned char)*text) && *text != '?' &&
                *text != '!' && *text != '.') {
            return false;
        }
        text++;
    }

    return letters >= 2 && letters <= 12;
}

static bool looks_like_url(char const *text)
{
    return starts_with(text, "http://") || starts_with(text, "https://") ||
           starts_with(text, "www.") || strstr(text, ".com/") ||
           strstr(text, ".org/") || strstr(text, ".edu/");
}

static bool looks_like_code(char const *text)
{
    return strstr(text, "print(") || strstr(text, "console.log") ||
           strstr(text, "def ") || strstr(text, "function ") ||
           strstr(text, "if(") || strstr(text, "if (") ||
           strstr(text, "for(") || strstr(text, "for (") ||
           strstr(text, "while(") || strstr(text, "while (") ||
           strstr(text, "#include") || strstr(text, "<html") ||
           strstr(text, "public static void") || strstr(text, "=>");
}

static bool has_suffix(char const *word, char const *suffix)
{
    size_t lw = strlen(word), ls = strlen(suffix);
    if(ls > lw) return false;
    return strcmp(word + lw - ls, suffix) == 0;
}


typedef struct {
    char const *short_name;
    char const *expanded;
    char const *note;
} acronym_t;

static acronym_t const acronyms[] = {
    {"dna", "deoxyribonucleic acid", "DNA stores hereditary information."},
    {"rna", "ribonucleic acid", "RNA has roles in gene expression, regulation, and protein synthesis."},
    {"atp", "adenosine triphosphate", "ATP is a major cellular energy-transfer molecule."},
    {"gdp", "gross domestic product", "GDP measures the market value of final goods and services produced within a country over a period."},
    {"cpu", "central processing unit", "The CPU executes instructions and coordinates core computation."},
    {"gpu", "graphics processing unit", "A GPU is specialized for highly parallel computation, originally centered on graphics workloads."},
    {"ram", "random access memory", "RAM is fast working memory used while programs run."},
    {"rom", "read-only memory", "ROM broadly refers to nonvolatile memory intended primarily for reading stored data/firmware."},
    {"html", "hypertext markup language", "HTML gives web documents semantic structure."},
    {"css", "cascading style sheets", "CSS controls web-page presentation and layout."},
    {"url", "uniform resource locator", "A URL identifies the location/address of a resource."},
    {"http", "hypertext transfer protocol", "HTTP is an application-layer protocol used for web communication."},
    {"https", "hypertext transfer protocol secure", "HTTPS is HTTP carried over an encrypted and authenticated TLS connection."},
    {"sql", "structured query language", "SQL is used to define, query, and modify relational database data."},
    {"api", "application programming interface", "An API defines how software components can communicate or expose functionality."},
    {"ai", "artificial intelligence", "AI is the broad field of systems performing tasks associated with reasoning, perception, learning, language, or decision-making."},
    {"ml", "machine learning", "Machine learning fits models from data to make predictions or decisions."},
    {"ip", "internet protocol", "Internet Protocol handles addressing and routing packets across networks."},
    {"tcp", "transmission control protocol", "TCP provides ordered, reliable byte-stream transport over IP."},
    {"udp", "user datagram protocol", "UDP provides connectionless datagram transport with low overhead and no built-in delivery guarantee."},
    {"usb", "universal serial bus", "USB is a standard for connecting, powering, and communicating with peripheral devices."},
    {"led", "light-emitting diode", "An LED is a semiconductor device that emits light when current flows appropriately."},
    {"lcd", "liquid-crystal display", "An LCD uses liquid crystals to control light and form images."},
    {"gps", "global positioning system", "GPS is a satellite-based positioning and timing system."},
    {"isbn", "international standard book number", "An ISBN identifies a specific book edition/product form."},
    {"mla", "modern language association", "MLA is also the name behind a widely used humanities citation/style system."},
    {"apa", "american psychological association", "APA is also the name behind a widely used social-science citation/style system."},
    {"pemdas", "parentheses, exponents, multiplication/division, addition/subtraction", "PEMDAS is a mnemonic for standard arithmetic operation precedence."},
    {"foil", "first, outer, inner, last", "FOIL is a mnemonic for multiplying two binomials."}
};

static bool try_acronym_response(char const *prompt, char *out, size_t out_size)
{
    char lower[160];
    char token[40];
    char const *p;
    size_t len = 0;
    unsigned int i;
    bool asks_expand = false;

    lowercase_into(prompt, lower, sizeof(lower));

    if(looks_like_url(lower) || looks_like_code(lower)) return false;

    if(strstr(lower, "stand for") || starts_with(lower, "expand ") ||
       starts_with(lower, "meaning of ")) {
        asks_expand = true;
    }

    p = lower;
    if(starts_with(p, "what does ")) p += 10;
    else if(starts_with(p, "what is ")) p += 8;
    else if(starts_with(p, "expand ")) p += 7;
    else if(starts_with(p, "meaning of ")) p += 11;

    while(*p && !isalnum((unsigned char)*p)) p++;
    while(*p && isalnum((unsigned char)*p) && len + 1 < sizeof(token)) {
        token[len++] = *p++;
    }
    token[len] = '\0';

    if(!token[0]) return false;

    for(i = 0; i < sizeof(acronyms)/sizeof(acronyms[0]); i++) {
        if(strcmp(token, acronyms[i].short_name) == 0) {
            if(asks_expand || is_all_upper_word(prompt) || strlen(token) <= 5) {
                snprintf(out, out_size,
                    "%s = %s. %s",
                    acronyms[i].short_name, acronyms[i].expanded, acronyms[i].note);
                return true;
            }
        }
    }

    return false;
}

static bool try_numeric_claim(char const *text, char *out, size_t out_size)
{
    char claim[220];
    char *op;
    char opbuf[3] = "";
    char *left;
    char *right;
    double lv, rv;
    bool truth = false;

    snprintf(claim, sizeof(claim), "%s", text);

    if(starts_with(claim, "is ")) memmove(claim, claim + 3, strlen(claim + 3) + 1);
    else if(starts_with(claim, "does ")) memmove(claim, claim + 5, strlen(claim + 5) + 1);

    {
        size_t n = strlen(claim);
        while(n > 0 && (claim[n-1] == '?' || claim[n-1] == '!' ||
              isspace((unsigned char)claim[n-1]))) claim[--n] = '\0';
    }

    op = strstr(claim, ">=");
    if(op) strcpy(opbuf, ">=");
    if(!op) {
        op = strstr(claim, "<=");
        if(op) strcpy(opbuf, "<=");
    }
    if(!op) {
        op = strstr(claim, "==");
        if(op) strcpy(opbuf, "==");
    }
    if(!op) {
        op = strchr(claim, '=');
        if(op) strcpy(opbuf, "=");
    }
    if(!op) {
        op = strchr(claim, '>');
        if(op) strcpy(opbuf, ">");
    }
    if(!op) {
        op = strchr(claim, '<');
        if(op) strcpy(opbuf, "<");
    }

    if(!op || strchr(claim, 'x') || strchr(claim, 'X')) return false;

    *op = '\0';
    left = claim;
    right = op + strlen(opbuf);
    if(opbuf[1]) op[1] = '\0';

    while(*left && isspace((unsigned char)*left)) left++;
    while(*right && isspace((unsigned char)*right)) right++;

    if(!eval_expression(left, &lv) || !eval_expression(right, &rv)) return false;

    if(strcmp(opbuf, "=") == 0 || strcmp(opbuf, "==") == 0) truth = fabs(lv-rv) < 1e-10;
    else if(strcmp(opbuf, ">") == 0) truth = lv > rv;
    else if(strcmp(opbuf, "<") == 0) truth = lv < rv;
    else if(strcmp(opbuf, ">=") == 0) truth = lv >= rv;
    else if(strcmp(opbuf, "<=") == 0) truth = lv <= rv;

    snprintf(out, out_size,
        "%s. Left side = %.12g; right side = %.12g, so %.12g %s %.12g is %s.",
        truth ? "TRUE" : "FALSE", lv, rv, lv, opbuf, rv, truth ? "true" : "false");
    return true;
}


static bool try_text_tools(char const *prompt, char *out, size_t out_size)
{
    char lower[700];
    char text[300];
    char reversed[300];
    char cleaned[300];
    size_t i, len, j;
    int words = 0;
    int letters = 0;
    bool in_word = false;
    char const *p = NULL;

    lowercase_into(prompt, lower, sizeof(lower));

    if(starts_with(lower, "reverse ")) {
        p = prompt + 8;
        snprintf(text, sizeof(text), "%.299s", p);
        len = strlen(text);
        for(i = 0; i < len && i + 1 < sizeof(reversed); i++) {
            reversed[i] = text[len - 1 - i];
        }
        reversed[i] = '\0';
        snprintf(out, out_size, "Reversed: %s", reversed);
        return true;
    }

    if(starts_with(lower, "count letters in ") ||
       starts_with(lower, "how many letters in ")) {
        p = starts_with(lower, "count letters in ") ? prompt + 17 : prompt + 20;
        while(*p) {
            if(isalpha((unsigned char)*p)) letters++;
            p++;
        }
        snprintf(out, out_size, "Letter count = %d.", letters);
        return true;
    }

    if(starts_with(lower, "count words in ") ||
       starts_with(lower, "how many words in ")) {
        p = starts_with(lower, "count words in ") ? prompt + 15 : prompt + 18;
        while(*p) {
            bool isword = isalnum((unsigned char)*p) != 0;
            if(isword && !in_word) words++;
            in_word = isword;
            p++;
        }
        snprintf(out, out_size, "Word count = %d.", words);
        return true;
    }

    if(starts_with(lower, "is ") && strstr(lower, " a palindrome")) {
        char *marker;
        snprintf(text, sizeof(text), "%.299s", prompt + 3);
        marker = strstr(text, " a palindrome");
        if(!marker) marker = strstr(text, " A palindrome");
        if(marker) *marker = '\0';

        j = 0;
        for(i = 0; text[i] && j + 1 < sizeof(cleaned); i++) {
            if(isalnum((unsigned char)text[i])) {
                cleaned[j++] = (char)tolower((unsigned char)text[i]);
            }
        }
        cleaned[j] = '\0';

        if(j == 0) return false;

        for(i = 0; i < j / 2; i++) {
            if(cleaned[i] != cleaned[j - 1 - i]) {
                snprintf(out, out_size, "No. \"%s\" is not a palindrome.", text);
                return true;
            }
        }

        snprintf(out, out_size, "Yes. \"%s\" is a palindrome.", text);
        return true;
    }

    if(starts_with(lower, "binary of ") || starts_with(lower, "to binary ")) {
        char *end;
        long value;
        char bits[80];
        int pos = 0;
        unsigned long u;

        p = starts_with(lower, "binary of ") ? prompt + 10 : prompt + 10;
        value = strtol(p, &end, 10);
        if(end == p || value < 0) return false;
        u = (unsigned long)value;

        if(u == 0) {
            snprintf(out, out_size, "%ld in binary = 0", value);
            return true;
        }

        while(u && pos < (int)sizeof(bits) - 1) {
            bits[pos++] = (char)('0' + (u & 1UL));
            u >>= 1;
        }
        for(i = 0; i < (size_t)pos / 2; i++) {
            char c = bits[i];
            bits[i] = bits[pos - 1 - (int)i];
            bits[pos - 1 - (int)i] = c;
        }
        bits[pos] = '\0';

        snprintf(out, out_size, "%ld in binary = %s", value, bits);
        return true;
    }

    if(starts_with(lower, "hex of ") || starts_with(lower, "to hex ")) {
        char *end;
        long value;
        p = starts_with(lower, "hex of ") ? prompt + 7 : prompt + 7;
        value = strtol(p, &end, 10);
        if(end == p) return false;
        snprintf(out, out_size, "%ld in hexadecimal = 0x%lX", value, (unsigned long)value);
        return true;
    }

    return false;
}


static bool is_prime_long(long n)
{
    long d;

    if(n < 2) return false;
    if(n == 2) return true;
    if((n % 2) == 0) return false;

    for(d = 3; d <= n / d; d += 2) {
        if((n % d) == 0) return false;
    }

    return true;
}

static bool try_literal_analysis(char const *prompt, char *out, size_t out_size)
{
    char text[180];
    char *p = text;
    char *end;
    size_t len;
    double dv;

    snprintf(text, sizeof(text), "%.179s", prompt);
    while(*p && isspace((unsigned char)*p)) p++;

    {
        char lower[180];
        lowercase_into(p, lower, sizeof(lower));
        if(looks_like_url(lower) || looks_like_code(lower)) return false;
    }

    len = strlen(p);
    while(len > 0 && isspace((unsigned char)p[len - 1])) p[--len] = '\0';

    if(len == 1 && isalpha((unsigned char)p[0])) {
        char c = (char)toupper((unsigned char)p[0]);
        int index = c - 'A' + 1;
        bool vowel = strchr("AEIOU", c) != NULL;

        snprintf(out, out_size,
            "%c is letter %d of the English alphabet and is a %s.",
            c, index, vowel ? "vowel" : "consonant");
        return true;
    }

    if(len > 3 && p[0] == '0' && (p[1] == 'b' || p[1] == 'B')) {
        unsigned long value = 0;
        size_t i;

        for(i = 2; i < len; i++) {
            if(p[i] != '0' && p[i] != '1') return false;
            value = (value << 1) | (unsigned long)(p[i] - '0');
        }

        snprintf(out, out_size, "%s in decimal = %lu.", p, value);
        return true;
    }

    if(len > 3 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        unsigned long value = strtoul(p + 2, &end, 16);
        if(end == p + 2 || *end != '\0') return false;

        snprintf(out, out_size, "%s in decimal = %lu.", p, value);
        return true;
    }

    dv = strtod(p, &end);
    if(end != p && *end == '\0') {
        double rounded = floor(dv);
        if(fabs(dv - rounded) < 1e-10 &&
           rounded >= -2147483647.0 && rounded <= 2147483647.0) {
            long n = (long)rounded;
            bool even = (n % 2L) == 0L;
            bool prime = is_prime_long(n);
            long absn = n < 0 ? -n : n;
            long root = (long)(sqrt((double)absn) + 0.5);
            bool square = absn >= 0 && root * root == absn;

            snprintf(out, out_size,
                "%ld is %s, %s, %s%s.",
                n,
                n > 0 ? "positive" : (n < 0 ? "negative" : "zero"),
                even ? "even" : "odd",
                prime ? "prime" : "not prime",
                square ? ", and a perfect square" : "");
        }
        else {
            snprintf(out, out_size,
                "%.12g is a real number; it is %s.",
                dv, dv > 0.0 ? "positive" : (dv < 0.0 ? "negative" : "zero"));
        }
        return true;
    }

    if(strchr(p, '@') && strchr(p, '.') && !strchr(p, ' ')) {
        snprintf(out, out_size,
            "\"%.120s\" looks like an email address. Standalone QBAI cannot send mail, "
            "but it can recognize the address shape and reason about text you paste from a message.",
            p);
        return true;
    }

    {
        char const *dot = strrchr(p, '.');
        if(dot && dot != p && !strchr(p, ' ') && strlen(dot) <= 12) {
            snprintf(out, out_size,
                "\"%.120s\" looks like a filename. Extension: %s.",
                p, dot);
            return true;
        }
    }

    return false;
}


typedef struct {
    char const *symbol;
    char const *name;
} element_t;

static element_t const elements[] = {
    {"H","hydrogen"},{"He","helium"},{"Li","lithium"},{"Be","beryllium"},
    {"B","boron"},{"C","carbon"},{"N","nitrogen"},{"O","oxygen"},
    {"F","fluorine"},{"Ne","neon"},{"Na","sodium"},{"Mg","magnesium"},
    {"Al","aluminum"},{"Si","silicon"},{"P","phosphorus"},{"S","sulfur"},
    {"Cl","chlorine"},{"Ar","argon"},{"K","potassium"},{"Ca","calcium"},
    {"Sc","scandium"},{"Ti","titanium"},{"V","vanadium"},{"Cr","chromium"},
    {"Mn","manganese"},{"Fe","iron"},{"Co","cobalt"},{"Ni","nickel"},
    {"Cu","copper"},{"Zn","zinc"},{"Ga","gallium"},{"Ge","germanium"},
    {"As","arsenic"},{"Se","selenium"},{"Br","bromine"},{"Kr","krypton"},
    {"Rb","rubidium"},{"Sr","strontium"},{"Y","yttrium"},{"Zr","zirconium"},
    {"Nb","niobium"},{"Mo","molybdenum"},{"Tc","technetium"},{"Ru","ruthenium"},
    {"Rh","rhodium"},{"Pd","palladium"},{"Ag","silver"},{"Cd","cadmium"},
    {"In","indium"},{"Sn","tin"},{"Sb","antimony"},{"Te","tellurium"},
    {"I","iodine"},{"Xe","xenon"},{"Cs","cesium"},{"Ba","barium"},
    {"La","lanthanum"},{"Ce","cerium"},{"Pr","praseodymium"},{"Nd","neodymium"},
    {"Pm","promethium"},{"Sm","samarium"},{"Eu","europium"},{"Gd","gadolinium"},
    {"Tb","terbium"},{"Dy","dysprosium"},{"Ho","holmium"},{"Er","erbium"},
    {"Tm","thulium"},{"Yb","ytterbium"},{"Lu","lutetium"},{"Hf","hafnium"},
    {"Ta","tantalum"},{"W","tungsten"},{"Re","rhenium"},{"Os","osmium"},
    {"Ir","iridium"},{"Pt","platinum"},{"Au","gold"},{"Hg","mercury"},
    {"Tl","thallium"},{"Pb","lead"},{"Bi","bismuth"},{"Po","polonium"},
    {"At","astatine"},{"Rn","radon"},{"Fr","francium"},{"Ra","radium"},
    {"Ac","actinium"},{"Th","thorium"},{"Pa","protactinium"},{"U","uranium"},
    {"Np","neptunium"},{"Pu","plutonium"},{"Am","americium"},{"Cm","curium"},
    {"Bk","berkelium"},{"Cf","californium"},{"Es","einsteinium"},{"Fm","fermium"},
    {"Md","mendelevium"},{"No","nobelium"},{"Lr","lawrencium"},{"Rf","rutherfordium"},
    {"Db","dubnium"},{"Sg","seaborgium"},{"Bh","bohrium"},{"Hs","hassium"},
    {"Mt","meitnerium"},{"Ds","darmstadtium"},{"Rg","roentgenium"},{"Cn","copernicium"},
    {"Nh","nihonium"},{"Fl","flerovium"},{"Mc","moscovium"},{"Lv","livermorium"},
    {"Ts","tennessine"},{"Og","oganesson"}
};

static bool same_ci(char const *a, char const *b)
{
    while(*a && *b) {
        if(tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
        a++; b++;
    }
    return *a=='\0' && *b=='\0';
}

static element_t const *element_by_symbol(char const *symbol, int *atomic_number)
{
    unsigned int i;
    for(i=0;i<sizeof(elements)/sizeof(elements[0]);i++) {
        if(same_ci(symbol,elements[i].symbol)) {
            if(atomic_number) *atomic_number=(int)i+1;
            return &elements[i];
        }
    }
    return NULL;
}

static element_t const *element_by_name(char const *name, int *atomic_number)
{
    unsigned int i;
    for(i=0;i<sizeof(elements)/sizeof(elements[0]);i++) {
        if(same_ci(name,elements[i].name)) {
            if(atomic_number) *atomic_number=(int)i+1;
            return &elements[i];
        }
    }
    return NULL;
}

static bool try_periodic_table(char const *subject, char const *prompt,
                               char *out, size_t out_size)
{
    char raw[120];
    char lower[120];
    char token[48];
    char const *p;
    element_t const *e=NULL;
    int number=0;
    long requested=0;
    char *end;

    snprintf(raw,sizeof(raw),"%.119s",prompt);
    trim_text(raw);
    lowercase_into(raw,lower,sizeof(lower));

    if(starts_with(lower,"atomic number ")) {
        requested=strtol(lower+14,&end,10);
        if(end!=lower+14 && requested>=1 && requested<=118) {
            e=&elements[requested-1];
            snprintf(out,out_size,
                "Atomic number %ld = %s (%s).",requested,e->name,e->symbol);
            return true;
        }
    }

    if(starts_with(lower,"element ")) p=raw+8;
    else if(starts_with(lower,"symbol for ")) {
        p=raw+11;
        while(*p && isspace((unsigned char)*p)) p++;
        e=element_by_name(p,&number);
        if(e) {
            snprintf(out,out_size,"%s has symbol %s and atomic number %d.",e->name,e->symbol,number);
            return true;
        }
        return false;
    }
    else if(starts_with(lower,"what element is ")) p=raw+16;
    else if(starts_with(lower,"what is element ")) p=raw+16;
    else if(starts_with(lower,"what is ")) p=raw+8;
    else p=raw;

    while(*p && isspace((unsigned char)*p)) p++;
    snprintf(token,sizeof(token),"%.47s",p);
    trim_text(token);

    if(strchr(token,' ')) return false;

    e=element_by_symbol(token,&number);
    if(!e) e=element_by_name(token,&number);

    if(e) {
        bool chemistry_context = subject && (
            strcmp(subject,"chemistry")==0 ||
            strcmp(subject,"organic_chemistry")==0 ||
            strcmp(subject,"biochemistry")==0 ||
            strcmp(subject,"physical_science")==0
        );

        if(strlen(token)==1 && !chemistry_context &&
           !starts_with(lower,"element ") &&
           !starts_with(lower,"what element is ") &&
           !starts_with(lower,"what is element ")) {
            return false;
        }

        snprintf(out,out_size,
            "%s (%s) is element %d on the periodic table.",
            e->name,e->symbol,number);
        return true;
    }

    return false;
}

typedef struct {
    char const *part;
    char const *meaning;
} wordpart_t;

static wordpart_t const wordparts[] = {
    {"bio","life"},{"geo","earth"},{"hydro","water"},{"thermo","heat"},
    {"photo","light"},{"chrono","time"},{"micro","small"},{"macro","large"},
    {"mono","one"},{"bi","two"},{"tri","three"},{"poly","many"},
    {"auto","self"},{"anti","against/opposed"},{"pre","before"},{"post","after"},
    {"inter","between"},{"intra","within"},{"trans","across"},{"sub","under/below"},
    {"super","above/beyond"},{"hyper","over/excessive"},{"hypo","under/below"},
    {"neo","new"},{"paleo","ancient"},{"aero","air"},{"astro","star/space"},
    {"cardio","heart"},{"neuro","nerve/nervous system"},{"derm","skin"},
    {"hemo","blood"},{"cyto","cell"},{"zoo","animal"},{"anthrop","human"},
    {"psych","mind"},{"socio","society"},{"eco","environment/home"},
    {"phon","sound"},{"graph","write/record"},{"meter","measure"},
    {"scope","view/examine"},{"logy","study/field"},{"phobia","fear/aversion"},
    {"phile","liking/affinity"},{"itis","inflammation"},{"osis","condition/process"}
};

static bool try_word_parts(char const *subject, char const *prompt, char *out, size_t out_size)
{
    char topic[120];
    char lower[120];
    char subject_name[64];
    unsigned int i;
    size_t used=0;
    int hits=0;

    copy_topic(prompt,topic,sizeof(topic));
    lowercase_into(topic,lower,sizeof(lower));
    friendly_subject(subject,subject_name,sizeof(subject_name));

    if(suggest_fact_entry(topic)) return false;
    if(strchr(lower,' ') || strlen(lower)<5) return false;

    for(i=0;i<sizeof(wordparts)/sizeof(wordparts[0]);i++) {
        if(strstr(lower,wordparts[i].part)) {
            if(hits==0) {
                used+=(size_t)snprintf(out+used,out_size-used,
                    "In %s, I don't have a verified exact dictionary entry for \"%s\", but its word parts suggest: ",
                    subject_name,topic);
            }
            used+=(size_t)snprintf(out+used,out_size-used,
                "%s%s = %s",hits?"; ":"",wordparts[i].part,wordparts[i].meaning);
            hits++;
            if(hits>=4 || used+60>=out_size) break;
        }
    }

    if(hits) {
        snprintf(out+strlen(out),out_size-strlen(out),
            ". Word-root analysis is only a clue, not a guaranteed definition.");
        return true;
    }

    return false;
}


static bool roman_to_int(char const *text, int *value)
{
    int total=0, prev=0;
    size_t len=strlen(text);
    int i;

    if(len==0 || len>20) return false;

    for(i=(int)len-1;i>=0;i--) {
        int v;
        switch(toupper((unsigned char)text[i])) {
            case 'I': v=1; break;
            case 'V': v=5; break;
            case 'X': v=10; break;
            case 'L': v=50; break;
            case 'C': v=100; break;
            case 'D': v=500; break;
            case 'M': v=1000; break;
            default: return false;
        }
        if(v<prev) total-=v;
        else { total+=v; prev=v; }
    }

    if(total<=0 || total>3999) return false;
    *value=total;
    return true;
}

static bool int_to_roman(int value, char *out, size_t out_size)
{
    static int const vals[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};
    static char const *syms[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    size_t used=0;
    int i;

    if(value<1 || value>3999 || out_size==0) return false;
    out[0]='\0';

    for(i=0;i<13;i++) {
        while(value>=vals[i]) {
            size_t n=strlen(syms[i]);
            if(used+n+1>=out_size) return false;
            memcpy(out+used,syms[i],n);
            used+=n;
            out[used]='\0';
            value-=vals[i];
        }
    }
    return true;
}

static bool try_misc_text_math(char const *prompt, char *out, size_t out_size)
{
    char lower[500];

    lowercase_into(prompt,lower,sizeof(lower));

    if(starts_with(lower,"roman ") || starts_with(lower,"roman numeral ")) {
        char const *p=starts_with(lower,"roman numeral ")?prompt+14:prompt+6;
        char *end;
        long n=strtol(p,&end,10);
        char roman[64];
        if(end!=p && int_to_roman((int)n,roman,sizeof(roman))) {
            snprintf(out,out_size,"%ld in Roman numerals = %s.",n,roman);
            return true;
        }
    }

    if(strstr(lower," roman to decimal") || starts_with(lower,"roman to decimal ")) {
        char token[32];
        char const *p=starts_with(lower,"roman to decimal ")?prompt+17:prompt;
        int value;
        size_t i=0;
        while(*p && !isalpha((unsigned char)*p)) p++;
        while(*p && isalpha((unsigned char)*p) && i+1<sizeof(token)) token[i++]=*p++;
        token[i]='\0';
        if(roman_to_int(token,&value)) {
            snprintf(out,out_size,"%s = %d.",token,value);
            return true;
        }
    }

    if(starts_with(lower,"rot13 ")) {
        char text[420];
        size_t i;
        snprintf(text,sizeof(text),"%.419s",prompt+6);
        for(i=0;text[i];i++) {
            char c=text[i];
            if(c>='a'&&c<='z') text[i]=(char)('a'+(c-'a'+13)%26);
            else if(c>='A'&&c<='Z') text[i]=(char)('A'+(c-'A'+13)%26);
        }
        snprintf(out,out_size,"%s",text);
        return true;
    }

    if(starts_with(lower,"caesar ")) {
        char *end;
        long shift=strtol(prompt+7,&end,10);
        char text[420];
        size_t i;
        int sh=(int)(shift%26);
        while(*end && isspace((unsigned char)*end)) end++;
        if(end==prompt+7 || !*end) return false;
        if(sh<0) sh+=26;
        snprintf(text,sizeof(text),"%.419s",end);
        for(i=0;text[i];i++) {
            char c=text[i];
            if(c>='a'&&c<='z') text[i]=(char)('a'+(c-'a'+sh)%26);
            else if(c>='A'&&c<='Z') text[i]=(char)('A'+(c-'A'+sh)%26);
        }
        snprintf(out,out_size,"%s",text);
        return true;
    }

    if(starts_with(lower,"is ") && strstr(lower," an anagram of ")) {
        char work[500], *sep, *a, *b;
        int ca[26]={0}, cb[26]={0};
        int i;
        snprintf(work,sizeof(work),"%.499s",lower+3);
        sep=strstr(work," an anagram of ");
        if(!sep) return false;
        *sep='\0';
        a=work;
        b=sep+15;
        trim_text(a);
        trim_text(b);
        for(;*a;a++) if(isalpha((unsigned char)*a)) ca[*a-'a']++;
        for(;*b;b++) if(isalpha((unsigned char)*b)) cb[*b-'a']++;
        for(i=0;i<26;i++) if(ca[i]!=cb[i]) {
            snprintf(out,out_size,"No, those are not anagrams when spaces/punctuation are ignored.");
            return true;
        }
        snprintf(out,out_size,"Yes, those are anagrams when spaces/punctuation are ignored.");
        return true;
    }

    return false;
}


static bool leap_year_int(int year)
{
    return (year%4==0 && year%100!=0) || (year%400==0);
}

static int days_in_month_int(int year, int month)
{
    static int const d[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(month<1 || month>12) return 0;
    if(month==2 && leap_year_int(year)) return 29;
    return d[month-1];
}

static bool valid_date_int(int y,int m,int d)
{
    int maxd=days_in_month_int(y,m);
    return y>=1 && m>=1 && m<=12 && d>=1 && d<=maxd;
}

static long days_from_civil_int(int y, unsigned m, unsigned d)
{
    int era;
    unsigned yoe,doy,doe;
    y -= m <= 2;
    era = (y >= 0 ? y : y-399) / 400;
    yoe = (unsigned)(y - era*400);
    doy = (153*(m > 2 ? m-3 : m+9) + 2)/5 + d-1;
    doe = yoe*365 + yoe/4 - yoe/100 + doy;
    return (long)era*146097L + (long)doe - 719468L;
}

static int weekday_index_int(int y,int m,int d)
{
    long z=days_from_civil_int(y,(unsigned)m,(unsigned)d);
    int idx=(int)((z+4)%7);
    if(idx<0) idx+=7;
    return idx;
}

static bool try_date_tools(char const *prompt, char *out, size_t out_size)
{
    char lower[300];
    int y,m,d,y2,m2,d2;
    static char const *wd[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

    lowercase_into(prompt,lower,sizeof(lower));

    if((starts_with(lower,"leap year ") && sscanf(lower,"leap year %d",&y)==1) ||
       (starts_with(lower,"is ") && strstr(lower," a leap year") &&
        sscanf(lower,"is %d a leap year",&y)==1)) {
        snprintf(out,out_size,"%d %s a leap year.",y,leap_year_int(y)?"is":"is not");
        return true;
    }

    if(sscanf(lower,"day of week %d-%d-%d",&y,&m,&d)==3 ||
       sscanf(lower,"weekday %d-%d-%d",&y,&m,&d)==3) {
        if(!valid_date_int(y,m,d)) snprintf(out,out_size,"That is not a valid calendar date.");
        else snprintf(out,out_size,"%04d-%02d-%02d is %s.",y,m,d,wd[weekday_index_int(y,m,d)]);
        return true;
    }

    if(sscanf(lower,"days in february %d",&y)==1) {
        snprintf(out,out_size,"February %d has %d days.",y,leap_year_int(y)?29:28);
        return true;
    }

    if(sscanf(lower,"days between %d-%d-%d and %d-%d-%d",&y,&m,&d,&y2,&m2,&d2)==6) {
        long a,b,diff;
        if(!valid_date_int(y,m,d) || !valid_date_int(y2,m2,d2)) {
            snprintf(out,out_size,"One of those dates is invalid.");
            return true;
        }
        a=days_from_civil_int(y,(unsigned)m,(unsigned)d);
        b=days_from_civil_int(y2,(unsigned)m2,(unsigned)d2);
        diff=b-a;
        if(diff<0) diff=-diff;
        snprintf(out,out_size,"Days between the dates = %ld.",diff);
        return true;
    }

    return false;
}

typedef struct {
    char ch;
    char const *code;
    char const *nato;
} letter_code_t;

static letter_code_t const letter_codes[] = {
    {'A',".-","Alpha"},{'B',"-...","Bravo"},{'C',"-.-.","Charlie"},{'D',"-..","Delta"},
    {'E',".","Echo"},{'F',"..-.","Foxtrot"},{'G',"--.","Golf"},{'H',"....","Hotel"},
    {'I',"..","India"},{'J',".---","Juliett"},{'K',"-.-","Kilo"},{'L',".-..","Lima"},
    {'M',"--","Mike"},{'N',"-.","November"},{'O',"---","Oscar"},{'P',".--.","Papa"},
    {'Q',"--.-","Quebec"},{'R',".-.","Romeo"},{'S',"...","Sierra"},{'T',"-","Tango"},
    {'U',"..-","Uniform"},{'V',"...-","Victor"},{'W',".--","Whiskey"},{'X',"-..-","X-ray"},
    {'Y',"-.--","Yankee"},{'Z',"--..","Zulu"},
    {'0',"-----","Zero"},{'1',".----","One"},{'2',"..---","Two"},{'3',"...--","Three"},
    {'4',"....-","Four"},{'5',".....","Five"},{'6',"-....","Six"},{'7',"--...","Seven"},
    {'8',"---..","Eight"},{'9',"----.","Nine"}
};

static letter_code_t const *code_for_char(char c)
{
    unsigned int i;
    c=(char)toupper((unsigned char)c);
    for(i=0;i<sizeof(letter_codes)/sizeof(letter_codes[0]);i++)
        if(letter_codes[i].ch==c) return &letter_codes[i];
    return NULL;
}

static char char_for_morse(char const *code)
{
    unsigned int i;
    for(i=0;i<sizeof(letter_codes)/sizeof(letter_codes[0]);i++)
        if(strcmp(letter_codes[i].code,code)==0) return letter_codes[i].ch;
    return '?';
}

static bool try_encoding_tools(char const *prompt, char *out, size_t out_size)
{
    char lower[500];

    lowercase_into(prompt,lower,sizeof(lower));

    if(starts_with(lower,"morse ")) {
        char const *p=prompt+6;
        bool first=true;
        out[0]='\0';
        while(*p) {
            size_t used=strlen(out);
            if(*p==' ') {
                snprintf(out+used,out_size-used," / ");
                first=true;
                p++;
                continue;
            }
            {
                letter_code_t const *e=code_for_char(*p++);
                if(e) {
                    used=strlen(out);
                    snprintf(out+used,out_size-used,"%s%s",first?"":" ",e->code);
                    first=false;
                }
            }
            if(strlen(out)+8>=out_size) break;
        }
        if(out[0]) return true;
    }

    if(starts_with(lower,"decode morse ")) {
        char work[500];
        char *tok;
        snprintf(work,sizeof(work),"%.499s",prompt+13);
        out[0]='\0';
        tok=strtok(work," ");
        while(tok) {
            size_t used=strlen(out);
            if(strcmp(tok,"/")==0) snprintf(out+used,out_size-used," ");
            else {
                char c=char_for_morse(tok);
                snprintf(out+used,out_size-used,"%c",c);
            }
            tok=strtok(NULL," ");
            if(strlen(out)+2>=out_size) break;
        }
        return true;
    }

    if(starts_with(lower,"nato ")) {
        char const *p=prompt+5;
        bool first=true;
        out[0]='\0';
        while(*p) {
            if(*p==' ') {
                size_t used=strlen(out);
                snprintf(out+used,out_size-used," / ");
                first=true;
                p++;
                continue;
            }
            {
                letter_code_t const *e=code_for_char(*p++);
                if(e) {
                    size_t used=strlen(out);
                    snprintf(out+used,out_size-used,"%s%s",first?"":" ",e->nato);
                    first=false;
                }
            }
            if(strlen(out)+16>=out_size) break;
        }
        if(out[0]) return true;
    }

    return false;
}

static bool write_input_shape_response(char const *subject, char const *prompt,
                                       char *out, size_t out_size)
{
    char lower[220];
    char subject_name[64];

    lowercase_into(prompt, lower, sizeof(lower));
    friendly_subject(subject, subject_name, sizeof(subject_name));

    if(looks_like_url(lower)) {
        snprintf(out, out_size,
            "That looks like a URL: \"%.150s\". Standalone QBAI has no internet connection, "
            "so I can't open it, but I can still reason about any text, domain name, path, or clue you type from it.",
            prompt);
        return true;
    }

    if(looks_like_code(lower)) {
        snprintf(out, out_size,
            "That looks like source code or a code fragment. In %s mode, I can inspect it locally for structure, "
            "variables, control flow, likely syntax, and obvious logic patterns. Exact input: \"%.170s\".",
            subject_name, prompt);
        return true;
    }

    if(is_all_upper_word(prompt)) {
        snprintf(out, out_size,
            "\"%.40s\" looks like an acronym or initialism. I don't have a verified expansion stored for that exact token, "
            "so I won't invent one. Add the class/topic or expand one letter and I'll narrow it down.",
            prompt);
        return true;
    }

    if(alpha_word_count(prompt) == 1) {
        if(has_suffix(lower, "ology")) {
            snprintf(out, out_size,
                "\"%s\" looks like a field-of-study word because it ends in -ology. "
                "I don't have its exact stored definition, so add context and I'll analyze the roots/topic.",
                lower);
            return true;
        }
        if(has_suffix(lower, "ism")) {
            snprintf(out, out_size,
                "\"%s\" ends in -ism, a suffix often used for systems, doctrines, practices, or conditions. "
                "I don't have enough stored context to assign the exact meaning safely.",
                lower);
            return true;
        }
        if(has_suffix(lower, "tion") || has_suffix(lower, "ment")) {
            snprintf(out, out_size,
                "\"%s\" has a noun-like ending. I don't have an exact dictionary entry stored, "
                "but I can use the word as the subject of a definition, cause/effect, or comparison question.",
                lower);
            return true;
        }
    }

    return false;
}

static char quiz_answer[700];
static char quiz_topic[120];
static bool quiz_pending = false;

static bool try_study_request(char const *subject, char const *prompt,
                              char *out, size_t out_size)
{
    char topic[180];
    char topic_lower[180];
    fact_t const *fact;
    char const *ref;

    copy_topic(prompt, topic, sizeof(topic));
    lowercase_into(topic, topic_lower, sizeof(topic_lower));
    fact = find_fact_match(topic_lower);
    ref = qb_reference_text(subject);

    if(starts_with(lowerbuf, "summarize ") || starts_with(lowerbuf, "summary of ")) {
        if(fact) {
            snprintf(out, out_size, "Summary - %s: %s", fact->needle, fact->answer);
        }
        else {
            snprintf(out, out_size,
                "Summary target: \"%s\". I don't have an exact stored entry, so here is the closest subject-level reference: %.300s",
                topic, ref);
        }
        return true;
    }

    if(starts_with(lowerbuf, "flashcards on ") || starts_with(lowerbuf, "flashcards about ")) {
        if(fact) {
            snprintf(out, out_size,
                "FLASHCARD\nQ: What is %s?\nA: %s\n\nQ: What key idea should you remember?\nA: Connect the definition to one example or application.",
                fact->needle, fact->answer);
        }
        else {
            snprintf(out, out_size,
                "FLASHCARD\nQ: Explain %s.\nA: Use the %s reference and identify definition, key mechanism/rule, and one example.\nReference: %.220s",
                topic, subject, ref);
        }
        return true;
    }

    if(starts_with(lowerbuf, "quiz me on ") || starts_with(lowerbuf, "quiz me about ")) {
        snprintf(quiz_topic, sizeof(quiz_topic), "%.119s", topic);
        if(fact) snprintf(quiz_answer, sizeof(quiz_answer), "%.699s", fact->answer);
        else snprintf(quiz_answer, sizeof(quiz_answer), "%.699s", ref);
        quiz_pending = true;

        snprintf(out, out_size,
            "QUIZ: Without looking anything up, explain \"%s\" in your own words and give one important detail. "
            "When you're ready, type 'show answer'.",
            topic);
        return true;
    }

    if(starts_with(lowerbuf, "give me an example of ")) {
        if(fact) {
            snprintf(out, out_size,
                "Concept: %s\n%s\nExample strategy: choose one concrete case where the definition/rule clearly applies, "
                "identify the input/cause, then show the result.",
                fact->needle, fact->answer);
        }
        else {
            snprintf(out, out_size,
                "For \"%s\", I don't have a stored specific example. In %s, build one by choosing a simple concrete case, "
                "applying the subject rule, and checking that the example actually satisfies the definition.",
                topic, subject);
        }
        return true;
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
    else if(starts_with(lowerbuf, "wat is ")) p = prompt + 7;
    else if(starts_with(lowerbuf, "wut is ")) p = prompt + 7;
    else if(starts_with(lowerbuf, "what are ")) p = prompt + 9;
    else if(starts_with(lowerbuf, "what's ")) p = prompt + 7;
    else if(starts_with(lowerbuf, "whats ")) p = prompt + 6;
    else if(starts_with(lowerbuf, "what does ") && strstr(lowerbuf, " mean")) p = prompt + 10;
    else if(starts_with(lowerbuf, "define ")) p = prompt + 7;
    else if(starts_with(lowerbuf, "explain ")) p = prompt + 8;
    else if(starts_with(lowerbuf, "pls explain ")) p = prompt + 12;
    else if(starts_with(lowerbuf, "plz explain ")) p = prompt + 12;
    else if(starts_with(lowerbuf, "can u explain ")) p = prompt + 14;
    else if(starts_with(lowerbuf, "can you explain ")) p = prompt + 16;
    else if(starts_with(lowerbuf, "tell me about ")) p = prompt + 14;
    else if(starts_with(lowerbuf, "summarize ")) p = prompt + 10;
    else if(starts_with(lowerbuf, "summary of ")) p = prompt + 11;
    else if(starts_with(lowerbuf, "quiz me on ")) p = prompt + 11;
    else if(starts_with(lowerbuf, "quiz me about ")) p = prompt + 14;
    else if(starts_with(lowerbuf, "flashcards on ")) p = prompt + 14;
    else if(starts_with(lowerbuf, "flashcards about ")) p = prompt + 17;
    else if(starts_with(lowerbuf, "give me an example of ")) p = prompt + 22;
    else if(starts_with(lowerbuf, "why ")) p = prompt + 4;
    else if(starts_with(lowerbuf, "how ")) p = prompt + 4;
    else if(starts_with(lowerbuf, "who ")) p = prompt + 4;
    else if(starts_with(lowerbuf, "when ")) p = prompt + 5;
    else if(starts_with(lowerbuf, "where ")) p = prompt + 6;
    else if(starts_with(lowerbuf, "compare ")) p = prompt + 8;
    else if(starts_with(lowerbuf, "difference between ")) p = prompt + 19;

    while(*p && isspace((unsigned char)*p)) p++;
    snprintf(topic, topic_size, "%s", p);

    len = strlen(topic);
    while(len > 0 && (isspace((unsigned char)topic[len - 1]) ||
          topic[len - 1] == '?' || topic[len - 1] == '!' ||
          topic[len - 1] == '.')) {
        topic[--len] = '\0';
    }

    if(len >= 5) {
        char tail[6];
        size_t j;
        for(j = 0; j < 5; j++) {
            tail[j] = (char)tolower((unsigned char)topic[len - 5 + j]);
        }
        tail[5] = '\0';
        if(strcmp(tail, " mean") == 0) {
            topic[len - 5] = '\0';
            len -= 5;
        }
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





static bool parse_quadratic_side(char const *text, double *a, double *b, double *c)
{
    double qa = 0.0, qb = 0.0, qc = 0.0;
    double sign = 1.0;
    bool got = false;

    while(*text) {
        char *end;
        double coeff = 1.0;
        bool explicit_coeff = false;

        while(*text && isspace((unsigned char)*text)) text++;
        if(!*text) break;

        if(*text == '+') { sign = 1.0; text++; continue; }
        if(*text == '-') { sign = -1.0; text++; continue; }

        while(*text && isspace((unsigned char)*text)) text++;

        if(isdigit((unsigned char)*text) || *text == '.') {
            coeff = strtod(text, &end);
            if(end == text) return false;
            text = end;
            explicit_coeff = true;
            while(*text && isspace((unsigned char)*text)) text++;
            if(*text == '*') {
                text++;
                while(*text && isspace((unsigned char)*text)) text++;
            }
        }

        if(*text == 'x' || *text == 'X') {
            text++;
            while(*text && isspace((unsigned char)*text)) text++;

            if(*text == '^') {
                text++;
                while(*text && isspace((unsigned char)*text)) text++;
                if(*text != '2') return false;
                text++;
                qa += sign * coeff;
            }
            else {
                qb += sign * coeff;
            }
        }
        else {
            if(!explicit_coeff) return false;
            qc += sign * coeff;
        }

        got = true;
        sign = 1.0;

        while(*text && isspace((unsigned char)*text)) text++;
        if(*text && *text != '+' && *text != '-') return false;
    }

    if(!got) return false;
    *a = qa; *b = qb; *c = qc;
    return true;
}

static bool try_quadratic_equation(char const *text, char *out, size_t out_size)
{
    char equation[220];
    char *eq;
    char *left;
    char *right;
    double a1,b1,c1,a2,b2,c2;
    double a,b,c,d;

    if(!strstr(text, "x^2") && !strstr(text, "X^2")) return false;

    if(starts_with(text, "solve ")) snprintf(equation, sizeof(equation), "%s", text + 6);
    else snprintf(equation, sizeof(equation), "%s", text);

    eq = strchr(equation, '=');
    if(!eq || strchr(eq + 1, '=')) return false;

    *eq = '\0';
    left = equation;
    right = eq + 1;

    if(!parse_quadratic_side(left, &a1,&b1,&c1) ||
       !parse_quadratic_side(right, &a2,&b2,&c2)) return false;

    a = a1-a2; b = b1-b2; c = c1-c2;
    if(fabs(a) < 1e-12) return false;

    d = b*b - 4.0*a*c;

    if(d > 1e-12) {
        double r1 = (-b + sqrt(d))/(2.0*a);
        double r2 = (-b - sqrt(d))/(2.0*a);
        snprintf(out, out_size,
            "Quadratic: %.12gx^2 + %.12gx + %.12g = 0\n"
            "Discriminant = %.12g\nx1 = %.12g\nx2 = %.12g",
            a,b,c,d,r1,r2);
    }
    else if(fabs(d) <= 1e-12) {
        double r = -b/(2.0*a);
        snprintf(out, out_size,
            "Quadratic: %.12gx^2 + %.12gx + %.12g = 0\n"
            "Discriminant = 0\nRepeated root: x = %.12g",
            a,b,c,r);
    }
    else {
        double real = -b/(2.0*a);
        double imag = sqrt(-d)/fabs(2.0*a);
        snprintf(out, out_size,
            "Quadratic: %.12gx^2 + %.12gx + %.12g = 0\n"
            "Discriminant = %.12g\nRoots: %.12g +/- %.12gi",
            a,b,c,d,real,imag);
    }

    return true;
}

static bool try_proportion(char const *text, char *out, size_t out_size)
{
    char compact[220];
    size_t i=0, j=0;
    double a,b,c,d,x;

    while(text[i] && j + 1 < sizeof(compact)) {
        if(!isspace((unsigned char)text[i]) && text[i] != '?') {
            compact[j++] = (char)tolower((unsigned char)text[i]);
        }
        i++;
    }
    compact[j] = '\0';

    if(starts_with(compact, "solve")) memmove(compact, compact+5, strlen(compact+5)+1);

    if(sscanf(compact, "%lf/%lf=x/%lf", &a,&b,&d) == 3 && b != 0.0) {
        x = a*d/b;
    }
    else if(sscanf(compact, "x/%lf=%lf/%lf", &b,&c,&d) == 3 && d != 0.0) {
        x = b*c/d;
    }
    else if(sscanf(compact, "%lf/x=%lf/%lf", &a,&c,&d) == 3 && c != 0.0) {
        x = a*d/c;
    }
    else if(sscanf(compact, "%lf/%lf=%lf/x", &a,&b,&c) == 3 && c != 0.0) {
        x = b*c/a;
    }
    else return false;

    snprintf(out, out_size,
        "Proportion solved by cross-multiplying. x = %.12g", x);
    return true;
}

static bool parse_xy_side(char const *text, double *xcoef, double *ycoef, double *constant)
{
    double ax=0.0, ay=0.0, c=0.0;
    double sign=1.0;
    bool got=false;

    while(*text) {
        char *end;
        double coeff=1.0;
        bool explicit_coeff=false;

        while(*text && isspace((unsigned char)*text)) text++;
        if(!*text) break;
        if(*text=='+') { sign=1.0; text++; continue; }
        if(*text=='-') { sign=-1.0; text++; continue; }

        while(*text && isspace((unsigned char)*text)) text++;

        if(isdigit((unsigned char)*text) || *text=='.') {
            coeff=strtod(text,&end);
            if(end==text) return false;
            text=end;
            explicit_coeff=true;
            while(*text && isspace((unsigned char)*text)) text++;
            if(*text=='*') { text++; while(*text && isspace((unsigned char)*text)) text++; }
        }

        if(*text=='x' || *text=='X') { ax += sign*coeff; text++; }
        else if(*text=='y' || *text=='Y') { ay += sign*coeff; text++; }
        else {
            if(!explicit_coeff) return false;
            c += sign*coeff;
        }

        got=true;
        sign=1.0;
        while(*text && isspace((unsigned char)*text)) text++;
        if(*text && *text!='+' && *text!='-') return false;
    }

    if(!got) return false;
    *xcoef=ax; *ycoef=ay; *constant=c;
    return true;
}

static bool parse_xy_equation(char *equation, double *a, double *b, double *c)
{
    char *eq = strchr(equation, '=');
    double al,bl,cl,ar,br,cr;

    if(!eq || strchr(eq+1,'=')) return false;
    *eq='\0';

    if(!parse_xy_side(equation,&al,&bl,&cl) ||
       !parse_xy_side(eq+1,&ar,&br,&cr)) return false;

    *a=al-ar;
    *b=bl-br;
    *c=cr-cl;
    return true;
}

static bool try_system_2x2(char const *text, char *out, size_t out_size)
{
    char copy[300];
    char *p;
    char *sep;
    double a1,b1,c1,a2,b2,c2;
    double det,x,y;

    if(!strstr(text,"system") && !(strchr(text,';') && strchr(text,'x') && strchr(text,'y'))) return false;

    snprintf(copy,sizeof(copy),"%s",text);
    p=copy;
    if(starts_with(p,"solve system")) {
        p += 12;
        while(*p==':' || isspace((unsigned char)*p)) p++;
    }
    else if(starts_with(p,"system")) {
        p += 6;
        while(*p==':' || isspace((unsigned char)*p)) p++;
    }

    sep=strchr(p,';');
    if(!sep) return false;
    *sep='\0';

    if(!parse_xy_equation(p,&a1,&b1,&c1) ||
       !parse_xy_equation(sep+1,&a2,&b2,&c2)) return false;

    det=a1*b2-a2*b1;
    if(fabs(det)<1e-12) {
        snprintf(out,out_size,
            "The 2x2 system has determinant 0, so it does not have one unique solution.");
        return true;
    }

    x=(c1*b2-c2*b1)/det;
    y=(a1*c2-a2*c1)/det;

    snprintf(out,out_size,
        "2x2 system solution:\nx = %.12g\ny = %.12g\n"
        "Check both values in both original equations.",
        x,y);
    return true;
}


static long gcd_long(long a, long b)
{
    if(a < 0) a = -a;
    if(b < 0) b = -b;
    while(b) {
        long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool try_number_theory(char const *text, char *out, size_t out_size)
{
    double v[16];
    int n = extract_flexible_numbers(text, v, 16);

    if(starts_with(text, "gcd ") || strstr(text, "greatest common factor") ||
       strstr(text, "greatest common divisor")) {
        if(n >= 2) {
            long a=(long)v[0], b=(long)v[1];
            snprintf(out,out_size,"gcd(%ld,%ld) = %ld",a,b,gcd_long(a,b));
            return true;
        }
    }

    if(starts_with(text, "lcm ") || strstr(text, "least common multiple")) {
        if(n >= 2) {
            long a=(long)v[0], b=(long)v[1];
            long g=gcd_long(a,b);
            long l=(g==0)?0:labs((a/g)*b);
            snprintf(out,out_size,"lcm(%ld,%ld) = %ld",a,b,l);
            return true;
        }
    }

    if(starts_with(text, "factor ") || starts_with(text, "prime factors ") ||
       strstr(text, "prime factorization")) {
        if(n >= 1) {
            long x=(long)v[0];
            long value=x<0?-x:x;
            long d;
            char buf[420];
            size_t used=0;
            bool first=true;

            if(value < 2) {
                snprintf(out,out_size,"%ld has no prime factorization into positive primes.",x);
                return true;
            }

            if(x<0) used += (size_t)snprintf(buf+used,sizeof(buf)-used,"-1 * ");

            for(d=2; d<=value/d; d += (d==2 ? 1 : 2)) {
                int count=0;
                while(value%d==0) {
                    value/=d;
                    count++;
                }
                if(count) {
                    used += (size_t)snprintf(buf+used,sizeof(buf)-used,
                        "%s%ld%s",first?"":" * ",d,count>1?"^":"");
                    if(count>1) used += (size_t)snprintf(buf+used,sizeof(buf)-used,"%d",count);
                    first=false;
                }
            }
            if(value>1) {
                used += (size_t)snprintf(buf+used,sizeof(buf)-used,
                    "%s%ld",first?"":" * ",value);
            }

            snprintf(out,out_size,"Prime factorization: %s",buf);
            return true;
        }
    }

    if(starts_with(text, "factorial ") || strstr(text, " factorial")) {
        if(n>=1) {
            long k=(long)v[0];
            unsigned long long result=1;
            long i;
            if(k<0 || k>20) {
                snprintf(out,out_size,"Standalone factorial supports integers 0 through 20.");
                return true;
            }
            for(i=2;i<=k;i++) result *= (unsigned long long)i;
            snprintf(out,out_size,"%ld! = %llu",k,result);
            return true;
        }
    }

    if(starts_with(text, "ncr ") || strstr(text, " combination")) {
        if(n>=2) {
            long nn=(long)v[0], rr=(long)v[1], i;
            unsigned long long result=1;
            if(nn<0 || rr<0 || rr>nn || nn>20) {
                snprintf(out,out_size,"Use 0 <= r <= n <= 20 for exact standalone nCr.");
                return true;
            }
            if(rr>nn-rr) rr=nn-rr;
            for(i=1;i<=rr;i++) result=result*(unsigned long long)(nn-rr+i)/(unsigned long long)i;
            snprintf(out,out_size,"%ldC%ld = %llu",nn,(long)v[1],result);
            return true;
        }
    }

    if(starts_with(text, "npr ") || strstr(text, " permutation")) {
        if(n>=2) {
            long nn=(long)v[0], rr=(long)v[1], i;
            unsigned long long result=1;
            if(nn<0 || rr<0 || rr>nn || nn>20) {
                snprintf(out,out_size,"Use 0 <= r <= n <= 20 for exact standalone nPr.");
                return true;
            }
            for(i=0;i<rr;i++) result *= (unsigned long long)(nn-i);
            snprintf(out,out_size,"%ldP%ld = %llu",nn,rr,result);
            return true;
        }
    }

    return false;
}


static bool try_fraction_percent_tools(char const *text, char *out, size_t out_size)
{
    long a,b;
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if((starts_with(text,"simplify fraction ") || starts_with(text,"reduce fraction ") ||
        starts_with(text,"fraction ")) &&
       sscanf(text,"%*[^-0-9]%ld/%ld",&a,&b)==2) {
        long g;
        if(b==0) {
            snprintf(out,out_size,"A fraction with denominator 0 is undefined.");
            return true;
        }
        if(b<0) { a=-a; b=-b; }
        g=gcd_long(a,b);
        if(g==0) g=1;
        snprintf(out,out_size,"%ld/%ld simplifies to %ld/%ld.",a,b,a/g,b/g);
        return true;
    }

    if(n>=2 && (strstr(text,"what percent is") || strstr(text,"what percentage is"))) {
        if(fabs(v[1])<1e-12) {
            snprintf(out,out_size,"Percent relative to a zero whole is undefined.");
        } else {
            snprintf(out,out_size,"%.12g is %.12g%% of %.12g.",v[0],100.0*v[0]/v[1],v[1]);
        }
        return true;
    }

    if(n>=2 && starts_with(text,"increase ") && strstr(text," by ") && strstr(text,"percent")) {
        double base=v[0], pct=v[1];
        snprintf(out,out_size,
            "Increase %.12g by %.12g%% = %.12g.",base,pct,base*(1.0+pct/100.0));
        return true;
    }

    if(n>=2 && starts_with(text,"decrease ") && strstr(text," by ") && strstr(text,"percent")) {
        double base=v[0], pct=v[1];
        snprintf(out,out_size,
            "Decrease %.12g by %.12g%% = %.12g.",base,pct,base*(1.0-pct/100.0));
        return true;
    }

    return false;
}

static int compare_double_asc(void const *a, void const *b)
{
    double x=*(double const *)a;
    double y=*(double const *)b;
    return (x>y)-(x<y);
}


static double median_sorted_slice(double const *v, int start, int count)
{
    int mid;
    if(count<=0) return 0.0;
    mid=start+count/2;
    if(count%2) return v[mid];
    return (v[mid-1]+v[mid])/2.0;
}

static bool try_statistics_tools(char const *text, char *out, size_t out_size)
{
    double v[32];
    int n=extract_flexible_numbers(text,v,32);
    int i;

    if(n<=0) return false;

    if(starts_with(text,"median ") || starts_with(text,"median of ")) {
        double copy[32];
        double med;
        for(i=0;i<n;i++) copy[i]=v[i];
        qsort(copy,(size_t)n,sizeof(double),compare_double_asc);
        med = (n%2)?copy[n/2]:(copy[n/2-1]+copy[n/2])/2.0;
        snprintf(out,out_size,"Median of %d values = %.12g",n,med);
        return true;
    }

    if(starts_with(text,"range ") || starts_with(text,"range of ")) {
        double lo=v[0], hi=v[0];
        for(i=1;i<n;i++) { if(v[i]<lo) lo=v[i]; if(v[i]>hi) hi=v[i]; }
        snprintf(out,out_size,"Range = max-min = %.12g - %.12g = %.12g",hi,lo,hi-lo);
        return true;
    }

    if(starts_with(text,"stdev ") || starts_with(text,"standard deviation ") ||
       starts_with(text,"standard deviation of ")) {
        double mean=0.0, ss=0.0;
        for(i=0;i<n;i++) mean+=v[i];
        mean/=n;
        for(i=0;i<n;i++) { double d=v[i]-mean; ss+=d*d; }
        snprintf(out,out_size,
            "Population stdev = %.12g (mean %.12g, n=%d)",
            sqrt(ss/n),mean,n);
        return true;
    }

    if(starts_with(text,"min ") || starts_with(text,"minimum ")) {
        double m=v[0];
        for(i=1;i<n;i++) if(v[i]<m) m=v[i];
        snprintf(out,out_size,"Minimum = %.12g",m);
        return true;
    }

    if(starts_with(text,"max ") || starts_with(text,"maximum ")) {
        double m=v[0];
        for(i=1;i<n;i++) if(v[i]>m) m=v[i];
        snprintf(out,out_size,"Maximum = %.12g",m);
        return true;
    }

    if(starts_with(text,"mode ") || starts_with(text,"mode of ")) {
        double copy[32];
        int best_count=1;
        int current=1;
        int modes=0;
        char buf[300];
        size_t used=0;

        for(i=0;i<n;i++) copy[i]=v[i];
        qsort(copy,(size_t)n,sizeof(double),compare_double_asc);

        for(i=1;i<=n;i++) {
            if(i<n && approx_equal(copy[i],copy[i-1])) current++;
            else {
                if(current>best_count) best_count=current;
                current=1;
            }
        }

        if(best_count==1) {
            snprintf(out,out_size,"No mode: every value occurs once.");
            return true;
        }

        current=1;
        buf[0]='\0';
        for(i=1;i<=n;i++) {
            if(i<n && approx_equal(copy[i],copy[i-1])) current++;
            else {
                if(current==best_count) {
                    double value=copy[i-1];
                    used=strlen(buf);
                    snprintf(buf+used,sizeof(buf)-used,"%s%.12g",modes?", ":"",value);
                    modes++;
                }
                current=1;
            }
        }
        snprintf(out,out_size,"Mode%s (frequency %d): %s",modes==1?"":"s",best_count,buf);
        return true;
    }

    if(starts_with(text,"quartiles ") || starts_with(text,"quartiles of ") ||
       starts_with(text,"iqr ") || starts_with(text,"iqr of ") ||
       strstr(text,"five number summary") || strstr(text,"five-number summary")) {
        double copy[32];
        double q1,q2,q3,iqr;
        int lower_count, upper_start, upper_count;

        for(i=0;i<n;i++) copy[i]=v[i];
        qsort(copy,(size_t)n,sizeof(double),compare_double_asc);

        q2=median_sorted_slice(copy,0,n);
        lower_count=n/2;
        upper_start=(n+1)/2;
        upper_count=n/2;
        q1=median_sorted_slice(copy,0,lower_count);
        q3=median_sorted_slice(copy,upper_start,upper_count);
        iqr=q3-q1;

        snprintf(out,out_size,
            "Five-number summary: min %.12g, Q1 %.12g, median %.12g, Q3 %.12g, max %.12g. IQR = %.12g.",
            copy[0],q1,q2,q3,copy[n-1],iqr);
        return true;
    }

    if(starts_with(text,"zscore ") || starts_with(text,"z-score ") ||
       starts_with(text,"z score ")) {
        if(n<3) {
            snprintf(out,out_size,"Use: zscore value mean standard_deviation");
            return true;
        }
        if(fabs(v[2])<1e-12) snprintf(out,out_size,"Standard deviation cannot be zero for a z-score.");
        else snprintf(out,out_size,"z = (x-mean)/sd = %.12g",(v[0]-v[1])/v[2]);
        return true;
    }

    return false;
}

static bool parse_polynomial(char const *text, double coeff[7])
{
    double sign=1.0;
    bool got=false;
    int i;

    for(i=0;i<7;i++) coeff[i]=0.0;

    while(*text) {
        char *end;
        double c=1.0;
        bool explicit_c=false;
        int power=0;

        while(*text && isspace((unsigned char)*text)) text++;
        if(!*text) break;

        if(*text=='+') { sign=1.0; text++; continue; }
        if(*text=='-') { sign=-1.0; text++; continue; }

        while(*text && isspace((unsigned char)*text)) text++;

        if(isdigit((unsigned char)*text) || *text=='.') {
            c=strtod(text,&end);
            if(end==text) return false;
            text=end;
            explicit_c=true;
            while(*text && isspace((unsigned char)*text)) text++;
            if(*text=='*') { text++; while(*text && isspace((unsigned char)*text)) text++; }
        }

        if(*text=='x' || *text=='X') {
            power=1;
            text++;
            if(*text=='^') {
                long p;
                text++;
                p=strtol(text,&end,10);
                if(end==text || p<0 || p>6) return false;
                power=(int)p;
                text=end;
            }
        }
        else {
            if(!explicit_c) return false;
            power=0;
        }

        coeff[power] += sign*c;
        sign=1.0;
        got=true;

        while(*text && isspace((unsigned char)*text)) text++;
        if(*text && *text!='+' && *text!='-') return false;
    }

    return got;
}

static void format_polynomial(double const coeff[8], int max_power,
                              char *out, size_t out_size, bool integral_constant)
{
    size_t used=0;
    bool first=true;
    int p;

    if(out_size==0) return;
    out[0]='\0';

    for(p=max_power;p>=0;p--) {
        double c=coeff[p];
        double abs_c;
        char term[80];

        if(fabs(c)<1e-12) continue;
        abs_c=fabs(c);

        if(p==0) snprintf(term,sizeof(term),"%.12g",abs_c);
        else if(p==1) {
            if(fabs(abs_c-1.0)<1e-12) snprintf(term,sizeof(term),"x");
            else snprintf(term,sizeof(term),"%.12gx",abs_c);
        }
        else {
            if(fabs(abs_c-1.0)<1e-12) snprintf(term,sizeof(term),"x^%d",p);
            else snprintf(term,sizeof(term),"%.12gx^%d",abs_c,p);
        }

        used += (size_t)snprintf(out+used,out_size-used,
            "%s%s%s",first?(c<0?"-":""):(c<0?" - ":" + "),term,"");
        if(used>=out_size) break;
        first=false;
    }

    if(first) snprintf(out,out_size,"0");
    if(integral_constant && strlen(out)+4<out_size) strcat(out," + C");
}

static bool try_polynomial_calculus(char const *text, char *out, size_t out_size)
{
    char expr[220];
    char const *p=NULL;
    double c[7], result[8]={0};
    int i;
    char formatted[420];

    if(starts_with(text,"derive ")) p=text+7;
    else if(starts_with(text,"derivative of ")) p=text+14;
    else if(starts_with(text,"differentiate ")) p=text+14;
    else if(starts_with(text,"integrate ")) p=text+10;
    else if(starts_with(text,"integral of ")) p=text+12;
    else return false;

    snprintf(expr,sizeof(expr),"%s",p);
    trim_text(expr);
    if(!parse_polynomial(expr,c)) return false;

    if(starts_with(text,"integrate ") || starts_with(text,"integral of ")) {
        for(i=0;i<=6;i++) result[i+1]=c[i]/(i+1);
        format_polynomial(result,7,formatted,sizeof(formatted),true);
        snprintf(out,out_size,"Integral: %s",formatted);
    }
    else {
        for(i=1;i<=6;i++) result[i-1]=c[i]*i;
        format_polynomial(result,6,formatted,sizeof(formatted),false);
        snprintf(out,out_size,"Derivative: %s",formatted);
    }

    return true;
}


static bool approx_equal(double a, double b)
{
    double scale = fabs(a) + fabs(b) + 1.0;
    return fabs(a-b) <= 1e-9 * scale;
}


static double eval_poly_coeff(double const c[7], double x)
{
    double y=0.0;
    int p;
    for(p=6;p>=0;p--) y=y*x+c[p];
    return y;
}

static char const *strip_function_prefix(char const *text)
{
    if(starts_with(text,"f(x)=")) return text+5;
    if(starts_with(text,"y=")) return text+2;
    return text;
}

static bool try_function_analysis(char const *text, char *out, size_t out_size)
{
    char work[300];
    char expr[220];
    char *at;
    char *p;
    double c[7];
    double x,y;
    int degree=6;

    if(starts_with(text,"evaluate f(x)=") || starts_with(text,"evaluate y=") ||
       (starts_with(text,"f(x)=") && strstr(text," at x="))) {
        char *end;
        if(starts_with(text,"evaluate ")) snprintf(work,sizeof(work),"%s",text+9);
        else snprintf(work,sizeof(work),"%s",text);

        at=strstr(work," at x=");
        if(!at) return false;
        *at='\0';
        x=strtod(at+6,&end);
        if(end==at+6) return false;

        p=(char *)strip_function_prefix(work);
        snprintf(expr,sizeof(expr),"%s",p);
        trim_text(expr);
        if(!parse_polynomial(expr,c)) return false;

        y=eval_poly_coeff(c,x);
        snprintf(out,out_size,"f(%.12g) = %.12g",x,y);
        return true;
    }

    if(starts_with(text,"vertex ") || starts_with(text,"axis of symmetry ") ||
       starts_with(text,"axis ") || starts_with(text,"zeros ") ||
       starts_with(text,"roots of ") || starts_with(text,"y intercept ") ||
       starts_with(text,"y-intercept ")) {
        char const *src=text;
        double a,b,cc;

        if(starts_with(src,"vertex ")) src+=7;
        else if(starts_with(src,"axis of symmetry ")) src+=17;
        else if(starts_with(src,"axis ")) src+=5;
        else if(starts_with(src,"zeros ")) src+=6;
        else if(starts_with(src,"roots of ")) src+=9;
        else if(starts_with(src,"y intercept ")) src+=12;
        else if(starts_with(src,"y-intercept ")) src+=12;

        src=strip_function_prefix(src);
        snprintf(expr,sizeof(expr),"%s",src);
        trim_text(expr);
        if(!parse_polynomial(expr,c)) return false;

        while(degree>0 && fabs(c[degree])<1e-12) degree--;
        a=c[2]; b=c[1]; cc=c[0];

        if(starts_with(text,"y intercept ") || starts_with(text,"y-intercept ")) {
            snprintf(out,out_size,"y-intercept = (0, %.12g)",cc);
            return true;
        }

        if(starts_with(text,"vertex ") || starts_with(text,"axis")) {
            if(degree!=2 || fabs(a)<1e-12) {
                snprintf(out,out_size,"Vertex/axis analysis here expects a quadratic polynomial.");
                return true;
            }
            x=-b/(2.0*a);
            y=eval_poly_coeff(c,x);
            if(starts_with(text,"vertex "))
                snprintf(out,out_size,"Vertex = (%.12g, %.12g). Axis of symmetry: x = %.12g",x,y,x);
            else
                snprintf(out,out_size,"Axis of symmetry: x = %.12g",x);
            return true;
        }

        if(starts_with(text,"zeros ") || starts_with(text,"roots of ")) {
            if(degree==1 && fabs(b)>1e-12) {
                snprintf(out,out_size,"Zero: x = %.12g",-cc/b);
                return true;
            }
            if(degree==2 && fabs(a)>1e-12) {
                double d=b*b-4.0*a*cc;
                if(d>1e-12) {
                    snprintf(out,out_size,"Zeros: x = %.12g and x = %.12g",
                             (-b+sqrt(d))/(2.0*a),(-b-sqrt(d))/(2.0*a));
                } else if(fabs(d)<=1e-12) {
                    snprintf(out,out_size,"Repeated zero: x = %.12g",-b/(2.0*a));
                } else {
                    double real=-b/(2.0*a), imag=sqrt(-d)/fabs(2.0*a);
                    snprintf(out,out_size,"Complex zeros: %.12g +/- %.12gi",real,imag);
                }
                return true;
            }
            snprintf(out,out_size,"Zero finding currently supports linear and quadratic polynomials.");
            return true;
        }
    }

    return false;
}

static bool try_sequence_pattern(char const *text, char *out, size_t out_size)
{
    double v[20];
    int n;
    int i;
    bool arithmetic=true, geometric=true, fibonacci=true;
    double d=0.0, r=0.0, next;

    if(!(strstr(text,"sequence") || strstr(text,"pattern") ||
         starts_with(text,"next ") || starts_with(text,"next number"))) return false;

    n=extract_flexible_numbers(text,v,20);
    if(n<3) return false;

    d=v[1]-v[0];
    for(i=2;i<n;i++) {
        if(!approx_equal(v[i]-v[i-1],d)) arithmetic=false;
    }
    if(arithmetic) {
        next=v[n-1]+d;
        snprintf(out,out_size,
            "Arithmetic sequence: common difference %.12g. Next term = %.12g.",
            d,next);
        return true;
    }

    if(fabs(v[0])<1e-12) geometric=false;
    else {
        r=v[1]/v[0];
        for(i=2;i<n;i++) {
            if(fabs(v[i-1])<1e-12 || !approx_equal(v[i]/v[i-1],r)) geometric=false;
        }
    }
    if(geometric) {
        next=v[n-1]*r;
        snprintf(out,out_size,
            "Geometric sequence: common ratio %.12g. Next term = %.12g.",
            r,next);
        return true;
    }

    for(i=2;i<n;i++) {
        if(!approx_equal(v[i],v[i-1]+v[i-2])) fibonacci=false;
    }
    if(fibonacci) {
        next=v[n-1]+v[n-2];
        snprintf(out,out_size,
            "Fibonacci-style pattern: each term is the previous two added. Next term = %.12g.",
            next);
        return true;
    }

    {
        double dd=v[2]-2.0*v[1]+v[0];
        bool second=true;
        for(i=3;i<n;i++) {
            double cur=v[i]-2.0*v[i-1]+v[i-2];
            if(!approx_equal(cur,dd)) second=false;
        }
        if(second) {
            double last_diff=v[n-1]-v[n-2];
            next=v[n-1]+last_diff+dd;
            snprintf(out,out_size,
                "Constant second-difference pattern: second difference %.12g. Next term = %.12g.",
                dd,next);
            return true;
        }
    }

    snprintf(out,out_size,
        "I can read the sequence values, but they do not match my built-in arithmetic, geometric, Fibonacci-style, or constant-second-difference patterns.");
    return true;
}

static bool try_finance_tools(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=3 && strstr(text,"simple interest")) {
        double p=v[0], rate=v[1], years=v[2];
        double r=fabs(rate)>1.0 ? rate/100.0 : rate;
        double interest=p*r*years;
        snprintf(out,out_size,
            "Simple interest I=P*r*t = %.12g. Final amount = %.12g.",
            interest,p+interest);
        return true;
    }

    if(n>=2 && (strstr(text,"discount") || strstr(text,"percent off") || strstr(text,"% off"))) {
        double percent=v[0], price=v[1];
        double saved=price*percent/100.0;
        snprintf(out,out_size,
            "%.12g%% discount on %.12g saves %.12g; final price = %.12g.",
            percent,price,saved,price-saved);
        return true;
    }

    if(n>=2 && (strstr(text,"sales tax") || strstr(text,"tax on "))) {
        double percent=v[0], price=v[1];
        double tax=price*percent/100.0;
        snprintf(out,out_size,
            "%.12g%% tax on %.12g = %.12g; total = %.12g.",
            percent,price,tax,price+tax);
        return true;
    }

    if(n>=2 && strstr(text,"tip")) {
        double percent=v[0], bill=v[1];
        double tip=bill*percent/100.0;
        snprintf(out,out_size,
            "%.12g%% tip on %.12g = %.12g; total = %.12g.",
            percent,bill,tip,bill+tip);
        return true;
    }

    if(n>=4 && (strstr(text,"compound interest") || strstr(text,"compound amount"))) {
        double p=v[0], rate=v[1], periods=v[2], years=v[3];
        double r=fabs(rate)>1.0 ? rate/100.0 : rate;
        if(periods<=0.0) {
            snprintf(out,out_size,"Compounding periods per year must be positive.");
            return true;
        }
        snprintf(out,out_size,
            "Compound amount A=P(1+r/n)^(nt) = %.12g.",
            p*pow(1.0+r/periods,periods*years));
        return true;
    }

    return false;
}

static int compare_cstr(const void *a, const void *b)
{
    char const * const *sa=(char const * const *)a;
    char const * const *sb=(char const * const *)b;
    return strcmp(*sa,*sb);
}

static bool try_language_tools(char const *prompt, char *out, size_t out_size)
{
    char lower[500];
    char text[420];
    char *p=NULL;
    size_t i;

    lowercase_into(prompt,lower,sizeof(lower));

    if(starts_with(lower,"uppercase ")) {
        snprintf(text,sizeof(text),"%.419s",prompt+10);
        for(i=0;text[i];i++) text[i]=(char)toupper((unsigned char)text[i]);
        snprintf(out,out_size,"%s",text);
        return true;
    }

    if(starts_with(lower,"lowercase ")) {
        snprintf(text,sizeof(text),"%.419s",prompt+10);
        for(i=0;text[i];i++) text[i]=(char)tolower((unsigned char)text[i]);
        snprintf(out,out_size,"%s",text);
        return true;
    }

    if(starts_with(lower,"title case ")) {
        bool new_word=true;
        snprintf(text,sizeof(text),"%.419s",prompt+11);
        for(i=0;text[i];i++) {
            if(isalpha((unsigned char)text[i])) {
                text[i]=new_word?(char)toupper((unsigned char)text[i]):(char)tolower((unsigned char)text[i]);
                new_word=false;
            } else new_word=true;
        }
        snprintf(out,out_size,"%s",text);
        return true;
    }

    if(starts_with(lower,"count vowels in ") || starts_with(lower,"how many vowels in ")) {
        int count=0;
        p=starts_with(lower,"count vowels in ")?(char *)prompt+16:(char *)prompt+19;
        while(*p) {
            char c=(char)tolower((unsigned char)*p++);
            if(strchr("aeiou",c)) count++;
        }
        snprintf(out,out_size,"Vowel count = %d.",count);
        return true;
    }

    if(starts_with(lower,"count consonants in ") || starts_with(lower,"how many consonants in ")) {
        int count=0;
        p=starts_with(lower,"count consonants in ")?(char *)prompt+20:(char *)prompt+23;
        while(*p) {
            char c=(char)tolower((unsigned char)*p++);
            if(isalpha((unsigned char)c) && !strchr("aeiou",c)) count++;
        }
        snprintf(out,out_size,"Consonant count = %d.",count);
        return true;
    }

    if(starts_with(lower,"initials of ") || starts_with(lower,"acronym of ")) {
        char initials[80];
        int k=0;
        bool start=true;
        p=starts_with(lower,"initials of ")?(char *)prompt+12:(char *)prompt+11;
        while(*p && k+1<(int)sizeof(initials)) {
            if(isalnum((unsigned char)*p)) {
                if(start) initials[k++]=(char)toupper((unsigned char)*p);
                start=false;
            } else start=true;
            p++;
        }
        initials[k]='\0';
        snprintf(out,out_size,"Initials: %s",initials);
        return true;
    }

    if(starts_with(lower,"alphabetize ") || starts_with(lower,"sort words ")) {
        char work[420];
        char *words[32];
        int count=0;
        char *tok;

        p=starts_with(lower,"alphabetize ")?(char *)prompt+12:(char *)prompt+11;
        snprintf(work,sizeof(work),"%.419s",p);
        tok=strtok(work," ,;\t\r\n");
        while(tok && count<32) {
            words[count++]=tok;
            tok=strtok(NULL," ,;\t\r\n");
        }
        if(count==0) return false;
        qsort(words,(size_t)count,sizeof(words[0]),compare_cstr);
        out[0]='\0';
        for(i=0;i<(size_t)count;i++) {
            size_t used=strlen(out);
            snprintf(out+used,out_size-used,"%s%s",i?", ":"",words[i]);
            if(strlen(out)+2>=out_size) break;
        }
        return true;
    }

    if(starts_with(lower,"sentence type ")) {
        snprintf(text,sizeof(text),"%.419s",prompt+14);
        trim_text(text);
        {
            size_t len=strlen(prompt+14);
            while(len>0 && isspace((unsigned char)(prompt+14)[len-1])) len--;
            if(len>0 && (prompt+14)[len-1]=='?') snprintf(out,out_size,"Question.");
            else if(len>0 && (prompt+14)[len-1]=='!') snprintf(out,out_size,"Exclamation or emphatic sentence.");
            else snprintf(out,out_size,"Declarative statement based on punctuation.");
        }
        return true;
    }

    return false;
}


static bool try_triangle_trig(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);
    double pi=3.14159265358979323846;

    if(n>=2 && (strstr(text,"hypotenuse") || strstr(text,"right triangle")) &&
       !strstr(text,"missing leg") && !strstr(text,"find leg")) {
        double c=sqrt(v[0]*v[0]+v[1]*v[1]);
        snprintf(out,out_size,"Right triangle hypotenuse = sqrt(a^2+b^2) = %.12g",c);
        return true;
    }

    if(n>=2 && (strstr(text,"missing leg") || strstr(text,"find leg"))) {
        double c=v[0], a=v[1];
        if(c<a || c<0.0 || a<0.0) {
            snprintf(out,out_size,"The hypotenuse must be at least as long as the known leg.");
        } else {
            snprintf(out,out_size,"Missing leg = sqrt(c^2-a^2) = %.12g",sqrt(c*c-a*a));
        }
        return true;
    }

    if(n>=1 && (starts_with(text,"sin ") || starts_with(text,"sine "))) {
        snprintf(out,out_size,"sin(%.12g deg) = %.12g",v[0],sin(v[0]*pi/180.0));
        return true;
    }
    if(n>=1 && (starts_with(text,"cos ") || starts_with(text,"cosine "))) {
        snprintf(out,out_size,"cos(%.12g deg) = %.12g",v[0],cos(v[0]*pi/180.0));
        return true;
    }
    if(n>=1 && (starts_with(text,"tan ") || starts_with(text,"tangent "))) {
        double c=cos(v[0]*pi/180.0);
        if(fabs(c)<1e-12) snprintf(out,out_size,"tan(%.12g deg) is undefined.",v[0]);
        else snprintf(out,out_size,"tan(%.12g deg) = %.12g",v[0],tan(v[0]*pi/180.0));
        return true;
    }

    return false;
}

static bool try_extra_science(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=1 && (starts_with(text,"ph ") || strstr(text,"ph from h"))) {
        double h=v[0];
        if(h<=0.0) snprintf(out,out_size,"Hydrogen-ion concentration must be positive for pH=-log10[H+].");
        else snprintf(out,out_size,"pH = -log10[H+] = %.12g",-log10(h));
        return true;
    }

    if(n>=1 && (strstr(text,"h+ concentration from ph") || strstr(text,"hydrogen concentration from ph"))) {
        snprintf(out,out_size,"[H+] = 10^(-pH) = %.12g M",pow(10.0,-v[0]));
        return true;
    }

    if(n>=3 && strstr(text,"ideal gas") && strstr(text,"pressure")) {
        double moles=v[0], temp=v[1], volume=v[2];
        if(volume<=0.0) snprintf(out,out_size,"Gas volume must be positive.");
        else snprintf(out,out_size,"Ideal-gas pressure P=nRT/V = %.12g atm",
                      moles*0.082057366080960*temp/volume);
        return true;
    }

    if(n>=2 && (strstr(text,"gravitational potential energy") || strstr(text,"potential energy mass"))) {
        snprintf(out,out_size,"Gravitational potential energy U=mgh = %.12g J",v[0]*9.80665*v[1]);
        return true;
    }

    if(n>=2 && starts_with(text,"speed ") && strstr(text,"distance") && strstr(text,"time")) {
        if(fabs(v[1])<1e-12) snprintf(out,out_size,"Speed is undefined for zero elapsed time.");
        else snprintf(out,out_size,"speed = distance/time = %.12g",v[0]/v[1]);
        return true;
    }

    return false;
}

static void canonical_genotype(char a, char b, char out[3])
{
    if(isupper((unsigned char)b) && islower((unsigned char)a)) {
        char t=a; a=b; b=t;
    }
    out[0]=a; out[1]=b; out[2]='\0';
}

static bool valid_simple_genotype(char const *g)
{
    return g && strlen(g)==2 && isalpha((unsigned char)g[0]) &&
           isalpha((unsigned char)g[1]) &&
           tolower((unsigned char)g[0])==tolower((unsigned char)g[1]);
}

static bool try_genetics_cross(char const *prompt, char *out, size_t out_size)
{
    char lower[220], work[220];
    char *p,*sep;
    char g1[3],g2[3];
    char offspring[4][3];
    int counts[4]={0,0,0,0};
    char unique[4][3];
    int unique_n=0, i,j;

    lowercase_into(prompt,lower,sizeof(lower));
    if(!(starts_with(lower,"cross ") || starts_with(lower,"punnett ") || starts_with(lower,"punnett square "))) return false;

    snprintf(work,sizeof(work),"%.219s",prompt);
    p=work;
    if(starts_with(lower,"punnett square ")) p+=15;
    else if(starts_with(lower,"punnett ")) p+=8;
    else p+=6;
    while(*p && isspace((unsigned char)*p)) p++;

    sep=strstr(p," x ");
    if(!sep) sep=strstr(p," X ");
    if(!sep) return false;
    *sep='\0';
    snprintf(g1,sizeof(g1),"%.2s",p);
    snprintf(g2,sizeof(g2),"%.2s",sep+3);
    trim_text(g1); trim_text(g2);

    if(!valid_simple_genotype(g1) || !valid_simple_genotype(g2) ||
       tolower((unsigned char)g1[0])!=tolower((unsigned char)g2[0])) return false;

    canonical_genotype(g1[0],g2[0],offspring[0]);
    canonical_genotype(g1[0],g2[1],offspring[1]);
    canonical_genotype(g1[1],g2[0],offspring[2]);
    canonical_genotype(g1[1],g2[1],offspring[3]);

    for(i=0;i<4;i++) {
        int idx=-1;
        for(j=0;j<unique_n;j++) if(strcmp(unique[j],offspring[i])==0) idx=j;
        if(idx<0) {
            idx=unique_n++;
            strcpy(unique[idx],offspring[i]);
        }
        counts[idx]++;
    }

    out[0]='\0';
    snprintf(out,out_size,"Punnett cross %s x %s: ",g1,g2);
    for(i=0;i<unique_n;i++) {
        size_t used=strlen(out);
        snprintf(out+used,out_size-used,"%s%s %d%%",i?", ":"",unique[i],counts[i]*25);
    }
    {
        int dominant=0;
        for(i=0;i<4;i++) if(isupper((unsigned char)offspring[i][0]) || isupper((unsigned char)offspring[i][1])) dominant++;
        {
            size_t used=strlen(out);
            snprintf(out+used,out_size-used,". Dominant phenotype %d%%; recessive phenotype %d%%.",
                     dominant*25,(4-dominant)*25);
        }
    }
    return true;
}


static bool split_numeric_sides(char const *text,
                                double *left, int *nl,
                                double *right, int *nr, int cap)
{
    char work[500];
    char *sep;

    snprintf(work,sizeof(work),"%.499s",text);
    sep=strchr(work,';');
    if(!sep) return false;
    *sep='\0';

    *nl=extract_flexible_numbers(work,left,cap);
    *nr=extract_flexible_numbers(sep+1,right,cap);
    return *nl>0 && *nr>0;
}

static bool try_linear_algebra_tools(char const *text, char *out, size_t out_size)
{
    double v[16];
    double a[8],b[8];
    int n=extract_flexible_numbers(text,v,16);
    int na=0,nb=0;

    if((starts_with(text,"det ") || starts_with(text,"determinant ")) && n>=4) {
        double det=v[0]*v[3]-v[1]*v[2];
        snprintf(out,out_size,
            "det([[%.12g,%.12g],[%.12g,%.12g]]) = %.12g",
            v[0],v[1],v[2],v[3],det);
        return true;
    }

    if((starts_with(text,"inverse ") || starts_with(text,"matrix inverse ")) && n>=4) {
        double det=v[0]*v[3]-v[1]*v[2];
        if(fabs(det)<1e-12) {
            snprintf(out,out_size,"That 2x2 matrix is singular because its determinant is 0.");
        } else {
            snprintf(out,out_size,
                "2x2 inverse = [[%.12g, %.12g], [%.12g, %.12g]]",
                v[3]/det,-v[1]/det,-v[2]/det,v[0]/det);
        }
        return true;
    }

    if((starts_with(text,"vector magnitude ") || starts_with(text,"magnitude ")) && n>=2) {
        double ss=0.0;
        int i;
        for(i=0;i<n;i++) ss+=v[i]*v[i];
        snprintf(out,out_size,"Vector magnitude = sqrt(sum(component^2)) = %.12g",sqrt(ss));
        return true;
    }

    if(starts_with(text,"dot ") && split_numeric_sides(text,a,&na,b,&nb,8)) {
        double dot=0.0;
        int i;
        if(na!=nb) {
            snprintf(out,out_size,"Dot product needs vectors with the same number of components.");
            return true;
        }
        for(i=0;i<na;i++) dot+=a[i]*b[i];
        snprintf(out,out_size,"Dot product = %.12g",dot);
        return true;
    }

    if(starts_with(text,"cross ") && split_numeric_sides(text,a,&na,b,&nb,8)) {
        if(na!=3 || nb!=3) {
            snprintf(out,out_size,"Cross product here expects two 3D vectors.");
            return true;
        }
        snprintf(out,out_size,
            "Cross product = <%.12g, %.12g, %.12g>",
            a[1]*b[2]-a[2]*b[1],
            a[2]*b[0]-a[0]*b[2],
            a[0]*b[1]-a[1]*b[0]);
        return true;
    }

    if((starts_with(text,"matmul ") || starts_with(text,"matrix multiply ")) &&
       split_numeric_sides(text,a,&na,b,&nb,8)) {
        if(na!=4 || nb!=4) {
            snprintf(out,out_size,"2x2 matrix multiplication expects four entries on each side of ';'.");
            return true;
        }
        snprintf(out,out_size,
            "Product = [[%.12g, %.12g], [%.12g, %.12g]]",
            a[0]*b[0]+a[1]*b[2],
            a[0]*b[1]+a[1]*b[3],
            a[2]*b[0]+a[3]*b[2],
            a[2]*b[1]+a[3]*b[3]);
        return true;
    }

    return false;
}

static double choose_double(int n, int k)
{
    double result=1.0;
    int i;
    if(k<0 || k>n) return 0.0;
    if(k>n-k) k=n-k;
    for(i=1;i<=k;i++) result=result*(double)(n-k+i)/(double)i;
    return result;
}

static bool try_probability_tools(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=2 && strstr(text,"probability") && strstr(text,"out of")) {
        if(v[1]<=0.0) snprintf(out,out_size,"Total outcomes must be positive.");
        else {
            double p=v[0]/v[1];
            snprintf(out,out_size,"Probability = %.12g = %.12g%%",p,100.0*p);
        }
        return true;
    }

    if(n>=1 && starts_with(text,"complement ")) {
        double p=v[0];
        if(p>1.0 && p<=100.0) p/=100.0;
        if(p<0.0 || p>1.0) snprintf(out,out_size,"Probability must be between 0 and 1, or 0%% and 100%%.");
        else snprintf(out,out_size,"Complement = 1-p = %.12g = %.12g%%",1.0-p,100.0*(1.0-p));
        return true;
    }

    if(n>=3 && starts_with(text,"binomial ")) {
        int trials=(int)v[0];
        double p=v[1];
        int k=(int)v[2];
        double prob;
        if(p>1.0 && p<=100.0) p/=100.0;
        if(trials<0 || k<0 || k>trials || p<0.0 || p>1.0) {
            snprintf(out,out_size,"Use binomial n p k with n>=0, 0<=k<=n, and probability p from 0 to 1 (or percent).");
            return true;
        }
        prob=choose_double(trials,k)*pow(p,k)*pow(1.0-p,trials-k);
        snprintf(out,out_size,
            "P(X=%d) = C(%d,%d)*p^k*(1-p)^(n-k) = %.12g = %.12g%%",
            k,trials,k,prob,100.0*prob);
        return true;
    }

    return false;
}

static bool try_log_root_tools(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=2 && strstr(text,"log base ") && strstr(text," of ")) {
        double base=v[0], x=v[1];
        if(base<=0.0 || approx_equal(base,1.0) || x<=0.0)
            snprintf(out,out_size,"For real logarithms: base > 0, base != 1, and argument > 0.");
        else snprintf(out,out_size,"log base %.12g of %.12g = %.12g",base,x,log(x)/log(base));
        return true;
    }

    if(n>=1 && (starts_with(text,"log ") || starts_with(text,"log10 "))) {
        if(v[0]<=0.0) snprintf(out,out_size,"Real log10 requires a positive argument.");
        else snprintf(out,out_size,"log10(%.12g) = %.12g",v[0],log10(v[0]));
        return true;
    }

    if(n>=1 && starts_with(text,"ln ")) {
        if(v[0]<=0.0) snprintf(out,out_size,"Real natural log requires a positive argument.");
        else snprintf(out,out_size,"ln(%.12g) = %.12g",v[0],log(v[0]));
        return true;
    }

    if(n>=2 && (strstr(text,"nth root") || strstr(text,"root of"))) {
        double degree=v[0], x=v[1];
        if(fabs(degree)<1e-12) snprintf(out,out_size,"Root degree cannot be zero.");
        else if(x<0.0 && fmod(fabs(degree),2.0)<1e-12 && ((long)fabs(degree))%2==0)
            snprintf(out,out_size,"An even root of a negative number is not real.");
        else {
            double value;
            if(x<0.0) value=-pow(-x,1.0/degree);
            else value=pow(x,1.0/degree);
            snprintf(out,out_size,"%.12g-th root of %.12g = %.12g",degree,x,value);
        }
        return true;
    }

    return false;
}


typedef struct {
    char const *symbol;
    double mass;
} atomic_mass_t;

static atomic_mass_t const common_masses[] = {
    {"H",1.008},{"He",4.0026},{"Li",6.94},{"Be",9.0122},{"B",10.81},
    {"C",12.011},{"N",14.007},{"O",15.999},{"F",18.998},{"Ne",20.180},
    {"Na",22.990},{"Mg",24.305},{"Al",26.982},{"Si",28.085},{"P",30.974},
    {"S",32.06},{"Cl",35.45},{"Ar",39.948},{"K",39.0983},{"Ca",40.078},
    {"Sc",44.956},{"Ti",47.867},{"V",50.942},{"Cr",51.996},{"Mn",54.938},
    {"Fe",55.845},{"Co",58.933},{"Ni",58.693},{"Cu",63.546},{"Zn",65.38},
    {"Br",79.904},{"Ag",107.8682},{"I",126.90447},{"Ba",137.327},
    {"Au",196.96657},{"Hg",200.59},{"Pb",207.2}
};

static bool atomic_mass_lookup(char const *symbol, double *mass)
{
    unsigned int i;
    for(i=0;i<sizeof(common_masses)/sizeof(common_masses[0]);i++) {
        if(strcmp(symbol,common_masses[i].symbol)==0) {
            *mass=common_masses[i].mass;
            return true;
        }
    }
    return false;
}

static bool simple_formula_mass(char const *formula, double *mass_out)
{
    char const *p=formula;
    double total=0.0;
    bool any=false;

    while(*p) {
        char symbol[3]={0,0,0};
        char *end;
        long count=1;
        double mass;

        if(!isupper((unsigned char)*p)) return false;
        symbol[0]=*p++;
        if(islower((unsigned char)*p)) symbol[1]=*p++;

        if(!atomic_mass_lookup(symbol,&mass)) return false;

        if(isdigit((unsigned char)*p)) {
            count=strtol(p,&end,10);
            if(end==p || count<=0) return false;
            p=end;
        }

        total += mass*(double)count;
        any=true;
    }

    if(!any) return false;
    *mass_out=total;
    return true;
}

static char const *last_token_ptr(char const *text)
{
    char const *last=text;
    char const *p=text;
    while(*p) {
        if(isspace((unsigned char)*p) && p[1] && !isspace((unsigned char)p[1])) last=p+1;
        p++;
    }
    return last;
}

static bool try_chemistry_formula_tools(char const *prompt, char *out, size_t out_size)
{
    char lower[260];
    char formula[64];
    char const *tok;
    double mass;
    double v[8];
    int n;

    lowercase_into(prompt,lower,sizeof(lower));

    if(starts_with(lower,"molar mass ") || starts_with(lower,"formula mass ")) {
        tok=last_token_ptr(prompt);
        snprintf(formula,sizeof(formula),"%.63s",tok);
        trim_text(formula);
        if(simple_formula_mass(formula,&mass)) {
            snprintf(out,out_size,"Molar mass of %s = %.6g g/mol.",formula,mass);
        } else {
            snprintf(out,out_size,
                "I couldn't parse that simple formula. This standalone molar-mass tool supports common element symbols and numeric subscripts without parentheses.");
        }
        return true;
    }

    if(starts_with(lower,"moles from ") && strstr(lower," g ")) {
        n=extract_flexible_numbers(lower,v,8);
        tok=last_token_ptr(prompt);
        snprintf(formula,sizeof(formula),"%.63s",tok);
        trim_text(formula);
        if(n>=1 && simple_formula_mass(formula,&mass)) {
            snprintf(out,out_size,"Moles = mass/molar_mass = %.12g mol.",v[0]/mass);
        } else {
            snprintf(out,out_size,"Use: moles from <grams> g <simple formula>, for example: moles from 18 g H2O.");
        }
        return true;
    }

    if(starts_with(lower,"grams from ") && strstr(lower," mol ")) {
        n=extract_flexible_numbers(lower,v,8);
        tok=last_token_ptr(prompt);
        snprintf(formula,sizeof(formula),"%.63s",tok);
        trim_text(formula);
        if(n>=1 && simple_formula_mass(formula,&mass)) {
            snprintf(out,out_size,"Mass = moles*molar_mass = %.12g g.",v[0]*mass);
        } else {
            snprintf(out,out_size,"Use: grams from <moles> mol <simple formula>, for example: grams from 2 mol CO2.");
        }
        return true;
    }

    if((starts_with(lower,"particles from ") || starts_with(lower,"molecules from ")) &&
       strstr(lower," mol")) {
        n=extract_flexible_numbers(lower,v,8);
        if(n>=1) {
            snprintf(out,out_size,"Particles = moles*N_A = %.12g",v[0]*6.02214076e23);
            return true;
        }
    }

    return false;
}

static bool parse_binary_u64(char const *bits, unsigned long long *value, int *width)
{
    unsigned long long v=0;
    int n=0;
    while(*bits) {
        if(*bits!='0' && *bits!='1') return false;
        if(n>=64) return false;
        v=(v<<1) | (unsigned long long)(*bits-'0');
        bits++;
        n++;
    }
    if(n==0) return false;
    *value=v;
    if(width) *width=n;
    return true;
}

static void format_binary_u64(unsigned long long value, int width, char *out, size_t out_size)
{
    int i;
    size_t used=0;
    if(width<1) width=1;
    if(width>64) width=64;
    for(i=width-1;i>=0 && used+1<out_size;i--) {
        out[used++]=(char)('0'+((value>>i)&1ULL));
    }
    out[used]='\0';
}

static bool try_cs_bit_tools(char const *prompt, char *out, size_t out_size)
{
    char lower[260];
    char a[80],b[80],bits[80];
    unsigned long long va,vb;
    int wa,wb,width;

    lowercase_into(prompt,lower,sizeof(lower));

    if(sscanf(lower,"binary and %79s %79s",a,b)==2) {
        if(!parse_binary_u64(a,&va,&wa) || !parse_binary_u64(b,&vb,&wb)) return false;
        width=wa>wb?wa:wb;
        format_binary_u64(va&vb,width,bits,sizeof(bits));
        snprintf(out,out_size,"Binary AND = %s",bits);
        return true;
    }

    if(sscanf(lower,"binary or %79s %79s",a,b)==2) {
        if(!parse_binary_u64(a,&va,&wa) || !parse_binary_u64(b,&vb,&wb)) return false;
        width=wa>wb?wa:wb;
        format_binary_u64(va|vb,width,bits,sizeof(bits));
        snprintf(out,out_size,"Binary OR = %s",bits);
        return true;
    }

    if(sscanf(lower,"binary xor %79s %79s",a,b)==2) {
        if(!parse_binary_u64(a,&va,&wa) || !parse_binary_u64(b,&vb,&wb)) return false;
        width=wa>wb?wa:wb;
        format_binary_u64(va^vb,width,bits,sizeof(bits));
        snprintf(out,out_size,"Binary XOR = %s",bits);
        return true;
    }

    if(sscanf(lower,"binary not %79s",a)==1) {
        unsigned long long mask;
        if(!parse_binary_u64(a,&va,&wa)) return false;
        mask = wa==64 ? ~0ULL : ((1ULL<<wa)-1ULL);
        format_binary_u64((~va)&mask,wa,bits,sizeof(bits));
        snprintf(out,out_size,"Binary NOT = %s",bits);
        return true;
    }

    if(starts_with(lower,"ascii ")) {
        char const *p=prompt+6;
        while(*p && isspace((unsigned char)*p)) p++;
        if(*p && !p[1]) {
            snprintf(out,out_size,"ASCII code for '%c' = %u",*p,(unsigned int)(unsigned char)*p);
            return true;
        }
    }

    if(starts_with(lower,"char ")) {
        long code;
        char *end;
        code=strtol(prompt+5,&end,10);
        if(end!=prompt+5 && code>=0 && code<=127) {
            snprintf(out,out_size,"ASCII %ld = '%c'",code,(char)code);
            return true;
        }
    }

    return false;
}


static bool try_geometry_extra(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=2 && (strstr(text,"third angle") || strstr(text,"missing angle"))) {
        double angle=180.0-v[0]-v[1];
        if(angle<=0.0) snprintf(out,out_size,"Those two angles do not leave a positive third angle in a triangle.");
        else snprintf(out,out_size,"Third triangle angle = 180 - %.12g - %.12g = %.12g degrees.",v[0],v[1],angle);
        return true;
    }

    if(n>=1 && (strstr(text,"interior angle sum") || strstr(text,"sum of interior angles"))) {
        int sides=(int)v[0];
        if(sides<3) snprintf(out,out_size,"A polygon needs at least 3 sides.");
        else snprintf(out,out_size,"Interior-angle sum = (n-2)*180 = %d degrees.",(sides-2)*180);
        return true;
    }

    if(n>=1 && strstr(text,"regular polygon") && strstr(text,"interior angle")) {
        int sides=(int)v[0];
        if(sides<3) snprintf(out,out_size,"A polygon needs at least 3 sides.");
        else snprintf(out,out_size,"Each regular interior angle = (n-2)*180/n = %.12g degrees.",
                      (double)(sides-2)*180.0/(double)sides);
        return true;
    }

    if(n>=1 && strstr(text,"exterior angle") && (strstr(text,"regular") || strstr(text,"polygon"))) {
        int sides=(int)v[0];
        if(sides<3) snprintf(out,out_size,"A polygon needs at least 3 sides.");
        else snprintf(out,out_size,"Each regular exterior angle = 360/n = %.12g degrees.",360.0/(double)sides);
        return true;
    }

    if(n>=4 && (starts_with(text,"line through ") || strstr(text,"equation of line through"))) {
        double x1=v[0],y1=v[1],x2=v[2],y2=v[3];
        if(approx_equal(x1,x2)) {
            snprintf(out,out_size,"Vertical line: x = %.12g",x1);
        } else {
            double m=(y2-y1)/(x2-x1);
            double b=y1-m*x1;
            snprintf(out,out_size,"Line through the points: y = %.12gx %+.12g",m,b);
        }
        return true;
    }

    return false;
}

static bool try_kinematics_tools(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);
    const double g=9.80665;

    if(n>=3 && (starts_with(text,"final velocity ") || strstr(text,"final velocity from"))) {
        snprintf(out,out_size,"v = v0 + a*t = %.12g",v[0]+v[1]*v[2]);
        return true;
    }

    if(n>=3 && (starts_with(text,"kinematic distance ") || starts_with(text,"displacement "))) {
        snprintf(out,out_size,"Delta x = v0*t + 1/2*a*t^2 = %.12g",v[0]*v[2]+0.5*v[1]*v[2]*v[2]);
        return true;
    }

    if(n>=1 && strstr(text,"free fall time") && strstr(text,"height")) {
        if(v[0]<0.0) snprintf(out,out_size,"Height must be nonnegative.");
        else snprintf(out,out_size,"Free-fall time from rest t=sqrt(2h/g) = %.12g s",sqrt(2.0*v[0]/g));
        return true;
    }

    if(n>=3 && strstr(text,"acceleration") && strstr(text,"velocity") && strstr(text,"time")) {
        if(approx_equal(v[2],0.0)) snprintf(out,out_size,"Elapsed time cannot be zero.");
        else snprintf(out,out_size,"a=(vf-vi)/t = %.12g",(v[1]-v[0])/v[2]);
        return true;
    }

    return false;
}

static bool try_business_tools(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=2 && starts_with(text,"profit ") && strstr(text,"revenue") && strstr(text,"cost")) {
        snprintf(out,out_size,"Profit = revenue - cost = %.12g",v[0]-v[1]);
        return true;
    }

    if(n>=3 && (strstr(text,"break even") || strstr(text,"break-even")) &&
       strstr(text,"fixed") && strstr(text,"price") && strstr(text,"variable")) {
        double contribution=v[1]-v[2];
        if(contribution<=0.0) snprintf(out,out_size,"Price must exceed variable cost for a finite unit break-even point.");
        else snprintf(out,out_size,"Break-even units = fixed_cost/(price-variable_cost) = %.12g",v[0]/contribution);
        return true;
    }

    if(n>=2 && starts_with(text,"roi ") && strstr(text,"profit") && strstr(text,"investment")) {
        if(approx_equal(v[1],0.0)) snprintf(out,out_size,"Investment cannot be zero for ROI.");
        else snprintf(out,out_size,"ROI = profit/investment * 100 = %.12g%%",100.0*v[0]/v[1]);
        return true;
    }

    if(n>=2 && strstr(text,"profit margin") && strstr(text,"revenue")) {
        if(approx_equal(v[1],0.0)) snprintf(out,out_size,"Revenue cannot be zero for profit margin.");
        else snprintf(out,out_size,"Profit margin = profit/revenue * 100 = %.12g%%",100.0*v[0]/v[1]);
        return true;
    }

    if(n>=2 && strstr(text,"markup") && strstr(text,"cost") && strstr(text,"price")) {
        if(approx_equal(v[0],0.0)) snprintf(out,out_size,"Cost cannot be zero for markup percentage.");
        else snprintf(out,out_size,"Markup = (price-cost)/cost * 100 = %.12g%%",100.0*(v[1]-v[0])/v[0]);
        return true;
    }

    return false;
}

static bool try_unit_rate_word_problem(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=3 && strstr(text,"cost") && strstr(text,"how much")) {
        if(approx_equal(v[0],0.0)) snprintf(out,out_size,"The original item count cannot be zero.");
        else snprintf(out,out_size,
            "Unit-rate method: %.12g/%.12g * %.12g = %.12g",
            v[1],v[0],v[2],v[1]/v[0]*v[2]);
        return true;
    }

    if(n>=2 && strstr(text,"miles") && strstr(text,"hours") &&
       (strstr(text,"how fast") || strstr(text,"average speed"))) {
        if(approx_equal(v[1],0.0)) snprintf(out,out_size,"Elapsed time cannot be zero.");
        else snprintf(out,out_size,"Average speed = distance/time = %.12g mph",v[0]/v[1]);
        return true;
    }

    if(n>=3 && strstr(text,"per ") && (strstr(text,"how many") || strstr(text,"total"))) {
        snprintf(out,out_size,"Rate calculation: %.12g * %.12g = %.12g",v[0],v[1],v[0]*v[1]);
        return true;
    }

    return false;
}


static bool try_relationship_word_problem(char const *text, char *out, size_t out_size)
{
    double v[10];
    int n=extract_flexible_numbers(text,v,10);

    if(n>=2 && strstr(text,"years older than") &&
       (strstr(text,"how old") || strstr(text,"age"))) {
        snprintf(out,out_size,
            "Age relationship: %.12g + %.12g = %.12g years old.",
            v[1],v[0],v[1]+v[0]);
        return true;
    }

    if(n>=2 && strstr(text,"years younger than") &&
       (strstr(text,"how old") || strstr(text,"age"))) {
        snprintf(out,out_size,
            "Age relationship: %.12g - %.12g = %.12g years old.",
            v[1],v[0],v[1]-v[0]);
        return true;
    }

    if(n>=2 && strstr(text,"sum") && strstr(text,"difference") &&
       (strstr(text,"two numbers") || strstr(text,"numbers"))) {
        double sum=v[0], diff=v[1];
        snprintf(out,out_size,
            "If x+y=%.12g and x-y=%.12g, the numbers are %.12g and %.12g.",
            sum,diff,(sum+diff)/2.0,(sum-diff)/2.0);
        return true;
    }

    if(n>=3 && strstr(text,"split") && strstr(text,"ratio")) {
        double total=v[0], a=v[1], b=v[2];
        if(a<0.0 || b<0.0 || approx_equal(a+b,0.0)) {
            snprintf(out,out_size,"Ratio parts must have a positive total weight.");
        } else {
            snprintf(out,out_size,
                "Split %.12g in ratio %.12g:%.12g -> %.12g and %.12g.",
                total,a,b,total*a/(a+b),total*b/(a+b));
        }
        return true;
    }

    if(n>=2 && (strstr(text,"grade") || strstr(text,"score") ||
       strstr(text,"percent score")) && strstr(text,"out of")) {
        if(approx_equal(v[1],0.0)) snprintf(out,out_size,"Total possible points cannot be zero.");
        else snprintf(out,out_size,
            "Score = %.12g/%.12g * 100 = %.12g%%.",
            v[0],v[1],100.0*v[0]/v[1]);
        return true;
    }

    if(n>=2 && (strstr(text,"per hour") || strstr(text,"an hour")) &&
       (strstr(text,"earn") || strstr(text,"pay") || strstr(text,"wage")) &&
       (strstr(text,"hours") || strstr(text,"worked"))) {
        snprintf(out,out_size,
            "Pay = hourly rate * hours = %.12g * %.12g = %.12g.",
            v[0],v[1],v[0]*v[1]);
        return true;
    }

    if(n>=2 && (strstr(text,"unit price") || strstr(text,"cost per") ||
       strstr(text,"price per")) {
        if(approx_equal(v[1],0.0)) snprintf(out,out_size,"Item count cannot be zero.");
        else snprintf(out,out_size,
            "Unit price = total cost / quantity = %.12g / %.12g = %.12g.",
            v[0],v[1],v[0]/v[1]);
        return true;
    }

    if(n>=4 && (strstr(text,"weighted average") || strstr(text,"weighted mean"))) {
        double sum=0.0, weights=0.0;
        int i;
        if(n%2!=0) {
            snprintf(out,out_size,"Weighted average expects value/weight pairs.");
            return true;
        }
        for(i=0;i<n;i+=2) {
            double w=v[i+1];
            if(w>1.0) w/=100.0;
            sum+=v[i]*w;
            weights+=w;
        }
        if(approx_equal(weights,0.0)) snprintf(out,out_size,"Total weight cannot be zero.");
        else snprintf(out,out_size,"Weighted average = %.12g",sum/weights);
        return true;
    }

    if(n>=2 && strstr(text,"each") && (strstr(text,"how many total") ||
       strstr(text,"how many altogether") || strstr(text,"total"))) {
        snprintf(out,out_size,
            "Equal groups: %.12g * %.12g = %.12g total.",
            v[0],v[1],v[0]*v[1]);
        return true;
    }

    return false;
}

static bool try_science_formula(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n=extract_flexible_numbers(text,v,8);

    if(n>=2 && strstr(text,"kinetic energy")) {
        snprintf(out,out_size,"K=1/2*m*v^2 = %.12g J",0.5*v[0]*v[1]*v[1]);
        return true;
    }
    if(n>=2 && strstr(text,"momentum")) {
        snprintf(out,out_size,"p=mv = %.12g kg*m/s",v[0]*v[1]);
        return true;
    }
    if(n>=2 && strstr(text,"density") && !strstr(text,"population")) {
        if(v[1]==0.0) snprintf(out,out_size,"Density is undefined for zero volume.");
        else snprintf(out,out_size,"density=mass/volume = %.12g",v[0]/v[1]);
        return true;
    }
    if(n>=2 && strstr(text,"power") && strstr(text,"voltage") && strstr(text,"current")) {
        snprintf(out,out_size,"Electrical power P=VI = %.12g W",v[0]*v[1]);
        return true;
    }
    if(n>=2 && strstr(text,"wave speed")) {
        snprintf(out,out_size,"Wave speed v=f*lambda = %.12g",v[0]*v[1]);
        return true;
    }
    if(n>=2 && strstr(text,"work") && strstr(text,"force") && strstr(text,"distance")) {
        snprintf(out,out_size,"For force parallel to motion, W=F*d = %.12g J",v[0]*v[1]);
        return true;
    }

    return false;
}

static bool parse_linear_side(char const *text, double *xcoef, double *constant)
{
    double a = 0.0;
    double b = 0.0;
    double sign = 1.0;
    bool got_term = false;

    while(*text) {
        char *end;
        double value;

        while(*text && isspace((unsigned char)*text)) text++;
        if(!*text) break;

        if(*text == '+') {
            sign = 1.0;
            text++;
            continue;
        }
        if(*text == '-') {
            sign = -1.0;
            text++;
            continue;
        }

        while(*text && isspace((unsigned char)*text)) text++;

        if(*text == 'x' || *text == 'X') {
            a += sign;
            text++;
            sign = 1.0;
            got_term = true;
            continue;
        }

        value = strtod(text, &end);
        if(end == text) return false;
        text = end;

        while(*text && isspace((unsigned char)*text)) text++;
        if(*text == '*') {
            text++;
            while(*text && isspace((unsigned char)*text)) text++;
        }

        if(*text == 'x' || *text == 'X') {
            a += sign * value;
            text++;
        }
        else {
            b += sign * value;
        }

        sign = 1.0;
        got_term = true;

        while(*text && isspace((unsigned char)*text)) text++;
        if(*text && *text != '+' && *text != '-') return false;
    }

    if(!got_term) return false;
    *xcoef = a;
    *constant = b;
    return true;
}

static bool try_linear_equation(char const *text, char *out, size_t out_size)
{
    char equation[180];
    char *eq;
    char *left;
    char *right;
    double a1, b1, a2, b2;
    double denom, rhs, x;

    if(starts_with(text, "solve ")) {
        snprintf(equation, sizeof(equation), "%s", text + 6);
    }
    else {
        if(!strchr(text, '=') || (!strchr(text, 'x') && !strchr(text, 'X'))) return false;
        snprintf(equation, sizeof(equation), "%s", text);
    }

    eq = strchr(equation, '=');
    if(!eq || strchr(eq + 1, '=')) return false;

    *eq = '\0';
    left = equation;
    right = eq + 1;

    if(!strchr(left, 'x') && !strchr(left, 'X') &&
       !strchr(right, 'x') && !strchr(right, 'X')) return false;

    if(!parse_linear_side(left, &a1, &b1) ||
       !parse_linear_side(right, &a2, &b2)) return false;

    denom = a1 - a2;
    rhs = b2 - b1;

    if(fabs(denom) < 1e-12) {
        if(fabs(rhs) < 1e-12) {
            snprintf(out, out_size,
                "Both sides simplify to the same expression, so there are infinitely many solutions.");
        }
        else {
            snprintf(out, out_size,
                "The x-terms cancel but the constants disagree, so there is no solution.");
        }
        return true;
    }

    x = rhs / denom;
    snprintf(out, out_size,
        "Linear equation: (%.12g)x + %.12g = (%.12g)x + %.12g\n"
        "(%.12g)x = %.12g\nx = %.12g",
        a1, b1, a2, b2, denom, rhs, x);
    return true;
}


static void flip_inequality(char op[3])
{
    if(strcmp(op,"<")==0) strcpy(op,">");
    else if(strcmp(op,">")==0) strcpy(op,"<");
    else if(strcmp(op,"<=")==0) strcpy(op,">=");
    else if(strcmp(op,">=")==0) strcpy(op,"<=");
}

static bool try_linear_inequality(char const *text, char *out, size_t out_size)
{
    char expr[220];
    char *pos=NULL;
    char op[3]="";
    char *right;
    double a1,b1,a2,b2;
    double coeff,rhs,value;
    bool truth=false;

    if(starts_with(text,"solve ")) snprintf(expr,sizeof(expr),"%s",text+6);
    else snprintf(expr,sizeof(expr),"%s",text);

    pos=strstr(expr,"<=");
    if(pos) strcpy(op,"<=");
    if(!pos) { pos=strstr(expr,">="); if(pos) strcpy(op,">="); }
    if(!pos) { pos=strchr(expr,'<'); if(pos) strcpy(op,"<"); }
    if(!pos) { pos=strchr(expr,'>'); if(pos) strcpy(op,">"); }

    if(!pos || (!strchr(expr,'x') && !strchr(expr,'X'))) return false;

    right=pos+strlen(op);
    *pos='\0';

    if(!parse_linear_side(expr,&a1,&b1) || !parse_linear_side(right,&a2,&b2)) return false;

    coeff=a1-a2;
    rhs=b2-b1;

    if(fabs(coeff)<1e-12) {
        if(strcmp(op,"<")==0) truth=0.0<rhs;
        else if(strcmp(op,">")==0) truth=0.0>rhs;
        else if(strcmp(op,"<=")==0) truth=0.0<=rhs;
        else if(strcmp(op,">=")==0) truth=0.0>=rhs;
        snprintf(out,out_size,"%s: the inequality simplifies to %.12g %s %.12g.",
                 truth?"All real x work":"No real x works",0.0,op,rhs);
        return true;
    }

    if(coeff<0.0) flip_inequality(op);
    value=rhs/coeff;

    snprintf(out,out_size,
        "Linear inequality: %.12gx %s %.12g\nSolution: x %s %.12g",
        coeff, coeff<0.0 ? (strcmp(op,"<")==0?">":strcmp(op,">")==0?"<":strcmp(op,"<=")==0?">=":"<=") : op,
        rhs,op,value);
    return true;
}

static bool try_unit_conversion(char const *text, char *out, size_t out_size)
{
    double v[4];
    int n = extract_flexible_numbers(text, v, 4);
    double x;

    if(n < 1) return false;
    x = v[0];

    if((strstr(text, "cm to m") || strstr(text, "centimeters to meters") ||
        strstr(text, "centimetres to metres"))) {
        snprintf(out, out_size, "%.12g cm = %.12g m", x, x / 100.0);
        return true;
    }
    if((strstr(text, "m to cm") || strstr(text, "meters to centimeters") ||
        strstr(text, "metres to centimetres"))) {
        snprintf(out, out_size, "%.12g m = %.12g cm", x, x * 100.0);
        return true;
    }
    if(strstr(text, "km to m") || strstr(text, "kilometers to meters")) {
        snprintf(out, out_size, "%.12g km = %.12g m", x, x * 1000.0);
        return true;
    }
    if(strstr(text, "m to km") || strstr(text, "meters to kilometers")) {
        snprintf(out, out_size, "%.12g m = %.12g km", x, x / 1000.0);
        return true;
    }
    if(strstr(text, "inches to feet") || strstr(text, "inch to feet") ||
       strstr(text, "in to ft")) {
        snprintf(out, out_size, "%.12g in = %.12g ft", x, x / 12.0);
        return true;
    }
    if(strstr(text, "feet to inches") || strstr(text, "foot to inches") ||
       strstr(text, "ft to in")) {
        snprintf(out, out_size, "%.12g ft = %.12g in", x, x * 12.0);
        return true;
    }
    if(strstr(text, "feet to miles") || strstr(text, "ft to mi")) {
        snprintf(out, out_size, "%.12g ft = %.12g mi", x, x / 5280.0);
        return true;
    }
    if(strstr(text, "miles to feet") || strstr(text, "mi to ft")) {
        snprintf(out, out_size, "%.12g mi = %.12g ft", x, x * 5280.0);
        return true;
    }
    if(strstr(text, "ounces to pounds") || strstr(text, "oz to lb")) {
        snprintf(out, out_size, "%.12g oz = %.12g lb", x, x / 16.0);
        return true;
    }
    if(strstr(text, "pounds to ounces") || strstr(text, "lb to oz")) {
        snprintf(out, out_size, "%.12g lb = %.12g oz", x, x * 16.0);
        return true;
    }
    if(strstr(text, "seconds to minutes") || strstr(text, "sec to min")) {
        snprintf(out, out_size, "%.12g s = %.12g min", x, x / 60.0);
        return true;
    }
    if(strstr(text, "minutes to seconds") || strstr(text, "min to sec")) {
        snprintf(out, out_size, "%.12g min = %.12g s", x, x * 60.0);
        return true;
    }
    if(strstr(text, "minutes to hours") || strstr(text, "min to hr")) {
        snprintf(out, out_size, "%.12g min = %.12g hr", x, x / 60.0);
        return true;
    }
    if(strstr(text, "hours to minutes") || strstr(text, "hr to min")) {
        snprintf(out, out_size, "%.12g hr = %.12g min", x, x * 60.0);
        return true;
    }
    if(strstr(text, "kilograms to pounds") || strstr(text, "kg to lb") || strstr(text, "kg to pounds")) {
        snprintf(out, out_size, "%.12g kg = %.12g lb", x, x * 2.20462262185);
        return true;
    }
    if(strstr(text, "pounds to kilograms") || strstr(text, "lb to kg") || strstr(text, "pounds to kg")) {
        snprintf(out, out_size, "%.12g lb = %.12g kg", x, x / 2.20462262185);
        return true;
    }
    if(strstr(text, "grams to kilograms") || strstr(text, "g to kg")) {
        snprintf(out, out_size, "%.12g g = %.12g kg", x, x / 1000.0);
        return true;
    }
    if(strstr(text, "kilograms to grams") || strstr(text, "kg to g")) {
        snprintf(out, out_size, "%.12g kg = %.12g g", x, x * 1000.0);
        return true;
    }
    if(strstr(text, "liters to milliliters") || strstr(text, "litres to millilitres") || strstr(text, "l to ml")) {
        snprintf(out, out_size, "%.12g L = %.12g mL", x, x * 1000.0);
        return true;
    }
    if(strstr(text, "milliliters to liters") || strstr(text, "millilitres to litres") || strstr(text, "ml to l")) {
        snprintf(out, out_size, "%.12g mL = %.12g L", x, x / 1000.0);
        return true;
    }
    if(strstr(text, "inches to centimeters") || strstr(text, "inches to centimetres") || strstr(text, "in to cm")) {
        snprintf(out, out_size, "%.12g in = %.12g cm", x, x * 2.54);
        return true;
    }
    if(strstr(text, "centimeters to inches") || strstr(text, "centimetres to inches") || strstr(text, "cm to in")) {
        snprintf(out, out_size, "%.12g cm = %.12g in", x, x / 2.54);
        return true;
    }
    if(strstr(text, "feet to meters") || strstr(text, "feet to metres") || strstr(text, "ft to m")) {
        snprintf(out, out_size, "%.12g ft = %.12g m", x, x * 0.3048);
        return true;
    }
    if(strstr(text, "meters to feet") || strstr(text, "metres to feet") || strstr(text, "m to ft")) {
        snprintf(out, out_size, "%.12g m = %.12g ft", x, x / 0.3048);
        return true;
    }
    if(strstr(text, "miles to kilometers") || strstr(text, "mi to km")) {
        snprintf(out, out_size, "%.12g mi = %.12g km", x, x * 1.609344);
        return true;
    }
    if(strstr(text, "kilometers to miles") || strstr(text, "km to mi")) {
        snprintf(out, out_size, "%.12g km = %.12g mi", x, x / 1.609344);
        return true;
    }
    if(strstr(text, "mph to kph") || strstr(text, "mph to km/h")) {
        snprintf(out, out_size, "%.12g mph = %.12g km/h", x, x * 1.609344);
        return true;
    }
    if(strstr(text, "kph to mph") || strstr(text, "km/h to mph")) {
        snprintf(out, out_size, "%.12g km/h = %.12g mph", x, x / 1.609344);
        return true;
    }
    if(strstr(text, "days to hours") || strstr(text, "day to hours")) {
        snprintf(out, out_size, "%.12g days = %.12g hours", x, x * 24.0);
        return true;
    }
    if(strstr(text, "hours to days") || strstr(text, "hour to days")) {
        snprintf(out, out_size, "%.12g hours = %.12g days", x, x / 24.0);
        return true;
    }
    if(strstr(text, "degrees to radians") || strstr(text, "deg to rad")) {
        snprintf(out,out_size,"%.12g degrees = %.12g radians",x,x*3.14159265358979323846/180.0);
        return true;
    }
    if(strstr(text, "radians to degrees") || strstr(text, "rad to deg")) {
        snprintf(out,out_size,"%.12g radians = %.12g degrees",x,x*180.0/3.14159265358979323846);
        return true;
    }

    return false;
}

static bool contains_alnum(char const *text)
{
    while(*text) {
        if(isalnum((unsigned char)*text)) return true;
        text++;
    }
    return false;
}


static void trim_text(char *text)
{
    char *start = text;
    size_t len;

    while(*start && isspace((unsigned char)*start)) start++;
    if(start != text) memmove(text, start, strlen(start) + 1);

    len = strlen(text);
    while(len > 0 && (isspace((unsigned char)text[len - 1]) ||
          text[len - 1] == '?' || text[len - 1] == '!' ||
          text[len - 1] == '.' || text[len - 1] == ',')) {
        text[--len] = '\0';
    }
}

static bool same_topic(char const *x, char const *y)
{
    return strcmp(x, y) == 0;
}

static bool try_known_comparison(char const *text, char *out, size_t out_size)
{
    char topic[180];
    char left[90];
    char right[90];
    char *sep = NULL;
    size_t sep_len = 0;
    unsigned int i;

    snprintf(topic, sizeof(topic), "%s", text);
    trim_text(topic);

    if(starts_with(topic, "compare ")) memmove(topic, topic + 8, strlen(topic + 8) + 1);
    else if(starts_with(topic, "difference between ")) memmove(topic, topic + 19, strlen(topic + 19) + 1);

    sep = strstr(topic, " versus ");
    if(sep) sep_len = 8;
    if(!sep) {
        sep = strstr(topic, " vs ");
        if(sep) sep_len = 4;
    }
    if(!sep) {
        sep = strstr(topic, " and ");
        if(sep) sep_len = 5;
    }

    if(!sep) return false;

    *sep = '\0';
    snprintf(left, sizeof(left), "%.89s", topic);
    snprintf(right, sizeof(right), "%.89s", sep + sep_len);
    trim_text(left);
    trim_text(right);

    if(!left[0] || !right[0]) return false;

    for(i = 0; i < sizeof(comparisons)/sizeof(comparisons[0]); i++) {
        if((same_topic(left, comparisons[i].a) && same_topic(right, comparisons[i].b)) ||
           (same_topic(left, comparisons[i].b) && same_topic(right, comparisons[i].a))) {
            snprintf(out, out_size, "%s", comparisons[i].answer);
            return true;
        }
    }

    return false;
}

static bool try_prefixed_expression(char const *text, char *out, size_t out_size)
{
    char expr[180];
    char const *p = NULL;
    size_t len;
    double value;

    if(starts_with(text, "calculate ")) p = text + 10;
    else if(starts_with(text, "evaluate ")) p = text + 9;
    else if(starts_with(text, "compute ")) p = text + 8;
    else if(starts_with(text, "what is ")) p = text + 8;
    else if(starts_with(text, "what's ")) p = text + 7;
    else if(starts_with(text, "whats ")) p = text + 6;

    if(!p) return false;

    snprintf(expr, sizeof(expr), "%s", p);
    trim_text(expr);
    len = strlen(expr);

    if(len == 0 || !looks_like_expression(expr)) return false;

    if(eval_expression(expr, &value)) {
        snprintf(out, out_size, "%s = %.12g", expr, value);
        return true;
    }

    return false;
}


static int basic_number_word(char const *word)
{
    static char const *ones[] = {
        "zero","one","two","three","four","five","six","seven","eight","nine",
        "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
        "seventeen","eighteen","nineteen"
    };
    static char const *tens_words[] = {
        "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
    };
    int i;

    for(i = 0; i < 20; i++) {
        if(strcmp(word, ones[i]) == 0) return i;
    }

    for(i = 0; i < 8; i++) {
        if(strcmp(word, tens_words[i]) == 0) return (i + 2) * 10;
    }

    return -1;
}

static bool token_is_number_word(char const *word)
{
    return basic_number_word(word) >= 0 ||
           strcmp(word, "hundred") == 0 ||
           strcmp(word, "thousand") == 0;
}

static int extract_flexible_numbers(char const *text, double *values, int cap)
{
    int count = 0;
    char copy[700];
    char *p;

    snprintf(copy, sizeof(copy), "%s", text);

    p = copy;
    while(*p && count < cap) {
        char *end;
        double numeric;

        if(isdigit((unsigned char)*p) || *p == '.' ||
           ((*p == '+' || *p == '-') &&
            (isdigit((unsigned char)p[1]) || p[1] == '.'))) {
            numeric = strtod(p, &end);
            if(end != p) {
                values[count++] = numeric;
                p = end;
                continue;
            }
        }

        if(isalpha((unsigned char)*p)) {
            char token[32];
            size_t len = 0;
            long current = 0;
            long total = 0;
            bool got = false;

            while(*p && count < cap) {
                char *q = p;
                int val;

                while(*q && !isalpha((unsigned char)*q)) q++;
                if(!*q) {
                    p = q;
                    break;
                }

                len = 0;
                while(q[len] && isalpha((unsigned char)q[len]) &&
                      len + 1 < sizeof(token)) {
                    token[len] = (char)tolower((unsigned char)q[len]);
                    len++;
                }
                token[len] = '\0';

                val = basic_number_word(token);

                if(val >= 0) {
                    current += val;
                    got = true;
                }
                else if(strcmp(token, "hundred") == 0 && got) {
                    if(current == 0) current = 1;
                    current *= 100;
                }
                else if(strcmp(token, "thousand") == 0 && got) {
                    if(current == 0) current = 1;
                    total += current * 1000;
                    current = 0;
                }
                else if(strcmp(token, "and") == 0 && got) {
                    /* allow: one hundred and five */
                }
                else {
                    if(got) {
                        p = q;
                        break;
                    }
                    p = q + len;
                    break;
                }

                p = q + len;

                {
                    char look[32];
                    char *r = p;
                    size_t k = 0;
                    while(*r && !isalpha((unsigned char)*r) &&
                          !isdigit((unsigned char)*r)) r++;
                    while(r[k] && isalpha((unsigned char)r[k]) &&
                          k + 1 < sizeof(look)) {
                        look[k] = (char)tolower((unsigned char)r[k]);
                        k++;
                    }
                    look[k] = '\0';

                    if(!token_is_number_word(look) && strcmp(look, "and") != 0) {
                        break;
                    }
                }
            }

            if(got) {
                values[count++] = (double)(total + current);
                continue;
            }

            continue;
        }

        p++;
    }

    return count;
}

static bool try_word_problem(char const *text, char *out, size_t out_size)
{
    double v[8];
    int n = extract_flexible_numbers(text, v, 8);
    double result;

    if(n >= 2 &&
       (strstr(text, "has ") || strstr(text, "have ") ||
        strstr(text, "had ") || strstr(text, "starts with ")) &&
       (strstr(text, "gets ") || strstr(text, "get ") ||
        strstr(text, "buys ") || strstr(text, "buy ") ||
        strstr(text, "receives ") || strstr(text, "receive ") ||
        strstr(text, "finds ") || strstr(text, "gains ") ||
        strstr(text, "adds "))) {
        result = v[0] + v[1];
        snprintf(out, out_size,
            "This looks like an addition word problem: %.12g + %.12g = %.12g.",
            v[0], v[1], result);
        return true;
    }

    if(n >= 2 &&
       (strstr(text, "has ") || strstr(text, "have ") ||
        strstr(text, "had ") || strstr(text, "starts with ")) &&
       (strstr(text, "gives ") || strstr(text, "give ") ||
        strstr(text, "loses ") || strstr(text, "lose ") ||
        strstr(text, "sells ") || strstr(text, "sell ") ||
        strstr(text, "uses ") || strstr(text, "spends ") ||
        strstr(text, "takes away "))) {
        result = v[0] - v[1];
        snprintf(out, out_size,
            "This looks like a subtraction word problem: %.12g - %.12g = %.12g.",
            v[0], v[1], result);
        return true;
    }

    if(n >= 2 &&
       (strstr(text, "each ") || strstr(text, "per group") ||
        strstr(text, "groups of ") || strstr(text, "rows of ")) &&
       (strstr(text, "groups") || strstr(text, "rows") ||
        strstr(text, "boxes") || strstr(text, "bags") ||
        strstr(text, "teams"))) {
        result = v[0] * v[1];
        snprintf(out, out_size,
            "This looks like equal-group multiplication: %.12g * %.12g = %.12g.",
            v[0], v[1], result);
        return true;
    }

    if(n >= 2 &&
       (strstr(text, "split ") || strstr(text, "shared ") ||
        strstr(text, "divide ") || strstr(text, "equally among") ||
        strstr(text, "each person"))) {
        if(v[1] == 0.0) {
            snprintf(out, out_size, "The word problem would require division by zero, which is undefined.");
        }
        else {
            result = v[0] / v[1];
            snprintf(out, out_size,
                "This looks like equal sharing/division: %.12g / %.12g = %.12g.",
                v[0], v[1], result);
        }
        return true;
    }

    if(n >= 2 && strstr(text, "miles per hour") &&
       (strstr(text, "for ") || strstr(text, "hours"))) {
        result = v[0] * v[1];
        snprintf(out, out_size,
            "Distance = rate*time = %.12g mph * %.12g h = %.12g miles.",
            v[0], v[1], result);
        return true;
    }

    if(n >= 2 && strstr(text, "miles") && strstr(text, "hours") &&
       (strstr(text, "speed") || strstr(text, "average speed") ||
        strstr(text, "rate"))) {
        if(v[1] == 0.0) {
            snprintf(out, out_size, "Average speed is undefined for zero elapsed time.");
        }
        else {
            result = v[0] / v[1];
            snprintf(out, out_size,
                "Average speed = distance/time = %.12g / %.12g = %.12g mph.",
                v[0], v[1], result);
        }
        return true;
    }

    if(n >= 2 && strstr(text, "percent increase")) {
        if(v[0] == 0.0) {
            snprintf(out, out_size, "Percent increase from zero is not defined by the usual (new-old)/old formula.");
        }
        else {
            result = (v[1] - v[0]) / v[0] * 100.0;
            snprintf(out, out_size,
                "Percent change = (new-old)/old * 100 = %.12g%%.", result);
        }
        return true;
    }

    if(n >= 2 && strstr(text, "percent decrease")) {
        if(v[0] == 0.0) {
            snprintf(out, out_size, "Percent decrease from zero is not defined by the usual (old-new)/old formula.");
        }
        else {
            result = (v[0] - v[1]) / v[0] * 100.0;
            snprintf(out, out_size,
                "Percent decrease = (old-new)/old * 100 = %.12g%%.", result);
        }
        return true;
    }

    return false;
}

static bool try_natural_math(char const *text, char *out, size_t out_size)
{
    double v[16];
    int n = extract_flexible_numbers(text, v, 16);
    double result;

    if((strstr(text, "square root of ") || starts_with(text, "sqrt of ")) && n >= 1) {
        if(v[0] < 0.0) {
            snprintf(out, out_size, "The real square root of %.12g is not defined.", v[0]);
        }
        else {
            snprintf(out, out_size, "sqrt(%.12g) = %.12g", v[0], sqrt(v[0]));
        }
        return true;
    }

    if(strstr(text, " percent of ") && n >= 2) {
        result = (v[0] / 100.0) * v[1];
        snprintf(out, out_size, "%.12g%% of %.12g = %.12g", v[0], v[1], result);
        return true;
    }

    if((strstr(text, " plus ") || strstr(text, " added to ")) && n >= 2) {
        snprintf(out, out_size, "%.12g + %.12g = %.12g", v[0], v[1], v[0] + v[1]);
        return true;
    }

    if((strstr(text, " minus ") || strstr(text, " subtract ")) && n >= 2) {
        snprintf(out, out_size, "%.12g - %.12g = %.12g", v[0], v[1], v[0] - v[1]);
        return true;
    }

    if((strstr(text, " times ") || strstr(text, " multiplied by ")) && n >= 2) {
        snprintf(out, out_size, "%.12g * %.12g = %.12g", v[0], v[1], v[0] * v[1]);
        return true;
    }

    if((strstr(text, " divided by ") || strstr(text, " over ")) && n >= 2) {
        if(v[1] == 0.0) {
            snprintf(out, out_size, "Division by zero is undefined.");
        }
        else {
            snprintf(out, out_size, "%.12g / %.12g = %.12g", v[0], v[1], v[0] / v[1]);
        }
        return true;
    }

    if((strstr(text, "power of ") || strstr(text, " raised to ")) && n >= 2) {
        snprintf(out, out_size, "%.12g ^ %.12g = %.12g", v[0], v[1], pow(v[0], v[1]));
        return true;
    }

    if((strstr(text, "average of ") || strstr(text, "mean of ")) && n >= 1) {
        int i;
        result = 0.0;
        for(i = 0; i < n; i++) result += v[i];
        result /= n;
        snprintf(out, out_size, "Mean of %d values = %.12g", n, result);
        return true;
    }

    if(strstr(text, "area") && strstr(text, "circle") &&
       (strstr(text, "radius") || strstr(text, " r ")) && n >= 1) {
        result = 3.14159265358979323846 * v[0] * v[0];
        snprintf(out, out_size, "Circle area = pi*r^2 = %.12g", result);
        return true;
    }

    if((strstr(text, "circumference") || strstr(text, "perimeter of a circle")) &&
       n >= 1) {
        result = 2.0 * 3.14159265358979323846 * v[0];
        snprintf(out, out_size, "Circumference = 2*pi*r = %.12g", result);
        return true;
    }

    if(strstr(text, "area") && strstr(text, "rectangle") && n >= 2) {
        snprintf(out, out_size, "Rectangle area = length*width = %.12g", v[0] * v[1]);
        return true;
    }

    if(strstr(text, "perimeter") && strstr(text, "rectangle") && n >= 2) {
        snprintf(out, out_size, "Rectangle perimeter = 2(L+W) = %.12g", 2.0 * (v[0] + v[1]));
        return true;
    }

    if(strstr(text, "area") && strstr(text, "triangle") && n >= 2) {
        snprintf(out, out_size, "Triangle area = 1/2*b*h = %.12g", 0.5 * v[0] * v[1]);
        return true;
    }

    if(strstr(text, "distance between") && n >= 4) {
        double dx = v[2] - v[0];
        double dy = v[3] - v[1];
        snprintf(out, out_size, "Distance = sqrt((x2-x1)^2+(y2-y1)^2) = %.12g",
                 sqrt(dx*dx + dy*dy));
        return true;
    }

    if(strstr(text, "midpoint") && n >= 4) {
        snprintf(out,out_size,"Midpoint = ((x1+x2)/2,(y1+y2)/2) = (%.12g, %.12g)",
                 (v[0]+v[2])/2.0,(v[1]+v[3])/2.0);
        return true;
    }

    if(strstr(text, "fahrenheit") && strstr(text, "celsius") && n >= 1) {
        result = (v[0] - 32.0) * 5.0 / 9.0;
        snprintf(out, out_size, "%.12g F = %.12g C", v[0], result);
        return true;
    }

    if(strstr(text, "celsius") && strstr(text, "fahrenheit") && n >= 1) {
        result = v[0] * 9.0 / 5.0 + 32.0;
        snprintf(out, out_size, "%.12g C = %.12g F", v[0], result);
        return true;
    }

    if(strstr(text, "volume") && (strstr(text, "rectangular prism") || strstr(text, "box")) && n >= 3) {
        snprintf(out,out_size,"Volume of rectangular prism = L*W*H = %.12g",v[0]*v[1]*v[2]);
        return true;
    }

    if(strstr(text, "volume") && strstr(text, "cylinder") && n >= 2) {
        snprintf(out,out_size,"Cylinder volume = pi*r^2*h = %.12g",
                 3.14159265358979323846*v[0]*v[0]*v[1]);
        return true;
    }

    if(strstr(text, "volume") && strstr(text, "sphere") && n >= 1) {
        snprintf(out,out_size,"Sphere volume = 4/3*pi*r^3 = %.12g",
                 (4.0/3.0)*3.14159265358979323846*v[0]*v[0]*v[0]);
        return true;
    }

    if(strstr(text, "surface area") && strstr(text, "sphere") && n >= 1) {
        snprintf(out,out_size,"Sphere surface area = 4*pi*r^2 = %.12g",
                 4.0*3.14159265358979323846*v[0]*v[0]);
        return true;
    }

    return false;
}

static char const *infer_subject_local(char const *text)
{
    if(strstr(text, "cell") || strstr(text, "dna") || strstr(text, "mitosis") ||
       strstr(text, "meiosis") || strstr(text, "photosynthesis") ||
       strstr(text, "ecosystem") || strstr(text, "genetic")) return "biology";

    if(strstr(text, "atom") || strstr(text, "mole") || strstr(text, "bond") ||
       strstr(text, "acid") || strstr(text, "base") || strstr(text, "stoichi") ||
       strstr(text, "periodic")) return "chemistry";

    if(strstr(text, "force") || strstr(text, "velocity") || strstr(text, "acceleration") ||
       strstr(text, "momentum") || strstr(text, "energy") || strstr(text, "voltage") ||
       strstr(text, "current") || strstr(text, "wave")) return "physics";

    if(strstr(text, "derivative") || strstr(text, "integral") || strstr(text, "limit") ||
       strstr(text, "calculus")) return "calculus";

    if(strstr(text, "triangle") || strstr(text, "circle") || strstr(text, "angle") ||
       strstr(text, "area") || strstr(text, "perimeter")) return "geometry";

    if(strstr(text, "equation") || strstr(text, "slope") || strstr(text, "quadratic") ||
       strstr(text, "polynomial") || strstr(text, "inequality")) return "algebra_1";

    if(strstr(text, "mean") || strstr(text, "median") || strstr(text, "probability") ||
       strstr(text, "standard deviation") || strstr(text, "correlation")) return "statistics";

    if(strstr(text, "theme") || strstr(text, "metaphor") || strstr(text, "symbol") ||
       strstr(text, "irony") || strstr(text, "literature")) return "literature";

    if(strstr(text, "essay") || phrase_match(text, "thesis") || strstr(text, "paragraph") ||
       strstr(text, "citation") || strstr(text, "rhetoric")) return "composition";

    if(strstr(text, "constitution") || strstr(text, "federalism") ||
       strstr(text, "amendment") || strstr(text, "congress") ||
       strstr(text, "government")) return "government";

    if(strstr(text, "war") || strstr(text, "revolution") || strstr(text, "renaissance") ||
       strstr(text, "history") || strstr(text, "empire")) return "history";

    if(strstr(text, "inflation") || strstr(text, "gdp") || strstr(text, "supply") ||
       strstr(text, "demand") || strstr(text, "market")) return "economics";

    if(strstr(text, "marketing") || strstr(text, "revenue") || strstr(text, "profit") ||
       strstr(text, "business") || strstr(text, "branding")) return "business";

    if(strstr(text, "code") || strstr(text, "algorithm") || strstr(text, "array") ||
       strstr(text, "loop") || strstr(text, "function") || strstr(text, "database") ||
       strstr(text, "programming")) return "computer_science";

    if(strstr(text, "music") || strstr(text, "chord") || strstr(text, "rhythm") ||
       strstr(text, "tempo")) return "music";

    if(strstr(text, "photography") || phrase_match(text, "photo") ||
       strstr(text, "aperture") || strstr(text, "shutter")) return "photography";
    if(strstr(text, "nutrition") || strstr(text, "vitamin") || strstr(text, "calorie")) return "nutrition";
    if(strstr(text, "latitude") || strstr(text, "longitude") || strstr(text, "map")) return "geography";

    return "auto";
}

static int edit_distance_small(char const *a, char const *b)
{
    unsigned char prev[64], curr[64];
    size_t la = strlen(a), lb = strlen(b);
    size_t i, j;

    if(la >= sizeof(prev) || lb >= sizeof(prev)) return 99;

    for(j = 0; j <= lb; j++) prev[j] = (unsigned char)j;

    for(i = 1; i <= la; i++) {
        curr[0] = (unsigned char)i;
        for(j = 1; j <= lb; j++) {
            int cost = a[i - 1] == b[j - 1] ? 0 : 1;
            int del = prev[j] + 1;
            int ins = curr[j - 1] + 1;
            int sub = prev[j - 1] + cost;
            int best = del < ins ? del : ins;
            if(sub < best) best = sub;
            curr[j] = (unsigned char)best;
        }
        memcpy(prev, curr, lb + 1);
    }

    return prev[lb];
}

static void normalize_phrase(char const *src, char *dst, size_t dst_size)
{
    size_t i=0;
    bool space=false;

    if(dst_size==0) return;

    while(*src && i+1<dst_size) {
        unsigned char c=(unsigned char)*src++;

        if(isalnum(c)) {
            dst[i++]=(char)tolower(c);
            space=false;
        }
        else if(i>0 && !space) {
            dst[i++]=' ';
            space=true;
        }
    }

    while(i>0 && dst[i-1]==' ') i--;
    dst[i]='\0';
}

static int phrase_typo_score(char const *a, char const *b)
{
    char aa[180], bb[180];
    char *pa, *pb;
    int total=0;
    int words=0;

    normalize_phrase(a,aa,sizeof(aa));
    normalize_phrase(b,bb,sizeof(bb));

    pa=aa;
    pb=bb;

    while(*pa || *pb) {
        char wa[48], wb[48];
        size_t ia=0, ib=0;
        int d;

        while(*pa==' ') pa++;
        while(*pb==' ') pb++;

        if(!*pa || !*pb) return 99;

        while(*pa && *pa!=' ' && ia+1<sizeof(wa)) wa[ia++]=*pa++;
        while(*pb && *pb!=' ' && ib+1<sizeof(wb)) wb[ib++]=*pb++;
        wa[ia]='\0';
        wb[ib]='\0';

        d=edit_distance_small(wa,wb);
        if(d>2) return 99;

        if(ia<=4 || ib<=4) {
            if(d>1) return 99;
        }

        total += d;
        words++;

        if(words>8 || total>5) return 99;
    }

    return total;
}

static fact_t const *suggest_fact_entry(char const *topic)
{
    char lower[180];
    fact_t const *candidate=NULL;
    int best=99;
    unsigned int i;

    if(!topic || !*topic || strlen(topic)>=sizeof(lower)) return NULL;
    lowercase_into(topic,lower,sizeof(lower));

    for(i=0;i<sizeof(facts)/sizeof(facts[0]);i++) {
        int d;

        if(strcmp(lower,facts[i].needle)==0) return &facts[i];

        d=phrase_typo_score(lower,facts[i].needle);
        if(d<best) {
            best=d;
            candidate=&facts[i];
        }
    }

    return (best>=1 && best<=4) ? candidate : NULL;
}

static char const *suggest_fact(char const *topic)
{
    fact_t const *entry=suggest_fact_entry(topic);
    return entry ? entry->needle : NULL;
}

static char memory_topic[180];
static char memory_answer[700];
static char memory_subject[64];
static bool memory_valid = false;

/* Small RAM-only conversational memory; never persisted to storage. */
static char session_name[48];
static char session_note[180];

static void first_sentence(char const *text, char *out, size_t out_size)
{
    size_t i = 0;

    if(out_size == 0) return;

    while(text[i] && i + 1 < out_size) {
        out[i] = text[i];
        if(text[i] == '.' || text[i] == '!' || text[i] == '?' || text[i] == '\n') {
            i++;
            break;
        }
        i++;
    }

    out[i] = '\0';
}

static bool handle_followup(char const *prompt, char const *mode, char const *level,
                            char *out, size_t out_size)
{
    char lower[96];
    char short_answer[260];

    if(!memory_valid) return false;
    lowercase_into(prompt, lower, sizeof(lower));

    if(exact_or_punct(lower, "repeat") || exact_or_punct(lower, "repeat that") ||
       exact_or_punct(lower, "say that again") || exact_or_punct(lower, "again")) {
        snprintf(out, out_size, "%s", memory_answer);
        return true;
    }

    if(exact_or_punct(lower, "simpler") || strstr(lower, "make it simpler") ||
       strstr(lower, "simplify that")) {
        first_sentence(memory_answer, short_answer, sizeof(short_answer));
        snprintf(out, out_size,
            "Simpler version about \"%s\": %s",
            memory_topic, short_answer);
        return true;
    }

    if(exact_or_punct(lower, "more") || strstr(lower, "explain more") ||
       strstr(lower, "more detail") || strstr(lower, "go deeper")) {
        snprintf(out, out_size,
            "Continuing \"%s\": %s\n"
            "To go deeper in %s, %s.",
            memory_topic, memory_answer, memory_subject,
            subject_strategy(memory_subject));
        return true;
    }

    if(exact_or_punct(lower, "why") || starts_with(lower, "why is that") ||
       starts_with(lower, "why does that") || starts_with(lower, "why?")) {
        snprintf(out, out_size,
            "About \"%s\": %s\n"
            "For the WHY layer, focus on cause/mechanism: %s.",
            memory_topic, memory_answer, subject_strategy(memory_subject));
        return true;
    }

    if(exact_or_punct(lower, "how") || starts_with(lower, "how?") ||
       starts_with(lower, "how does that") || starts_with(lower, "how is that")) {
        snprintf(out, out_size,
            "About \"%s\": %s\n"
            "For the HOW layer, work through the process in order: %s.",
            memory_topic, memory_answer, subject_strategy(memory_subject));
        return true;
    }

    if(strstr(lower, "what do you mean") || exact_or_punct(lower, "wdym") ||
       exact_or_punct(lower, "wdym?")) {
        first_sentence(memory_answer, short_answer, sizeof(short_answer));
        snprintf(out, out_size,
            "I mean this about \"%s\": %s",
            memory_topic, short_answer);
        return true;
    }

    if(strstr(lower, "what does that mean") || strstr(lower, "explain that") ||
       strstr(lower, "explain it") || strstr(lower, "tell me more about that") ||
       strstr(lower, "tell me more about it")) {
        first_sentence(memory_answer, short_answer, sizeof(short_answer));
        snprintf(out, out_size,
            "We were talking about \"%s\". In simpler terms: %s "
            "If you want, ask why, how, for an example, or for steps.",
            memory_topic, short_answer);
        return true;
    }

    if(exact_or_punct(lower, "shorter") || strstr(lower, "short answer") ||
       strstr(lower, "make it shorter")) {
        first_sentence(memory_answer, short_answer, sizeof(short_answer));
        snprintf(out, out_size, "%s", short_answer);
        return true;
    }

    if(exact_or_punct(lower, "longer") || strstr(lower, "more explanation") ||
       strstr(lower, "more detail please")) {
        snprintf(out, out_size,
            "More detail on \"%s\": %s\n"
            "Next layer: %s.",
            memory_topic, memory_answer, subject_strategy(memory_subject));
        return true;
    }

    if(strstr(lower, "show me steps") || strstr(lower, "show the steps") ||
       exact_or_punct(lower, "steps")) {
        snprintf(out, out_size,
            "Steps for \"%s\": 1) identify what is known, 2) identify what must be found, "
            "3) choose the governing rule/evidence, 4) apply it one step at a time, "
            "5) check the result against the original question. Subject strategy: %s.",
            memory_topic, subject_strategy(memory_subject));
        return true;
    }

    if(exact_or_punct(lower, "what about that") || exact_or_punct(lower, "what about it")) {
        snprintf(out, out_size,
            "You're still on \"%s\". The previous answer was: %s",
            memory_topic, memory_answer);
        return true;
    }

    if(exact_or_punct(lower, "yes") || exact_or_punct(lower, "yeah") ||
       exact_or_punct(lower, "yep") || exact_or_punct(lower, "sure")) {
        snprintf(out, out_size,
            "Got it. We're still on \"%s\". Ask why, how, for steps, an example, or switch to a new topic.",
            memory_topic);
        return true;
    }

    if(exact_or_punct(lower, "no") || exact_or_punct(lower, "nope") ||
       exact_or_punct(lower, "nah")) {
        snprintf(out, out_size,
            "Okay. Then let's not assume the previous explanation solved it. "
            "For \"%s\", tell me which part seems wrong or confusing and I'll reframe it.",
            memory_topic);
        return true;
    }

    if(exact_or_punct(lower, "maybe") || exact_or_punct(lower, "hmm") ||
       exact_or_punct(lower, "idk")) {
        snprintf(out, out_size,
            "Uncertain is fine. For \"%s\", we can test one claim at a time instead of guessing.",
            memory_topic);
        return true;
    }

    if(strstr(lower, "give me an example") || exact_or_punct(lower, "example")) {
        if(strstr(memory_subject, "math") || strstr(memory_subject, "algebra") ||
           strstr(memory_subject, "geometry") || strstr(memory_subject, "calculus")) {
            snprintf(out, out_size,
                "Example for \"%s\": choose a simple numerical case, apply the rule one step at a time, then substitute the result back to check it.",
                memory_topic);
        }
        else if(strstr(memory_subject, "computer")) {
            snprintf(out, out_size,
                "Example for \"%s\": use one small input, trace each step/state change, and compare the actual output with the expected output.",
                memory_topic);
        }
        else if(strstr(memory_subject, "literature") || strstr(memory_subject, "composition")) {
            snprintf(out, out_size,
                "Example for \"%s\": make one specific claim, point to one concrete detail, then explain how that detail supports the claim.",
                memory_topic);
        }
        else {
            snprintf(out, out_size,
                "Example for \"%s\": pick one simple real case, identify the cause/input, show the process, then state the result and why it fits the concept.",
                memory_topic);
        }
        return true;
    }

    (void)mode;
    (void)level;
    return false;
}

static bool write_conversation(char const *subject, char const *prompt,
                               char *out, size_t out_size)
{
    char subject_name[64];

    friendly_subject(subject, subject_name, sizeof(subject_name));

    if(starts_with(lowerbuf, "my name is ") || starts_with(lowerbuf, "call me ")) {
        char name[64];
        char const *p = starts_with(lowerbuf, "my name is ") ? prompt + 11 : prompt + 8;
        snprintf(name, sizeof(name), "%.63s", p);
        trim_text(name);
        if(name[0]) {
            snprintf(session_name, sizeof(session_name), "%.47s", name);
            snprintf(out, out_size,
                "Got it. I'll remember your name as %s for this calculator session.",
                session_name);
            return true;
        }
    }

    if(strstr(lowerbuf, "what is my name") || strstr(lowerbuf, "what's my name") ||
       strstr(lowerbuf, "whats my name") || exact_or_punct(lowerbuf, "who am i")) {
        if(session_name[0]) snprintf(out, out_size, "You told me your name is %s.", session_name);
        else snprintf(out, out_size, "You haven't told me your name in this calculator session yet.");
        return true;
    }

    if(starts_with(lowerbuf, "remember that ") || starts_with(lowerbuf, "remember ")) {
        char note[220];
        char const *p = starts_with(lowerbuf, "remember that ") ? prompt + 14 : prompt + 9;
        snprintf(note, sizeof(note), "%.219s", p);
        trim_text(note);
        if(note[0]) {
            snprintf(session_note, sizeof(session_note), "%.179s", note);
            snprintf(out, out_size,
                "Remembered for this calculator session: %s", session_note);
            return true;
        }
    }

    if(strstr(lowerbuf, "what do you remember") ||
       strstr(lowerbuf, "what did i tell you") ||
       strstr(lowerbuf, "what have i told you")) {
        if(session_name[0] && session_note[0]) {
            snprintf(out, out_size,
                "This session: your name is %s, and you asked me to remember: %s",
                session_name, session_note);
        }
        else if(session_name[0]) {
            snprintf(out, out_size, "This session, you told me your name is %s.", session_name);
        }
        else if(session_note[0]) {
            snprintf(out, out_size, "This session, you asked me to remember: %s", session_note);
        }
        else {
            snprintf(out, out_size, "I don't have any user-supplied session memory yet.");
        }
        return true;
    }

    if(exact_or_punct(lowerbuf, "forget my name")) {
        session_name[0] = '\0';
        snprintf(out, out_size, "Cleared your name from this calculator session.");
        return true;
    }

    if(exact_or_punct(lowerbuf, "forget that") ||
       exact_or_punct(lowerbuf, "forget what i said")) {
        session_note[0] = '\0';
        snprintf(out, out_size, "Cleared the remembered note from this calculator session.");
        return true;
    }

    if(exact_or_punct(lowerbuf, "forget everything") ||
       exact_or_punct(lowerbuf, "clear memory")) {
        session_name[0] = '\0';
        session_note[0] = '\0';
        snprintf(out, out_size, "Cleared the user-supplied RAM memory for this calculator session.");
        return true;
    }

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

    if(exact_or_punct(lowerbuf, "lol") || exact_or_punct(lowerbuf, "lmao") ||
       exact_or_punct(lowerbuf, "bruh") || exact_or_punct(lowerbuf, "bro")) {
        snprintf(out, out_size,
            "Fair enough. I'm still here in %s Standalone mode—send literally whatever you want next.",
            subject_name);
        return true;
    }

    if(exact_or_punct(lowerbuf, "ok") || exact_or_punct(lowerbuf, "okay") ||
       exact_or_punct(lowerbuf, "cool") || exact_or_punct(lowerbuf, "nice") ||
       exact_or_punct(lowerbuf, "bet")) {
        snprintf(out, out_size,
            "Got it. Keep going—I'm staying in %s Standalone mode.", subject_name);
        return true;
    }

    if(exact_or_punct(lowerbuf, "idk") || strstr(lowerbuf, "i don't know") ||
       strstr(lowerbuf, "i dont know")) {
        snprintf(out, out_size,
            "That's fine. In %s, send the part you do know—even one word, number, formula, or clue—and I'll build from that.",
            subject_name);
        return true;
    }

    if(exact_or_punct(lowerbuf, "wsp") || exact_or_punct(lowerbuf, "wassgood") ||
       exact_or_punct(lowerbuf, "whatup")) {
        snprintf(out, out_size,
            "QBAI Standalone is up. Current subject: %s. Send me anything.", subject_name);
        return true;
    }

    if(exact_or_punct(lowerbuf, "wyd") || exact_or_punct(lowerbuf, "whatcha doing")) {
        snprintf(out, out_size,
            "Running locally on the calculator and ready for your next %s question, calculation, or random input.",
            subject_name);
        return true;
    }

    if(strstr(lowerbuf, "what's up") || strstr(lowerbuf, "whats up") ||
       strstr(lowerbuf, "how's it going") || strstr(lowerbuf, "hows it going")) {
        snprintf(out, out_size,
            "QBAI Standalone is up and running. Current subject: %s. Throw me anything.", subject_name);
        return true;
    }

    if(strstr(lowerbuf, "i'm bored") || strstr(lowerbuf, "im bored")) {
        snprintf(out, out_size,
            "Challenge: give me a random %s term, equation, question, or claim and try to make the standalone engine fail.",
            subject_name);
        return true;
    }

    (void)prompt;
    return false;
}

static bool write_intent_response(char const *subject, char const *mode,
                                  char const *prompt, char *out, size_t out_size)
{
    char subject_name[64];
    char topic[180];

    friendly_subject(subject, subject_name, sizeof(subject_name));
    copy_topic(prompt, topic, sizeof(topic));

    if(starts_with(lowerbuf, "what do you think about ") ||
       starts_with(lowerbuf, "what do you think of ") ||
       starts_with(lowerbuf, "do you like ") ||
       starts_with(lowerbuf, "which is better ")) {
        snprintf(out, out_size,
            "I don't have personal opinions, but I can evaluate \"%s\" using explicit criteria. "
            "In %s, useful criteria are accuracy/evidence, purpose, tradeoffs, and how well it fits the question.",
            topic, subject_name);
        return true;
    }

    if(starts_with(lowerbuf, "i think ") || starts_with(lowerbuf, "i believe ") ||
       starts_with(lowerbuf, "my claim is ") || starts_with(lowerbuf, "my answer is ")) {
        snprintf(out, out_size,
            "You gave a claim/answer: \"%.180s\". In %s, I would check it by identifying the key claim, "
            "matching it against the relevant rule/evidence, and looking for a counterexample or missing condition.",
            prompt, subject_name);
        return true;
    }

    if(starts_with(lowerbuf, "check this ") || starts_with(lowerbuf, "check my ") ||
       starts_with(lowerbuf, "is this right") || starts_with(lowerbuf, "is this correct")) {
        snprintf(out, out_size,
            "CHECK mode for \"%.180s\": I can inspect the logic locally, but I need the actual work/claim in the text. "
            "For %s, I check the first step that conflicts with the governing rule or evidence.",
            prompt, subject_name);
        return true;
    }

    if(starts_with(lowerbuf, "write about ") || starts_with(lowerbuf, "write an essay about ") ||
       starts_with(lowerbuf, "write a paragraph about ") ||
       starts_with(lowerbuf, "make an essay about ")) {
        snprintf(out, out_size,
            "Writing plan for \"%s\": 1) make a specific thesis, 2) choose 2-3 supporting points, "
            "3) attach evidence/examples to each point, 4) explain the connection, 5) conclude by returning to the thesis. "
            "Standalone mode can build the structure even when it cannot generate unlimited freeform prose.",
            topic);
        return true;
    }

    if(starts_with(lowerbuf, "list ") || starts_with(lowerbuf, "give me a list of ")) {
        snprintf(out, out_size,
            "You want a list about \"%s\". In %s, organize the list by category or importance, "
            "keep each item parallel, and include a short reason/example when the assignment needs explanation.",
            topic, subject_name);
        return true;
    }

    if(starts_with(lowerbuf, "give me reasons ") || starts_with(lowerbuf, "reasons why ") ||
       starts_with(lowerbuf, "causes of ") || starts_with(lowerbuf, "effects of ")) {
        snprintf(out, out_size,
            "For \"%s\", separate causes from effects and rank them by directness/evidence. "
            "In %s, a strong answer names the factor, explains the mechanism, and connects it to the outcome.",
            topic, subject_name);
        return true;
    }

    if(starts_with(lowerbuf, "prove ") || strcmp(mode, "proof") == 0) {
        snprintf(out, out_size,
            "Proof setup for \"%s\": state the hypotheses and exact conclusion first, then choose a valid strategy "
            "(direct, contrapositive, contradiction, induction, construction, or theorem application). "
            "Every step must follow from a definition, hypothesis, or previously established result.",
            topic);
        return true;
    }

    if(starts_with(lowerbuf, "derive ") || strcmp(mode, "derive") == 0) {
        snprintf(out, out_size,
            "Derivation setup for \"%s\": start from definitions/governing equations, state assumptions, "
            "substitute one relationship at a time, preserve units, and verify the final form with a limiting or dimensional check.",
            topic);
        return true;
    }

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
    char const *suggestion;
    int words;

    copy_topic(prompt, topic, sizeof(topic));
    friendly_subject(subject, subject_name, sizeof(subject_name));
    strategy = subject_strategy(subject);
    ref = qb_reference_text(subject);
    words = alpha_word_count(prompt);
    suggestion = suggest_fact(topic);

    if(suggestion) {
        fact_t const *entry = suggest_fact_entry(topic);
        if(entry) {
            snprintf(out, out_size,
                "Likely correction: \"%s\" -> \"%s\". %s",
                topic, entry->needle, entry->answer);
            return;
        }
    }

    if(starts_with(lowerbuf, "what is ") || starts_with(lowerbuf, "wat is ") ||
       starts_with(lowerbuf, "wut is ") || starts_with(lowerbuf, "what are ") ||
       starts_with(lowerbuf, "what's ") || starts_with(lowerbuf, "whats ") ||
       (starts_with(lowerbuf, "what does ") && strstr(lowerbuf, " mean")) ||
       starts_with(lowerbuf, "define ") || starts_with(lowerbuf, "explain ") ||
       starts_with(lowerbuf, "pls explain ") || starts_with(lowerbuf, "plz explain ") ||
       starts_with(lowerbuf, "can u explain ") || starts_with(lowerbuf, "can you explain ") ||
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
        char const *suggestion = suggest_fact(topic);

        if(suggestion) {
            fact_t const *entry = suggest_fact_entry(topic);
            snprintf(out, out_size,
                "Likely correction: \"%s\" -> \"%s\". %s",
                topic, suggestion, entry ? entry->answer : "Add context if that correction is not what you meant.");
        }
        else if(looks_like_unknown_token(lowerbuf)) {
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

    if(!strchr(prompt, '?') &&
       !word_prefix(lowerbuf, "what") && !word_prefix(lowerbuf, "why") &&
       !word_prefix(lowerbuf, "how") && !word_prefix(lowerbuf, "who") &&
       !word_prefix(lowerbuf, "when") && !word_prefix(lowerbuf, "where") &&
       !word_prefix(lowerbuf, "is") && !word_prefix(lowerbuf, "are") &&
       !word_prefix(lowerbuf, "can") && !word_prefix(lowerbuf, "could") &&
       !word_prefix(lowerbuf, "should")) {
        snprintf(out, out_size,
            "You entered a statement rather than a direct question: \"%s\". "
            "In %s, I can treat it as a claim to explain, check, compare, or support. "
            "A good next move is to %s.",
            topic, subject_name, strategy);
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

static bool qb_offline_answer_core(
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
    char fact_topic[180];
    char fact_topic_lower[180];
    char const *fact_source;
    (void)mode;
    (void)level;

    if(!out || out_size == 0 || !prompt || !*prompt) return false;
    out[0] = '\0';
    lowercase(prompt);

    if(write_conversation(subject, prompt, out, out_size)) {
        return true;
    }

    if(try_periodic_table(subject, prompt, out, out_size)) {
        return true;
    }

    if(try_literal_analysis(prompt, out, out_size)) {
        return true;
    }

    if(try_acronym_response(prompt, out, out_size)) {
        return true;
    }

    if(try_text_tools(prompt, out, out_size)) {
        return true;
    }

    if(try_language_tools(prompt, out, out_size)) {
        return true;
    }

    if(try_misc_text_math(prompt, out, out_size)) {
        return true;
    }

    if(try_date_tools(prompt, out, out_size)) {
        return true;
    }

    if(try_encoding_tools(prompt, out, out_size)) {
        return true;
    }

    if(try_cs_bit_tools(prompt, out, out_size)) {
        return true;
    }

    if(try_chemistry_formula_tools(prompt, out, out_size)) {
        return true;
    }

    if(try_study_request(subject, prompt, out, out_size)) {
        return true;
    }

    if(try_genetics_cross(prompt, out, out_size)) {
        return true;
    }

    if(try_polynomial_calculus(lowerbuf, out, out_size)) {
        return true;
    }

    if(write_intent_response(subject, mode, prompt, out, out_size)) {
        return true;
    }

    if(try_known_comparison(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_prefixed_expression(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_numeric_claim(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_system_2x2(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_quadratic_equation(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_proportion(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_linear_inequality(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_linear_equation(lowerbuf, out, out_size)) {
        return true;
    }

    if(strchr(lowerbuf, '=') && strchr(lowerbuf, 'x')) {
        snprintf(out, out_size,
            "I can see an x-equation, but I couldn't parse its exact form. "
            "Standalone linear solving supports forms like 3x+7=25 or 2x-4=x+9. "
            "Your exact input was: \"%.180s\".",
            prompt);
        return true;
    }

    if(try_unit_conversion(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_number_theory(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_fraction_percent_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_statistics_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_function_analysis(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_linear_algebra_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_probability_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_log_root_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_sequence_pattern(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_finance_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_business_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_geometry_extra(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_kinematics_tools(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_unit_rate_word_problem(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_relationship_word_problem(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_triangle_trig(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_extra_science(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_science_formula(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_word_problem(lowerbuf, out, out_size)) {
        return true;
    }

    if(try_natural_math(lowerbuf, out, out_size)) {
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

    /*
     * Search stored knowledge against the semantic topic of natural-language
     * questions instead of every word in the sentence. This prevents intent
     * words such as "mean" from hijacking "what does X mean?".
     */
    copy_topic(prompt, fact_topic, sizeof(fact_topic));
    lowercase_into(fact_topic, fact_topic_lower, sizeof(fact_topic_lower));

    if(starts_with(lowerbuf, "what is ") || starts_with(lowerbuf, "wat is ") ||
       starts_with(lowerbuf, "wut is ") || starts_with(lowerbuf, "what are ") ||
       starts_with(lowerbuf, "what's ") || starts_with(lowerbuf, "whats ") ||
       starts_with(lowerbuf, "what does ") || starts_with(lowerbuf, "define ") ||
       starts_with(lowerbuf, "explain ") || starts_with(lowerbuf, "pls explain ") ||
       starts_with(lowerbuf, "plz explain ") || starts_with(lowerbuf, "can u explain ") ||
       starts_with(lowerbuf, "can you explain ") || starts_with(lowerbuf, "tell me about ") ||
       starts_with(lowerbuf, "why ") || starts_with(lowerbuf, "how ") ||
       starts_with(lowerbuf, "who ") || starts_with(lowerbuf, "when ") ||
       starts_with(lowerbuf, "where ") || starts_with(lowerbuf, "compare ") ||
       starts_with(lowerbuf, "difference between ")) {
        fact_source = fact_topic_lower;
    }
    else {
        fact_source = lowerbuf;
    }

    for(i = 0; i < sizeof(facts)/sizeof(facts[0]); i++) {
        if(phrase_match(fact_source, facts[i].needle)) {
            snprintf(out, out_size, "%s", facts[i].answer);
            return true;
        }
    }

    if(try_word_parts(subject, prompt, out, out_size)) {
        return true;
    }

    if(write_input_shape_response(subject, prompt, out, out_size)) {
        return true;
    }

    write_contextual_fallback(subject, mode, level, prompt, out, out_size);
    return true;
}


static bool normalize_request_prefix(char const *prompt, char *out, size_t out_size)
{
    char work[700];
    char lower[700];
    char *p;
    int pass;
    bool changed=false;

    if(!prompt || out_size==0) return false;
    snprintf(work,sizeof(work),"%s",prompt);
    p=work;

    for(pass=0;pass<4;pass++) {
        char *start=p;
        while(*start && isspace((unsigned char)*start)) start++;
        lowercase_into(start,lower,sizeof(lower));

        if(starts_with(lower,"please ")) start += 7;
        else if(starts_with(lower,"pls ")) start += 4;
        else if(starts_with(lower,"plz ")) start += 4;
        else if(starts_with(lower,"can you ")) start += 8;
        else if(starts_with(lower,"could you ")) start += 10;
        else if(starts_with(lower,"would you ")) start += 10;
        else if(starts_with(lower,"will you ")) start += 9;
        else if(starts_with(lower,"hey qbai ")) start += 9;
        else if(starts_with(lower,"hey qbai, ")) start += 10;
        else if(starts_with(lower,"yo qbai ")) start += 8;
        else if(starts_with(lower,"yo qbai, ")) start += 9;
        else if(starts_with(lower,"qbai ")) start += 5;
        else if(starts_with(lower,"qbai, ")) start += 6;
        else if(starts_with(lower,"quantum breaks ai ")) start += 18;
        else break;

        while(*start && (isspace((unsigned char)*start) || *start==',' || *start==':')) start++;
        p=start;
        changed=true;
    }

    if(!changed || !*p) return false;
    snprintf(out,out_size,"%s",p);
    return true;
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
    char lower[700];
    char normalized[700];
    char topic[180];
    char const *effective_subject = subject;
    bool ok;

    if(!out || out_size == 0 || !prompt || !*prompt) return false;

    if(normalize_request_prefix(prompt, normalized, sizeof(normalized))) {
        return qb_offline_answer(subject, mode, level, normalized, out, out_size);
    }

    lowercase_into(prompt, lower, sizeof(lower));

    if((!effective_subject || strcmp(effective_subject, "auto") == 0)) {
        char const *inferred = infer_subject_local(lower);
        if(strcmp(inferred, "auto") != 0) effective_subject = inferred;
    }

    if(!effective_subject) effective_subject = "auto";

    if(quiz_pending && (exact_or_punct(lower, "show answer") ||
       exact_or_punct(lower, "answer") || exact_or_punct(lower, "reveal answer"))) {
        snprintf(out, out_size, "ANSWER - %s: %s", quiz_topic, quiz_answer);
        quiz_pending = false;
        snprintf(memory_topic, sizeof(memory_topic), "%s", quiz_topic);
        snprintf(memory_answer, sizeof(memory_answer), "%.699s", out);
        snprintf(memory_subject, sizeof(memory_subject), "%s", effective_subject);
        memory_valid = true;
        return true;
    }

    if(!contains_alnum(prompt)) {
        snprintf(out, out_size,
            "I received only spaces/punctuation/symbols: \"%.120s\". "
            "That still counts as input. Add any letter or number and I'll treat it as a topic, question, or calculation.",
            prompt);
        return true;
    }

    if(memory_valid &&
       (starts_with(lower, "same for ") || starts_with(lower, "and ")) &&
       strlen(prompt) > (starts_with(lower, "same for ") ? 9u : 4u)) {
        char synthetic[220];
        char const *new_topic = starts_with(lower, "same for ") ? prompt + 9 : prompt + 4;
        snprintf(synthetic, sizeof(synthetic), "explain %.190s", new_topic);
        ok = qb_offline_answer_core(
            effective_subject,
            mode,
            level,
            synthetic,
            out,
            out_size
        );
        if(ok && out[0]) {
            copy_topic(synthetic, topic, sizeof(topic));
            snprintf(memory_topic, sizeof(memory_topic), "%s", topic);
            snprintf(memory_answer, sizeof(memory_answer), "%.699s", out);
            snprintf(memory_subject, sizeof(memory_subject), "%s", effective_subject);
            memory_valid = true;
        }
        return ok;
    }

    if(memory_valid && starts_with(lower, "what about ") &&
       !exact_or_punct(lower, "what about that") &&
       !exact_or_punct(lower, "what about it")) {
        char synthetic[220];
        snprintf(synthetic, sizeof(synthetic), "explain %.190s", prompt + 11);
        ok = qb_offline_answer_core(
            effective_subject,
            mode,
            level,
            synthetic,
            out,
            out_size
        );
        if(ok && out[0]) {
            copy_topic(synthetic, topic, sizeof(topic));
            snprintf(memory_topic, sizeof(memory_topic), "%s", topic);
            snprintf(memory_answer, sizeof(memory_answer), "%.699s", out);
            snprintf(memory_subject, sizeof(memory_subject), "%s", effective_subject);
            memory_valid = true;
        }
        return ok;
    }

    if(handle_followup(prompt, mode, level, out, out_size)) {
        if(out[0]) {
            snprintf(memory_answer, sizeof(memory_answer), "%.699s", out);
        }
        return true;
    }

    ok = qb_offline_answer_core(
        effective_subject,
        mode,
        level,
        prompt,
        out,
        out_size
    );

    if(ok && out[0]) {
        copy_topic(prompt, topic, sizeof(topic));
        snprintf(memory_topic, sizeof(memory_topic), "%s", topic);
        snprintf(memory_answer, sizeof(memory_answer), "%.699s", out);
        snprintf(memory_subject, sizeof(memory_subject), "%s", effective_subject);
        memory_valid = true;
    }

    return ok;
}
