"""Subject and tutoring-mode routing for Quantum Breaks AI CG50."""

SUBJECTS = {
    "general": {
        "label": "General",
        "aliases": ("gen", "general"),
        "keywords": (),
        "instruction": "Handle general schoolwork clearly and at the student's level."
    },
    "math": {
        "label": "Math",
        "aliases": ("math", "mathematics"),
        "keywords": ("equation", "solve", "fraction", "decimal", "percent", "ratio", "proportion", "radical"),
        "instruction": "Show mathematically correct work, define variables, preserve units, and verify arithmetic when practical."
    },
    "algebra": {
        "label": "Algebra",
        "aliases": ("alg", "algebra"),
        "keywords": ("linear equation", "quadratic", "polynomial", "factor", "slope", "system of equations", "inequality"),
        "instruction": "Use clean algebraic steps, keep both sides balanced, and check solutions when practical."
    },
    "geometry": {
        "label": "Geometry",
        "aliases": ("geo", "geometry"),
        "keywords": ("triangle", "angle", "circle", "perimeter", "area", "volume", "pythagorean", "congruent", "similar"),
        "instruction": "State relevant theorems or formulas and track units carefully."
    },
    "statistics": {
        "label": "Statistics",
        "aliases": ("stat", "stats", "statistics"),
        "keywords": ("mean", "median", "probability", "standard deviation", "regression", "distribution", "sample"),
        "instruction": "Distinguish population from sample and explain what calculated values mean in context."
    },
    "calculus": {
        "label": "Calculus",
        "aliases": ("calc", "calculus"),
        "keywords": ("derivative", "integral", "limit", "differentiate", "integrate", "slope field"),
        "instruction": "Show symbolic steps and note domain, constants, and units where relevant."
    },
    "biology": {
        "label": "Biology",
        "aliases": ("bio", "biology"),
        "keywords": ("cell", "dna", "rna", "mitosis", "meiosis", "organism", "photosynthesis", "evolution"),
        "instruction": "Use accurate biological terminology and connect structures, processes, and functions."
    },
    "chemistry": {
        "label": "Chemistry",
        "aliases": ("chem", "chemistry"),
        "keywords": ("mole", "molar", "atom", "ion", "bond", "reaction", "periodic table", "stoichiometry", "ph"),
        "instruction": "Balance equations when needed, track units and significant figures, and explain chemical reasoning."
    },
    "physics": {
        "label": "Physics",
        "aliases": ("phys", "physics"),
        "keywords": ("velocity", "acceleration", "force", "energy", "momentum", "voltage", "current", "resistance"),
        "instruction": "List knowns and unknowns when useful, select the governing equation, and keep units throughout."
    },
    "earth": {
        "label": "Earth/Space",
        "aliases": ("earth", "space", "astronomy", "geology"),
        "keywords": ("planet", "star", "rock", "weathering", "plate tectonics", "climate", "solar system"),
        "instruction": "Explain Earth and space science with clear cause-and-effect relationships."
    },
    "environmental_science": {
        "label": "Environmental Science",
        "aliases": ("environment", "environmental", "env_sci"),
        "keywords": ("ecosystem", "biodiversity", "pollution", "sustainability", "carbon cycle", "renewable", "conservation"),
        "instruction": "Explain environmental systems, tradeoffs, evidence, and cause-and-effect relationships."
    },
    "ela": {
        "label": "English/ELA",
        "aliases": ("ela", "english", "grammar"),
        "keywords": ("grammar", "sentence", "verb", "noun", "adjective", "punctuation", "figurative language"),
        "instruction": "Explain language conventions and literary concepts with concise examples."
    },
    "literature": {
        "label": "Literature",
        "aliases": ("lit", "literature"),
        "keywords": ("character", "setting", "plot", "symbolism", "motif", "tone", "dramatic irony", "poem"),
        "instruction": "Ground literary analysis in the supplied text or clearly identified work; distinguish evidence from interpretation."
    },
    "writing": {
        "label": "Writing",
        "aliases": ("write", "writing", "essay"),
        "keywords": ("essay", "thesis", "paragraph", "introduction", "conclusion", "revise", "argumentative", "narrative"),
        "instruction": "Help plan, draft, revise, and explain writing while preserving the student's intended voice and assignment requirements."
    },
    "history": {
        "label": "History",
        "aliases": ("hist", "history"),
        "keywords": ("war", "revolution", "empire", "century", "historical", "civilization", "industrialization"),
        "instruction": "Use dates and chronology carefully, distinguish primary facts from interpretation, and explain causes and consequences."
    },
    "social_studies": {
        "label": "Social Studies",
        "aliases": ("social", "social_studies"),
        "keywords": ("social studies", "culture", "society", "citizenship", "community", "human geography"),
        "instruction": "Connect history, geography, civics, economics, and culture without collapsing distinct concepts."
    },
    "geography": {
        "label": "Geography",
        "aliases": ("geog", "geography"),
        "keywords": ("map", "latitude", "longitude", "region", "migration", "population density", "physical geography"),
        "instruction": "Explain spatial patterns, regions, human-environment interactions, and map concepts accurately."
    },
    "government": {
        "label": "Government/Civics",
        "aliases": ("gov", "government", "civics"),
        "keywords": ("congress", "senate", "house", "constitution", "amendment", "court", "federalism", "election"),
        "instruction": "Explain institutions, constitutional concepts, and civic processes neutrally and factually."
    },
    "economics": {
        "label": "Economics",
        "aliases": ("econ", "economics"),
        "keywords": ("supply", "demand", "inflation", "gdp", "market", "scarcity", "opportunity cost"),
        "instruction": "Define economic terms, identify assumptions, and separate positive analysis from value judgments."
    },
    "business": {
        "label": "Business",
        "aliases": ("biz", "business", "marketing"),
        "keywords": ("marketing", "management", "entrepreneur", "business", "revenue", "profit", "customer"),
        "instruction": "Use standard business terminology and connect concepts to realistic, age-appropriate examples."
    },
    "accounting": {
        "label": "Accounting",
        "aliases": ("acct", "accounting"),
        "keywords": ("debit", "credit", "balance sheet", "income statement", "asset", "liability", "journal entry"),
        "instruction": "Track debits, credits, classifications, and accounting equations carefully."
    },
    "computer_science": {
        "label": "Computer Science",
        "aliases": ("cs", "computer", "coding", "programming"),
        "keywords": ("code", "python", "javascript", "algorithm", "variable", "loop", "function", "program"),
        "instruction": "Explain code precisely, prefer small testable examples, and call out assumptions and likely errors."
    },
    "engineering": {
        "label": "Engineering",
        "aliases": ("eng", "engineering", "stem"),
        "keywords": ("design process", "prototype", "cad", "engineering", "constraint", "tolerance", "mechanism"),
        "instruction": "Use the engineering design process, identify constraints, and separate calculations from design choices."
    },
    "cte": {
        "label": "CTE/Career Tech",
        "aliases": ("cte", "career", "career_tech", "shop"),
        "keywords": ("career tech", "workplace", "technical drawing", "trade", "employability", "safety procedure"),
        "instruction": "Teach practical career/technical concepts with safe, school-appropriate procedures and terminology."
    },
    "agriculture": {
        "label": "Agriculture",
        "aliases": ("ag", "agriculture"),
        "keywords": ("agriculture", "crop", "soil", "livestock", "horticulture", "ffa", "plant science"),
        "instruction": "Explain agricultural science, plant/animal systems, and agribusiness concepts accurately and safely."
    },
    "psychology": {
        "label": "Psychology",
        "aliases": ("psych", "psychology"),
        "keywords": ("psychology", "memory", "learning", "behavior", "cognition", "developmental", "experiment"),
        "instruction": "Explain psychological concepts and research methods without diagnosing the student or other people."
    },
    "sociology": {
        "label": "Sociology",
        "aliases": ("soc", "sociology"),
        "keywords": ("sociology", "socialization", "institution", "group behavior", "culture", "social structure"),
        "instruction": "Explain sociological concepts, institutions, and research perspectives with careful definitions."
    },
    "art": {
        "label": "Art",
        "aliases": ("art", "visual_art"),
        "keywords": ("drawing", "painting", "composition", "perspective", "value", "color theory", "art history"),
        "instruction": "Explain visual-art concepts, techniques, critique vocabulary, and art-history context constructively."
    },
    "music": {
        "label": "Music",
        "aliases": ("music", "band", "chorus"),
        "keywords": ("rhythm", "tempo", "chord", "scale", "melody", "harmony", "music theory"),
        "instruction": "Explain music theory, notation, history, and performance concepts without reproducing copyrighted lyrics."
    },
    "media": {
        "label": "Media/Audio-Video",
        "aliases": ("media", "av", "audio_video", "film"),
        "keywords": ("camera", "editing", "premiere", "shot", "storyboard", "audio", "video", "film", "lighting"),
        "instruction": "Teach media production, audio/video, film language, editing, and project planning with practical school-safe guidance."
    },
    "language": {
        "label": "World Language",
        "aliases": ("lang", "language", "spanish", "french", "german", "latin"),
        "keywords": ("translate", "conjugate", "vocabulary", "spanish", "french", "german", "latin"),
        "instruction": "Teach vocabulary, grammar, translation, and usage; preserve nuance and explain corrections."
    },
    "health": {
        "label": "Health/Nutrition",
        "aliases": ("health", "nutrition"),
        "keywords": ("nutrition", "nutrient", "dietary", "wellness", "health"),
        "instruction": "Give age-appropriate educational health information and avoid diagnosis, unsafe body-image guidance, or restrictive-eating advice."
    },
    "physical_education": {
        "label": "Physical Education",
        "aliases": ("pe", "physical_education", "fitness"),
        "keywords": ("physical education", "fitness", "warm up", "cardio", "sports rules", "training principle"),
        "instruction": "Explain PE concepts, sports rules, and healthy activity principles without promoting over-exercise or unsafe training."
    },
}

MODES = {
    "answer": "Answer the question directly, then give a compact explanation.",
    "explain": "Teach the concept clearly, using a short explanation and an example when useful.",
    "steps": "Work through the problem in ordered steps, showing enough reasoning for the student to learn the method.",
    "check": "Review the student's work, identify the first meaningful error if any, and explain how to fix it.",
    "quiz": "Act as a tutor: ask one useful question at a time and wait for the student's response instead of immediately giving the final answer.",
    "summary": "Produce a compact study summary with the most important facts and relationships.",
    "flashcards": "Create concise question/answer flashcards suitable for quick review on a small screen.",
}

DEFAULT_MODE = "explain"


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
    if value in MODES:
        return value
    return DEFAULT_MODE


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
    if requested != "auto":
        return requested
    return detect_subject(prompt)


def subject_label(subject):
    return SUBJECTS.get(subject, SUBJECTS["general"])["label"]


def build_system_prompt(base_prompt, subject, mode):
    subject = subject if subject in SUBJECTS else "general"
    mode = normalize_mode(mode)
    subject_rule = SUBJECTS[subject]["instruction"]
    mode_rule = MODES[mode]

    return (
        base_prompt.strip()
        + "\n\nSUBJECT: " + SUBJECTS[subject]["label"]
        + "\nTUTOR MODE: " + mode
        + "\nSUBJECT RULE: " + subject_rule
        + "\nMODE RULE: " + mode_rule
        + "\nSCREEN RULE: Prefer compact formatting that reads well on a 384x216 calculator display. "
          "Use short paragraphs, short equations, and minimal filler."
        + "\nASSESSMENT RULE: If the student explicitly says this is a currently active, locked, or graded test/exam, "
          "do not provide a hidden-answer or bypass workflow. Give concept help, explain the method, or help them study instead."
    )
