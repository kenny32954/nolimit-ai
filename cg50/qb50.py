# Quantum Breaks AI - fx-CG50 pocket client prototype
# Designed for Casio's adapted MicroPython 1.9.4.
# Conservative built-ins only for calculator compatibility.

WIDTH = 21
VERSION = "0.3.0"

history = []
subject = "auto"
mode = "explain"

SUBJECTS = (
    "auto", "math", "algebra",
    "geometry", "statistics",
    "calculus", "biology",
    "chemistry", "physics",
    "earth",
    "environmental_science",
    "ela", "literature",
    "writing", "history",
    "social_studies",
    "geography", "government",
    "economics", "business",
    "accounting",
    "computer_science",
    "engineering", "cte",
    "agriculture",
    "psychology", "sociology",
    "art", "music", "media",
    "language", "health",
    "physical_education"
)

MODES = (
    "answer", "explain",
    "steps", "check",
    "quiz", "summary",
    "flashcards"
)


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
    print("CG50 SCHOOL CLIENT")
    rule("=")
    print("Prototype " + VERSION)
    print("Type /help")
    rule()
    show_status()


def show_status():
    say("S: ", subject)
    say("M: ", mode)


def list_subjects():
    print("Subjects:")
    i = 0
    while i < len(SUBJECTS):
        print(SUBJECTS[i])
        i += 1


def list_modes():
    print("Tutor modes:")
    i = 0
    while i < len(MODES):
        print(MODES[i])
        i += 1


def set_subject(value):
    global subject
    value = value.strip().lower()
    if value in SUBJECTS:
        subject = value
        say("Subject: ", subject)
    else:
        print("Unknown subject.")
        print("Use /subjects")


def set_mode(value):
    global mode
    value = value.strip().lower()
    if value in MODES:
        mode = value
        say("Mode: ", mode)
    else:
        print("Unknown mode.")
        print("Use /modes")


def help_screen():
    print("/subject NAME")
    print("/mode NAME")
    print("/subjects")
    print("/modes")
    print("/status")
    print("/history")
    print("/clear")
    print("/math")
    print("/about")
    print("/quit")
    rule()
    print("All-subject routing")
    print("is ready in bridge.")


def about():
    print("Quantum Breaks AI")
    print("fx-CG50 school")
    print("assistant prototype.")
    print("Named subject packs")
    print("+ general fallback.")


def math_mode():
    print("Quick Calc")
    print("Examples: 2+2")
    print("or (8*7)-3")
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
        "Saved. Live mode will "
        "send this as " + subject
        + " / " + mode + "."
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
        list_subjects()
    elif text == "/modes":
        list_modes()
    elif text == "/status":
        show_status()
    elif text.startswith("/subject "):
        set_subject(text[9:])
    elif text.startswith("/mode "):
        set_mode(text[6:])
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
