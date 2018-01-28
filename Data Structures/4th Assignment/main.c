#include <stdio.h>
#include <stdlib.h>
typedef struct Node Node;
typedef struct LeafHolder LeafHolder;
struct Node {
    int identity;
    struct Node* sibling;
    struct Node* child;
    struct Node* parent;
};
struct LeafHolder{
    int size;
    int* leafArray;
    int index;
};
Node* createNode(Node* root,int identity){//Creates a new node and returns the address of this new node.
    Node* newNode = (Node*) malloc(sizeof(Node));
    if(newNode!=NULL){
        newNode->sibling = NULL;
        newNode->child = NULL;
        newNode->parent=root;
        newNode->identity = identity;
    }
    return newNode;
}
int isContain(Node* parentRoot,LeafHolder* leafNode){//Checks whether parentRoot's identity exists in the leafArray
    int i;
    int isContain=0;
    for(i=0;i<leafNode->size;i++){
        if((leafNode->leafArray)[i]==parentRoot->identity){
            return 1;
        }
    }
    return 0;
}
Node* insertNode(Node* parentRoot,Node* root,int* nodeArray,LeafHolder* leafNode,int* index,int count,int* isTurn){//Recursive function inserts the node to the tree
    Node* rootOfTree = root;
    if(!((*index)<count)){
        return root;
    }
    if(!(*isTurn)){
        if(root==NULL){//If the insertion process happens for the first time
            root=createNode(parentRoot,nodeArray[*index]);
            *index=*index+1;
            leafNode->index++;
            if(!(leafNode->index < leafNode->size)){
                leafNode->size=(leafNode->size)*2;
                int* tempArray = (int*)realloc(leafNode->leafArray,(leafNode->size)*sizeof(int));
                if(tempArray!=NULL)
                    leafNode->leafArray=tempArray;
            }
            leafNode->leafArray[leafNode->index]=parentRoot->identity;
            return root;
        }
    }
    else{//If the insertion process happens not for the first time
        if (root!=NULL && isContain(parentRoot,leafNode)){
            if(isContain){
                Node* tempRoot=root;
                while(tempRoot->sibling!=NULL)
                    tempRoot=tempRoot->sibling;
                tempRoot->sibling=createNode(parentRoot,nodeArray[*index]);
                *index=*index+1;
            }
            return root;
        }
    }
    while(rootOfTree!=NULL){
        rootOfTree->child=insertNode(rootOfTree,rootOfTree->child,nodeArray,leafNode,index,count,isTurn);
        rootOfTree=rootOfTree->sibling;
    }
  return root;
}
Node* readFirstFile(Node* root,FILE* Node1,FILE* Row1){//Reads first file with the help of the two file pointers which point to the same file.Data structures are created and the insertion function is called.
    int nodeIdentity,rowNumber,tempRowNumber,i,count;
    int isFinished=0;
    int isRoot=0;
    int index;
    int isTurn;
    fscanf(Row1," %d %d",&nodeIdentity,&rowNumber);
    while(fscanf(Node1," %d %d",&nodeIdentity,&rowNumber)!=EOF && !isFinished){
        index=0;
        count=0;
        isTurn=0;
        isRoot++;
        if(isRoot==1)
            root=createNode(root,nodeIdentity);
        tempRowNumber=rowNumber;
        int* nodeArray = (int*)malloc(rowNumber*sizeof(int));
        for(i=0;i<tempRowNumber;i++)
            nodeArray[i]=-1;
        while(count<tempRowNumber){
            if(!(fscanf(Row1," %d %d",&nodeIdentity,&rowNumber)!=EOF)){
                isFinished=1;
                break;
            }
            nodeArray[count]=nodeIdentity;
            count++;
        }
        count=0;
        for(i=0;i<tempRowNumber;i++){
            if(nodeArray[i]!=-1)
                count++;
            else
                break;
        }
        LeafHolder* leafNode = (LeafHolder*) malloc(sizeof(LeafHolder));
        leafNode->size=5;
        leafNode->leafArray=(int*)malloc(5*sizeof(int));
        leafNode->index=-1;
        root=insertNode(root,root,nodeArray,leafNode,&index,count,&isTurn);
        while(index<count){
            isTurn=1;
            root=insertNode(root,root,nodeArray,leafNode,&index,count,&isTurn);
        }
        if(isRoot==1){
            Node* rootSibling = root->sibling;
            Node* rootChild = root->child;
            if(rootChild!=NULL)
                rootChild->sibling=rootSibling;
            root->sibling=NULL;
        }
        isTurn=0;
        free(nodeArray);
        free(leafNode);
    }
    return root;
}
Node* deleteNode(Node* root,Node* rootOfTree,int identity){//Deletes the node which has the specified unique number.
    if(root==NULL)
        return rootOfTree;
    Node* rootParent = root->parent;
    if(rootParent==NULL){//Checks whether the deleted node is the root node.
        Node* newParent = root->child;
        if(newParent!=NULL){
            Node* rootChild = root->child;
            while(rootChild->sibling!=NULL){
                rootChild=rootChild->sibling;
                rootChild->parent=newParent;
            }
            rootChild=newParent;
            Node* childOfNewParent = rootChild->child;
            if(childOfNewParent!=NULL){
                while(childOfNewParent->sibling!=NULL)
                    childOfNewParent=childOfNewParent->sibling;
                childOfNewParent->sibling=newParent->sibling;
            }
            else
                newParent->child=newParent->sibling;
            newParent->sibling=NULL;
        }
        free(root);
        root=NULL;
        rootOfTree=newParent;
    }
    else if(rootParent!=NULL && root->child!=NULL)//Checks whether the deleted node is the middle node.
    {
        if(root==rootParent->child){//Checks whether the middle node is the first child
            Node* rootChild = root->child;
            rootParent->child=rootChild;
            rootChild->parent=root->parent;
            while(rootChild->sibling!=NULL){
                rootChild=rootChild->sibling;
                rootChild->parent=root->parent;
            }
            rootChild->sibling=root->sibling;
            free(root);
            root=NULL;
        }
        else{
            Node* ptr = rootParent->child;
            while(!(ptr->sibling->identity==root->identity))
                ptr=ptr->sibling;
            ptr->sibling=root->child;
            Node* rootChild = root->child;
            rootChild->parent=root->parent;
            while(rootChild->sibling!=NULL){
                rootChild=rootChild->sibling;
                rootChild->parent=root->parent;
            }
            rootChild->sibling=root->sibling;
            free(root);
            root=NULL;
        }
    }
    else if(rootParent!=NULL && root->child==NULL)//Checks whether the deleted node is the leaf node.
    {
        if(rootParent->child->identity==identity){//Checks whether the leaf node is the first child
            rootParent->child=root->sibling;
            free(root);
            root=NULL;
        }
        else{
            Node* ptr1=rootParent->child;
            Node* ptr2=rootParent->child->sibling;
            while(ptr2!=NULL && ptr2->sibling!=NULL && ptr2->identity!=identity){
                ptr1=ptr2;
                ptr2=ptr2->sibling;
            }
            ptr1->sibling = ptr2->sibling;
            free(ptr2);
            ptr2=NULL;
        }
    }
  return rootOfTree;
}
void findNode(Node** nodeAddress,Node* root,int identity){//Traverses the tree to find the specified node which has the unique number
    if(root==NULL)
        return;
    else if(root->identity==identity){
        *nodeAddress=root;
        return;
    }
    findNode(nodeAddress,root->child,identity);
    findNode(nodeAddress,root->sibling,identity);
}
void printTree(Node* root,FILE* Output,int** printArray,int* index,int* size){//Lists the all nodes in the tree which has the specified root node to the output file.
    if(root==NULL)
        return;
    if((*index)<(*size)){
        (*printArray)[*index]=root->identity;
        *index=*index+1;
    }
    else{
        *size=(*size)*2;
        int* tempArray = (int*)realloc(*printArray,(*size)*sizeof(int));
        if(tempArray!=NULL){
            *printArray=tempArray;
            (*printArray)[*index]=root->identity;
            *index=*index+1;
        }
    }
    printTree(root->child,Output,printArray,index,size);
    printTree(root->sibling,Output,printArray,index,size);
}
Node* readLine(Node* root,char* command,int nodeIdentity,FILE* Output){//Checks the command's type , calls the functions according to the command type.
        Node* nodeAddress=NULL;
        if(strcmp(command,"d")==0){
            findNode(&nodeAddress,root,nodeIdentity);
            root=deleteNode(nodeAddress,root,nodeIdentity);
        }
        else if(strcmp(command,"l")==0){
            findNode(&nodeAddress,root,nodeIdentity);
            if(nodeAddress!=NULL){//Checks whether there is a node which has the nodeIdentity unique number
                Node* siblingOfRoot = nodeAddress->sibling;
                nodeAddress->sibling=NULL;
                int i;
                int index=0;
                int size=10;
                int* printArray = (int*) malloc(size*sizeof(int));
                printTree(nodeAddress,Output,&printArray,&index,&size);
                for(i=0;i<index;i++){
                    if(i!=(index-1))
                        fprintf(Output,"%d,",printArray[i]);
                    else
                        fprintf(Output,"%d",printArray[i]);
                }
                fprintf(Output,"\n");
                nodeAddress->sibling=siblingOfRoot;
            }
    }
    return root;
}
Node* readSecondFile(Node* root,FILE* Command2,FILE* Output){//Reads the second input file and calls the readLine() function to process the commands.
    int nodeIdentity;
    char command[25];
    while(fscanf(Command2,"%s %d",command,&nodeIdentity)!=EOF)
        root=readLine(root,command,nodeIdentity,Output);
}
int main() //Takes two input files from the console and calls the readFirstFile() and readSecondFile() functions to read these files.
{
    char firstInput[100];
    char secondInput[100];
    scanf("%s",firstInput);
    scanf("%s",secondInput);
    FILE* Node1 = fopen(firstInput,"r");
    FILE* Row1 = fopen(firstInput,"r");
    FILE* Command2 = fopen(secondInput,"r");
    FILE* Output = fopen("output.txt","w");
    Node* root = NULL;
    root=readFirstFile(root,Node1,Row1);
    readSecondFile(root,Command2,Output);
}
