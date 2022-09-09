import sensor, image, time
from pyb import Pin, Timer,LED,ExtInt
import math
from pyb import UART

# 颜色追踪的例子，一定要控制环境的光，保持光线是稳定的。
red_threshold  = (13, 49, 18, 61, 6, 47)
red_th=((46, 73, 20, 65, -22, 19))
green_th=(31, 86, -67, -32, -19, 54)
green_th2=((47, 59, -44, -15, -8, 30))
blue_th=(47, 65, -28, 0, -56, -11)
blue_th_day=(47, 72, -28, 0, -56, -11)
count_1=0
pur_th=(30, 59, 5, 33, -56, -13)
org_th=(47, 72, 6, 51, 17, 75)
gre_th=(28, 79, -78, -16, -55, 69)
pur_2_th=(28, 59, 12, 52, -59, 3)
blue_led  = LED(3)
pan_error=0
tilt_error=0
dx=1
dy=1
cx1=0
cy1=0
cx2=0
cy2=0
tan=1
jiaodu=0
tt=1
su=0
su_la=0
jd=0
jl=0
cx=0
cy=0
zzjl=0
zjl=0.0
cx_la=0
cy_la=0
jd_s="\0"
bz_1=0
bz_2=0
SU_LLL=0
SU_LL=0
SU_L=0
SU=0
zjl_ks=0
uart = UART(3, 115200, timeout_char=1000)    #串口3 波特率19200 超时时间1s  Tx_PB4 Rx_PB5
#global data
global max_blob1
global max_blob2
sensor.reset() # 初始化摄像头
sensor.set_pixformat(sensor.RGB565) # 格式为 RGB565.
#sensor.set_framesize(sensor.CIF) # 使用 QQVGA 速度快一些
sensor.set_framesize(sensor.VGA) # 使用 QQVGA 速度快一些
#sensor.set_windowing((352,288))
sensor.set_windowing((620,290))
sensor.skip_frames(time = 2000) # 跳过2000s，使新设置生效,并自动调节白平衡
sensor.set_auto_gain(False) # 关闭自动自动增益。默认开启的，在颜色识别中，一定要关闭白平衡。
sensor.set_auto_whitebal(False)
#关闭白平衡。白平衡是默认开启的，在颜色识别中，一定要关闭白平衡。
clock = time.clock() # 追踪帧率
#extint = pyb.ExtInt(pin, pyb.ExtInt.IRQ_FALLING, pyb.Pin.PULL_UP, callback)


def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

'''
def tick(timer):
    global cx
    global cx1
    global cy
    global cy1
    blue_led.toggle()
    cx=cx1
    cy=cy1
    uart.write(cx1)
    #cy=cy*1.7

    #jl=math.sqrt(cx-cx_la)**2+(cy-cy_la)**2
    #zjl=zjl+jl
    #cx_la=cx
    #cy_la=cy

    print(jd)
    print("mmmmmmmmm")
'''

#tim = Timer(2, freq=10)      # 使用定时器2创建定时器对象-以1Hz触发
#tim.callback(tick)          # 将回调设置为tick函数


while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.

    blobs1 = img.find_blobs([pur_2_th])
    if blobs1:
        max_blob1 = find_max(blobs1)
        #pan_error = max_blob.cx()-img.width()/2
        #tilt_error = max_blob.cy()-img.height()/2
        cx1=max_blob1.cx()
        cy1=img.height()-max_blob1.cy()
        img.draw_rectangle(max_blob1.rect()) # rect

        blobs2 = img.find_blobs([gre_th])
        if blobs2:

            count_1=count_1+1
            max_blob2 = find_max(blobs2)
            cx2=max_blob2.cx()
            cy2=img.height()-max_blob2.cy()
            #pan_error = max_blob.cx()-img.width()/2
            #tilt_error = max_blob.cy()-img.height()/2
            img.draw_rectangle(max_blob2.rect()) # rect
            #global max_blob1
            #global max_blob2
            dy=(cy2-cy1)
            dx=(cx2-cx1)


            if (dx==0):
               # global jiaodu
                jiaodu=90
                jd=90
            else:

                # global tan
                tan=dy/dx
                jiaodu=math.atan(tan)*57.3
                if(dx>0 and dy>=0):
                   jd=jiaodu
                elif(dx>0 and dy<0):
                   jd=360+jiaodu
                else:
                   jd=180+jiaodu
            if(count_1%4==0):
                cx=cx1
                #cy=cy1*(0.8+cy1/320)
                #cx=310+(cx1-310)*(0.5-cy/990)
                cy=cy1*(0.85+cy1/290)
                cy_r=cy1*1.5
                if(count_1%12==0):

                    tt=12/clock.fps()
                    jl=math.sqrt((cx-cx_la)**2+(cy-cy_la)**2)+0.01
                    su=(jl/tt)*0.7+su_la*0.3
                    SU=jl/tt
                    zjl=zjl+jl
                    su=su*(0.42+cy1/850)
                    SU=SU*(0.42+cy1/850)
                    su=su*(0.82+cy*0.0004)
                    SU=SU*(0.82+cy*0.0004)
                    #su=su*1.1
                    #SU=SU*1.1
                    #uart.write(str(jl))
                    jd=int(jd)
                    if(cy1-cy2>=10 and cy1-cy2<=70):
                        jd_s=str(jd+4000)
                    elif(cy2-cy1>=10 and cy2-cy1<=70):
                        jd_s=str(jd+6000)
                    elif(cx1-cx2>=10 and cx1-cx2<=70):
                        jd_s=str(jd+7000)
                    elif(cx2-cx1>=10 and cx2-cx1<=70):
                        jd_s=str(jd+5000)
                    else:
                        jd_s=str(8000)
                    if(su>=49):
                      su=49

                    if(SU_LLL>2 and SU_LL>2 and SU_L>2 and SU>2 and bz_1==0):
                        bz_1=1
                        zjl_ks=zjl
                    if(SU_LLL<3 and SU_LL<3 and SU_L<3 and SU<3 and bz_1==1):
                        bz_1=2
                    cx_la=cx
                    cy_la=cy
                    su_la=su
                    SU_LLL=SU_LL
                    SU_LL=SU_L
                    SU_L=SU
                    zzjl=zzjl+tt*su
                    #print(str(SU_LLL)+"///")

                if(bz_1==0):  #go on
                    #global data
                    cx_s= str(cx+2000)
                    #data= str(cx+2000)+str(int(cy)+2000)+str(int(su*10)+3000)+jd_s+str(int(zjl)+900000)+"\r"+"\n"
                    #uart.write(data）
                    #print(data)

                elif(bz_1==1):    #go on
                    cx_s= str(cx+1000)
                    #data= str(cx+1000)+str(int(cy)+2000)+str(int(su*10)+3000)+jd_s+str(int(zjl)+900000)+"\r"+"\n"
                    #uart.write(data）
                    #print(data)
                elif(bz_1==2):    #go on
                    cx_s= str(cx+3000)
                    #data= str(cx+3000)+str(int(cy)+2000)+str(int(su*10)+3000)+jd_s+str(int(zjl)+900000)+"\r"+"\n"
                    #uart.write(data）
                    #print(data)
                #global data
                #uart.write(data）
                #su=su*0.51+cy1/1160

                data= cx_s+str(int(cy_r)+2000)+str(int(su*200)+30000)+jd_s+str(int(zzjl)+900000)+"\r"+"\n"
                a=uart.write(data)
                print(zzjl)
                print(jd_s)
                print(data)
                print(su)
                print(bz_1)
                #print(su)


    #print(clock.fps())
