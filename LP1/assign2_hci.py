import tkinter as tk

def calculate(expression):
    try:
        num1 = ""
        operator = ""
        num2 = ""
        
        for char in expression:
            if char.isdigit() or char == '.':
                if operator == "":
                    num1 += char
                else:
                    num2 += char
            else:
                operator += char

        num1 = float(num1)
        num2 = float(num2)
        
        if operator == "+":
            return num1 + num2
        elif operator == "-":
            return num1 - num2
        elif operator == "*":
            return num1 * num2
        elif operator == "/":
            if num2 != 0:
                return num1 / num2
            else:
                return "Error: Division by zero"
        else:
            return "Error: Invalid operator"
    except (ValueError, ZeroDivisionError):
        return "Error: Invalid input"

def on_button_click(event):
    text = event.widget.cget("text")

    if text == "=":
        calculation = entry.get()
        result = calculate(calculation)
        entry.delete(0, tk.END)
        entry.insert(tk.END, result)
    elif text == "C":
        entry.delete(0, tk.END)
    else:
        entry.insert(tk.END, text)

root = tk.Tk()
root.geometry("400x400")
root.title("Simple Calculator")

entry = tk.Entry(root, font=("Arial", 18))
entry.pack(fill=tk.BOTH, ipadx=8, padx=10, pady=10, expand=True)

button_frame = tk.Frame(root)
button_frame.pack()

buttons = [
    '7', '8', '9', '+',
    '4', '5', '6', '-',
    '1', '2', '3', '*',
    'C', '0', '=', '/'
]

row, col = 0, 0

for button_text in buttons:
    button = tk.Button(button_frame, text=button_text,
                       font=("Arial", 18), width=5, height=2)
    button.grid(row=row, column=col, padx=5, pady=5)
    button.bind("<Button-1>", on_button_click)
    col += 1
    if col > 3:
        col = 0
        row += 1

root.mainloop()
