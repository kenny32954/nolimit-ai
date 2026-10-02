# Quantum Breaks AI - fx-CG50 pocket client prototype
# Designed for Casio's adapted MicroPython 1.9.4.
# Conservative built-ins only for calculator compatibility.

WIDTH = 21
VERSION = "0.4.0"

history = []
subject = "auto"
mode = "explain"
level = "auto"

SUBJECTS = (
    "auto", "math", "algebra", "geometry", "statistics", "calculus",
    "linear_algebra", "discrete_math", "differential_equations",
    "number_theory", "real_analysis", "abstract_algebra",
    "biology", "genetics", "microbiology", "anatomy_physiology",
    "chemistry", "organic_chemistry", "biochemistry",
    "physics", "thermodynamics", "circuits",
    "earth", "environmental_science",
    "ela", "literature", "writing",
    "history", "social_studies", "geography", "government", "political_science",
    "economics", "finance", "business", "accounting",
    "computer_science", "data_structures", "algorithms", "databases",
    "computer_architecture", "engineering", "statics_dynamics",
    "materials_science", "cte", "agriculture",
    "psychology", "research_methods", "sociology", "philosophy_logic",
    "art", "music", "media", "language", "health", "physical_education"
)

MODES = (
    "answer", "explain", "steps", "check", "quiz",
    "summary", "flashcards", "derive", "proof", "research"
)

LEVELS = ("auto", "school", "college", "advanced")


def rule(ch="-"):
    print(ch * WIDTH)


def wrap(text, width=WIDTH):
    text = str(text)
    out = []
    while len(text) > width:
        cut = width
        space = text.rfind(" ", 0, width + 1)
        if space > 0:
            cut = space
        out.append(text[:cut])
        text = text[cut:].lstrip()
    out.append(text)
    return out


def say(prefix, text):
    first = True
    for line in wrap(text):
        if first:
            print(prefix + line)
            first = False
        else:
            print("   " + line)


def banner():
    print("QUANTUM BREAKS AI")
    print("CG50 ACADEMIC CLIENT")
    rule("=")
    print("Prototype " + VERSION)
    print("Type /help")
    rule()
    show_status()


def show_status():
    say("S: ", subject)
    say("M: ", mode)
    say("L: ", level)


def list_values(title, values):
    print(title)
    i = 0
    while i < len(values):
        print(values[i])
        i += 1


def set_subject(value):
    global subject
    value = value.strip().lower()
    if value in SUBJECTS:
        subject = value
        say("Subject: ", subject)
    else:
        print("Unknown subject.")


def set_mode(value):
    global mode
    value = value.strip().lower()
    if value in MODES:
        mode = value
        say("Mode: ", mode)
    else:
        print("Unknown mode.")


def set_level(value):
    global level
    value = value.strip().lower()
    if value in LEVELS:
        level = value
        say("Level: ", level)
    else:
        print("Unknown level.")


def help_screen():
    print("/subject NAME")
    print("/mode NAME")
    print("/level NAME")
    print("/subjects")
    print("/modes")
    print("/levels")
    print("/status")
    print("/history")
    print("/clear")
    print("/math")
    print("/about")
    print("/quit")


def about():
    print("Quantum Breaks AI")
    print("fx-CG50 academic")
    print("assistant prototype.")
    print("School -> college")
    print("-> advanced routing.")


def math_mode():
    print("Quick Calc")
    expr = input("expr> ")
    try:
        value = eval(expr, {"__builtins__": {}}, {})
        say("= ", value)
    except:
        print("Could not evaluate.")


def show_history():
    if not history:
        print("No history yet.")
        return
    start = len(history) - 6
    if start < 0:
        start = 0
    i = start
    while i < len(history):
        say("> ", history[i])
        i += 1


def local_reply(text):
    return (
        "Saved. Live mode: "
        + subject + " / "
        + mode + " / "
        + level + "."
    )


def handle_command(text):
    if text == "/help":
        help_screen()
    elif text == "/about":
        about()
    elif text == "/history":
        show_history()
    elif text == "/clear":
        history[:] = []
        print("History cleared.")
    elif text == "/math":
        math_mode()
    elif text == "/subjects":
        list_values("Subjects:", SUBJECTS)
    elif text == "/modes":
        list_values("Modes:", MODES)
    elif text == "/levels":
        list_values("Levels:", LEVELS)
    elif text == "/status":
        show_status()
    elif text.startswith("/subject "):
        set_subject(text[9:])
    elif text.startswith("/mode "):
        set_mode(text[6:])
    elif text.startswith("/level "):
        set_level(text[7:])
    else:
        print("Unknown command.")
        print("Use /help")


def main():
    banner()
    while True:
        try:
            text = input("> ")
        except:
            print("Input ended.")
            break

        if not text:
            continue

        if text == "/quit":
            print("Bye.")
            break
        elif text.startswith("/"):
            handle_command(text)
        else:
            history.append(text)
            say("Q: ", text)
            say("A: ", local_reply(text))


main()
