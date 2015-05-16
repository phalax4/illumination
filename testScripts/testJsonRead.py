import json
mylist = [234,23,78,0]
file = open("data.txt",'a+')
json.dump(mylist,file)
file.write('\n')
file.close()


data = []
f2 = open('data.txt','a+')
lity = json.loads(line in f2)
f2.close()
with open('data.txt') as f:
    for line in f:
       # data.append(json.loads(line))
        #print data
        print json.loads(line)
        print type(json.loads(line))
f.close()

print "Writing done"