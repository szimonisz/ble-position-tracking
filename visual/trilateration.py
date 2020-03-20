#A function to apply trilateration formulas to return the (x,y) intersection point of three circles
def trackPhone(x1,y1,r1,x2,y2,r2,x3,y3,r3):
  A = 2*x2 - 2*x1
  B = 2*y2 - 2*y1
  C = r1**2 - r2**2 - x1**2 + x2**2 - y1**2 + y2**2
  D = 2*x3 - 2*x2
  E = 2*y3 - 2*y2
  F = r2**2 - r3**2 - x2**2 + x3**2 - y2**2 + y3**2
  x = (C*E - F*B) / (E*A - B*D)
  y = (C*D - A*F) / (B*D - A*E)
  return x,y

#Generate and represent data to be used by the trilateration algorithm

x1 = 0
y1 = 0

x2 = -3.236
y2 = 2.286

x3 = -2.9062
y3 = 6.668

r1 = 3.414549
r2 = 3.414549
r3 = 2.511886

#Apply trilateration algorithm to locate phone
x,y = trackPhone(x1,y1,r1,x2,y2,r2,x3,y3,r3)

#Output phone location / coordinates
print("BLE Device Location:")
print(x,y)
