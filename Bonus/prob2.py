### FILE SYSTEM
# dependency: pyfilesystem, fuse

from __future__ import with_statement
from fs.osfs import OSFS
from fs import open_fs
import os
import sys
import errno
from fuse import FUSE, FuseOSError, Operations

class myFileSystem(Operations):
    def __init__(self, root):
        self.root = root

    def myls(self, path):
        if dir == "":
            my_fs = open_fs('.')
            print(my_fs.tree())
        else:
            my_fs = open_fs(dir)
            print(my_fs.tree())

    def mychmod(self, path, mode):
        full_path = self._full_path(path)
        return os.chmod(full_path, mode)

    def mychown(self, path, uid, gid):
        full_path = self._full_path(path)
        return os.chown(full_path, uid, gid)

    def myreaddir(self, path, fh):
        full_path = self._full_path(path)
        dirents = ['.', '..']
        if os.path.isdir(full_path):
            dirents.extend(os.listdir(full_path))
        for r in dirents:
            yield r

    def myrmdir(self, path):
        full_path = self._full_path(path)
        return os.rmdir(full_path)

    def mymkdir(self, path, mode):
        return os.mkdir(self._full_path(path), mode)

    def myopen(self, path, flags):
        full_path = self._full_path(path)
        return os.open(full_path, flags)

    def mycreate(self, path, mode, fi=None):
        full_path = self._full_path(path)
        return os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)

    def myread(self, path, length, offset, fh):
        os.lseek(fh, offset, os.SEEK_SET)
        return os.read(fh, length)

    def mywrite(self, path, buf, offset, fh):
        os.lseek(fh, offset, os.SEEK_SET)
        return os.write(fh, buf)

    def unmount(self, path, fh):
        return os.close(fh)


# mountpoint and root should be absolute paths
def main(mountpoint, root): #root directories content will be copied to mountpoint 
    FUSE(myFileSystem(root), mountpoint, nothreads=True, foreground=True)

if __name__ == '__main__':
    main(sys.argv[2], sys.argv[1])


