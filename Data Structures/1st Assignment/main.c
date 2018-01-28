#include<stdio.h>
#include <ctype.h>
#include<stdlib.h>
int* PushRow(int isRealloc , int* pRow,int* arraySize,int* pTop,int value) // Pushes the row number to the row stack array
{
    if(isRealloc){// Reallocates the row stack array
        *pTop=*pTop+1;
        int* tmp;
        tmp =(int*) realloc(pRow,(*arraySize)*sizeof(int));
        if(tmp!= NULL){
            pRow=tmp;
            pRow[*pTop] = value;
            return pRow;
        }
        else{
            //Returns NULL
            return NULL;
        }
    }
    else {
        *pTop=*pTop+1;
        pRow[*pTop] = value;
        return pRow;
    }
}
int* PushColumn(int isRealloc, int* pColumn,int* arraySize,int* pTop,int value){ // Pushes the column number to the column stack
      if(isRealloc){// Reallocates the column stack array
        int* tmp;
        tmp =(int*) realloc(pColumn,(*arraySize)*sizeof(int));
        if(tmp!= NULL){
            pColumn=tmp;
            pColumn[*pTop] = value;
            return pColumn;
        }
        else{
            //Returns NULL
            return NULL;
        }
      }
      else{
        pColumn[*pTop] = value;
        return pColumn;
      }
}
char* PushElements(int isRealloc, char* pElements,int* arraySize,int* pTop,char value){ // Pushes the instant position element to the element stack
    if(isRealloc){// Reallocates the element stack array
        char* tmp;
        tmp =(char*) realloc(pElements,(*arraySize)*sizeof(int));
        if(tmp!= NULL){
            pElements=tmp;
            pElements[*pTop] = value;
            return pElements;
        }
        else{
            //Returns NULL
            return NULL;
        }
    }
    else{
        pElements[*pTop] = value;
        return pElements;
    }
}
void Pop(int* pTop) //Move one back
{
	if(*pTop == -1) { // Checks whether there is an element to pop
		return;
	}
	*pTop = *pTop - 1;
}
int Top(int* pRow,int* pColumn,char* pElements,int* pTop,int isRow) // Returns the last elements of the stack arrays
{
    if(isRow==1){
        return pRow[*pTop];
    }
    else if(isRow == 0){
        return pColumn[*pTop];
    }
    else{
        return pElements[*pTop];
    }
}
int IsEmpty(int* pTop) // Checks whether stack is empty
{
    if(*pTop == 0) return 1;
    return 0;
}
int hasKey(int* pIndx,char* pKeys,char door){ // Checks whether there is a key which can open a specific door
    int incr;
    for(incr = 0;incr<=(*pIndx);incr++){
		if(pKeys[incr]==tolower(door)){
            return 1;
            }
    }
    return 0;
}
int isNotSameKey(int* pIndx,char* pKeys,char key){ // Checks whether the key which is just collected is already in the key array
    int j;
    int hasNoSameKey=1;
    for(j=0;j<=(*pIndx);j++){
        if(pKeys[j] == key){
            hasNoSameKey=0;
            break;
        }
    }
    if(hasNoSameKey){
        return key;
    }
    else{
        return 0;
    }
}
char* addKey(int* pIndx,char* pKeys,char key){ // Adds a key to the key array
        *pIndx = *pIndx + 1;
        char* tmp;
        tmp =(char*) realloc(pKeys,(*pIndx+1)*sizeof(int));
        if(tmp!= NULL){
            pKeys=tmp;
            pKeys[*pIndx] = key;
            return pKeys;
        }
        else{
            printf("\nReturns NULL\n");
            return NULL;
        }
    }
void clearMaze(char* mazeArray,int column , char p[][column],int key){ // Clears the maze in order to make possible to move freely
    int incr;
    int incr2;
    int incr3;
    int incr4;
    int mazeIndex=0;
    for(incr=0;incr<column;incr++){
        for(incr2=0;incr2<column;incr2++){
             p[incr][incr2]=mazeArray[column*(incr)+incr2];
        }
    }
}
int path(int* pRow,int* pColumn,char* pElements,char* pKeys,
        int* arraySize,int* pTop , int* pIndx,char* mazeArray,
        int sideLength,char p[][sideLength],int isCrashed,int isDone){ // Finds a valid path for any maze
        int isRealloc=0;
        int rowNo =  Top(pRow,pColumn,pElements,pTop,1);
        int columnNo = Top(pRow,pColumn,pElements,pTop,0);
        if(isDone){ // Checks whether the valid path is found
            FILE *f = fopen("path.txt","w");
            int j;
            int k;
            int i;
            for(i = 0;i<=(*pTop);i++){
                if(pElements[i]=='S'){
                    fprintf(f,"Start ");
                }
                if(i!=0 && pRow[i]==pRow[i-1] && (pColumn[i]-pColumn[i-1])==1){
                    fprintf(f,"E ");
                }
                if(i!=0 && pColumn[i]==pColumn[i-1] && (pRow[i]-pRow[i-1])==1){
                    fprintf(f,"S ");
                }
                if(i!=0 && pRow[i]==pRow[i-1] && (pColumn[i-1]-pColumn[i])==1){
                    fprintf(f,"W ");
                }
                if(i!=0 && pColumn[i]==pColumn[i-1] && (pRow[i-1]-pRow[i])==1){
                    fprintf(f,"N ");
                }
                if(isalpha(pElements[i]) && pElements[i]!='S' && pElements[i]!='E'){
                    fprintf(f,"%c ",pElements[i]);
                }
                if(pElements[i]=='E'){
                    fprintf(f,"Exit ");
                }
            }
        fclose(f);
        return 1;
    }
        if(isCrashed){ // Checks whether there is no valid position to move
            if(IsEmpty(pTop)){
                FILE *f = fopen("path.txt","w");
                fprintf(f,"There is no valid path.");
                fclose(f);
                return 0;
            }
            else{
                p[rowNo][columnNo]='-';
                Pop(pTop);
                *arraySize = *arraySize - 1;
                path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
        }
        if(*pTop == (*arraySize)-1){ // Checks whether the reallocation is necessary for stack arrays
            *arraySize = (*arraySize) + 1;
            isRealloc=1;
        }
        if(!isCrashed){ // Checks four possible directions for specific situations in an order down,left,right,up
            if(rowNo+1<sideLength && p[rowNo+1][columnNo]=='E'){ // Checks the exit positions
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo+1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements (isRealloc,pElements,arraySize,pTop,'E');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,1);
            }
            else if(columnNo!=0 && p[rowNo][columnNo-1]=='E'){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo-1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'E');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,1);
            }
            else if(columnNo+1<sideLength && p[rowNo][columnNo+1]=='E'){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo+1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'E');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,1);
            }
            else if(rowNo!=0 && p[rowNo-1][columnNo]=='E'){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo-1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'E');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,1);
            }
            else if(rowNo+1<sideLength && p[rowNo+1][columnNo]=='S' && !IsEmpty(pTop)){ // Checks the starting position for another visit other than starting
                p[rowNo+1][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo+1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements =PushElements(isRealloc,pElements,arraySize,pTop,'+');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo!=0 && p[rowNo][columnNo-1]=='S' && !IsEmpty(pTop)){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo-1);
                pElements =PushElements(isRealloc,pElements,arraySize,pTop,'+');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo+1<sideLength && p[rowNo][columnNo+1]=='S' && !IsEmpty(pTop)) {
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo+1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'+');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo!=0 && p[rowNo-1][columnNo]=='S' && !IsEmpty(pTop)){//To the up
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo-1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'+');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo+1<sideLength && isalpha(p[rowNo+1][columnNo]) && isupper(p[rowNo+1][columnNo])&& hasKey(pIndx,pKeys,p[rowNo+1][columnNo])){ // Checks the door positions
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo+1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo+1][columnNo]);
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo!=0 && isalpha(p[rowNo][columnNo-1]) && isupper(p[rowNo][columnNo-1])&& hasKey(pIndx,pKeys,p[rowNo][columnNo-1])){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo-1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo][columnNo-1]);
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo+1<sideLength && isalpha(p[rowNo][columnNo+1]) && isupper(p[rowNo][columnNo+1]) && hasKey(pIndx,pKeys,p[rowNo][columnNo+1])) {
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo+1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo][columnNo+1]);
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo!=0 && isalpha(p[rowNo-1][columnNo]) && isupper(p[rowNo-1][columnNo])&& hasKey(pIndx,pKeys,p[rowNo-1][columnNo])){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo-1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo-1][columnNo]);
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo+1<sideLength && isalpha(p[rowNo+1][columnNo]) && islower(p[rowNo+1][columnNo])) { // Checks the key positions
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo+1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo+1][columnNo]);
                int hasNoSameKey = isNotSameKey(pIndx,pKeys,p[rowNo+1][columnNo]);
                if(hasNoSameKey){
                    pKeys = addKey(pIndx,pKeys,p[rowNo+1][columnNo]);
                    clearMaze(mazeArray,sideLength,p,hasNoSameKey);
                }
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo!=0 && isalpha(p[rowNo][columnNo-1]) && islower(p[rowNo][columnNo-1])) {
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo-1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo][columnNo-1]);
                int hasNoSameKey = isNotSameKey(pIndx,pKeys,p[rowNo][columnNo-1]);
                if(hasNoSameKey){
                    pKeys = addKey(pIndx,pKeys,p[rowNo][columnNo-1]);
                    clearMaze(mazeArray,sideLength,p,hasNoSameKey);
                }
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo+1<sideLength && isalpha(p[rowNo][columnNo+1]) && islower(p[rowNo][columnNo+1])) {
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo+1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo][columnNo+1]);
                int hasNoSameKey = isNotSameKey(pIndx,pKeys,p[rowNo][columnNo+1]);
                if(hasNoSameKey){
                    pKeys = addKey(pIndx,pKeys,p[rowNo][columnNo+1]);
                    clearMaze(mazeArray,sideLength,p,hasNoSameKey);
                }
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo!=0 && isalpha(p[rowNo-1][columnNo]) && islower(p[rowNo-1][columnNo])) {
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo-1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements =PushElements(isRealloc,pElements,arraySize,pTop,p[rowNo-1][columnNo]);
                int hasNoSameKey = isNotSameKey(pIndx,pKeys,p[rowNo-1][columnNo]);
                if(hasNoSameKey){
                    pKeys = addKey(pIndx,pKeys,p[rowNo-1][columnNo]);
                    clearMaze(mazeArray,sideLength,p,hasNoSameKey);
                }
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo+1<sideLength && p[rowNo+1][columnNo]=='0'){ // Checks the possible empty positions to move
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo+1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'0');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
             else if(columnNo!=0 && p[rowNo][columnNo-1]=='0'){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo-1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'0');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(columnNo+1<sideLength && p[rowNo][columnNo+1]=='0'){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo+1);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'0');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else if(rowNo!=0 && p[rowNo-1][columnNo]=='0'){
                p[rowNo][columnNo]='2';
                pRow = PushRow(isRealloc,pRow,arraySize,pTop,rowNo-1);
                pColumn = PushColumn(isRealloc,pColumn,arraySize,pTop,columnNo);
                pElements = PushElements(isRealloc,pElements,arraySize,pTop,'0');
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,0,0);
            }
            else{ // Checks whether there is no valid position to move
                return path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,p,1,0);
            }
    }
}
int sideLengthFromFile(FILE *pFile){ // Calculates the side length of the maze
    int sideLength=0;
    int kr;
    int firstLine=1;
    while((kr=fgetc(pFile)) != EOF){
        if(kr!='\n'){
            sideLength++;
        }
        else if(kr=='\n'){
            break;
        }
    }
    return sideLength;
}
char* readFiletoArray(FILE *pFile,int sideLength){ // Returns the one contiguous array which stores the elements of maze
    char* mazeArray = (char*)malloc(sideLength*sideLength*(sizeof(char)));
    int cr;
    int index=0;
    rewind(pFile);
    while((cr=fgetc(pFile)) != EOF){
        if(cr!='\n'){
            mazeArray[index]=cr;
            index++;
        }
    }
    return mazeArray;
}
int main(int argc,char *argv[]) { // Performs crucial actions like reading file , creating dynamically allocated arrays , calling recursive function which will find a valid path
    FILE *pFile = fopen(argv[1],"r");
    int sideLength = sideLengthFromFile(pFile);
    char* mazeArray = readFiletoArray(pFile,sideLength);
    fclose(pFile);
    char (*mPointer)[sideLength] = malloc(sizeof(char[sideLength][sideLength]));
    int* arraySize = malloc(sizeof(int));
    *arraySize = 1;
    int* pRow =(int*) malloc((*arraySize)*sizeof(int));
    int* pColumn =(int*) malloc((*arraySize)*sizeof(int));
    char* pElements =(char*) malloc((*arraySize)*sizeof(char));
    char* pKeys =(char*) malloc((*arraySize)*sizeof(char));
    int* pTop = malloc(sizeof(int));
    *pTop = -1;
    int* pIndx = malloc(sizeof(int));
    *pIndx = -1;
    int mazeIndex=0;
    int j;
    int i;
    for(j=0;j<sideLength;j++){
        for(i=0;i<sideLength;i++){
                mPointer[j][i] = mazeArray[mazeIndex];
                mazeIndex++;
        }
    }
    int m;
    int k;
    int r;
    for(r=0;r<sideLength;r++){
        if(mPointer[0][r]=='S'){
            PushRow(0,pRow,arraySize,pTop,0);
            PushColumn(0,pColumn,arraySize,pTop,r);
            PushElements(0,pElements,arraySize,pTop,'S');
        }
    }
    path(pRow,pColumn,pElements,pKeys,arraySize,pTop,pIndx,mazeArray,sideLength,mPointer,0,0);
}
