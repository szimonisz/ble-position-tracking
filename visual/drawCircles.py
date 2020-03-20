import turtle
from random import randint

def drawCellTowers():
  myPen = turtle
  myPen.hideturtle()
  myPen.tracer(0)
  myPen.speed(0)
  
  
  window = turtle.Screen()
  window.bgcolor("#F0F0F0")
  
  scale = 10
  while(1):  
    x1 = 0.0
    y1 = 0.0
    x2 = -3.236 * scale
    y2 = 2.286 * scale
    x3 = -2.9062 * scale
    y3 = 6.668 * scale

    r1 = (5.981072) * scale
    r2 = (7.011872) * scale
    r3 = (3.678804) * scale

    myPen.color("#ff5744")
    myPen.penup()
    myPen.goto(x1-5,y1)
    myPen.pendown()
    myPen.goto(x1+5,y1)
    myPen.penup()
    myPen.goto(x1,y1-5)
    myPen.pendown()
    myPen.goto(x1,y1+5)
    myPen.penup()
  
    myPen.goto(x1,y1-r1)
    myPen.pendown()
    myPen.circle(r1)
  
    myPen.color("#41befc")
    myPen.penup()
    myPen.goto(x2-5,y2)
    myPen.pendown()
    myPen.goto(x2+5,y2)
    myPen.penup()
    myPen.goto(x2,y2-5)
    myPen.pendown()
    myPen.goto(x2,y2+5)
    myPen.penup()
  
    myPen.goto(x2,y2-r2)
    myPen.pendown()
    myPen.circle(r2)
    myPen.penup()
  
    myPen.color("#52bf54")
    myPen.goto(x3-5,y3)
    myPen.pendown()
    myPen.goto(x3+5,y3)
    myPen.penup()
    myPen.goto(x3,y3-5)
    myPen.pendown()
    myPen.goto(x3,y3+5)
  
    myPen.penup()
    myPen.goto(x3,y3-r3)
    myPen.pendown()
    myPen.circle(r3)
 
   
    myPen.getscreen().update()
  return 0

drawCellTowers()
