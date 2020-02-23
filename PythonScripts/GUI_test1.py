import tkinter as tk
from tkinter import filedialog, Text
import os

root = tk.Tk()
# canvas = tk.Canvas(root,height=700,width=700,bg="#263D42")
# canvas.pack()

#frame
# frame = tk.Frame(root,bg="white")
# frame.place(relwidth=0.8,relheight=0.8,relx = 0.1,rely = 0.1)

#slider
slider1 = tk.Scale(root,from_=1100, to=1900,length=700,tickinterval=100)
slider1.pack()
def cj():
    a = e1.get()
    print(a)
    

# tk.Label(root,text="1").grid(row=0)
# e1 = tk.Entry(root)
# e1.grid(row=0,column=1)
# Button = tk.Button(root,text='see',command=cj).grid(row=3,column=0)
root.mainloop()
