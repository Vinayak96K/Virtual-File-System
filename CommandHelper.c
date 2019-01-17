#include"MyHeader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: ls_File
//	Description: list details of all the files.
//	Input:  -
//	Output: -
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ls_File()
{
    PINODE temp=head;
    if(sObj.Free_Blocks==MAX_INODES)
    {
        printf("There are no files!\n");
        return;
    }
    printf("File Name\tInode Number\tRef Count\tFile Mode\tFile Size\tFile Actual size\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    while(temp!=NULL)
    {
        if(temp->FileType!=0)
        {
            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->iNodeNumber,temp->Count,temp->Permission,temp->FileSize,temp->ActualFileSize);
        }
        temp=temp->Next;

    }
    printf("--------------------------------------------------------------------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: DisplayHelp
//	Description: Displays information of all commands.
//	Input:  -
//	Output: -
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayHelp()
{
    printf("ls\t\t:-Displays list of all files.\n");
    printf("clear\t\t:-Clears terminal.\n");
    printf("create\t\t:-Create a file with specified permission.\n");
    printf("open\t\t:-Opens a file with specified permission.\n");
    printf("close\t\t:-Close a file.\n");
    printf("closeall\t:-Close all the files.\n");
    printf("write\t\t:-Write into a file if file is available.\n");
    printf("read\t\t:-Read from a file if file is available.\n");
    printf("rm\t\t:-Remove/delete a file.\n");
    printf("man\t\t:-Displays information and usage of commands.\n");
    printf("stat\t\t:-Display information of a file from iNode table.\n");
    printf("fstat\t\t:-Display information of a file from iNode table through File table if the file is open.\n");
    printf("truncate\t:-Removes all data from the file.\n");
    printf("exit\t\t:-Close application.\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: CloseAllFiles
//	Description: Close all the files entries from FileTable.
//	Input:  -
//	Output: -
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseAllFiles()
{
    int i=0;
    while(i<MAX_INODES)
    {
        if(UFDTarr[i].pFileTable!=NULL)
        {
            UFDTarr[i].pFileTable->WriteOffset=0;
            UFDTarr[i].pFileTable->ReadOffset=0;
            (UFDTarr[i].pFileTable->RefCount)--;
            if((UFDTarr[i].pFileTable->RefCount)<=0)
            {
                (UFDTarr[i].pFileTable->inode->Count)--;
                free(UFDTarr[i].pFileTable);
                UFDTarr[i].pFileTable=NULL;
            }    
        }
        i++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: ls_OpenFile
//	Description: Displays list of all the files entries from FileTable.
//	Input:  -
//	Output: -
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ls_OpenFile()
{
    int i=0,j=0;
    if(sObj.Free_Blocks==MAX_INODES)
    {
        printf("Error: No files created!\n");
        return;
    }
    printf("File Descriptor\tFilename\tInode Number\tFile mode\tWriteOffeset\tReadOffset\tRef Count\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    while(i<MAX_INODES)
    {
        if(UFDTarr[i].pFileTable!=NULL)
        {
            printf("%d\t\t%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",i,
            UFDTarr[i].pFileTable->inode->FileName,
            UFDTarr[i].pFileTable->inode->iNodeNumber,
            UFDTarr[i].pFileTable->Mode,
            UFDTarr[i].pFileTable->WriteOffset,
            UFDTarr[i].pFileTable->ReadOffset,
            UFDTarr[i].pFileTable->RefCount);
            j++;
        }
        i++;
    }
    if((i==MAX_INODES)&&(j==0))
    {
        printf("No Open files in descriptor!\n");
    }
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: Man
//	Description: Displays information of a specified command.
//	Input:  char[IN-OUT]
//	Output: -
//	Author: Vinayak Mahendra Patil
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Man(char *cmd)
{
    if(strcasecmp(cmd,"ls")==0)
    {
        printf("Lists all the files created.\n");
        printf("Usage: ls\n");
        printf("-o: if specified, then it lists all the open files.\n");
    }
    else if(strcasecmp(cmd,"create")==0)
    {
        printf("Creates a file with specified name and mode.\n");
        printf("Usage: create [File_Name] [Mode(1: READ & 2: WRITE)]\n");
    }
    else if(strcasecmp(cmd,"close")==0)
    {
        printf("Closes a file with specified name.\n");
        printf("Usage: close [File_Name]\n");
    }
    else if(strcasecmp(cmd,"closeall")==0)
    {
        printf("Closes all files.\n");
        printf("Usage: closeall\n");
    }
    else if(strcasecmp(cmd,"open")==0)
    {
        printf("Opens a file with specified name and mode.\n");
        printf("Usage: open [File_Name] [Mode(1: READ & 2: WRITE)]\n");
    }
    else if(strcasecmp(cmd,"write")==0)
    {
        printf("Writes to a file with specified name if the file is opened in the descriptor table.\n");
        printf("Usage: write [File_Name]\n");
    }
    else if(strcasecmp(cmd,"read")==0)
    {
        printf("Reads number of bytes from a file.\n");
        printf("Usage: read [File_Name] [Number_Of_Bytes]\n");
    }
    else if(strcasecmp(cmd,"stat")==0)
    {
        printf("Displays information of a file with specified name.\n");
        printf("Usage: stat [File_Name]\n");
    }
    else if(strcasecmp(cmd,"fstat")==0)
    {
        printf("Displays information of a file with specified file descriptor.\n");
        printf("Usage: fstat [File_Descriptor]\n");
    }
    else if(strcasecmp(cmd,"[truncate]")==0)
    {
        printf("Removes data from a file with specified name.\n");
        printf("Usage: truncate [File_Name]\n");
    }
    else if(strcasecmp(cmd,"lseek")==0)
    {
        printf("Changes the offset a file with specified name and below parameters.\n");
        printf("Usage: lseek [File_Name] [Change_In_Offset] [Start_Point]\n");
    }
    else if(strcasecmp(cmd,"rm")==0)
    {
        printf("Used to delete file of a specified name.\n");
        printf("Usage: rm File_Name");
    }
    else
    {
        printf("Error: Command not found!\n");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: stat
//	Description: Accept filename from user and if file is present then returns the inode number of displayed information of a specified file name from inode otherwise retunrs error values.
//	Input:  char[IN-OUT]
//	Output: int
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int stat(char *filename)
{
    int i=0;
    PINODE temp=GetINodeByName(filename);
    if(temp==NULL)
    {
        return ERR_IncorrectParameters;
    }
    printf("----------------------Statistical Information about the file----------------------\n");
    printf("File name: %s\n",temp->FileName);
    printf("Inode number: %d\n",temp->iNodeNumber);
    printf("Total file size: %d\n",temp->FileSize);
    printf("Used file size: %d\n",temp->ActualFileSize);
    printf("File permission:");
    switch(temp->Permission)
    {
        case READ:
            printf("Read Only\n");
            break;
        case WRITE:
            printf("Write Only\n");
            break;       
        case (READ+WRITE):
            printf("Read & Write\n");
            break;     
        default:
            break;
    }
    printf("File type:");
    switch(temp->FileType)
    {
        case REGULAR:
            printf("Regular file.\n");
            break;
        case SPECIAL:
            printf("Special file.\n");
            break;   
        default:
            break;
    }
    printf("Link count: %d\n",temp->LinkCount);
    printf("Reference count: %d\n",temp->Count);
    printf("----------------------------------------------------------------------------------\n");
    return temp->iNodeNumber;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: fstat
//	Description: Accept file descriptor from user and if file is open in file table then returns the inode number of displayed information of a specified file name from inode otherwise retunrs error values.
//	Input:  int[IN]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int fstat(int fd)
{
    PINODE temp=NULL;
    if((fd<0)||(fd>=MAX_INODES)||(UFDTarr[fd].pFileTable==NULL))
    {
        return ERR_IncorrectParameters;
    }
    temp=UFDTarr[fd].pFileTable->inode;
    if(temp==NULL)
    {
        return ERR_IncorrectParameters;
    }
    printf("----------------------Statistical Information about the file----------------------\n");
    printf("File name: %s\n",temp->FileName);
    printf("Inode number: %d\n",temp->iNodeNumber);
    printf("Total file size: %d\n",temp->FileSize);
    printf("Used file size: %d\n",temp->ActualFileSize);
    printf("File permission:");
    switch(temp->Permission)
    {
        case READ:
            printf("Read Only\n");
            break;
        case WRITE:
            printf("Write Only\n");
            break;       
        case (READ+WRITE):
            printf("Read & Write\n");
            break;     
        default:
            break;
    }
    printf("File open mode:");
    switch(UFDTarr[fd].pFileTable->Mode)
    {
        case READ:
            printf("Read Only\n");
            break;
        case WRITE:
            printf("Write Only\n");
            break;       
        case (READ+WRITE):
            printf("Read & Write\n");
            break;     
        default:
            break;
    }
    printf("File type:");
    switch(UFDTarr[fd].pFileTable->inode->FileType)
    {
        case REGULAR:
            printf("Regular file.\n");
            break;
        case SPECIAL:
            printf("Special file.\n");
            break;   
        default:
            break;
    }
        
    printf("File write offset: %d\n",UFDTarr[fd].pFileTable->WriteOffset);
    printf("File read offset: %d\n",UFDTarr[fd].pFileTable->ReadOffset);
    printf("Link count: %d\n",temp->LinkCount);
    printf("Reference count: %d\n",temp->Count);
    printf("----------------------------------------------------------------------------------\n");
    return temp->iNodeNumber;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: rmFile
//	Description: Accept filename from user, if file is present then the file of a specified file name from inode otherwise retunrs error values.
//	Input:  char[IN-OUT]
//	Output: int
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rmFile(char *filename)
{
    PINODE temp=head;
    if((temp==NULL)||(sObj.Free_Blocks==MAX_INODES)||(filename==NULL))
    {
        return ERR_IncorrectParameters;
    }
    while(temp!=NULL)
    {
        if(strcmp(temp->FileName,filename)==0)
        {
            break;
        }
        temp = temp->Next;
    }
    if(temp==NULL)
    {
        return ERR_NoInodes;
    }
    (temp->LinkCount)--;
    if((temp->Count>0)||(temp->LinkCount>0))
    {
        return ERR_FileAreadyExits;
    }
    free(temp->Buffer);
    memset(temp->FileName,0,MAX_FILE_NAME_SIZE);
    temp->ActualFileSize=0;
    temp->FileType=0;
    temp->FileSize=0;
    (sObj.Free_Blocks)++;
    return temp->iNodeNumber;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: GetBackup
//	Description: Creates a backup of all files from inode list abd returns number of successfull backup files otherwise returns error values.
//	Input:  -
//	Output: int
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetBackup()
{
    int file;
    int i=0,j=0,iRet=0;
    const char *ch="123";
    char c;
    PINODE temp=head;

    if(sObj.Free_Blocks==MAX_INODES)
    {
        return ERR_FileIsNotOpened;
    }
    file=open(BACKUPFILE,O_RDWR|O_APPEND|O_CREAT);
    if(file<=(-1))
    {
        return ERR_UnableLocateFile;
    }
    while((temp!=NULL))
    {
        if((temp->FileType==REGULAR)&&(temp->Buffer!=NULL)&&(temp->bckup!=TRUE))
        {
            if(write(file,temp->FileName,MAX_FILE_NAME_SIZE)<0)
            {
                iRet = ERR_FailedToWriteBackupData;
                break;
            }
            c=ch[(temp->Permission-1)];
            if(write(file,&c,1)<0)
            {
                iRet = ERR_FailedToWriteBackupData;
                break;
            }
            if(write(file,temp->Buffer,MAX_FILE_SiZE)<0)
            {
                iRet = ERR_FailedToWriteBackupData;
                break;
            }
            temp->bckup=TRUE;
            j++;
            iRet=j;
        }
        temp=temp->Next;
    }
    close(file);
    return iRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: Restore
//	Description: Restores all backup files in the VFS.
//	Input:  -
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Restore()
{
    int iRet,iCnt=0,file;
    char FileName[MAX_FILE_NAME_SIZE];
    char FileData[MAX_FILE_SiZE];
    char Permission;
    char *StrData=(char *)malloc(MAX_FILE_NAME_SIZE+1+MAX_FILE_SiZE);
    file=open(BACKUPFILE,O_RDONLY);
    if(file<=(-1))
    {
        return ERR_UnableLocateFile;
    }
    while(read(file,StrData,MAX_FILE_NAME_SIZE+1+MAX_FILE_SiZE))
    {
        //printf("%s\n",StrData);
        strncpy(FileName,StrData,MAX_FILE_NAME_SIZE);
        //printf("%s\n",FileName);
        Permission=StrData[MAX_FILE_NAME_SIZE];
        //printf("%c\n",Permission);
        strncpy(FileData,StrData+MAX_FILE_NAME_SIZE+1,MAX_FILE_SiZE);
        //printf("%s\n",FileData);
        iRet=CreateFile(FileName,atoi(&Permission));
        if(iRet >= 0)
        {
            WriteFile(iRet,FileName,FileData);
            CloseFileByName(FileName);
            iCnt++;
        }
        else if(iRet==ERR_IncorrectParameters)
        {
            printf("Error: Incorrect input!\n");
        }
        else if(iRet==ERR_FileAreadyExits)
        {
            printf("Error: File already exists!\n");
        }
        else if(iRet==ERR_NoInodes)
        {
            printf("Error: No iNodes available!\n");
            break;
        }
        else if(iRet==ERR_MemAllocationFailure)
        {
        printf("Error: Failed to allocate memory!\n");
        break;
        }
        iRet=iCnt;
    }
    if(iCnt>0)
    {
        remove(BACKUPFILE);
    }
    free(StrData);
    return iCnt;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: lSeek
//	Description: repositions  the file offset of the open file description associated with the file descriptor fd to the argument offset according to the directive whence otherwise returns error values.
//	Input:  int[IN,IN,IN]
//	Output: int[OUT]
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int lSeek(int fd,int iSize, int from)
{
    if((fd<0)||(from>2)||(fd>=MAX_INODES)||(UFDTarr[fd].pFileTable==NULL))
    {
        return ERR_IncorrectParameters;
    }

    if((UFDTarr[fd].pFileTable->Mode==READ)||(UFDTarr[fd].pFileTable->Mode==READ+WRITE))
    {
        if(from==CURRENT)
        {
            if((((UFDTarr[fd].pFileTable->ReadOffset)+iSize)>UFDTarr[fd].pFileTable->inode->ActualFileSize)||(((UFDTarr[fd].pFileTable->ReadOffset)+iSize)<0))
            {
                return ERR_IncorrectParameters;
            }
            (UFDTarr[fd].pFileTable->ReadOffset)=(UFDTarr[fd].pFileTable->ReadOffset)+iSize;
        }
        else if(from==START)
        {
            if((iSize)>(UFDTarr[fd].pFileTable->inode->ActualFileSize)||(iSize<0))
            {
                return ERR_IncorrectParameters;
            }
            (UFDTarr[fd].pFileTable->ReadOffset)=iSize;
        }
        else if(from==END)
        {
            if((((UFDTarr[fd].pFileTable->inode->ActualFileSize)+iSize)>MAX_FILE_SiZE)||(((UFDTarr[fd].pFileTable->ReadOffset)+iSize)<0)||(((UFDTarr[fd].pFileTable->ReadOffset)+iSize)>UFDTarr[fd].pFileTable->inode->ActualFileSize))
            {
                return ERR_IncorrectParameters;
            }
            (UFDTarr[fd].pFileTable->ReadOffset)=(UFDTarr[fd].pFileTable->inode->ActualFileSize)+iSize;
        }
    }
    else if(UFDTarr[fd].pFileTable->Mode==WRITE)
    {
        if(from==CURRENT)
        {
            if((((UFDTarr[fd].pFileTable->WriteOffset)+iSize)>MAX_FILE_SiZE)||(((UFDTarr[fd].pFileTable->WriteOffset)+iSize)<0))
            {
                return ERR_IncorrectParameters;
            }
            if(((UFDTarr[fd].pFileTable->WriteOffset)+iSize)>(UFDTarr[fd].pFileTable->inode->ActualFileSize))
            {
                (UFDTarr[fd].pFileTable->inode->ActualFileSize)=((UFDTarr[fd].pFileTable->WriteOffset)+iSize);
            }
            (UFDTarr[fd].pFileTable->WriteOffset)=((UFDTarr[fd].pFileTable->WriteOffset)+iSize);
        }
        else if(from==START)
        {
            if((iSize<0)||(iSize>=MAX_FILE_SiZE))
            {
                return ERR_IncorrectParameters;
            }
            if(iSize>(UFDTarr[fd].pFileTable->inode->ActualFileSize))
            {
                (UFDTarr[fd].pFileTable->inode->ActualFileSize)=iSize;
            }
            (UFDTarr[fd].pFileTable->WriteOffset)=iSize;

        }
        else if(from==END)
        {
            if((((UFDTarr[fd].pFileTable->inode->ActualFileSize)+iSize) > MAX_FILE_SiZE)||(((UFDTarr[fd].pFileTable->WriteOffset)+iSize) <0)||(((UFDTarr[fd].pFileTable->ReadOffset)+iSize)>UFDTarr[fd].pFileTable->inode->ActualFileSize))
            {
                return ERR_IncorrectParameters;
            }
            (UFDTarr[fd].pFileTable->WriteOffset)=(UFDTarr[fd].pFileTable->inode->ActualFileSize)+iSize;

        }
    }
}