#!/usr/bin/python3.7
import tkinter as tk
import serial
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
import time

matplotlib.use("TkAgg")
style.use("ggplot")
initial_time = time.time()

ser = serial.Serial('/dev/ttyUSB2',115200)

root = tk.Tk()
root.title("AUVMEC")
root.geometry("1100x500")

sliderframe = tk.Frame(root,bd=55)
sliderframe.pack(side = tk.LEFT,expand=True)

plotframe = tk.Frame(root,bd=35)
plotframe.pack(side = tk.RIGHT,expand=True)

time_step = 50

values = []

fig = Figure(figsize=(5,5), dpi=100)
ax = fig.add_subplot(111)

xs = []
ys = []
zs = []

def animate(i, xs, ys,zs):

    values = [Depth.get(),Kp.get(),Ki.get(),Kd.get()]
    # print(values)

    xs.append(time.time() - initial_time)
    ys.append(values[0])

    xs = xs[-200:]
    ys = ys[-200:]

    # a = '2.00,50.00,50.00,50.00>'
    # ser.write(a.encode('ascii'))

    temp_str = ""
    temp_str += str(values[0])
    temp_str += ","
    temp_str += str(values[1])
    temp_str += ","
    temp_str += str(values[2])
    temp_str += ","
    temp_str += str(values[3])
    temp_str += ">"

    ser.write(temp_str.encode('ascii'))
    sensor = float(ser.readline())

    zs.append(sensor)
    zs = zs[-200:]


    ax.clear()
    ax.plot(xs,ys,'r')
    ax.plot(xs,zs,'b')
    ax.set_xlabel("Time(s)")
    ax.set_ylabel("Depth(m)")



Depth = tk.Scale(sliderframe,label="Depth",activebackground="#0000ff",orient='horizontal', length=400, from_=0.00, to=2.00, resolution=0.01)
Depth.pack()

Kp = tk.Scale(sliderframe,label="Kp",activebackground="#00ff00",orient='horizontal', length=400, from_=0.0, to=50.0, resolution=0.05)
Kp.pack()

Ki = tk.Scale(sliderframe,label="Ki",activebackground="#00ff00",orient='horizontal', length=400, from_=0.0, to=50.0, resolution=0.05)
Ki.pack()

Kd = tk.Scale(sliderframe,label="Kd",activebackground="#00ff00",orient='horizontal', length=400, from_=0.0, to=50.0, resolution=0.05)
Kd.pack()

canvas = FigureCanvasTkAgg(fig,plotframe)
canvas.draw()
canvas.get_tk_widget().pack()

# root.after(time_step,see)

ani = animation.FuncAnimation(fig,animate,fargs=(xs,ys,zs),interval=time_step)

root.mainloop()