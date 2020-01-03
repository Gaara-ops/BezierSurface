from PIL import Image
from PIL import ImageDraw
import random
import time


def Factorial(n):
    '''
    阶乘
    '''
    assert(n>=0)
    if n == 0:  
        return 1
    else:
        factor = 1
        for i in range(1,n +1):
            factor *= i
        return factor

def GetRandomPoint2D(x_range,y_range):
    '''
    return: range in [0,1)
    '''
    assert(x_range > 40 and y_range > 40)
    x = random.randint(20,x_range-20)
    y = random.randint(20,y_range-20)
    return [x,y]


def GetRandomPoint3D(x_range,y_range,z_range):
    '''
    return: range in [0,1)
    '''
    assert(x_range > 40 and y_range > 40 and z_range > 40)
    x = random.randint(20,x_range-20)
    y = random.randint(20,y_range-20)
    z = random.randint(20,z_range-20)
    return [x,y,z]

def Bernstein(u,n,i):
    '''
    伯恩斯坦基函数

    :param u: 插补点参数值 
    :range u: [0,1)

    :param n: 贝塞尔曲线次数
    :type  n: int
    :range n: [2,+oo) 

    :param i: range in 0-n, type int
    '''
    assert(i <= n and i >= 0 and u <1 and u >= 0)
    factor_0 = Factorial(n) / (Factorial(n - i) * Factorial(i))
    factor_1 = (1 - u)**(n - i)
    factor_3 = u**i
    return  factor_0 * factor_1 * factor_3


def BezierCurve(u,V):
    '''
    贝塞尔曲线方程

    u [0,1)

    V 控制点列表 
    
    eg: [[x0,y0],[x1,y1]...[xn,yn]] x,y E [0,1)
    '''
    assert(u >= 0 and u <= 1 and V)
    n = len(V)
    x_result = 0
    y_result = 0
    for i in range(0,n):
        x_result += Bernstein(u,n-1,i) * V[i][0]    # n个点，n-1次贝塞尔曲线，i [0,n-1]
        y_result += Bernstein(u,n-1,i) * V[i][1]
    return [x_result,y_result]

def BezierSurface(u,w,V):
    '''
    贝塞尔曲线方程

    u [0,1)

    V 控制点列表 
    
    eg: [[x0,y0],[x1,y1]...[xn,yn]] x,y E [0,1)
    '''
    assert(u >= 0 and u <= 1 and w >= 0 and w <= 1 and V)
    ii = len(V)
    jj = len(V[0])
    x_result = 0
    y_result = 0
    z_result = 0
    for i in range(0,ii):
        for j in range(0,jj):
            x_result += Bernstein(u,ii-1,i) * Bernstein(w,jj-1,i) * V[i][j][0]    
            y_result += Bernstein(u,ii-1,i) * Bernstein(w,jj-1,i) * V[i][j][1]
            z_result += Bernstein(u,ii-1,i) * Bernstein(w,jj-1,i) * V[i][j][2]
    return [x_result,y_result,z_result]

def DrawBezierCurveRandom(width,height,n):
    im = Image.new('RGB',(width,height),'white')
    draw = ImageDraw.Draw(im)

    #生成 n+1 个控制点
    ctrlV = []
    for i in range(0,n+1):
        ctrlV.append(GetRandomPoint2D(width,height))
    
    for i in range(0,n):
        draw.line((ctrlV[i][0],ctrlV[i][1],ctrlV[i+1][0],ctrlV[i+1][1]),(255,0,0))
    i_iter = 0.0
    while i_iter <= 1.0:
        [x,y] = BezierCurve(i_iter,ctrlV)
        im.putpixel((int(x),int(y)),(0,0,0))
        i_iter += 0.0001

    fileName = "O-BezierCurveRand__"+str(n)+ "N___" + time.strftime("%Y-%m-%d_%H_%M_%S", time.localtime())+".png"
    im.save(fileName)

def DrawBezierCurve(width,height,ctrlV):
    im = Image.new('RGB',(width,height),'white')
    draw = ImageDraw.Draw(im)

    n = len(ctrlV) - 1
    for i in range(0,n):
        draw.line((ctrlV[i][0],ctrlV[i][1],ctrlV[i+1][0],ctrlV[i+1][1]),(255,0,0))
    i_iter = 0.0

    while i_iter <= 1.0:
        [x,y] = BezierCurve(i_iter,ctrlV)
        im.putpixel((int(x),int(y)),(0,0,0))
        i_iter += 0.0001
    fileName = "O-BezierCurve__"+str(n)+ "N___"+ str(random.random())+" " + time.strftime("%Y-%m-%d_%H_%M_%S", time.localtime())+".png"
    im.save(fileName)


DrawBezierCurve(500,500,[[249,1],[499,249],[249,499],[32,44]])
DrawBezierCurve(500,500,[[44,20],[20,304],[321,121]])

for i in range(3,8):
    for j in range(0,10):
        DrawBezierCurveRandom(500,500,i)
