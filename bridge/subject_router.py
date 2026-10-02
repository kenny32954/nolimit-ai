"""Subject, academic-level, and tutoring-mode routing for Quantum Breaks AI CG50."""

SUBJECTS = {
    "general": {
        "label": "General",
        "aliases": ("gen", "general"),
        "keywords": (),
        "instruction": "Handle academic work clearly and at the requested level."
    },
    "math": {
        "label": "Mathematics",
        "aliases": ("math", "mathematics"),
        "keywords": ("equation", "fraction", "decimal", "percent", "ratio", "radical"),
        "instruction": "Use correct notation, define variables, preserve units, and verify arithmetic when practical."
    },
    "algebra": {
        "label": "Algebra",
        "aliases": ("alg", "algebra"),
        "keywords": ("linear equation", "quadratic", "polynomial", "factor", "system of equations", "inequality"),
        "instruction": "Show valid algebraic transformations, note domain restrictions, and check solutions when practical."
    },
    "geometry": {
        "label": "Geometry",
        "aliases": ("geo", "geometry"),
        "keywords": ("triangle", "angle", "circle", "perimeter", "area", "congruent", "similar"),
        "instruction": "State relevant definitions and theorems and justify geometric steps."
    },
    "statistics": {
        "label": "Statistics",
        "aliases": ("stat", "stats", "statistics"),
        "keywords": ("standard deviation", "regression", "distribution", "confidence interval", "hypothesis test", "p-value"),
        "instruction": "State assumptions, distinguish sample from population, and interpret results in context."
    },
    "calculus": {
        "label": "Calculus",
        "aliases": ("calc", "calculus"),
        "keywords": ("derivative", "integral", "limit", "differentiate", "integrate", "series"),
        "instruction": "Show symbolic work, state applicable theorems and conditions, and distinguish intuition from formal argument."
    },
    "linear_algebra": {
        "label": "Linear Algebra",
        "aliases": ("linalg", "linear_algebra"),
        "keywords": ("eigenvalue", "eigenvector", "matrix", "determinant", "vector space", "linear transformation", "basis"),
        "instruction": "Use precise vector-space language, distinguish coordinate representations from abstract objects, and justify row operations and theorem use."
    },
    "discrete_math": {
        "label": "Discrete Math",
        "aliases": ("discrete", "discrete_math"),
        "keywords": ("combinatorics", "graph theory", "induction", "recurrence", "bijection", "pigeonhole", "discrete math"),
        "instruction": "Use rigorous counting, logic, induction, recurrence, and graph-theoretic reasoning as appropriate."
    },
    "differential_equations": {
        "label": "Differential Equations",
        "aliases": ("de", "ode", "differential_equations"),
        "keywords": ("differential equation", "ode", "initial value", "laplace transform", "phase plane", "separable equation"),
        "instruction": "Classify the equation, state the solution method and assumptions, solve carefully, and verify against initial or boundary conditions."
    },
    "number_theory": {
        "label": "Number Theory",
        "aliases": ("nt", "number_theory"),
        "keywords": ("modular arithmetic", "congruence", "prime", "gcd", "diophantine", "number theory"),
        "instruction": "Use exact integer reasoning, congruences, divisibility, and proofs with clearly stated lemmas."
    },
    "real_analysis": {
        "label": "Real Analysis",
        "aliases": ("analysis", "real_analysis"),
        "keywords": ("epsilon delta", "epsilon-delta", "cauchy", "uniform continuity", "supremum", "real analysis"),
        "instruction": "Use definitions precisely and provide rigorous proofs, including quantifiers and theorem hypotheses."
    },
    "abstract_algebra": {
        "label": "Abstract Algebra",
        "aliases": ("abstract", "abstract_algebra"),
        "keywords": ("group", "ring", "field", "homomorphism", "isomorphism", "coset", "lagrange theorem"),
        "instruction": "Use formal definitions and proof structure for groups, rings, fields, morphisms, and related algebraic objects."
    },
    "biology": {
        "label": "Biology",
        "aliases": ("bio", "biology"),
        "keywords": ("cell", "organism", "photosynthesis", "evolution", "ecology"),
        "instruction": "Use accurate biological terminology and connect structure, mechanism, function, and evidence."
    },
    "genetics": {
        "label": "Genetics",
        "aliases": ("genetics", "molecular_genetics"),
        "keywords": ("genotype", "phenotype", "allele", "linkage", "pedigree", "gene expression", "genetic"),
        "instruction": "Distinguish inheritance patterns, molecular mechanisms, probabilities, and population-level claims."
    },
    "microbiology": {
        "label": "Microbiology",
        "aliases": ("micro", "microbiology"),
        "keywords": ("bacteria", "virus", "microbe", "microbiology", "gram stain", "culture medium"),
        "instruction": "Explain microbial structure, genetics, ecology, and host interactions conceptually and safely."
    },
    "anatomy_physiology": {
        "label": "Anatomy/Physiology",
        "aliases": ("a&p", "anatomy", "physiology", "anatomy_physiology"),
        "keywords": ("anatomy", "physiology", "homeostasis", "organ system", "neuron", "cardiovascular"),
        "instruction": "Connect anatomical structures to physiological mechanisms and homeostatic regulation without diagnosing individuals."
    },
    "chemistry": {
        "label": "Chemistry",
        "aliases": ("chem", "chemistry"),
        "keywords": ("mole", "molar", "atom", "ion", "bond", "stoichiometry", "equilibrium", "thermochemistry"),
        "instruction": "Track units, significant figures, stoichiometry, equilibrium assumptions, and molecular reasoning carefully."
    },
    "organic_chemistry": {
        "label": "Organic Chemistry",
        "aliases": ("ochem", "orgo", "organic_chemistry"),
        "keywords": ("organic chemistry", "sn1", "sn2", "e1", "e2", "nucleophile", "electrophile", "stereochemistry", "nmr"),
        "instruction": "Explain structure, mechanism, stereochemistry, spectroscopy, and reaction logic at a conceptual academic level; avoid unsafe procedural synthesis guidance."
    },
    "biochemistry": {
        "label": "Biochemistry",
        "aliases": ("biochem", "biochemistry"),
        "keywords": ("enzyme kinetics", "protein", "amino acid", "metabolism", "glycolysis", "krebs", "biochemistry"),
        "instruction": "Connect molecular structure, thermodynamics, kinetics, metabolism, and regulation."
    },
    "physics": {
        "label": "Physics",
        "aliases": ("phys", "physics"),
        "keywords": ("velocity", "acceleration", "force", "momentum", "electric field", "magnetic field", "wave"),
        "instruction": "State the model, assumptions, coordinate system, governing equations, units, and limiting checks."
    },
    "thermodynamics": {
        "label": "Thermodynamics",
        "aliases": ("thermo", "thermodynamics"),
        "keywords": ("entropy", "enthalpy", "heat engine", "first law", "second law", "thermodynamics"),
        "instruction": "Use consistent sign conventions, state the system and process, and apply thermodynamic laws with explicit assumptions."
    },
    "circuits": {
        "label": "Circuits",
        "aliases": ("circuits", "circuit_analysis", "electronics"),
        "keywords": ("kirchhoff", "node voltage", "mesh current", "thevenin", "norton", "rc circuit", "rl circuit", "op amp"),
        "instruction": "Use circuit laws, reference polarities, units, and systematic node/mesh analysis."
    },
    "earth": {
        "label": "Earth/Space",
        "aliases": ("earth", "space", "astronomy", "geology"),
        "keywords": ("planet", "star", "plate tectonics", "geology", "astronomy", "solar system"),
        "instruction": "Explain Earth and space systems with quantitative and causal reasoning where appropriate."
    },
    "environmental_science": {
        "label": "Environmental Science",
        "aliases": ("environment", "environmental", "env_sci"),
        "keywords": ("ecosystem", "biodiversity", "pollution", "sustainability", "carbon cycle", "conservation"),
        "instruction": "Explain environmental systems, evidence, uncertainty, and tradeoffs clearly."
    },
    "ela": {
        "label": "English/ELA",
        "aliases": ("ela", "english", "grammar"),
        "keywords": ("grammar", "sentence", "punctuation", "rhetoric", "figurative language"),
        "instruction": "Explain language, rhetoric, and literary conventions with precise examples."
    },
    "literature": {
        "label": "Literature",
        "aliases": ("lit", "literature"),
        "keywords": ("symbolism", "motif", "narrator", "close reading", "literary criticism", "poem"),
        "instruction": "Ground analysis in textual evidence and distinguish observation, interpretation, and critical framework."
    },
    "writing": {
        "label": "Academic Writing",
        "aliases": ("write", "writing", "essay"),
        "keywords": ("essay", "thesis", "research paper", "literature review", "argumentative", "citation"),
        "instruction": "Help develop claims, organization, evidence, revision, and source integration while preserving the student's voice."
    },
    "history": {
        "label": "History",
        "aliases": ("hist", "history"),
        "keywords": ("historical", "historiography", "primary source", "archive", "revolution", "empire"),
        "instruction": "Use chronology, source criticism, causation, context, and historiographic distinctions carefully."
    },
    "social_studies": {
        "label": "Social Studies",
        "aliases": ("social", "social_studies"),
        "keywords": ("social studies", "culture", "citizenship", "community", "human geography"),
        "instruction": "Connect history, geography, civics, economics, and culture without collapsing distinct concepts."
    },
    "geography": {
        "label": "Geography",
        "aliases": ("geog", "geography"),
        "keywords": ("gis", "spatial", "migration", "population density", "physical geography", "human geography"),
        "instruction": "Explain spatial patterns, scale, regions, human-environment interactions, and geographic methods."
    },
    "government": {
        "label": "Government/Civics",
        "aliases": ("gov", "government", "civics"),
        "keywords": ("constitution", "federalism", "legislature", "executive", "judicial", "civil liberties"),
        "instruction": "Explain institutions, constitutional concepts, and civic processes neutrally and factually."
    },
    "political_science": {
        "label": "Political Science",
        "aliases": ("poli_sci", "political_science"),
        "keywords": ("political science", "comparative politics", "international relations", "voting behavior", "political theory"),
        "instruction": "Explain political-science concepts, institutions, theories, and evidence neutrally; attribute contested interpretations and do not recommend political choices."
    },
    "economics": {
        "label": "Economics",
        "aliases": ("econ", "economics"),
        "keywords": ("elasticity", "marginal", "inflation", "gdp", "market failure", "monetary policy", "fiscal policy"),
        "instruction": "State assumptions, distinguish micro from macro reasoning, and separate positive analysis from value judgments."
    },
    "finance": {
        "label": "Finance",
        "aliases": ("fin", "finance"),
        "keywords": ("present value", "future value", "discount rate", "bond", "portfolio", "capm", "npv"),
        "instruction": "Show time-value-of-money and financial calculations clearly, state assumptions, and distinguish educational analysis from personal financial advice."
    },
    "business": {
        "label": "Business",
        "aliases": ("biz", "business", "marketing"),
        "keywords": ("marketing", "management", "strategy", "operations", "revenue", "customer"),
        "instruction": "Use standard business frameworks while distinguishing descriptive analysis from recommendations."
    },
    "accounting": {
        "label": "Accounting",
        "aliases": ("acct", "accounting"),
        "keywords": ("debit", "credit", "balance sheet", "cash flow", "journal entry", "gaap"),
        "instruction": "Track debits, credits, classifications, statements, and accounting equations carefully."
    },
    "computer_science": {
        "label": "Computer Science",
        "aliases": ("cs", "computer", "coding", "programming"),
        "keywords": ("code", "programming", "compiler", "operating system", "computer science"),
        "instruction": "Explain code and systems precisely, state complexity and assumptions when relevant, and prefer testable reasoning."
    },
    "data_structures": {
        "label": "Data Structures",
        "aliases": ("ds", "data_structures"),
        "keywords": ("linked list", "binary tree", "heap", "hash table", "stack", "queue", "data structure"),
        "instruction": "Explain invariants, operations, tradeoffs, and time/space complexity."
    },
    "algorithms": {
        "label": "Algorithms",
        "aliases": ("algo", "algorithms"),
        "keywords": ("big o", "dynamic programming", "greedy", "divide and conquer", "shortest path", "algorithm"),
        "instruction": "State correctness arguments, invariants, complexity, edge cases, and alternative approaches."
    },
    "databases": {
        "label": "Databases",
        "aliases": ("db", "database", "databases"),
        "keywords": ("sql", "normalization", "transaction", "index", "relational algebra", "database"),
        "instruction": "Explain schemas, relational reasoning, SQL, normalization, indexing, transactions, and tradeoffs."
    },
    "computer_architecture": {
        "label": "Computer Architecture",
        "aliases": ("architecture", "computer_architecture"),
        "keywords": ("assembly", "pipeline", "cache", "instruction set", "cpu architecture", "virtual memory"),
        "instruction": "Explain machine representation, instruction execution, memory hierarchy, and performance quantitatively."
    },
    "engineering": {
        "label": "Engineering",
        "aliases": ("eng", "engineering", "stem"),
        "keywords": ("design process", "prototype", "cad", "engineering", "constraint", "tolerance"),
        "instruction": "State assumptions, constraints, units, models, and validation checks in engineering work."
    },
    "statics_dynamics": {
        "label": "Statics/Dynamics",
        "aliases": ("statics", "dynamics", "statics_dynamics"),
        "keywords": ("free body diagram", "moment", "torque", "equilibrium", "kinetics", "rigid body"),
        "instruction": "Define the body/system, coordinate axes, free-body diagram, equilibrium or motion equations, and units."
    },
    "materials_science": {
        "label": "Materials Science",
        "aliases": ("materials", "materials_science"),
        "keywords": ("stress strain", "young modulus", "crystal structure", "phase diagram", "materials science"),
        "instruction": "Connect structure, processing, properties, and performance with quantitative mechanics where relevant."
    },
    "cte": {
        "label": "CTE/Career Tech",
        "aliases": ("cte", "career", "career_tech"),
        "keywords": ("career tech", "technical drawing", "trade", "workplace", "employability"),
        "instruction": "Teach practical technical concepts using safe, school-appropriate procedures."
    },
    "agriculture": {
        "label": "Agriculture",
        "aliases": ("ag", "agriculture"),
        "keywords": ("agriculture", "crop", "soil", "livestock", "horticulture", "plant science"),
        "instruction": "Explain agricultural science and agribusiness concepts accurately and safely."
    },
    "psychology": {
        "label": "Psychology",
        "aliases": ("psych", "psychology"),
        "keywords": ("cognition", "behavior", "psychology", "developmental", "experimental psychology"),
        "instruction": "Explain psychological theories and evidence without diagnosing the student or other people."
    },
    "research_methods": {
        "label": "Research Methods",
        "aliases": ("methods", "research_methods"),
        "keywords": ("research design", "independent variable", "dependent variable", "validity", "reliability", "methodology"),
        "instruction": "Explain study design, measurement, sampling, validity, reliability, statistics, ethics, and limitations; never fabricate sources or data."
    },
    "sociology": {
        "label": "Sociology",
        "aliases": ("soc", "sociology"),
        "keywords": ("sociology", "socialization", "institution", "social structure", "stratification"),
        "instruction": "Explain sociological concepts and evidence while distinguishing theories from empirical findings."
    },
    "philosophy_logic": {
        "label": "Philosophy/Logic",
        "aliases": ("philosophy", "logic", "philosophy_logic"),
        "keywords": ("validity", "soundness", "premise", "syllogism", "propositional logic", "philosophy"),
        "instruction": "Represent arguments charitably, distinguish validity from truth, formalize logic when useful, and identify assumptions."
    },
    "art": {
        "label": "Art",
        "aliases": ("art", "visual_art"),
        "keywords": ("art history", "composition", "perspective", "formal analysis", "visual culture"),
        "instruction": "Explain studio, critique, visual-analysis, and art-history concepts constructively."
    },
    "music": {
        "label": "Music",
        "aliases": ("music", "band", "chorus"),
        "keywords": ("counterpoint", "harmony", "voice leading", "music theory", "form analysis"),
        "instruction": "Explain music theory, history, analysis, and performance concepts without reproducing copyrighted lyrics."
    },
    "media": {
        "label": "Media/Audio-Video",
        "aliases": ("media", "av", "audio_video", "film"),
        "keywords": ("cinematography", "editing", "sound design", "media studies", "film theory", "production"),
        "instruction": "Teach media production, analysis, audio/video, film language, and project planning."
    },
    "language": {
        "label": "World Language",
        "aliases": ("lang", "language", "spanish", "french", "german", "latin"),
        "keywords": ("translation", "syntax", "morphology", "conjugate", "spanish", "french", "german", "latin"),
        "instruction": "Teach grammar, vocabulary, translation, composition, and linguistic nuance at the requested level."
    },
    "health": {
        "label": "Health/Nutrition",
        "aliases": ("health", "nutrition"),
        "keywords": ("nutrition", "nutrient", "dietary", "wellness", "public health"),
        "instruction": "Give age-appropriate educational health information and avoid diagnosis, unsafe body-image guidance, or restrictive-eating advice."
    },
    "physical_education": {
        "label": "Physical Education",
        "aliases": ("pe", "physical_education", "fitness"),
        "keywords": ("physical education", "fitness", "sports rules", "training principle"),
        "instruction": "Explain PE and exercise-science concepts without promoting over-exercise or unsafe training."
    },
}


# Common U.S. high-school course profiles. These supplement the broader
# discipline profiles above so the calculator can route by the actual course
# name a student sees on a schedule. The general profile remains a fallback
# for district-specific or uncommon electives.
SUBJECTS.update({
    "pre_algebra": {
        "label": "Pre-Algebra",
        "aliases": ("prealgebra", "pre-algebra"),
        "keywords": ("pre algebra", "pre-algebra", "integer operations", "proportions", "basic equations"),
        "instruction": "Emphasize number sense, proportional reasoning, introductory equations, and clear arithmetic checks."
    },
    "algebra_1": {
        "label": "Algebra I",
        "aliases": ("algebra1", "algebra_1"),
        "keywords": ("algebra 1", "algebra i", "linear function", "slope intercept", "systems of linear"),
        "instruction": "Focus on linear equations/functions, systems, inequalities, exponents, and introductory quadratics with explicit steps."
    },
    "algebra_2": {
        "label": "Algebra II",
        "aliases": ("algebra2", "algebra_2"),
        "keywords": ("algebra 2", "algebra ii", "complex number", "rational function", "exponential function", "logarithmic function"),
        "instruction": "Cover polynomial, rational, exponential, logarithmic, radical, and complex-number work with domain checks."
    },
    "trigonometry": {
        "label": "Trigonometry",
        "aliases": ("trig", "trigonometry"),
        "keywords": ("trigonometry", "trig identity", "unit circle", "sine law", "cosine law"),
        "instruction": "Use radians/degrees carefully, unit-circle definitions, identities, equations, and triangle laws."
    },
    "precalculus": {
        "label": "Precalculus",
        "aliases": ("precalc", "precalculus"),
        "keywords": ("precalculus", "pre calc", "function composition", "inverse function", "conic section"),
        "instruction": "Connect algebra, functions, trigonometry, analytic geometry, sequences, and limits as preparation for calculus."
    },
    "consumer_math": {
        "label": "Consumer Math",
        "aliases": ("consumer_math",),
        "keywords": ("consumer math", "sales tax", "unit price", "simple interest", "compound interest"),
        "instruction": "Use practical arithmetic, percentages, rates, taxes, discounts, interest, budgeting, and unit pricing."
    },
    "financial_math": {
        "label": "Financial Math",
        "aliases": ("financial_math",),
        "keywords": ("financial math", "amortization", "compound interest", "annuity", "loan payment"),
        "instruction": "Teach time value of money, interest, loans, annuities, budgeting, and financial formulas with stated assumptions."
    },
    "physical_science": {
        "label": "Physical Science",
        "aliases": ("physical_science",),
        "keywords": ("physical science", "matter and energy", "motion and forces", "chemical physical change"),
        "instruction": "Integrate introductory chemistry and physics concepts using units, models, and evidence."
    },
    "astronomy": {
        "label": "Astronomy",
        "aliases": ("astronomy",),
        "keywords": ("astronomy", "stellar", "galaxy", "hertzsprung", "redshift", "cosmology"),
        "instruction": "Explain celestial motion, stars, galaxies, spectra, gravity, and cosmology with quantitative reasoning when appropriate."
    },
    "forensic_science": {
        "label": "Forensic Science",
        "aliases": ("forensics", "forensic_science"),
        "keywords": ("forensic science", "crime lab", "trace evidence", "fingerprint analysis", "forensic chemistry"),
        "instruction": "Cover classroom forensic-science concepts, evidence evaluation, uncertainty, and lab interpretation without facilitating wrongdoing."
    },
    "marine_science": {
        "label": "Marine Science",
        "aliases": ("marine_biology", "marine_science"),
        "keywords": ("marine science", "marine biology", "ocean ecosystem", "salinity", "ocean current"),
        "instruction": "Connect oceanography, marine ecology, organisms, chemistry, geology, and human impacts."
    },
    "composition": {
        "label": "English Composition",
        "aliases": ("composition", "english_composition"),
        "keywords": ("composition class", "expository essay", "argument essay", "rhetorical analysis"),
        "instruction": "Teach thesis development, organization, evidence, rhetorical choices, revision, grammar, and source integration."
    },
    "american_literature": {
        "label": "American Literature",
        "aliases": ("american_lit", "american_literature"),
        "keywords": ("american literature", "american lit", "transcendentalism", "harlem renaissance"),
        "instruction": "Analyze American literature through textual evidence, historical context, literary movements, and multiple interpretations."
    },
    "british_literature": {
        "label": "British Literature",
        "aliases": ("british_lit", "british_literature"),
        "keywords": ("british literature", "british lit", "romantic poets", "victorian literature"),
        "instruction": "Analyze British literary texts, periods, genres, language, context, and evidence-based interpretations."
    },
    "world_literature": {
        "label": "World Literature",
        "aliases": ("world_lit", "world_literature"),
        "keywords": ("world literature", "world lit", "global literature"),
        "instruction": "Analyze literature across cultures and periods while grounding claims in the supplied text and context."
    },
    "creative_writing": {
        "label": "Creative Writing",
        "aliases": ("creative_writing",),
        "keywords": ("creative writing", "short story workshop", "poetry workshop", "fiction writing"),
        "instruction": "Help with character, voice, structure, imagery, revision, and craft while preserving the student's own creative choices."
    },
    "journalism": {
        "label": "Journalism",
        "aliases": ("journalism", "news_writing"),
        "keywords": ("journalism", "news article", "inverted pyramid", "feature story", "journalistic ethics"),
        "instruction": "Teach reporting, sourcing, verification, interviewing, news structure, media ethics, and attribution."
    },
    "speech_debate": {
        "label": "Speech/Debate",
        "aliases": ("speech", "debate", "speech_debate"),
        "keywords": ("speech class", "debate class", "public speaking", "argument case", "rebuttal"),
        "instruction": "Teach public speaking, argument structure, evidence evaluation, rebuttal, delivery, and audience adaptation."
    },
    "media_literacy": {
        "label": "Media Literacy",
        "aliases": ("media_literacy",),
        "keywords": ("media literacy", "source credibility", "misinformation", "advertising analysis"),
        "instruction": "Teach source evaluation, persuasive techniques, verification, bias analysis, and responsible media consumption."
    },
    "world_history": {
        "label": "World History",
        "aliases": ("world_history",),
        "keywords": ("world history", "global history", "ancient civilizations", "world war"),
        "instruction": "Use chronology, geography, causation, comparison, primary sources, and global context."
    },
    "us_history": {
        "label": "U.S. History",
        "aliases": ("us_history", "american_history"),
        "keywords": ("u.s. history", "us history", "american history", "reconstruction", "new deal"),
        "instruction": "Explain U.S. history with chronology, primary-source context, multiple causes, and evidence-based interpretations."
    },
    "european_history": {
        "label": "European History",
        "aliases": ("euro_history", "european_history"),
        "keywords": ("european history", "euro history", "renaissance", "reformation", "enlightenment"),
        "instruction": "Use chronology, political/social/economic context, causation, and source analysis across European history."
    },
    "civics": {
        "label": "Civics",
        "aliases": ("civics",),
        "keywords": ("civics class", "citizenship", "civic duty", "local government"),
        "instruction": "Explain civic institutions, rights, responsibilities, public processes, and constitutional principles neutrally."
    },
    "personal_finance": {
        "label": "Personal Finance",
        "aliases": ("personal_finance",),
        "keywords": ("personal finance", "budget", "credit score", "checking account", "saving"),
        "instruction": "Teach budgeting, banking, credit, saving, taxes, insurance, and investing concepts educationally, not as personalized financial advice."
    },
    "anthropology": {
        "label": "Anthropology",
        "aliases": ("anthropology",),
        "keywords": ("anthropology", "cultural anthropology", "archaeology", "human evolution"),
        "instruction": "Explain cultural, biological, linguistic, and archaeological perspectives using evidence and respectful terminology."
    },
    "marketing": {
        "label": "Marketing",
        "aliases": ("marketing",),
        "keywords": ("marketing class", "target market", "marketing mix", "segmentation", "branding"),
        "instruction": "Teach segmentation, targeting, positioning, consumer behavior, marketing mix, branding, research, and metrics."
    },
    "entrepreneurship": {
        "label": "Entrepreneurship",
        "aliases": ("entrepreneurship",),
        "keywords": ("entrepreneurship", "business plan", "startup idea", "value proposition"),
        "instruction": "Teach opportunity identification, value propositions, business models, costs/revenue, customer discovery, and ethical decision-making."
    },
    "business_law": {
        "label": "Business Law",
        "aliases": ("business_law",),
        "keywords": ("business law", "contract law", "tort", "consumer law"),
        "instruction": "Explain high-school business-law concepts educationally, distinguish legal principles from legal advice, and note jurisdictional differences."
    },
    "web_development": {
        "label": "Web Development",
        "aliases": ("web_dev", "web_development"),
        "keywords": ("web development", "html css", "javascript website", "web design"),
        "instruction": "Teach HTML, CSS, JavaScript, accessibility, responsive design, debugging, and safe web-development practices."
    },
    "cybersecurity": {
        "label": "Cybersecurity",
        "aliases": ("cyber", "cybersecurity"),
        "keywords": ("cybersecurity class", "network security", "authentication", "encryption basics"),
        "instruction": "Teach defensive security, networking, authentication, cryptography concepts, ethics, and safe lab practices; do not facilitate unauthorized access."
    },
    "information_technology": {
        "label": "Information Technology",
        "aliases": ("it", "information_technology"),
        "keywords": ("information technology", "computer hardware", "operating system", "networking basics"),
        "instruction": "Teach hardware, operating systems, networking, troubleshooting, data, and support concepts with safe procedures."
    },
    "robotics": {
        "label": "Robotics",
        "aliases": ("robotics",),
        "keywords": ("robotics", "robot control", "sensor", "actuator", "robot programming"),
        "instruction": "Teach robotics systems, sensors, actuators, control logic, programming, design, testing, and classroom safety."
    },
    "electronics": {
        "label": "Electronics",
        "aliases": ("electronics",),
        "keywords": ("electronics class", "resistor", "breadboard", "digital logic", "microcontroller"),
        "instruction": "Teach circuit theory, components, digital logic, measurement, and low-voltage classroom electronics with safety-first guidance."
    },
    "cad_drafting": {
        "label": "CAD/Drafting",
        "aliases": ("cad", "drafting", "cad_drafting"),
        "keywords": ("cad class", "drafting", "orthographic projection", "dimensioning"),
        "instruction": "Teach technical drawing, CAD constraints, dimensioning, tolerances, orthographic views, and design communication."
    },
    "career_readiness": {
        "label": "Career Readiness",
        "aliases": ("career_readiness", "work_based_learning"),
        "keywords": ("career readiness", "resume class", "interview skills", "workplace skills"),
        "instruction": "Teach workplace communication, resumes, interviews, teamwork, professionalism, planning, and career exploration."
    },
    "animal_science": {
        "label": "Animal Science",
        "aliases": ("animal_science",),
        "keywords": ("animal science", "livestock science", "animal nutrition", "animal genetics"),
        "instruction": "Explain animal biology, nutrition, genetics, management, welfare, and agribusiness with safe classroom-level guidance."
    },
    "plant_science": {
        "label": "Plant Science",
        "aliases": ("plant_science", "horticulture"),
        "keywords": ("plant science", "horticulture", "soil science", "plant propagation"),
        "instruction": "Teach plant biology, soils, horticulture, propagation concepts, ecosystems, and agribusiness safely."
    },
    "construction_trades": {
        "label": "Construction Trades",
        "aliases": ("construction_trades",),
        "keywords": ("construction class", "carpentry class", "building trades", "blueprint reading"),
        "instruction": "Teach measurements, drawings, materials, codes, estimating, and safety concepts; defer hazardous hands-on procedures to qualified instructors and official safety rules."
    },
    "automotive_technology": {
        "label": "Automotive Technology",
        "aliases": ("automotive", "auto_tech", "automotive_technology"),
        "keywords": ("automotive technology", "auto tech", "engine systems", "vehicle diagnostics"),
        "instruction": "Teach automotive systems, diagnostics theory, maintenance concepts, and safety; defer hazardous repair procedures and lifting/electrical work to qualified instruction."
    },
    "culinary_arts": {
        "label": "Culinary Arts",
        "aliases": ("culinary", "culinary_arts"),
        "keywords": ("culinary arts", "food safety", "mise en place", "culinary class"),
        "instruction": "Teach culinary principles, food safety, nutrition, recipe math, kitchen organization, and supervised school-lab techniques safely."
    },
    "family_consumer_science": {
        "label": "Family/Consumer Science",
        "aliases": ("facs", "family_consumer_science"),
        "keywords": ("family consumer science", "facs", "consumer science"),
        "instruction": "Teach family resources, consumer skills, nutrition, textiles, housing, relationships, and life-management concepts age-appropriately."
    },
    "child_development": {
        "label": "Child Development",
        "aliases": ("child_development",),
        "keywords": ("child development", "early childhood", "developmental milestone"),
        "instruction": "Teach developmental stages, learning, caregiving concepts, observation, and child-safety principles without diagnosing children."
    },
    "drawing_painting": {
        "label": "Drawing/Painting",
        "aliases": ("drawing", "painting", "drawing_painting"),
        "keywords": ("drawing class", "painting class", "charcoal drawing", "watercolor"),
        "instruction": "Teach observation, proportion, perspective, value, composition, color, materials, critique, and studio practice."
    },
    "graphic_design": {
        "label": "Graphic Design",
        "aliases": ("graphic_design",),
        "keywords": ("graphic design", "typography", "layout design", "visual hierarchy"),
        "instruction": "Teach typography, layout, hierarchy, color, branding, digital workflows, critique, and accessibility."
    },
    "photography": {
        "label": "Photography",
        "aliases": ("photography",),
        "keywords": ("photography class", "aperture", "shutter speed", "iso photography"),
        "instruction": "Teach exposure, composition, focus, lighting, lenses, visual storytelling, editing, and critique."
    },
    "ceramics_sculpture": {
        "label": "Ceramics/Sculpture",
        "aliases": ("ceramics", "sculpture", "ceramics_sculpture"),
        "keywords": ("ceramics class", "sculpture class", "clay", "three dimensional art"),
        "instruction": "Teach form, structure, composition, materials, critique, and supervised studio safety."
    },
    "art_history": {
        "label": "Art History",
        "aliases": ("art_history",),
        "keywords": ("art history", "art movement", "renaissance art", "modern art"),
        "instruction": "Analyze artworks using formal qualities, iconography, historical context, patronage, materials, and multiple scholarly interpretations."
    },
    "music_theory": {
        "label": "Music Theory",
        "aliases": ("music_theory",),
        "keywords": ("music theory", "voice leading", "roman numeral analysis", "chord progression"),
        "instruction": "Teach notation, scales, intervals, harmony, rhythm, form, analysis, ear-training concepts, and voice leading."
    },
    "band_orchestra": {
        "label": "Band/Orchestra",
        "aliases": ("band", "orchestra", "band_orchestra"),
        "keywords": ("band class", "orchestra class", "ensemble rehearsal", "instrumental music"),
        "instruction": "Teach ensemble skills, notation, rhythm, intonation, articulation, interpretation, rehearsal vocabulary, and music history."
    },
    "choir": {
        "label": "Choir",
        "aliases": ("choir", "chorus"),
        "keywords": ("choir class", "chorus class", "choral music", "sight singing"),
        "instruction": "Teach choral notation, rhythm, diction, ensemble skills, sight-singing, interpretation, and healthy non-straining vocal concepts."
    },
    "theater": {
        "label": "Theater",
        "aliases": ("theatre", "theater", "drama"),
        "keywords": ("theater class", "theatre class", "drama class", "stagecraft"),
        "instruction": "Teach acting analysis, directing, script study, stage vocabulary, design, production roles, and theater history."
    },
    "dance": {
        "label": "Dance",
        "aliases": ("dance",),
        "keywords": ("dance class", "choreography", "dance history"),
        "instruction": "Teach choreography, rhythm, terminology, history, analysis, and safe age-appropriate movement principles without promoting over-exercise."
    },
    "film_studies": {
        "label": "Film Studies",
        "aliases": ("film_studies",),
        "keywords": ("film studies", "film analysis", "cinema history", "mise en scene"),
        "instruction": "Analyze cinematography, editing, sound, narrative, genre, representation, history, and critical frameworks."
    },
    "spanish": {
        "label": "Spanish",
        "aliases": ("spanish",),
        "keywords": ("spanish class", "espanol", "spanish grammar", "spanish vocabulary"),
        "instruction": "Teach Spanish vocabulary, grammar, conjugation, reading, writing, listening concepts, and cultural context at the requested level."
    },
    "french": {
        "label": "French",
        "aliases": ("french",),
        "keywords": ("french class", "francais", "french grammar", "french vocabulary"),
        "instruction": "Teach French vocabulary, grammar, conjugation, reading, writing, and cultural context at the requested level."
    },
    "german": {
        "label": "German",
        "aliases": ("german",),
        "keywords": ("german class", "deutsch", "german grammar", "german vocabulary"),
        "instruction": "Teach German cases, vocabulary, grammar, word order, reading, writing, and cultural context."
    },
    "latin": {
        "label": "Latin",
        "aliases": ("latin",),
        "keywords": ("latin class", "latin grammar", "declension", "latin translation"),
        "instruction": "Teach Latin morphology, syntax, vocabulary, translation, Roman context, and derivations."
    },
    "asl": {
        "label": "American Sign Language",
        "aliases": ("asl", "american_sign_language"),
        "keywords": ("asl class", "american sign language", "deaf culture"),
        "instruction": "Teach ASL vocabulary/grammar concepts and Deaf-culture context while noting that visual signing is best learned with qualified human/video instruction."
    },
    "nutrition": {
        "label": "Nutrition",
        "aliases": ("nutrition",),
        "keywords": ("nutrition class", "macronutrient", "micronutrient", "dietary guideline"),
        "instruction": "Teach evidence-based nutrition concepts without restrictive-eating, weight-loss, or body-ideal guidance."
    },
    "sports_medicine": {
        "label": "Sports Medicine",
        "aliases": ("sports_medicine", "athletic_training"),
        "keywords": ("sports medicine class", "athletic training class", "sports injury prevention"),
        "instruction": "Teach anatomy, injury-prevention concepts, emergency-response principles, and rehabilitation theory educationally; do not diagnose or provide unsafe treatment instructions."
    },
    "exercise_science": {
        "label": "Exercise Science",
        "aliases": ("exercise_science", "kinesiology"),
        "keywords": ("exercise science", "kinesiology", "exercise physiology"),
        "instruction": "Teach biomechanics, physiology, fitness principles, and movement science without promoting over-exercise or unsafe training."
    },
    "drivers_education": {
        "label": "Driver Education",
        "aliases": ("drivers_ed", "drivers_education"),
        "keywords": ("driver education", "drivers ed", "road signs", "right of way"),
        "instruction": "Teach traffic laws, road signs, defensive-driving concepts, risk awareness, and safe decision-making; never encourage stunts or dangerous driving."
    },
    "jrotc_leadership": {
        "label": "JROTC/Leadership",
        "aliases": ("jrotc", "leadership"),
        "keywords": ("jrotc class", "leadership class", "leadership principles"),
        "instruction": "Teach leadership, organization, citizenship, communication, history, and fitness theory without weapons instruction."
    },
    "yearbook": {
        "label": "Yearbook",
        "aliases": ("yearbook",),
        "keywords": ("yearbook class", "yearbook spread", "caption writing"),
        "instruction": "Teach interviewing, captions, copy editing, layout, photography selection, deadlines, ethics, and publication workflows."
    },
    "study_skills": {
        "label": "Study Skills",
        "aliases": ("study_skills", "academic_support"),
        "keywords": ("study skills", "academic support", "note taking", "time management"),
        "instruction": "Teach planning, note-taking, retrieval practice, organization, reading strategies, and responsible academic habits."
    }
})

MODES = {
    "answer": "Answer directly, then give a compact justification.",
    "explain": "Teach the concept clearly with definitions, relationships, and an example when useful.",
    "steps": "Work through the problem in ordered steps, showing enough reasoning to learn the method.",
    "check": "Review the student's work, identify the first meaningful error if any, and explain how to fix it.",
    "quiz": "Ask one useful question at a time and wait for the student's response instead of immediately giving the final answer.",
    "summary": "Produce a compact study summary with the most important concepts and relationships.",
    "flashcards": "Create concise question/answer flashcards suitable for quick review on a small screen.",
    "derive": "Derive the requested result from definitions or governing equations, stating assumptions and intermediate steps.",
    "proof": "Give a rigorous proof appropriate to the requested academic level; identify definitions, hypotheses, and the proof strategy.",
    "research": "Help structure academic research, methodology, argumentation, and source use; never invent citations, quotations, or data."
}

LEVELS = {
    "auto": "Infer the appropriate academic depth from the course/topic wording and the user's prompt. Do not oversimplify advanced material.",
    "school": "Use secondary-school depth: clear foundations, standard notation, and explicit instructional steps.",
    "college": "Use undergraduate college rigor. Assume normal course prerequisites, use discipline-specific terminology, show derivations and theorem conditions where relevant, and explain both method and meaning.",
    "advanced": "Use upper-undergraduate to early-graduate rigor. Prefer formal definitions, proofs or derivations where appropriate, explicit assumptions, abstraction, edge cases, and connections to broader theory."
}

DEFAULT_MODE = "explain"
DEFAULT_LEVEL = "auto"


def normalize_subject(value):
    value = (value or "").strip().lower()
    if not value or value == "auto":
        return "auto"
    for key, data in SUBJECTS.items():
        if value == key or value in data["aliases"]:
            return key
    return "general"


def normalize_mode(value):
    value = (value or "").strip().lower()
    return value if value in MODES else DEFAULT_MODE


def normalize_level(value):
    value = (value or "").strip().lower()
    return value if value in LEVELS else DEFAULT_LEVEL


def detect_subject(text):
    lower = (text or "").lower()
    best_key = "general"
    best_score = 0

    for key, data in SUBJECTS.items():
        if key == "general":
            continue

        score = 0
        for keyword in data["keywords"]:
            if keyword and keyword in lower:
                score += 1

        if score > best_score:
            best_key = key
            best_score = score

    return best_key


def resolve_subject(requested, prompt):
    requested = normalize_subject(requested)
    return detect_subject(prompt) if requested == "auto" else requested


def subject_label(subject):
    return SUBJECTS.get(subject, SUBJECTS["general"])["label"]


def build_system_prompt(base_prompt, subject, mode, level="auto"):
    subject = subject if subject in SUBJECTS else "general"
    mode = normalize_mode(mode)
    level = normalize_level(level)

    return (
        base_prompt.strip()
        + "\n\nSUBJECT: " + SUBJECTS[subject]["label"]
        + "\nACADEMIC LEVEL: " + level
        + "\nTUTOR MODE: " + mode
        + "\nLEVEL RULE: " + LEVELS[level]
        + "\nSUBJECT RULE: " + SUBJECTS[subject]["instruction"]
        + "\nMODE RULE: " + MODES[mode]
        + "\nRIGOR RULE: For quantitative work, preserve units and assumptions. For theorem-based work, state hypotheses. "
          "For proofs, do not skip the key logical step. For research/writing, do not fabricate citations, quotations, or data."
        + "\nSCREEN RULE: Prefer compact formatting that reads well on a 384x216 calculator display. "
          "Use short paragraphs and equations, but do not sacrifice necessary rigor."
        + "\nASSESSMENT RULE: If the student explicitly says this is a currently active, locked, or graded test/exam, "
          "do not provide a hidden-answer or bypass workflow. Give concept help, explain the method, or help them study instead."
    )
