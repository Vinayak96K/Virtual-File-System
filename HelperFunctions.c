#include"MyHeader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: GetINodeByName
//	Description: Accepts filename and returns the address of iNode if the filename exits.
//	Input: char[IN-OUT]
//	Output: PINODE
//	Author: Vinayak Mahendra Patil
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PINODE GetINodeByName(char *FileName)
{
    PINODE temp=head;
    if(FileName==NULL)
    {
        return NULL;
    }
    while((temp!=NULL)&&(strcmp(temp->FileName,FileName)!=0))
    {
        temp=temp->Next;
    }
    return temp;
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: GetFdByName
//	Description: Accepts filename and returns the file descriptor number if the file exits in the filetable otherwhise returns error values.
//	Input: char[IN-OUT]
//	Output: int
//	Author: Vinayak Mahendra Patil
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetFdByName(char *filename)
{
    int i=0;
    if(filename==NULL)
    {
        return ERR_IncorrectParameters;
    }
    while((i<MAX_INODES))
    {
        if((UFDTarr[i].pFileTable!=NULL) && (strcmp(UFDTarr[i].pFileTable->inode->FileName,filename)==0) && ((UFDTarr[i].pFileTable->RefCount)>0))
        {
            break;
        }
        i++;
    }
    if(i==MAX_INODES)
    {
        return ERR_IndexOutOfUFDTarr;
    }
    else
    {
        return i;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: GetFdByMode
//	Description: Accepts filename, mode and returns the file descriptor number if the file exits in the filetable otherwhise returns error values.
//	Input: char[IN-OUT]
//	Output: int
//	Author: Vinayak Mahendra Patil
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetFdByMode(char *filename,int mode)
{
    int i=0;
    if((filename==NULL)||(mode>3)||(mode==0))
    {
        return ERR_IncorrectParameters;
    }
    while((i<MAX_INODES))
    {
        if((UFDTarr[i].pFileTable!=NULL) && (strcmp(UFDTarr[i].pFileTable->inode->FileName,filename)==0)&&((UFDTarr[i].pFileTable->Mode==mode)||(UFDTarr[i].pFileTable->Mode==READ+WRITE))&&(UFDTarr[i].pFileTable->RefCount!=0))
        {
            break;
        }
        i++;
    }
    if(i==MAX_INODES)
    {
        return ERR_IndexOutOfUFDTarr;
    }
    else
    {
        return i;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Function Name: StrCpy
//	Description: Accpets base address of two strings as src and dest along with an index of dest and returns the size of elements copied from src to dest.
//	Input: int[IN-OUT], char[IN-OUT], char[IN-OUT]
//	Output: int[OUT]
//	Author: Vinayak Maehndra Patil
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int StrCpy(int *i,char *src,char *dest)
{
    int j=0,iCnt=0;
    if((i==NULL)||(src==NULL)||(dest==NULL))
    {
        return ERR_IncorrectParameters;
    }
    while((src[j]!='\0')&&((*i)<(MAX_FILE_SiZE-1)))
    {
        //printf("%d\t%c\t%d\n",j,src[j],(*i));
        dest[(*i)]=src[j];
        (*i)++;
        j++;
    }

    return j;
}