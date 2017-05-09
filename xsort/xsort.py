# from subprocess import Popen,PIPE
# import sys

# def subprocess_cmd(dr,cmd1,cmd2):
#     p1 = Popen(cmd1.split(),stdout=PIPE,cwd=dr)
#     p2 = Popen(cmd2.split(),stdin=p1.stdout,stdout=PIPE,cwd=dr)
#     p1.stdout.close()
#     return p2.communicate()[0]

# subprocess_cmd('/Users/vijaychandra/Desktop/os/OsAssignments/xsort/','open -a Terminal /','python')


from subprocess import Popen, PIPE
import subprocess
print(Popen("python bubblesort.py".split(), stdin=PIPE, stdout=subprocess.PIPE).communicate()[0])   # set environment, start new shell
#x = subprocess.Popen(['python'],stdin=p.stdout, stdout=subprocess.PIPE).communicate()[0]
#p.communicate("python\n")
#print(x)