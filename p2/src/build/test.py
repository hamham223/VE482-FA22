
file_object1 = open("./sample/many_insert_delete.query",'r')
file_object2 = open("many_insert_delete0.query",'w')

ref = ["ADD","COU","MAX"]

try:
  while True:
        line = file_object1.readline()
        if line:
            if not (line[0:3] in ref):
                file_object2.write(line)
        else:
            break
finally:
    file_object1.close()
    file_object2.close()