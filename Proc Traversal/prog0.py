import platform
import multiprocessing
import cpuinfo
import subprocess
import os
import psutil

cmd1 = "sysctl -n hw.physicalcpu"

cmd = "sysctl -a |grep kern.boottime"
info = cpuinfo.get_cpu_info()
print('Number of CPUs in the Machine: ', multiprocessing.cpu_count() )
cores = ((subprocess.Popen(["sysctl", "-a", "hw.logicalcpu"], stdout=subprocess.PIPE).communicate()[0]))
print('Number of cores per CPU: ', cores)
print('Clock speed in Hz: {0}'.format(info['hz_actual']))
print('Version of Kernel: ', platform.platform())
output = subprocess.Popen(["sysctl", "-a"], stdout=subprocess.PIPE )
p1 = subprocess.Popen(['grep', 'kern.boottime'],stdin=output.stdout, stdout=subprocess.PIPE).communicate()[0]
print('Last Rebooted: ' , p1[:len(p1)-1])
print('Load average in the last 15 minutes: ', os.getloadavg()[2])
print('Total Usable: ', psutil.virtual_memory()[0], ' and Current free memory: ', psutil.virtual_memory()[4])
print('Total Swap space: ', psutil.swap_memory()[0], ' and currently used swap space: ', psutil.swap_memory()[1])
print('Swap Parition and sizes', 1, psutil.disk_usage('/'))
print('Cpu Times: ', psutil.cpu_times())
print('Number of context switches: ', psutil.cpu_stats()[0])
print('Number of Interrupts: ', psutil.cpu_stats()[1])

print('##### PART 2 #######')
print()
print('Process ID of this program: ', os.getpid())
id = int(input('enter pid: '))

a = (subprocess.Popen(["ps", "-p", str(id), '-o', 'args'], stdout=subprocess.PIPE).communicate()[0])
print("Command line with which the process started: ")
print(a)
print("\n")
print("Time spent by process in running:, " , (subprocess.Popen(["ps", "-p", str(id), '-o', 'etime='], stdout=subprocess.PIPE).communicate()[0]))
print("\n")
print("Time spend in user mode and environment: ", (subprocess.Popen(["ps", "-p", str(id), '-wwwE'], stdout=subprocess.PIPE).communicate()[0]))





