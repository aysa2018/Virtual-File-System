# Virtual-File-System
This repository contains the implementation of a Virtual File System (VFS) using C++. The VFS allows users to create, manage, and manipulate files and directories in a simulated file system environment, demonstrating key concepts of file system management and operation.

# Supported Commands

help: Prints the menu of commands.

pwd: Prints the path of the current inode.

ls: Prints the children of the current inode.

mkdir <foldername>: Creates a folder under the current folder.

touch <filename> <size>: Creates a file under the current inode location with the specified filename, size, and current datetime.

cd <foldername>: Change current inode to the specified folder.

cd <filename>: Return an error.

cd ..: Changes the current inode to its parent folder.

cd -: Changes the current inode to the previous working directory.

cd: Changes current inode to root.

cd /my/path/name: Changes the current inode to the specified path if it exists.

find <foldername> or find <filename>: Returns the path of the file or the folder if it exists.

mv <filename> <foldername>: Moves a file located under the current inode location to the specified folder path.

rm <foldername> or rm <filename>: Removes the specified folder or file and puts it in a Queue of MAXBIN=10.

size <foldername> or size <filename>: Returns the total size of the folder, including all its subfiles, or the size of the file.
emptybin: Empties the bin.

showbin: Shows the oldest inode of the bin, including its path.

recover: Reinstates the oldest inode back from the bin to its original position in the tree.

exit: Stops the program.

