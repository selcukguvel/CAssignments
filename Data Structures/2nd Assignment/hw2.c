#include <stdio.h>
#include <stdlib.h>
typedef struct { // Creates a data structure which represents the stack
    char* stackArray;
    int size;
    int top;
} Stack;
int isStackFull(Stack* stacksPointer){ // Checks whether the stack is full
    if(stacksPointer->top == stacksPointer->size-1)
        return 1;
    else
        return 0;
}
int isStackEmpty(Stack* stacksPointer){ // Checks whether the stack is empty
    if(stacksPointer->top==-1)
        return 1;
    else
        return 0;
}
int Top(Stack* stacksPointer){ // Returns the element which is added to the stack after the other elements
    if(isStackEmpty(stacksPointer))
        return 0;
    else
        return stacksPointer->stackArray[stacksPointer->top];
}
int PushToStack(Stack* stacksPointer,char info){ // Pushes the interrupt character to a client or server's stack
    if(isStackFull(stacksPointer))
        return 0;
    else{
        stacksPointer->top++;
        stacksPointer->stackArray[stacksPointer->top]=info;
        return 1;
    }
}
int PopFromStack(Stack* stacksPointer){ // Decrements the top value to points to the previous element of stack
    if(isStackEmpty(stacksPointer))
        return 0;
    else{
        return stacksPointer->stackArray[stacksPointer->top--];
    }
}
typedef struct { // Creates a data structure which represents the queue
    char* queueArray;
    int size;
    int front;
    int rear;
} Que;
int isQueueFull(Que* quePointer){ // Checks whether the queue is full
    if(((quePointer->rear+1) % quePointer->size) == quePointer->front)
        return 1;
    else
        return 0;
}
int isQueueEmpty(Que* quePointer){ // Checks whether the queue is empty
    if(quePointer->rear==-1)
        return 1;
    else
        return 0;
}
int Front(Que* quePointer){ // Returns the element which is added to queue before the other elements
    if(isQueueEmpty(quePointer))
        return 0;
    else
        return quePointer->queueArray[quePointer->front];
}
int PushToQueue(Que* quePointer,char info){ // Pushes the process or interrupt character to a client or server's queue
    if(isQueueFull(quePointer))
        return 0;
    if(isQueueEmpty(quePointer))
        quePointer->front=0;
    quePointer->rear = ((quePointer->rear)+1)%quePointer->size;
    quePointer->queueArray[quePointer->rear]=info;
    return 1;
    }
int PopFromQueue(Que* quePointer){ // Decrements the top value to points to the previous element of stack
    if(isQueueEmpty(quePointer))
        return 0;
    if(quePointer->rear==quePointer->front)
        quePointer->rear=quePointer->front=-1;
    else
        quePointer->front = (quePointer->front+1) % (quePointer->size);
}
typedef struct{ // Creates a data structure which represents the log history
    char* logArray;
    int logTop;
    int logSize;
}logHistory;
int PushToLogHistory(logHistory* logPointer , char info){ // Pushes the interrupt,process or error character to the log history
    if(isLogFull(logPointer))
        return 0;
    else{
        logPointer->logTop++;
        logPointer->logArray[logPointer->logTop]=info;
        return 1;
    }
}
int isLogFull(logHistory* logPointer){ // Checks whether the log history is full
    if(logPointer->logTop == logPointer->logSize-1)
        return 1;
    else
        return 0;
}
void printLogHistory(FILE* pFile3,int numberOfItems,int numberOfCommands,logHistory** logPointer){ // Prints the clients' and server's log histories to the text file
    int i;
    int k;
    for(i=0;i<numberOfItems;i++){
        for(k=0;k<=logPointer[i]->logTop;k++){
            if(!(k==logPointer[i]->logTop))
                fprintf(pFile3,"%c ",logPointer[i]->logArray[k]);
            else
                fprintf(pFile3,"%c",logPointer[i]->logArray[k]);
        }
        fprintf(pFile3,"\n");
    }
}


void commands(FILE* pFile2,FILE* pFile3,int numberOfItems,int numberOfCommands,Stack** stacksPointer,Que** quePointer,logHistory** logPointer){ // Reads the commands from text file and performs the actions according to them
    char firstSlot;
    char secondSlot;
    char thirdSlot;
    int intSecondSlot;
    int incr;
    for(incr=0;incr<numberOfCommands;incr++){
        fscanf(pFile2," %c %c %c ",&firstSlot,&secondSlot,&thirdSlot);
        intSecondSlot = secondSlot - '0';
        if(firstSlot=='I'){ // Checks the interrupt request command for the clients and the server
            if(!(PushToStack(stacksPointer[intSecondSlot-1],thirdSlot))){
                if(!isLogFull(logPointer[intSecondSlot-1]))
                        PushToLogHistory(logPointer[intSecondSlot-1],'2');
            }
        }
        else if(firstSlot=='A'){ // Checks the process request command for the clients
            if(!(PushToQueue(quePointer[intSecondSlot-1],thirdSlot))){
                if(!isLogFull(logPointer[intSecondSlot-1]))
                        PushToLogHistory(logPointer[intSecondSlot-1],'1');
            }
        }
        else if(firstSlot=='S'){ // Checks the send command for the clients
            if(!isStackEmpty(stacksPointer[intSecondSlot-1])){ // Checks whether the client's stack structure is empty which has more priority than queue
                char poppedValue = PopFromStack(stacksPointer[intSecondSlot-1]);
                PushToLogHistory(logPointer[intSecondSlot-1],poppedValue); // Adds the popped character to the client's log history whether server's queue is full or not.
                if(!isQueueFull(quePointer[numberOfItems-1]))
                    PushToQueue(quePointer[numberOfItems-1],poppedValue);// Adds the popped character to the server's queue
                else{
                     if(!isLogFull(logPointer[numberOfItems-1]))
                        PushToLogHistory(logPointer[numberOfItems-1],'1');// If the server's queue is full then error character '1' will be added to the server's log history.
            }
        }
            else if(!isQueueEmpty(quePointer[intSecondSlot-1])){ // Checks whether the client's queue structure is empty
                char poppedValue = Front(quePointer[intSecondSlot-1]);
                PopFromQueue(quePointer[intSecondSlot-1]);
                PushToLogHistory(logPointer[intSecondSlot-1],poppedValue); // Add the popped character to the client's log history whether server's queue is full or not.
                if(!isQueueFull(quePointer[numberOfItems-1]))
                    PushToQueue(quePointer[numberOfItems-1],poppedValue); // Adds the popped character to the server's queue
                else{
                    if(!isLogFull(logPointer[numberOfItems-1]))
                        PushToLogHistory(logPointer[numberOfItems-1],'1'); // If the server's queue is full then error character '1' will be added to the server's log history.
            }
        }
            else{ // Checks whether the client's stack and queue structure is empty
                if(!isLogFull(logPointer[intSecondSlot-1]))
                    PushToLogHistory(logPointer[intSecondSlot-1],'3'); // Adds error character '3' to the client's log history
            }
        }
        else if(firstSlot=='O'){ // Checks the operate command for server
            if(!isStackEmpty(stacksPointer[numberOfItems-1])){ // Checks whether the server's stack structure is empty
                char poppedValue = PopFromStack(stacksPointer[numberOfItems-1]);
                PushToLogHistory(logPointer[numberOfItems-1],poppedValue); // Adds the popped character to the server's log history
            }
            else if(!isQueueEmpty(quePointer[numberOfItems-1])){ // Checks whether the server's queue structure is empty
                char poppedValue = Front(quePointer[numberOfItems-1]);
                PopFromQueue(quePointer[numberOfItems-1]);
                PushToLogHistory(logPointer[numberOfItems-1],poppedValue); // Adds the popped character to the server's log history
            }
            else{ // Checks whether the server's stack and queue structure is empty
                if(!isLogFull(logPointer[numberOfItems-1]))
                    PushToLogHistory(logPointer[numberOfItems-1],'3'); //Adds error character '3' to the server's log history
            }
        }
}
    printLogHistory(pFile3,numberOfItems,numberOfCommands,logPointer); //Prints the clients' and the server's log histories to the text file
}
int main(int argc,char *argv[]) // Performs crucial actions like reading files , creating the array of structure pointers , initializing all structure's members and calling command function
{
    FILE *pFile1 = fopen(argv[1],"r");
    FILE *pFile2 = fopen(argv[2],"r");
    FILE *pFile3 = fopen(argv[3],"w");
    int cr;
    int numberOfItems;
    fscanf(pFile1,"%d",&numberOfItems);
    int numberOfCommands;
    fscanf(pFile2,"%d",&numberOfCommands);
    Stack** stacksPointer = (Stack**) malloc(numberOfItems*sizeof(Stack*));
    Que** quePointer = (Que**) malloc(numberOfItems*sizeof(Que*));
    logHistory** logPointer = (logHistory**) malloc(numberOfItems*sizeof(logHistory*));
    int i;
    for(i=0;i<numberOfItems;i++){
        stacksPointer[i] = (Stack*) malloc(sizeof(Stack));
        quePointer[i] = (Que*) malloc(sizeof(Que));
        logPointer[i] = (logHistory*) malloc(sizeof(logHistory));
    }
    int queueSize;
    int stackSize;
    int incr;
    for(incr=0;incr<numberOfItems;incr++){
        fscanf(pFile1," %d %d ",&queueSize,&stackSize);
        stacksPointer[incr]->stackArray = malloc(stackSize*sizeof(char));
        stacksPointer[incr]->size = stackSize;
        stacksPointer[incr]->top = -1;
        quePointer[incr]->queueArray = malloc(queueSize*sizeof(char));
        quePointer[incr]->size = queueSize;
        quePointer[incr]->front = -1;
        quePointer[incr]->rear = -1;
        logPointer[incr]->logArray = malloc(numberOfCommands*sizeof(char));
        logPointer[incr]->logSize = numberOfCommands;
        logPointer[incr]->logTop = -1;
    }
    commands(pFile2,pFile3,numberOfItems,numberOfCommands,stacksPointer,quePointer,logPointer);
    fclose(pFile1);
    fclose(pFile2);
    fclose(pFile3);
    return 1;
}
