# Recursive Traversal and Path Finding

import os

def find(name, path):
	for root, dirs, files in os.walk(path):
		if len(dirs)>0:
			if name in dirs:
				return str(os.path.join(root, name))+"      Type: Directory"
		if name in files:
			return str(os.path.join(root, name))+"      Type: File"
	   

def find_all(name, path):
    result = []
    for root, dirs, files in os.walk(path):
        if name in files:
            result.append(os.path.join(root, name))
    return result            

print(find("paramax.c","/Users/vijaychandra/Desktop/os"))            