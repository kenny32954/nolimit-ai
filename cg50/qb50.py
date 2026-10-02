# Quantum Breaks AI - fx-CG50 pocket client prototype
# Designed for Casio's adapted MicroPython 1.9.4.
# This file intentionally uses only basic built-ins for compatibility.

WIDTH = 21
VERSION = "0.1.0"

history = []

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
    print("CG50 POCKET CLIENT")
    rule("=")
    print("Prototype " + VERSION)
    print("Type /help")
    rule()

def help_screen():
    print("/help  commands")
    print("/about project info")
    print("/history recent text")
    print("/clear  clear history")
    print("/math   quick calc")
    print("/quit   exit")
    rule()
    print("Live AI transport is")
    print("being built as a")
    print("native CG50 add-in.")

def about():
    print("Quantum Breaks AI")
    print("fx-CG50 client.")
    print("Offline shell now;")
    print("serial live mode next.")

def math_mode():
    print("Quick Calc")
    print("Examples: 2+2")
    print("or (8*7)-3")
    expr = input("expr> ")
    try:
        # Local calculator input only. No globals or imported modules.
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
    # This deliberately does not pretend to be an AI model.
    if text.endswith("?"):
        return "Saved locally. Live AI answers will use the serial bridge."
    return "Saved locally. Live AI mode is the next integration stage."

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
        elif text == "/help":
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
        else:
            history.append(text)
            say("Q: ", text)
            say("A: ", local_reply(text))

main()
