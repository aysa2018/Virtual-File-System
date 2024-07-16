//============================================================================
// Name         : inode.hpp
// Author       : Aysa Binte masud
// Version      : 01
// Date Created : 10 November 2023
// Date Modified: 15 November 2023
// Description  : Inode implmentation in C++
//============================================================================
#ifndef INODE_H
#define INODE_H
#include <iostream>
#include<cstdlib>
#include<string>
#include<ctime>
#include<sstream>
#include<vector>
#include "vector.hpp"


using namespace std;
enum {File=0,Folder=1};

class Inode
{
	private:
		string name;				//name of the Inode
		bool type;					//type of the Inode 0 for File 1 for Folder
		unsigned int size;			//size of current Inode
		string cr_time; 			//time of creation
		
		Vector<Inode*> children;	//Children of Inode
		Inode* parent; 				//link to the parent 

	public:
		//constructor for initializing the attributes
		Inode(string name, Inode* parent, bool type, int size, string cr_time) : name(name),type(type),size(size),cr_time(cr_time),parent(parent)
		{ }

		//declare the friend classes
		template <typename U>
		friend class Vector;
		friend class VFS;

		// Getter methods
	    string getName() const { return name; }//for the name
	    bool getType() const { return type; }// for the type: file or folder
	    unsigned int getSize() const { return size; }// for the size
	    string getCreationTime() const { return cr_time; }// for the creation time
	    Inode* getParent() const { return parent; }// for the parent of that inode
	    Vector<Inode*>& getChildren() { return children; }//a vector containing all the children of the given inodee
	    void setSize(unsigned int i){size=i;}// as setter method for updating the private attribute size

	    // Method to add a child Inode
	    void addChild(Inode* child) {
	    	//if it is a folder, add the child, otherwise, show an error message
	        if (type == Folder) {
	            children.push_back(child);
	        } else {
	            cout << "Cannot add children to a File Inode." << endl;
	        }
	    }

	    // Method to remove a child Inode
	    void removeChild(string childName) {

	    	//traverse through all the children inodes
	        for (size_t i = 0; i < children.size(); ++i) {
	            if (children[i]->getName() == childName) {// if the given name matches the name of the child 
	                delete children[i]; // Free memory associated with the Inode
	                children.erase(i);// delete that from the vector as well
	                return;
	            }
	        }
	        cout << "Child with name '" << childName << "' not found." << endl;
	    }

	    // Method to find a child Inode by name
	    Inode* findChild(string childName) {
	    	//traverse through all the children inodes
	        for (size_t i = 0; i < children.size(); ++i) {
	        	// if the given name matches the name of the child 
	            if (children[i]->getName() == childName) {
	                return children[i];// return the child
	            }
	        }
	        //error message if child not found
	        cout << "Child with name '" << childName << "' not found." << endl;
	        return nullptr;
	    }

	    // Method to display children of the Inode
	    void displayChildren() {
	        cout << "Children of Inode '" << name << "':" << endl;
	        for (size_t i = 0; i < children.size(); ++i) {
	            cout << children[i]->getName() << endl;
	        }
	    }

	    // Destructor to free memory of children
	    ~Inode() {
	    	//traverse through all the children inodes
	        for (size_t i = 0; i < children.size(); ++i) {
	            delete children[i];//delete the children
	        }
	    }
};

#endif