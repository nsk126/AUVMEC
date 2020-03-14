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

ser = serial.Serial('COM6',baudrate=115200,timeout=0.1)

root = tk.Tk()
root.title("AUVMEC")
root.geometry("1600x600")

sliderframe = tk.Frame(root,bd=55)
sliderframe.pack(side = tk.LEFT,expand=True)

plotframe = tk.Frame(root,bd=35)
plotframe.pack(side = tk.RIGHT,expand=True)

stopButtonFrame = tk.Frame(root,bd=35)
stopButtonFrame.pack(side=tk.LEFT,expand=True)

time_step = 50

values = []

fig = Figure(figsize=(5,5), dpi=100)
ax = fig.add_subplot(111)

xs = []
ys = []
zs = []

def animate(i, xs, ys, zs):

    values = [Depth.get(),Kp.get(),Ki.get(),Kd.get(),G.get()]
    # print(values)

    xs.append(time.time() - initial_time)
    ys.append(values[0])

    xs = xs[-200:]
    ys = ys[-200:]

    temp_str = "X"
    temp_str += str(values[0])
    temp_str += "Y"
    temp_str += str(values[1])
    temp_str += "Z"
    temp_str += str(values[2])
    temp_str += "W"
    temp_str += str(values[3])
    temp_str += "U"
    temp_str += str(values[4])

    ser.write(temp_str.encode("ascii"))
    
    sens = ser.read(ser.inWaiting())
    # sens = ser.readline()
    if sens != b'':
        sens = sens.strip().decode("utf-8")
        sens = float(sens)
        zs.append(sens)
    else:
        zs.append(float(0))

    zs = zs[-200:]

    ax.clear()
    ax.plot(xs,ys,'r')
    ax.plot(xs,zs,'b')
    ax.set_xlabel("Time(s)")
    ax.set_ylabel("Depth(m)")
    ax.set_ylim(0,2)
    
    

Emergency_stop = tk.Button(stopButtonFrame,text="Stop",height=4,width=15,bd=5)
Emergency_stop.pack()

Depth = tk.Scale(sliderframe,label="Depth",activebackground="#0000ff",orient='horizontal', length=800, from_=0.00, to=0.60, resolution=0.01)
Depth.pack()

Kp = tk.Scale(sliderframe,label="Kp",activebackground="#00ff00",orient='horizontal', length=800, from_=0.00, to=10.00, resolution=0.01)
Kp.pack()

Ki = tk.Scale(sliderframe,label="Ki",activebackground="#00ff00",orient='horizontal', length=800, from_=0.0, to=3.0, resolution=0.001)
Ki.pack()

Kd = tk.Scale(sliderframe,label="Kd",activebackground="#00ff00",orient='horizontal', length=800, from_=0.0, to=3.0, resolution=0.001)
Kd.pack()

G = tk.Scale(sliderframe,label="G",activebackground="#FF00FF",orient='horizontal', length=800, from_=0.10, to=0.65, resolution=0.01)
G.pack()

canvas = FigureCanvasTkAgg(fig,plotframe)
canvas.draw()
canvas.get_tk_widget().pack()

# root.after(time_step,see)

ani = animation.FuncAnimation(fig,animate,fargs=(xs,ys,zs),interval=time_step)

root.mainloop()