# Problem 3
# Author: Vijay

# Test Case 1: 
# Total pages: 16
# elements = 0 1 2 3 0 1 2 3 0 1 2 3 4 5 6 7
# page frame size: 3
# page faults (FIFO) = 16
# page faults (LFU) = 12
# page faults (LRU) = 13

# Test Case 2:
# Total Pages: 10
# elements = 1 2 3 4 1 2 5 1 2 3
# page frame size = 4
# page faults (FIFO) = 8
# page faults (LFU) = 6
# page faults (LRU) =  6

# Test case 3:
# Total Pages: 15
# elements = 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2
# page frame size = 3
# page faults (FIFO) = 12
# page faults (LFU) = 10
# page faults (LRU) = 9

# Test Case 4:
# Total Pages: 14
# elements = 0 2 4 6 2 0 9 5 3 8 1 7 1 7
# page frame size = 3
# page faults (FIFO) = 11
# page faults (LFU) = 13
# page faults (LRU) = 11

pg = []
fr = []

def fifo():
    i=count=flag=num = 0
    f = 0 
    r = 0 
    s = 0
    psize = int(input("Enter size of page frame:"))
    for i in range(psize):   
        fr.append(-1)
    while(s<n):  
        flag=0
        num = pg[s]
        for i in range(psize):
            if(num == fr[i]):
                s += 1
                flag=1
                break
        if(flag==0):
            if(r<psize):
                fr[r]=pg[s]
                r += 1
                s += 1
                count += 1
            else:
                if(f<psize):
                    fr[f]=pg[s]
                    s += 1
                    f += 1
                    count += 1
                else: 
                    f=0         
        # for i in range(psize):
        #     print(fr[i], "\t")
    print("Page Faults (FIFO) = ",count)

def lfu():
    pgfaultcnt=0;
    usedcnt = [0]*100
    least = repin = sofarcnt = bn = 0
    nf = int(input('number of frames: '))
    usedcnt = [0]*nf
    p = [9999]*nf
    for i in range(n):
        if(pg[i] in p):
            hitind = p.index(pg[i])
            usedcnt[hitind] += 1
        else:
            pgfaultcnt += 1
            if(bn<nf):
                p[bn] = pg[i]
                usedcnt[bn]=usedcnt[bn]+1;
                bn += 1
            else:
                least=9999;
                for k in range(nf):
                    if(usedcnt[k]<least):
                        least=usedcnt[k]
                        repin=k
                p[repin] = pg[i]
                sofarcnt=0
                for k in range(i):
                    if(pg[i]==pg[k]):
                        sofarcnt = sofarcnt+1
                usedcnt[repin] = sofarcnt
    print("Total faults (LFU) = " ,pgfaultcnt)

def lru():
    count = []
    i=j=k=fault=f=flag=temp=current=c=dist=maax=m=cnt=p=x=0
    f = int(input('Enter frame size: '))
    for i in range(f):
        count.append(0)
        fr.append(-1)
    for i in range(n):
        flag = 0
        temp = pg[i]
        for j in range(f):
            if(temp == fr[j]):
                flag = 1
                break
        if((flag==0) and (k<f)):
            fault += 1
            fr[k] = temp
            k += 1
        elif((flag==0) and (k==f)):
            fault += 1            
            for cnt in range(f):    
                current = fr[cnt]
                for c in range(i,0,-1):
                    if(current != pg[c]):
                        count[cnt] += 1
                    else:
                        break
            maax = 0
            for m in range(f):
                if(count[m] > maax):
                    maax = count[m]
                    p = m
            fr[p] = temp         
        # for x in range(f): #for debugging
        #     print(fr[x] ,"\t")
    print("Total faults (LRU) = " ,fault)

ch = 0
n = int(input("Number of pages in reference string:"))
print("Enter pages sequence:");
for i in range(n):
    pg.append(int(input('Enter element '+ str(i+1)+ ': ')))
print("AVAILABLE ALGORITHMS\n")
print("1)FIFO")
print("2)LFU")
print("3)LRU")
ch = (input("\nEnter your choice (eg: fifo, lfu, lru, ''): "))
if ch == 'fifo':
    fifo()
if ch == 'lfu':
    lfu()
if ch == 'lru': 
    lru()
if ch == '':
    fifo()
    fr.clear()
    lfu()
    fr.clear()
    lru()

       