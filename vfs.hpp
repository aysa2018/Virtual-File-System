//============================================================================
// Name         : vfs.hpp
// Author       : Aysa Binte masud
// Version      : 01
// Date Created : 10 November 2023
// Date Modified: 19 November 2023
// Description  : Virtual filesystem implmentation in C++
//============================================================================
#ifndef VFS_H
#define VFS_H
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<string>
#include<ctime>
#include<fstream>
#include<sstream>
#include<vector>
#include "inode.hpp"
#include "queue.hpp"
#include "vector.hpp"
using namespace std;

class VFS
{
    private:
        Inode *root;                //root of the VFS
        Inode *curr_inode;          //current iNode
        Inode *prev_inode;          //previous iNode
        Queue<Inode*> bin;          //bin containing the deleted Inodes
        Queue<string> bin_paths;    //paths of the items in bin
    
    public:     
        //Required methods
        VFS();  
        void help();                        
        string pwd();
        void ls();                      
        void mkdir(string folder_name);
        void touch(string file_name, unsigned int size);
        void cd(string path);
        void rm(string file_name);
        void size(string path);
        void showbin();
        void emptybin();
        void exit();
        
        
        //Optional methods
        void find(string name);
        void mv(string file, string folder);
        void recover();

        //Optional helper methods
        // void load(ifstream &fin);                //Helper method to load the vfs.dat
        void find_helper(Inode* ptr, string name,string currentPath);
        Inode* getNode(string path);            //Helper method to get a pointer to iNode at given path
        Inode* getChild(Inode *ptr, string childname);  //returns a specific child of given Inode
        // bool find_helper(Inode *ptr, string name);       //recursive method to check if a given child is present under specific Inode or not
        // void updateSize(Inode *ptr);                    //re-calculatre the size of an Inode and its parents
        void write(ofstream &fout, Inode *ptr);         //traverse and write recrusively the vfs data
        string getTime();                               //return system time as an string
        void calculateFolderSize(Inode* folder, unsigned int& totalSize);
};


//constructor of the class
VFS::VFS() {
    // Initialize root, previous inode and current inode to nullptr
    root = new Inode("Root", nullptr, Folder, 0, getTime()); // Initialize the root with its name, parent as nullptr, type, and time
    curr_inode = root;//initialize current inode as root
    prev_inode = nullptr;//initialize previous inode as nullptr
}
//===========================================================
//create a function to print all the command lines
void VFS::help() {
        cout << "Available commands:" << endl;
        cout << "1. help : Prints the menu of commands." << endl;
        cout << "2. pwd : Prints the path of the current inode." << endl;
        cout << "3. ls : Prints the children of the current inode." << endl;
        cout << "4. mkdir foldername : Creates a folder under the current folder." << endl;
        cout << "5. touch filename size : Creates a file under the current inode location with the specified filename, size, and current datetime." << endl;
        cout << "6. cd foldername : Change current inode to the specified folder." << endl;
        cout << "7. cd filename: Return an error." << endl;
        cout << "9. cd ..: Changes the current inode to its parent folder." << endl;
        cout << "10. cd -: Changes the current inode to the previous working directory." << endl;
        cout << "11. cd : Changes current inode to root." << endl;
        cout << "12. cd /my/path/name: Changes the current inode to the specified path if it exists." << endl;
        cout << "13. find foldername or find filename : Returns the path of the file or the folder if it exists." << endl;
        cout << "14. mv filename foldername : Moves a file located under the current inode location to the specified folder path." << endl;
        cout << "15. rm foldername or rm filename : Removes the specified folder or file and puts it in a Queue of MAXBIN=10." << endl;
        cout << "16. size foldername or filename : Returns the total size of the folder, including all its subfiles, or the size of the file." << endl;
        cout << "17. emptybin : Empties the bin." << endl;
        cout << "18. showbin : Shows the oldest inode of the bin, including its path." << endl;
        cout << "19. recover : Reinstates the oldest inode back from the bin to its original position in the tree." << endl;
        cout << "20. exit : Stops the program." << endl;
    }

//create a method to get the node at a specified path
Inode* VFS::getNode(string path) {

    //if the path is at root, return root
    if (path == "/") {
        return root;
    }

    //if the path does not start with "/", it's invalid. Check the validity.
    if (path[0] != '/') {
        cout << "Invalid path format. Path must start with '/'.";
        return nullptr;
    }

    // parse the path
    vector<string> tokens;
    stringstream ss(path);
    string token;

    // tokenize the path
    while (getline(ss, token, '/')) { 
        //if it finds a '/', that means, it is a different file/folder name

        //if token exists, then push back to tokens
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    //set the current node to root
    Inode* current = root;


    //check the children of each node 
    for (const string& token : tokens) {
        Vector<Inode*>& children = current->getChildren();
        bool found = false;

        // Check if the token matches a child inode's name
        for (size_t i = 0; i < children.size(); ++i) {//traverse through the vector containing children

            if (children[i]->getName() == token) {//if token matches the name
                current = children[i];
                found = true;
                break;
            }
        }

        if (!found) { //if the token couldn't be found
            cout << "Path not found: " << token << endl;
            return nullptr;
        }
    }

    return current;//return the current node
}

//create a method to get all the clild nodes of the given inode
Inode* VFS::getChild(Inode* parent, string childName) {

    if (parent == nullptr) {// the root's parent is always a nullptr
        cout << "Invalid parent inode." << endl;
        return nullptr;
    }

    Vector<Inode*>& children = parent->getChildren();//store all the children in a Vector

    //traverse through all the children
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]->getName() == childName) {
        //if one of the children's name matches with the given childName, return that child
            return children[i];
        }
    }

    //if the given childName can not be found in the current directory, show the error message.
    cout << "Child '" << childName << "' not found under '" << parent->getName() << "'." << endl;
    return nullptr;
}

void VFS::write(ofstream &fout, Inode* ptr) {
    if (ptr == nullptr) {
        return;
    }

    fout << ptr->getName() << " " << ptr->getType() << " " << ptr->getSize() << " " << ptr->getCreationTime() << endl;

    Vector<Inode*>& children = ptr->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        write(fout, children[i]);
    }
}

// Get the current working directory
string VFS::pwd() {

    //if the path is at root, return root
    if (curr_inode == root) {
        return "/";
    }


    string path = "";//initialize the path as an empty string
    Inode* temp = curr_inode;//set the temporary Inode pointer as curr_inode

    while (temp != root) {//until the temporary inode pointer reaches to root
        path = "/" + temp->getName() + path; //assign the new path of "/parentNode/childnode's path" of this format to path
        temp = temp->getParent(); //forward the temporary inode pointer to its parent
    }

    return path;
}

// List contents of the current directory
void VFS::ls() {
    Vector<Inode*>& children = curr_inode->getChildren();//store all the children in a Vector

    //traverse through all the children
    for (size_t i = 0; i < children.size(); ++i) {
        cout << (children[i]->getType() == Folder ? "dir" : "file") << " ";//if the type is a folder, print "dir", otherwise "file"
        cout << children[i]->getName() << " ";//print the name of the file/folder
        
        cout << children[i]->getCreationTime() << " " ;//print the creation time
        cout << children[i]->getSize() << " "<< "bytes"<<endl;//print the size in bytes
    }
}

//create a method which creates a folder with a given name
void VFS::mkdir(string folder_name) {

    if (curr_inode == nullptr) {//show an error message, if the is no current working directory
        cout << "No current directory selected." << endl;
        return;
    }


    //create a new folder with given node, crrrent inode as the parent node, default size 10 bytes and the creation time
    Inode* new_folder = new Inode(folder_name, curr_inode, Folder, 10, getTime());

    //push the newly created folder in the Vector containing the children of current inode
    curr_inode->getChildren().push_back(new_folder);
    
    // Update the size of the current inode

    unsigned int totalSize = 0;//at first, initialize the total size with zero

    Vector<Inode*>& children = curr_inode->getChildren();//store all the children in a Vector

    for (size_t i = 0; i < children.size(); ++i) {//traverse through each child and add the size to parent node
        totalSize += children[i]->getSize();
    }

    curr_inode->setSize(totalSize);//set the new size as the size of the directory

    // Update parent sizes
    Inode* temp = curr_inode;
    while (temp != nullptr) {//stops after traversing through the root directory

        unsigned int parentSize = 0;//at first, initialize the total size with zero
        Vector<Inode*>& parentChildren = temp->getChildren();//store all the children in a Vector
        for (size_t i = 0; i < parentChildren.size(); ++i) {//traverse through each child and add the size to parent node
            parentSize += parentChildren[i]->getSize();
        }
        temp->setSize(parentSize);//set the new updated size
        temp = temp->getParent();//forward it to the parent inode
    }

    //print a message to lett the user know that the folder has been created
    cout << "Folder '" << folder_name << "' created." << endl;
}


//create a method that creates a file with a given name, and size
void VFS::touch(string file_name, unsigned int size = 10) {

    if (curr_inode == nullptr) {//show an error message, if the is no current working directory
        cout << "No current directory selected." << endl;
        return;
    }
    
    //check whether the user provides a valid name or not
    for (char ch : file_name) {
        // Check if the character is alphanumeric or a period
        if (!std::isalnum(ch) && ch != '.') {
            cout<<"File names should be alphaneumeric only. Try again!"<<endl;
            return ;
        }
    }

    //create a new file with given information
    Inode* new_file = new Inode(file_name, curr_inode, File, size, getTime());
    curr_inode->getChildren().push_back(new_file); //push the new file to children vector of the current node

    // Update the current inode's size
    unsigned int totalSize = 0;//at first, initialize the total size with zero
    Vector<Inode*>& children = curr_inode->getChildren();

    //traverse throug all the children
    for (size_t i = 0; i < children.size(); ++i) {
        totalSize += children[i]->getSize();//add the size of the newly created file to its parent directory
    }

    //set the new size 
    curr_inode->setSize(totalSize);

    // Update parent sizes
    Inode* temp = curr_inode;
    while (temp != nullptr) {//stops after traversing through the root directory
        unsigned int totalSize = 0;
        Vector<Inode*>& children = temp->getChildren();
        for (size_t i = 0; i < children.size(); ++i) {//traverse through all the children
            totalSize += children[i]->getSize();
        }
        temp->setSize(totalSize);//set the new updated size
        temp = temp->getParent();//forward it to the parent inode
    }
}

//create a method to go to a specific a directory
void VFS::cd(string path) {

    //if the path is "/", it's at root
    if (path == "/") {
        curr_inode = root;
        prev_inode = nullptr;
        return;
    }

    else if (path == "..") {  // for moving to the parent directory
        if (curr_inode != root) {//for every directory except root
            prev_inode = curr_inode;//set the previous node as the current node
            curr_inode = curr_inode->getParent();//set the current node as the parent of the current node
        } else {
            cout << "Already at root directory." << endl;
            
        }
        return;
    } 

    else if (path == "-") {  // for moving to previous working directory
        if (prev_inode != nullptr) {//unless it reaches the root

            //swap current node and previous node
            Inode* temp = curr_inode;//create a temporary pointer and assign current node to it
            curr_inode = prev_inode;//set the current node as the previous node
            prev_inode = temp;//set the previous node as temp

        } else {
            cout << "No previous working directory." << endl;

        }
        return;
    } 

    else if (path == "") {  // if the user just types "cd", it will take to root
        curr_inode = root; //set the current node as root and the previous one as nullptr
        prev_inode = nullptr;
        return;

    } 

    else if (path[0] == '/') {  // for moving to any specified path
        Inode* target = getNode(path);//get the inode at the given path

        if (target != nullptr) {//unless the given node is file or a nullptr
            prev_inode = curr_inode;//set the previous node as the current node
            curr_inode = target;//set the current node as the target node
        } else {
            //show an error message if the path cant be found
            cout << "Path not found." << endl; 
        }
        return;
    } 


    else {  // Move to specified folder
        Inode* target = getChild(curr_inode, path);//get the children node in the path

        //unless the given node is file or a nullptr
        if (target != nullptr && target->getType() == Folder) {//if the inode is a folder
            prev_inode = curr_inode;//set the previous node as the current node
            curr_inode = target;//set the current node as the target node
        } else {
            cout << "Invalid folder or file." << endl;
        }
        return;
    }
}


//create a method which removes the file with a given name
void VFS::rm(string file_name) {
    // Find the target file in the current inode's children
    Inode* target = nullptr;
    Vector<Inode*>& children = curr_inode->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {//traverse through all the children
        if (children[i]->getName() == file_name) {//if the file name matches any child's name, set that child's name as the target
            target = children[i];
            children.erase(i);//remove that child
            break;
        }
    }

    if (target != nullptr) {

        // Enqueue the removed file/folder to the bin
        bin.enqueue(target);
        bin_paths.enqueue(pwd() + "/" + file_name);//enqueue the path as well

        // Update the current inode's size
        unsigned int totalSize = 0;
        for (size_t j = 0; j < children.size(); ++j) {//traverse through all the children
            totalSize += children[j]->getSize();//update the child's size to the parent's size
        }

        //set the new size
        curr_inode->setSize(totalSize);
        // Update parent sizes
        Inode* temp = curr_inode;
        while (temp != nullptr) {//stops after traversing through root
            unsigned int totalSize = 0;//initialize the size as 0
            Vector<Inode*>& children = temp->getChildren();//store the children inodes to a vector
            for (size_t i = 0; i < children.size(); ++i) {//traverse through all the children
                totalSize += children[i]->getSize();
            }
            //set the new size
            temp->setSize(totalSize);

            //forward to the parent
            temp = temp->getParent();
        }

        cout << "File '" << file_name << "' removed and added to bin." << endl;
    } 

    else {
        cout << "File '" << file_name << "' not found." << endl;
    }
}


//create a method which returns the total size of a file/folder
void VFS::size(string path) {


    Inode* target = getNode("/"+path);//get the node at that path

    if (target == nullptr) {//show error message if the path does not exist
        cout << "Invalid path." << endl;
        return;
    }

    //if the target is folder, call the helper method to update the size
    if (target->getType() == Folder) {
        unsigned int totalSize = 0;
        calculateFolderSize(target, totalSize);//call the helper method
        cout << "Size of " << target->getName() << " and its contents: " << totalSize << " bytes" << endl;
    } 
    else {
        cout << "Size of " << target->getName() << ": " << target->getSize() << " bytes" << endl;
    }
}

//the helper method for calculating folder size
void VFS::calculateFolderSize(Inode* folder, unsigned int& totalSize) {
    if (folder == nullptr || folder->getType() != Folder) {//if it is not a folder or an invalid folder, return
        return;
    }

    Vector<Inode*>& children = folder->getChildren();//store the children in a Vector

    for (size_t i = 0; i < children.size(); ++i) {//traverse through all the children
        totalSize += children[i]->getSize();//add the child's size to the parents size

        if (children[i]->getType() == Folder) {//if it is a folder
            calculateFolderSize(children[i], totalSize); // Recursively calculate sizes of subfolders
        }
    }
}



// Show items in the bin
void VFS::showbin() {
    int bin_size = bin.getSize();//get the bin size

    if (bin_size == 0) {//print if the bin has nothing in it
        cout << "Bin is empty." << endl;
        return;
    }

    cout << "Items in the bin:" << endl;

    //traverse through the elements
    for (int i = 0; i < bin_size; ++i) {
        cout << bin_paths.front_element() << endl;//print the oldest erlement
        bin_paths.enqueue(bin_paths.dequeue()); // Displayed path moved to the end for next display
    }
}

// Empty the bin
void VFS::emptybin() {
    while (!bin.isEmpty()) {
        delete bin.dequeue(); // Free memory associated with the Inode
        bin_paths.dequeue(); // Remove the path from bin_paths
    }
}

//helper method to format the time
string VFS::getTime() {
    time_t currentTime = time(nullptr);// to get the current time from the system clock
    tm* localTime = localtime(&currentTime);//to convert it to the local tiem
    char formattedTime[100];//formatting the screen

    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", localTime);//writes in year, month, day, hour, minute, and second
    return string(formattedTime);//return the formatted version
}

// Find a file or directory
void VFS::find(string name) {
    // Start the search from the root
    find_helper(root, name,"");//call the helper method
}

//create a helper method to start finding from root
void VFS::find_helper(Inode* ptr, string name, string currentPath) {
    if (ptr->getName() == name) {

        //if the given name matches, print the path
        if (currentPath == "/Root") {
            cout << "Found at: /" << name << endl;
        } else {
            cout << "Found at: " << (currentPath.size() > 6 ? currentPath.substr(6) : currentPath) << "/" << name << endl;

        }
    }
    

    Vector<Inode*>& children = ptr->getChildren();//store all the children in a vector

    //traverse through all the children
    for (int i = 0; i < children.size(); ++i) {

        string newPath = currentPath == "/Root" ? "/" + ptr->getName() : currentPath + "/" + ptr->getName();
        find_helper(children[i], name, newPath); // Recursively search in children
    }
}

//create a function to move a file to a specific folder
void VFS::mv(string file, string folder) {
    // Find the target file and folder in the current inode's children
    Inode* targetFile = nullptr;
    Inode* targetFolder = nullptr;
    Vector<Inode*>& children = curr_inode->getChildren();//store all the children in a vector


    //traverse through all the children
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]->getName() == file) {//if file name matches with the given name, assign it to targetFile
            targetFile = children[i];
        }
        if (children[i]->getName() == folder) {//if folder name matches with the given name, assign it to targetFolder
            targetFolder = children[i];
        }
    }

    if (targetFile != nullptr && targetFolder != nullptr) {
        if (targetFolder->getType() == Folder) {
            // Move the file to the specified folder
            targetFolder->addChild(targetFile);//add it to the new folder
            curr_inode->removeChild(file);//remove it from the current node

            cout << "File '" << file << "' moved to folder '" << folder << "'." << endl;
        } else {
            cout << "Target '" << folder << "' is not a folder." << endl;
        }
    } else {
        cout << "File or folder not found." << endl;
    }
}



// Recover the oldest item from the bin
void VFS::recover() {
    if (bin.isEmpty()) {// if the bin is empty, print it
        cout << "Bin is empty." << endl;
        return;
    }

    Inode* recoveredItem = bin.dequeue(); // Get the oldest item from bin
    string recoveredPath = bin_paths.dequeue(); // Get its path

    // Try to find the parent Inode where the recovered item should be added
    size_t lastSlash = recoveredPath.find_last_of("/");
    string parentPath = recoveredPath.substr(0, lastSlash);
    Inode* parent = getNode(parentPath);




    if (parent != nullptr && parent->getType() == Folder) {
        parent->addChild(recoveredItem); // Add the recovered item to the parent Inode
    } else {
        cout << "Could not restore item to its original position." << endl;
    }


    // Update the current inode's size
    unsigned int totalSize = 0;//at first, initialize the total size with zero
    Vector<Inode*>& children = curr_inode->getChildren();

    //traverse throug all the children
    for (size_t i = 0; i < children.size(); ++i) {
        totalSize += children[i]->getSize();//add the size of the newly created file to its parent directory
    }

    //set the new size 
    curr_inode->setSize(totalSize);

    // Update parent sizes
    Inode* temp = curr_inode;
    while (temp != nullptr) {//stops after traversing through the root directory
        unsigned int totalSize = 0;
        Vector<Inode*>& children = temp->getChildren();
        for (size_t i = 0; i < children.size(); ++i) {//traverse through all the children
            totalSize += children[i]->getSize();
        }
        temp->setSize(totalSize);//set the new updated size
        temp = temp->getParent();//forward it to the parent inode
    }
}

//create a function to exit the function
void VFS::exit() {
    // writeToFile(); // Save changes before exiting
    std::exit(EXIT_SUCCESS); // or EXIT_FAILURE, depending on the context
}

#endif
