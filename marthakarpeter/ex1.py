#ex1
print("hello world")

file = open("ex1.dat","r") 
lines = file.readlines() # This is what you forgot 
file.close()
print(lines)