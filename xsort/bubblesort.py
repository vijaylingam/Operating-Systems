import sys
def bubbleSort(alist):
    for passnum in range(len(alist)-1,0,-1):
        for i in range(passnum):
            if alist[i]>alist[i+1]:
                temp = alist[i]
                alist[i] = alist[i+1]
                alist[i+1] = temp

with open('sort.txt') as f: #sys.argv[1]
    content = f.readlines()

# you may also want to remove whitespace characters like `\n` at the end of each line
content = [int(x.strip()) for x in content] 
bubbleSort(content)
for element in content:
	print(element)