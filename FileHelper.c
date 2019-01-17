#include"MyHeader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: InitializeSuperBlock
//	Description: Initializes superblock
//	Input: -
//	Output: -
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitializeSuperBlock()
{
    int i=0;
    while(i<MAX_INODES)
    {
        UFDTarr[i].pFileTable=NULL;
        i++;
    }

    sObj.Total_Blocks=MAX_INODES;
    sObj.Free_Blocks=MAX_INODES;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: CreateDILB
//	Description: Initilizes the Disk Inode List Block.
//	Input: -
//	Output: -
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateDILB()
{
    PINODE newn = NULL;
    PINODE temp = head = NULL;
    int i=1;

    while(i<=MAX_INODES)
    {
        newn =(PINODE) malloc(sizeof(INODE));
        newn->iNodeNumber=i;
        newn->Next=NULL;
        newn->Buffer=NULL;
        newn->LinkCount=newn->Count=0;
        newn->bckup=FALSE;
        newn->FileType=newn->FileSize=newn->ActualFileSize=0;
        if(temp==NULL)
        {
            head=newn;
            temp=head;
        }
        else
        {
            temp->Next=newn;
            temp=temp->Next;
        }
        i++;

    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: CreateFile
//	Description: Creates a file of 1024 bytes on heap.
//	Input:  char[IN-OUT],int[IN]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateFile(char *FileName,int Permission)
{
    int i=0,iCnt=0;
    PINODE temp=head;
    //printf("%d\n",Permission);
    if((Permission<=0)||(Permission>3)||(FileName==NULL))
    {
        return ERR_IncorrectParameters;
    }
    if(sObj.Free_Blocks==0)
    {
        return ERR_NoInodes;
    }
    if(GetINodeByName(FileName)!=NULL)
    {
        return ERR_FileAreadyExits;
    }
    while(i<MAX_INODES)
    {
        if(UFDTarr[i].pFileTable==NULL)
        {
            break;
        }
        i++;
    }
    if(i>=MAX_INODES)
    {
        return ERR_NoInodes;
    }
    UFDTarr[i].pFileTable=(PFILETABLE) malloc(sizeof(FILETABLE));
    if(UFDTarr[i].pFileTable==NULL)
    {
        return ERR_MemAllocationFailure;
    }
    UFDTarr[i].pFileTable->Mode=Permission;
    UFDTarr[i].pFileTable->ReadOffset = UFDTarr[i].pFileTable->WriteOffset = 0;
    (UFDTarr[i].pFileTable->RefCount)++;
    while(temp!=NULL)
    {
        if(temp->FileType==0)
        {
            break;
        }
        temp=temp->Next;
    }
    if(temp==NULL)
    {
        return ERR_NoInodes;
    }
    (sObj.Free_Blocks)--;
    UFDTarr[i].pFileTable->inode=temp;
    UFDTarr[i].pFileTable->inode->FileType=REGULAR;
    UFDTarr[i].pFileTable->inode->Permission=Permission;
    memset(UFDTarr[i].pFileTable->inode->FileName,0,MAX_FILE_NAME_SIZE);
    strcpy(UFDTarr[i].pFileTable->inode->FileName,FileName);
    UFDTarr[i].pFileTable->inode->FileSize=MAX_FILE_SiZE;
    UFDTarr[i].pFileTable->inode->Count = UFDTarr[i].pFileTable->inode->LinkCount = 1;
    UFDTarr[i].pFileTable->inode->ActualFileSize=0;
    UFDTarr[i].pFileTable->inode->Buffer=(char *)malloc(MAX_FILE_SiZE);
    memset(UFDTarr[i].pFileTable->inode->Buffer,0,MAX_FILE_SiZE);
    return i;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: CloseFileByName
//	Description: Closes a single file with specified name if available and opened.
//	Input:  char[IN-OUT]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CloseFileByName(char *filename)
{
    int i=0;
    if(filename==NULL)
    {
        return ERR_IncorrectParameters;
    }
    i=GetFdByName(filename);
    if(i==ERR_IndexOutOfUFDTarr)
    {
        return ERR_IndexOutOfUFDTarr;
    }
    UFDTarr[i].pFileTable->WriteOffset=0;
    UFDTarr[i].pFileTable->ReadOffset=0;
    (UFDTarr[i].pFileTable->RefCount)--;
    if((UFDTarr[i].pFileTable->RefCount)<=0)
    {
        (UFDTarr[i].pFileTable->inode->Count)--;
        free(UFDTarr[i].pFileTable);
        UFDTarr[i].pFileTable=NULL;
    }
    return i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: OpenFile
//	Description: Accepts filename, if file is available then returns the index of file descriptor otherwise returns Error values.
//	Input:  char[IN-OUT]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OpenFile(char *filename,int Permission)
{
    int i=0,j=-1;
    PINODE temp=NULL;
    if((filename==NULL)||(Permission<=0)||(Permission>3))
    {
        return ERR_IncorrectParameters;
    }
    temp=GetINodeByName(filename);
    if(temp==NULL)
    {
        return ERR_NoInodes;
    }
    if(temp->Permission!=(READ+WRITE))
    {
        if(((Permission==WRITE)&&(temp->Permission==READ))||((Permission==READ)&&(temp->Permission==WRITE))||(Permission==(READ+WRITE)))
        {
            return ERR_AccessDenied;
        }
    }
    while((i<MAX_INODES))
    {
        if((UFDTarr[i].pFileTable!=NULL) && (strcmp(UFDTarr[i].pFileTable->inode->FileName,filename)==0) && ((UFDTarr[i].pFileTable->Mode==Permission)))
        {
            (UFDTarr[i].pFileTable->RefCount)++;
            return ERR_FileAlreadyOpened;
        }
        else if((UFDTarr[i].pFileTable==NULL)&&(j==(-1)))
        {
            j=i;
        }
        i++;
    }
    if((i==MAX_INODES)&&(j==(-1)))
    {
        return ERR_IndexOutOfUFDTarr;
    }
    i=j;
    UFDTarr[i].pFileTable = (PFILETABLE) malloc (sizeof(FILETABLE));
    UFDTarr[i].pFileTable->inode=temp;
    UFDTarr[i].pFileTable->Mode=Permission; 
    (UFDTarr[i].pFileTable->RefCount)=1;
    if(UFDTarr[i].pFileTable->inode->Permission==(READ+WRITE))
    {
        UFDTarr[i].pFileTable->ReadOffset=0;
        UFDTarr[i].pFileTable->WriteOffset=UFDTarr[i].pFileTable->inode->ActualFileSize;
    }
    else if(UFDTarr[i].pFileTable->inode->Permission==READ)
    {
        UFDTarr[i].pFileTable->ReadOffset=0;
    }
    else if(UFDTarr[i].pFileTable->inode->Permission==WRITE)
    {
        UFDTarr[i].pFileTable->WriteOffset=UFDTarr[i].pFileTable->inode->ActualFileSize;
    }
    ((UFDTarr[i].pFileTable->inode->Count)++);
    return i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: WriteFile
//	Description: Accepts filename, file decriptor & data, if file is available then write data to the file buffer and returns the index of file descriptor otherwise returns Error values.
//	Input:  int[IN],char[IN-OUT],char[IN-OUT]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WriteFile(int fd,char *filename,char *data)
{
    int iRet=0;
    if((UFDTarr[fd].pFileTable==NULL)||(filename==NULL)||(data==NULL))
    {
        return ERR_IncorrectParameters;
    }
    if(((UFDTarr[fd].pFileTable->Mode!=WRITE)&&(UFDTarr[fd].pFileTable->Mode!=READ+WRITE))||((UFDTarr[fd].pFileTable->inode->Permission!=WRITE)&&(UFDTarr[fd].pFileTable->inode->Permission!=READ+WRITE)))
    {
        return ERR_AccessDenied;
    }
    if(UFDTarr[fd].pFileTable->WriteOffset==(MAX_FILE_SiZE-1))
    {
        return ERR_NoMemory;
    }
    if(UFDTarr[fd].pFileTable->inode->FileType!=REGULAR)
    {
        return ERR_InvalidFileType;
    }
    iRet=StrCpy(&(UFDTarr[fd].pFileTable->WriteOffset),data,(UFDTarr[fd].pFileTable->inode->Buffer));
    if(iRet==ERR_IncorrectParameters)
    {
        return ERR_IncorrectParameters;
    }
    UFDTarr[fd].pFileTable->inode->ActualFileSize+=iRet;
    return iRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: TruncateFile
//	Description: Accepts name of a file, removes all the data from file and returns its file descriptor if file is opened in file table otherwise returns the error values.
//	Input:  char[IN-OUT]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TruncateFile(char *filename)
{
    int i=0,j=0;
    i=GetFdByName(filename);
    if(i==ERR_IncorrectParameters)
    {
        return ERR_IncorrectParameters;
    }
    if(i==ERR_IndexOutOfUFDTarr)
    {
        return ERR_IncorrectParameters;
    }
    memset(UFDTarr[i].pFileTable->inode->Buffer,0,MAX_FILE_SiZE);
    UFDTarr[i].pFileTable->inode->ActualFileSize=0;
    UFDTarr[i].pFileTable->ReadOffset=UFDTarr[i].pFileTable->WriteOffset=0;
    while(j<MAX_INODES)
    {
        if(UFDTarr[j].pFileTable!=NULL)
        {
            if(strcmp(UFDTarr[j].pFileTable->inode->FileName,filename)==0)
            {
                UFDTarr[j].pFileTable->ReadOffset=UFDTarr[j].pFileTable->WriteOffset=0;
            }
        }
        j++;
    }
    return i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: ReadFile
//	Description: Accepts file descriptor and number of byte to read, and returns number of bytes readed if file is opened in file table otherwise returns the error values.
//	Input: int[IN],char[IN-OUT],int[IN]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ReadFile(int fd,char *arr,int iSize)
{
    int ReadSize=0;
    if((fd<0)||(fd>=MAX_INODES)||(UFDTarr[fd].pFileTable==NULL)||(iSize<=0)||(iSize>MAX_FILE_SiZE))
    {
        return ERR_IncorrectParameters;
    }
    if(((UFDTarr[fd].pFileTable->Mode!=READ)&&(UFDTarr[fd].pFileTable->Mode!=READ+WRITE))||((UFDTarr[fd].pFileTable->inode->Permission!=READ)&&(UFDTarr[fd].pFileTable->inode->Permission!=READ+WRITE)))
    {
        return ERR_AccessDenied;
    }
    if(UFDTarr[fd].pFileTable->ReadOffset==UFDTarr[fd].pFileTable->inode->ActualFileSize)
    {
        return ERR_NoDataFurther;
    }
    if(UFDTarr[fd].pFileTable->inode->FileType!=REGULAR)
    {
        return ERR_InvalidFileType;
    }
    ReadSize = (UFDTarr[fd].pFileTable->inode->ActualFileSize) - (UFDTarr[fd].pFileTable->ReadOffset);
    if(ReadSize<iSize)
    {
        strncpy(arr,UFDTarr[fd].pFileTable->inode->Buffer + UFDTarr[fd].pFileTable->ReadOffset,ReadSize);
        UFDTarr[fd].pFileTable->ReadOffset += ReadSize;
    }
    else
    {
        strncpy(arr,UFDTarr[fd].pFileTable->inode->Buffer + UFDTarr[fd].pFileTable->ReadOffset,iSize);
        UFDTarr[fd].pFileTable->ReadOffset += iSize;
    }
    return iSize;
}