#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
typedef struct Node Node;
typedef struct RFile RFile;
typedef struct PairThree PairThree;
struct Node{
    char* word;
    int frequency;
    Node* next;
    Node* pairWord;
};
struct RFile{
    char* fileName;
    RFile* next;
    Node* headNode;
};
struct PairThree{
    char* wordName;
    char* pairName;
    int frequency;
    PairThree* next;
};
char* readWord(FILE *pFile){ // Separates the words by the delimiter of non alphanumeric characters , returns the dynamically allocated word.
    char word[25];
    int ch, i=0;
    while(EOF!=(ch=fgetc(pFile)) && !isalnum(ch));
    if(ch == EOF)
        return NULL;
    do{
        word[i++] = tolower(ch);
    }while(EOF!=(ch=fgetc(pFile)) && isalnum(ch));

    word[i]='\0';
    return strdup(word);
}
int createFileLinkedList(RFile** fileHead , Node** head , char fileName [100]){ // Creates the file linked list
    char* dynamicFileName = (char*) malloc(100*sizeof(char));
    strncpy(dynamicFileName,fileName,100);
    RFile* newFile = (RFile*) malloc(sizeof(RFile));
    RFile* last = *fileHead;
    newFile->fileName = dynamicFileName;
    newFile->next = NULL;
    newFile->headNode = *head;
    if(*fileHead == NULL)
    {
        *fileHead = newFile;
        return;
    }
    while(last->next != NULL)
        last = last->next;
    last->next = newFile;
    return;
}
int hasNode(Node** head,char passedWord[25]){// Checks whether the passed word exist in the word linked list.
    int isSame=0;
    Node* node = *head;
    while(node != NULL)
    {
        if(strcmp(passedWord,node->word)==0){
            isSame=1;
            break;
        }
        else
            node = node->next;
    }
    if(isSame)
        return 1;
    return 0;
}
int createWordLinkedList(Node** head ,char passedWord[25] , char** wordArray , int wordSize){ // Creates the word linked list and the pair linked list for the passed word.
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->word = passedWord;
    newNode->frequency = 0;
    newNode->next = NULL;
    Node* last = *head;
    Node* pairHead = NULL;
    Node* tempPair = pairHead;
    Node* tempPair2 = pairHead;
    if(hasNode(head,passedWord)){
        return;
    }
    int i;
    for(i=0;i<wordSize;i++){
        int hasSamePair=0;
        if(strcmp(wordArray[i],passedWord)== 0){
            newNode->frequency++;
            if(!(i+1>=wordSize)){ // Checks the array bound
                tempPair = pairHead;
                while(tempPair != NULL){
                    if(strcmp(tempPair->word ,wordArray[i+1]) == 0){
                        tempPair->frequency++;
                        hasSamePair=1;
                    }
                    tempPair = tempPair->next;
                }
                tempPair=pairHead;
                if(!hasSamePair){
                    Node* newPairNode = (Node*) malloc(sizeof(Node));
                    newPairNode->word = wordArray[i+1];
                    newPairNode->frequency = 1;
                    newPairNode->next = NULL;
                    if(pairHead == NULL){
                        pairHead = newPairNode;
                        tempPair = pairHead;
                    }
                    else{
                        while(tempPair->next != NULL){
                            tempPair = tempPair->next;
                        }
                        tempPair->next = newPairNode;
                    }
                }
            }
               else{
                    tempPair=pairHead;
                    Node* newPairNode = (Node*) malloc(sizeof(Node));
                    newPairNode->word ="";
                    newPairNode->frequency = 1;
                    newPairNode->next = NULL;
                    if(pairHead == NULL){
                        pairHead = newPairNode;
                        tempPair = pairHead;
                    }
                    else{
                        while(tempPair->next != NULL){
                            tempPair = tempPair->next;
                        }
                        tempPair->next = newPairNode;
                }
            }
        }
    }
    if(*head == NULL){
        *head = newNode;
        newNode->pairWord = pairHead;
        return;
    }
    while(last->next != NULL)
        last = last->next;
    last->next = newNode;
    newNode->pairWord = pairHead;
    return;
}
void bubbleSort(Node** head) // Sorts the linked list with the bubble sort algorithm
{
    if (*head == NULL){
        return;
    }
    Node* pointer1;
    Node* pointer2=NULL;
    int in;
    int isDone=1;
    while(isDone){
        pointer1=*head;
        in=0;
        while(pointer1->next!=pointer2){
            if(pointer1->frequency < pointer1->next->frequency){
                swap(&pointer1,&(pointer1->next));
                isDone=1;
                in=1;
            }
            pointer1=pointer1->next;
        }
        if(in==1){
            pointer2=pointer1;
            isDone=1;
        }
        else
            isDone=0;
    }
}
void swap(Node** ptr,Node** ptrNext) // Swaps the nodes with the copying all members' information from one to another vice versa
{
    int tempFreq;
    char* tempWord;
    Node* tempPairWord;
    Node* tempPtr = *ptr;
    Node* tempPtrNext = *ptrNext;
    tempFreq = tempPtr->frequency;
    tempWord = tempPtr->word;
    tempPairWord = tempPtr->pairWord;
    tempPtr->frequency=tempPtrNext->frequency;
    tempPtr->word=tempPtrNext->word;
    tempPtr->pairWord=tempPtrNext->pairWord;
    tempPtrNext->frequency=tempFreq;
    tempPtrNext->word=tempWord;
    tempPtrNext->pairWord=tempPairWord;
}
void addToTheLinkedList(Node** intendedHead,char wordCommand[25],int freq){// Adds the wordCommand to the word linked list with the number of freq.
    Node* last = *intendedHead;
    char* dynamicWordName = (char*) malloc(25*sizeof(char));
    strncpy(dynamicWordName,wordCommand,25);
    while(last!=NULL){
        if(strcmp(dynamicWordName,last->word)==0){
            last->frequency+=freq;
            return;
        }
        last=last->next;
    }
    last=*intendedHead;
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->word = dynamicWordName;
    newNode->frequency = freq;
    newNode->next = NULL;
    Node* pairHead = NULL;
    newNode->pairWord = pairHead;
    if(*intendedHead == NULL)
    {
        *intendedHead = newNode;
        return;
    }
    while(last->next != NULL)
        last = last->next;
    last->next = newNode;
    return;
}
void findMostThreeWord(Node* intendedHead){
    PairThree* firstNode  =  (PairThree*) malloc(sizeof(Node));
    PairThree* secondNode =  (PairThree*) malloc(sizeof(Node));
    PairThree* thirdNode  =  (PairThree*) malloc(sizeof(Node));
    firstNode->next = secondNode;
    firstNode->frequency=0;
    secondNode->next = thirdNode;
    secondNode->frequency=0;
    thirdNode->next = NULL;
    thirdNode->frequency=0;
    PairThree* tHead = firstNode;
    PairThree* innertHead = firstNode;
    PairThree* inner2Head = firstNode;
    Node* tempHead = intendedHead;
    while(tempHead != NULL){
        Node* pairHead = tempHead->pairWord;
        while(pairHead != NULL){
                while(tHead != NULL){
                     if(pairHead->frequency > tHead->frequency){
                            while(innertHead != NULL){
                                if(tHead->frequency > innertHead->frequency){
                                    while(inner2Head!=NULL){
                                        if(innertHead->frequency > inner2Head->frequency){
                                            inner2Head->wordName = innertHead->wordName;
                                            inner2Head->pairName = innertHead->pairName;
                                            inner2Head->frequency = innertHead->frequency;
                                            break;
                                        }
                                    inner2Head=inner2Head->next;
                                    }
                                    inner2Head=firstNode;
                                    innertHead->wordName = tHead->wordName;
                                    innertHead->pairName = tHead->pairName;
                                    innertHead->frequency = tHead->frequency;
                                    break;
                                }
                                innertHead = innertHead->next;
                            }
                        innertHead = firstNode;
                        tHead->wordName = tempHead->word;
                        tHead->pairName = pairHead->word;
                        tHead->frequency = pairHead->frequency;
                        break;
                     }
                    tHead = tHead->next;
                }
            tHead = firstNode;
            pairHead = pairHead->next;
        }
        tempHead = tempHead->next;
    }
    tHead = firstNode;
    while(tHead != NULL && tHead->frequency!=0){
            printf("%s - %s (%d)\n",tHead->wordName,tHead->pairName,tHead->frequency);
            tHead = tHead->next;
    }
}
void deleteNode(Node** intendedHead,char deletedWord[25]){ // Deletes the node which has the word of deletedWord
    Node* tempHead = *intendedHead;
    Node* previous = *intendedHead;
    if(tempHead != NULL && strcmp(tempHead->word,deletedWord)==0){
        *intendedHead=tempHead->next;
        free(tempHead);
        return;
    }
    while(tempHead!=NULL){
        previous=tempHead;
        tempHead=tempHead->next;
        if(tempHead!=NULL && strcmp(tempHead->word,deletedWord)==0){
            previous->next = tempHead->next;
            free(tempHead);
        }
    }
    tempHead=*intendedHead;
    return;
}
int readCommand(char* command,RFile** pfileHead){ // Takes the command , and directs it to the corresponding condition.
    RFile* fileHead = *pfileHead;
    if(strcmp(command,"-r") == 0){ // Reads the file , creates the word array , the word and file linked lists , sorts the word linked list.
        char firstTxtCommand[100];
        scanf("%s",firstTxtCommand);
        FILE* pFile = fopen(firstTxtCommand,"r");
        if(pFile==NULL){
            printf("\nPlease enter a valid filename.\n");
            return 1;
        }
        char* word;
        Node* head = NULL;
        int wordSize=0;
        while(word=readWord(pFile))
            wordSize++;
        char** array2D;
        array2D = (char**) malloc(wordSize*sizeof(char*));
        int rowIncr;
        for(rowIncr=0;rowIncr<wordSize;rowIncr++)
            array2D[rowIncr] = (char*) malloc(25*sizeof(char));
        rewind(pFile);
        int count=0;
            while(word=readWord(pFile)){
                array2D[count] = word;
                count++;
            }
        int j;
        for(j=0;j<wordSize;j++)
            createWordLinkedList(&head,array2D[j],array2D,wordSize);
        createFileLinkedList(&fileHead,&head,firstTxtCommand);
        bubbleSort(&head);
        *pfileHead=fileHead;
        return 1;
    }
    else if(strcmp(command,"-a") == 0){ // Adds the word , the specified number of times into the corresponding file's word linked list then sorts the word linked list.
        char wordCommand[25];
        char firstTxtCommand[100];
        int val=0;
        scanf("%s",wordCommand);
        scanf("%d",&val);
        scanf("%s",firstTxtCommand);
        RFile* tempFileHead = fileHead;
        Node** intendedHead=NULL;
        int fileExist=0;
        while(tempFileHead != NULL){
            if(strcmp(tempFileHead->fileName,firstTxtCommand)==0){
                intendedHead=&(tempFileHead->headNode);
                fileExist=1;
                break;
            }
            else{
                tempFileHead=tempFileHead->next;
                fileExist=0;
            }
        }
        if(val>0){
            if(fileExist)
                addToTheLinkedList(intendedHead,wordCommand,val);
            else{
                printf("\nCommand is not valid.\n");
                return 1;
            }
            if(*intendedHead!=NULL){
                bubbleSort(intendedHead);
            }
            else{
                printf("\nCommand is not valid.\n");
            }
        }
        else
            printf("\nCommand is not valid.\n");
        return 1;
    }
    else if(strcmp(command,"-n2") == 0){ // Finds the first 3 pairs of words that pass the most on the provided file name.
        char firstTxtCommand[100];
        scanf("%s",firstTxtCommand);
        RFile* tempFileHead = fileHead;
        Node* intendedHead=NULL;
        int fileExist=0;
        while(tempFileHead != NULL){
            if(strcmp(tempFileHead->fileName,firstTxtCommand)==0){
                intendedHead=tempFileHead->headNode;
                fileExist=1;
                break;
            }
            else{
                tempFileHead=tempFileHead->next;
                fileExist=0;
            }
        }
        if(!fileExist){
            printf("\nPlease enter a valid filename.\n");
            return 1;
        }
        if(intendedHead != NULL)
            findMostThreeWord(intendedHead);
        return 1;
    }
    else if(strcmp(command,"-d") == 0){ // Deletes the word from the corresponding file's word linked list.
        char firstTxtCommand[100];
        char wordCommand[25];
        scanf("%s",wordCommand);
        scanf("%s",firstTxtCommand);
        RFile* tempFileHead = fileHead;
        Node** intendedHead=NULL;
        int fileExist=0;
        while(tempFileHead != NULL){
            if(strcmp(tempFileHead->fileName,firstTxtCommand)==0){
                intendedHead=&(tempFileHead->headNode);
                fileExist=1;
                break;
            }
            else{
                tempFileHead=tempFileHead->next;
                fileExist=0;
            }
        }
        if(!fileExist){
            printf("\nPlease enter a valid filename.\n");
            return 1;
        }
        int hasWord=0;
        if(intendedHead!=NULL){
            Node* tempHead = *intendedHead;
            if(fileExist){
                while(tempHead!=NULL){
                    if(strcmp(tempHead->word,wordCommand)==0){
                        hasWord=1;
                        break;
                    }
                    tempHead=tempHead->next;
                }
            }
        }
        if(hasWord){
            if(fileExist && *intendedHead != NULL){
                deleteNode(intendedHead,wordCommand);
            }
            else
                printf("\nCommand is not valid.\n");
        }
        else
            printf("\nThis word doesn't exist.\n");
        return 1;
    }
    else if(strcmp(command,"-s") == 0){ // Calculates the cosine similarity of first provided file and the second provided file.
        char firstTxtCommand[100];
        char secondTxtCommand[100];
        scanf("%s",firstTxtCommand);
        scanf("%s",secondTxtCommand);
        RFile* tempFileHead = fileHead;
        Node* firstTxtHead=NULL;
        Node* secondTxtHead=NULL;
        int isFirstFileExist=0;
        while(tempFileHead != NULL){
            if(strcmp(tempFileHead->fileName,firstTxtCommand)==0){
                firstTxtHead=tempFileHead->headNode;
                isFirstFileExist=1;
                break;

            }
            else{
                tempFileHead=tempFileHead->next;
                isFirstFileExist=0;
            }
        }
        int isSecondFileExist=0;
        tempFileHead=fileHead;
         while(tempFileHead != NULL){
            if(strcmp(tempFileHead->fileName,secondTxtCommand)==0){
                secondTxtHead=tempFileHead->headNode;
                isSecondFileExist=1;
                break;
            }
            else{
                tempFileHead=tempFileHead->next;
                isSecondFileExist=0;
            }
        }
        Node* tempFirstTxt = firstTxtHead;
        Node* tempSecondTxt = secondTxtHead;
        int firstValueArray[20];
        int secondValueArray[20];
        char** wordArray;
        int isArrayCreated=0;
        if(tempFirstTxt!=NULL && tempSecondTxt!=NULL){
            wordArray = (char**) malloc(20*sizeof(char*));
            int rowIncr;
            int rowIncr2;
            for(rowIncr=0;rowIncr<20;rowIncr++)
                wordArray[rowIncr] = (char*) malloc(25*sizeof(char));
            isArrayCreated=1;
        }
        if(!isFirstFileExist || !isSecondFileExist){
            printf("\nPlease enter a valid filename.\n");
            return 1;
        }
        int counter=0;
        int arraySize=0;
        if(isArrayCreated){
            tempSecondTxt=secondTxtHead;
            counter=0;
            int hasSame=0;
            while(tempFirstTxt!=NULL && counter<10){
                wordArray[counter]=tempFirstTxt->word;
                firstValueArray[counter]=tempFirstTxt->frequency;
                while(tempSecondTxt!=NULL){
                    if(strcmp(tempSecondTxt->word,tempFirstTxt->word)==0){
                        secondValueArray[counter]=tempSecondTxt->frequency;
                        hasSame=1;
                        break;
                    }
                        tempSecondTxt=tempSecondTxt->next;
                        hasSame=0;
                }
                tempSecondTxt=secondTxtHead;
                if(!hasSame)
                    secondValueArray[counter]=0;
                counter++;
                arraySize++;
                tempFirstTxt=tempFirstTxt->next;
                hasSame=0;
            }
            hasSame=0;
            int hasInFirstTxt=0;
            tempFirstTxt=firstTxtHead;
            int i;
            while(tempSecondTxt!=NULL && counter<20){
                for(i=0;i<arraySize;i++){
                    if(strcmp(tempSecondTxt->word,wordArray[i])==0){
                        hasSame=1;
                        break;
                    }
                }
                if(!hasSame){
                    wordArray[arraySize]=tempSecondTxt->word;
                    secondValueArray[arraySize]=tempSecondTxt->frequency;
                    while(tempFirstTxt!=NULL){
                        if(strcmp(tempSecondTxt->word,tempFirstTxt->word)==0){
                            firstValueArray[arraySize]=tempFirstTxt->frequency;
                            hasInFirstTxt=1;
                            break;
                        }
                        tempFirstTxt=tempFirstTxt->next;
                        hasInFirstTxt=0;
                    }
                    tempFirstTxt=firstTxtHead;
                    if(!hasInFirstTxt)
                        firstValueArray[arraySize]=0;
                    arraySize++;
                }
                tempSecondTxt=tempSecondTxt->next;
                hasSame=0;
                hasInFirstTxt=0;
                counter++;
            }
        }
        int k;
        double numerator=0;
        double denominator=0;
        double firstSquareSum=0;
        double secondSquareSum=0;
        double result=0;
        int z;
        for(z=0;z<arraySize;z++){
            numerator+=firstValueArray[z]*secondValueArray[z];
            firstSquareSum+=pow(firstValueArray[z],2);
            secondSquareSum+=pow(secondValueArray[z],2);
        }
        denominator=sqrt(firstSquareSum)*sqrt(secondSquareSum);
        if(!(numerator==0 || denominator==0)){
            result=numerator/denominator;
            printf("Cosine Similarity of %s and %s is %f\n",firstTxtCommand,secondTxtCommand,result);
        }
        else
            printf("Cosine Similarity of %s and %s is %d\n",firstTxtCommand,secondTxtCommand,0);
        return 1;
    }
    else if(strcmp(command,"-q") == 0){ // Quit the program
        return 0;
    }
    else{
        printf("\nCommand is not valid.\n");
        return 1;
    }
}
int main(){ //Command type is received from the user and readFile function is called in order to operate the command.
    RFile* fileHead = NULL;
    while(1){
        char command[100];
        int val;
        scanf("%s",command);
        if(!readCommand(command,&fileHead))
            break;
    }
    return 0;
}
