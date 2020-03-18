import tkinter as tk
from tkinter import messagebox
import serial
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
import time
import csv

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

stopButtonFrame = tk.Frame(root,bd=5)
stopButtonFrame.pack(side=tk.LEFT,expand=True)

time_step = 50

values = []

#Global variables for logging
Log_kgf = []
Log_vel = []

fig = Figure(figsize=(5,5), dpi=100)
ax = fig.add_subplot(111)

xs = []
ys = []
zs = []


def toggle_status():
    if Emergency_stop.config('text')[-1] == 'On':
        Emergency_stop.config(text='Off')
    else:
        Emergency_stop.config(text='On')


def animate(i, xs, ys, zs):

    if Emergency_stop.config('text')[-1] == 'On':
        status = 1
    else:
        status = 0

    values = [Depth.get(),Kp.get(),Ki.get(),Kd.get(),G.get(),status]

    xs.append(time.time() - initial_time)
    ys.append(values[0])

    
    

    xs = xs[-100:]
    ys = ys[-100:]

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
    temp_str += "A"
    temp_str += str(values[5])
    

    ser.write(temp_str.encode("ascii"))
    
    sens = ser.read(ser.inWaiting())
    sens = ser.readline()
    if sens != b'':
        sens = sens.strip().decode("utf-8")
        sens = sens.split(",")
        sens = sens[:-1]
        # print(sens)
        current_depth = float(sens[0])
        current_vel = float(sens[1])
        flow_vel = float(sens[2])
        kfg_now = float(sens[3])
        pwm_1 = float(sens[4])
        pwm_2 = float(sens[5])

        depthvar.set(current_depth)
        velvar.set(current_vel)
        flowvar.set(flow_vel)
        PWM1.set(pwm_1)
        PWM2.set(pwm_2)
        KGF.set(kfg_now)

        zs.append(current_depth)


        #for logging
        Log_kgf.append(kfg_now)
        Log_vel.append(current_vel)
    else:
        zs.append(float(0))

    
    # zs.append(float(0))
    zs = zs[-100:]

    ax.clear()
    ax.plot(xs,ys,'r')
    ax.plot(xs,zs,'b')
    ax.set_xlabel("Time(s)")
    ax.set_ylabel("Depth(m)")
    ax.set_ylim(0,1)
    

#Label Widgets to display Variables as Depth, Differentiated Velocity, Flow meter Velocity
depthvar = tk.StringVar()
Measured_Depth = tk.Label(plotframe,height=1,width=8,font=("Courier", 15),textvariable=depthvar, relief=tk.RAISED )
Measured_Depth.pack()

velvar = tk.StringVar()
Calc_Vel = tk.Label(plotframe,height=1,width=8,font=("Courier", 15),textvariable=velvar, relief=tk.RAISED )
Calc_Vel.pack()

flowvar = tk.StringVar()
Flow_velo = tk.Label(plotframe,height=1,width=8,font=("Courier", 15),textvariable=flowvar, relief=tk.RAISED )
Flow_velo.pack()

PWM1 = tk.StringVar()

PWM_1_var = tk.Label(sliderframe,height=1,width=8,font=("Courier", 15),textvariable=PWM1, relief=tk.RAISED )
PWM_1_var.pack(side=tk.TOP)

PWM2 = tk.StringVar()
PWM_2_var = tk.Label(sliderframe,height=1,width=8,font=("Courier", 15),textvariable=PWM2, relief=tk.RAISED )
PWM_2_var.place(x = 230, y = -2)

KGF = tk.StringVar()
KGF_var = tk.Label(sliderframe,height=1,width=8,font=("Courier", 15),textvariable=KGF, relief=tk.RAISED )
KGF_var.pack(side=tk.TOP)




#Stop Button for thruster function
Emergency_stop = tk.Button(stopButtonFrame,text="On",height=4,width=15,bd=5,command = toggle_status)
Emergency_stop.pack()

#Label widgets to display Txt
depth_txt_2dislay = tk.StringVar()
depth_txt_2dislay.set("Depth")
displayDepth = tk.Label(plotframe,height=1,width=8,font=("Symbol ", 15),textvariable=depth_txt_2dislay)
displayDepth.place(x = 130, y = 5, width=60, height=20)

vel_txt_2dislay = tk.StringVar()
vel_txt_2dislay.set("Calculated Vel")
displayVel = tk.Label(plotframe,height=1,width=8,font=("Symbol ", 15),textvariable=vel_txt_2dislay)
displayVel.place(x = 43, y = 33, width=160, height=20)

flow_vel_txt_2dislay = tk.StringVar()
flow_vel_txt_2dislay.set("Flow Vel")
displayFlowVel = tk.Label(plotframe,height=1,width=8,font=("Symbol ", 15),textvariable=flow_vel_txt_2dislay)
displayFlowVel.place(x = 98, y = 58, width=100, height=20)

#Slider Widgets
Depth = tk.Scale(sliderframe,label="Depth",activebackground="#0000ff",orient='horizontal', length=800, from_=0.00, to=0.60, resolution=0.01)
Depth.pack()

Kp = tk.Scale(sliderframe,label="Kp",activebackground="#00ff00",orient='horizontal', length=800, from_=0.00, to=4.00, resolution=0.01)
Kp.pack()

Ki = tk.Scale(sliderframe,label="Ki",activebackground="#00ff00",orient='horizontal', length=800, from_=0.0, to=1.0, resolution=0.001)
Ki.pack()

Kd = tk.Scale(sliderframe,label="Kd",activebackground="#00ff00",orient='horizontal', length=800, from_=0.0, to=1.0, resolution=0.001)
Kd.pack()

G = tk.Scale(sliderframe,label="G",activebackground="#FF00FF",orient='horizontal', length=800, from_=0.00, to=0.65, resolution=0.01)
G.pack()

canvas = FigureCanvasTkAgg(fig,plotframe)
canvas.draw()
canvas.get_tk_widget().pack()

# root.after(time_step,see)

ani = animation.FuncAnimation(fig,animate,fargs=(xs,ys,zs),interval=time_step)

def on_closing():
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        print("Exit Call")
        Logging_Data = [Log_kgf,Log_vel]
        Logging_Data = zip(*Logging_Data)
        with open('AUV_Log.csv', 'w', newline='') as file:
            writer = csv.writer(file)
            writer.writerows(Logging_Data)
        root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)

root.mainloop()


