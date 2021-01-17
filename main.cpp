//
//  main.cpp
//
//  Created by TripleTheGreatDali  on 2020/12/28.
//
#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct{
    int weight;
    char Data;                                     //Characters to hold nodes
    int Parent,Lchild,Rchild;
}HTNode,*HuffmanTree;

typedef char** HuffmanCode;
void HuffmanCoding(HuffmanTree &,char *,int *,int);
void select(HuffmanTree HT,int j,int *s1,int *s2);   //Select two nodes with zero parents and minimum weight
void Initialization();                                            // Initialize heffman tree
void Coding();                                                         //Huffman code
void Decoding();                                                     //Huffman decoding
void find(HuffmanTree &HT,char *code,char *text,int i,int m);
HuffmanTree HT;
int n=0;                                                   //Number of leaf nodes of heffman tree

//------------------------------------------------------------------------------
                                                                        //Main function

int    main()
{
    char T;
    while (1)
    {
        printf("\n");
        printf("      **============================================================**\n");
        printf("    ****=                                                          =****\n");
        printf("  ******=             Design of Huffman Encoder/Decoder            =******\n");
        printf("    ****=                                                          =****\n");
        printf("      **============================================================**\n");
        
        printf("        ============================================================\n");
        printf("        =                  Huffman Encoder/Decoder                 =\n");
        printf("        ============================================================\n");
        printf("        =   Initialization     -----------  I                      =\n");
        printf("        =   Encoding           -----------  C                      =\n");
        printf("        =   Decoding           -----------  D                      =\n");
        printf("        =   Exit               -----------  Q                      =\n");
        printf("        ============================================================\n");
        printf("        =   Please input ( I C D Q) :                              =\n");
        printf("        ------------------------------------------------------------\n");
        scanf("%c",&T);
        switch(T)
        {
            case 'I':
                Initialization();
                break ;
            case 'C':
                Coding();
                break ;
            case 'D':
                Decoding();
                break ;
            case 'Q':
                exit(1);
            default        :
                printf(    "\t\t\t\t Input error \n" );
        }
        getchar();
    }
    return    0;
}

//------------------------------------------------------------------------------
                   //Initialization function, input n characters and their corresponding weights, and establish heffman tree according to the weights

void Initialization()
{
    
    FILE *fp;
    int    i,w[52];                                                //Weight of stored characters
    char character[52];                                             // Store n characters
    printf("\t\t\t\t Please enter the number of characters:");
    scanf("%d",&n);
    printf("\t\t\t\t Please input%d Characters and weight:\n\t\t\t\t" ,n);
    for(i=0;i<n;i++)
    {
        char  b=getchar();
        scanf("%c" ,&character[i]);
        scanf("%d",&w[i]);                                 // Input characters and corresponding weights
    }
    HuffmanCoding(HT,character,w,n);                                 // Establish heffman tree
    if ((fp=fopen("/Users/ahmedmdfoysal/Desktop/Huffman Testcpp01/Huffman Testcpp01/hfmTree.txt","w"))==NULL)
        printf("\t\t\t\thfmTree.txt    Open failure \n"  );
    for(i=1;i<=2*n-1;i++)
    {
        if(fwrite(&(HT[i].Data),sizeof (char),1,fp)!=1)
            printf(    "\t\t\t\t fail to write to file  \n");
        if(fwrite(&(HT[i].weight),sizeof(int),1,fp)!=1)
            printf(    "\t\t\t\t fail to write to file  \n");
        if(fwrite(&(HT[i].Parent),sizeof(int),1,fp)!=1)
            printf(    "\t\t\t\t fail to write to file  \n");
        if(fwrite(&(HT[i].Lchild),sizeof(int),1,fp)!=1)
            printf(    "\t\t\t\t fail to write to file  \n");
        if(fwrite(&(HT[i].Rchild),sizeof(int),1,fp)!=1)
            printf(    "\t\t\t\t fail to write to file  \n");
    }
    printf("\t\t\t\t Huffman tree has been established and stored in hfmTree.txt\n");
    fclose(fp);

}

//------------------------------------------------------------------------------
                                                            //An algorithm for constructing heffman tree

void HuffmanCoding(HuffmanTree &HT,char *character, int *w, int n)
{
    int    m,i,s1,s2;
    HuffmanTree p;
    if(n<=1)
        return  ;
    m=2*n-1;
    HT=(HuffmanTree)malloc((m+1)* sizeof(HTNode));
    for(p=HT+1,i=1;i<=n;++i,++p,++character,++w)                       // Initial value
    {
        p->Data=*character;
        p->weight=*w;
        p->Parent=0;
        p->Lchild=0;
        p->Rchild=0;
    }
    for(;i<=m;++i,++p)                                   // Assign an initial value of 0 to subsequent nodes
    {
        p->Data=0;
        p->weight=0;
        p->Parent=0;
        p->Lchild=0;
        p->Rchild=0;
    }
    for(i=n+1;i<=m;++i)                                           // Generate new node
    {
        select(HT,i-1,&s1,&s2);
        HT[s1].Parent=i;HT[s2].Parent=i;
        HT[i].Lchild=s1;HT[i].Rchild=s2;
        HT[i].weight=HT[s1].weight+HT[s2].weight;
    }
}

//------------------------------------------------------------------------------
                                              //Select two nodes with zero parents and minimum weight

void select(HuffmanTree HT,int j, int *s1, int *s2)
{
    int    i;
    for(i=1;i<=j;i++)
        if (HT[i].Parent==0)
        {
            *s1=i;
            break;
        }
    for(;i<=j;i++)
        if((HT[i].Parent==0)&&(HT[i].weight<HT[*s1].weight))
        *s1=i;                                    // Then s1 is the sequence number of the smallest node
    HT[*s1].Parent=1;    // Assign a value of 1 to HT[*s1].Parent in advance, so as to avoid being affected when finding summary points and judging conditions
        
        
    for(i=1;i<=j;i++)
        if(HT[i].Parent==0)
        {
            *s2=i;
            break;
        }                        // There's a cycle for those who haven't been visited, which is the starting point for comparison
    for(;i<=j;i++)
        if((HT[i].Parent==0)&&(i!=*s1)&&(HT[i].weight<HT[*s2].weight))
        *s2=i;                                      // Find the node with less weight
}

//------------------------------------------------------------------------------
//Carry out Huffman coding

void Coding()
{
    FILE *fp,*fw;
    int    i,f,c,r,start;
    char *cd;
    char temp;
    HuffmanCode HC;
    {
        HC=(HuffmanCode)malloc((n+1)* sizeof (char*));
        cd=(char *)malloc(n*sizeof(char));
        cd[n-1]='\0';
        for(i=1;i<=n;++i)
        {
            start=n-1;
            for(c=i,f=HT[i].Parent;f!=0;c=f,f=HT[f].Parent)
                if (HT[f].Lchild==c)
                    cd[--start]='0';
                else
                    cd[--start]='1';
            HC[i]=(char *)malloc((n-start)* sizeof(char));
            strcpy(HC[i],&cd[start]);
        }
        free(cd);
    }
        if((fp=fopen("/Users/ahmedmdfoysal/Desktop/Huffman Testcpp01/Huffman Testcpp01/ToBeTran.txt","rb"))==NULL)
            printf("\t\t\t\tToBeTran.txt Open failure \n");
        if((fw=fopen("CodeFile.txt","wb+"))==NULL)      // Create new file for read / write
            printf("\t\t\t\tCodeFile.txt Open failure \n");
        fscanf(fp,"%c",&temp);    // Read the first character into the temp variable from the file referred to by fp
        while(!feof(fp))
        {
            for(i=1;i<=n;i++)
                if (HT[i].Data==temp)
                    break ;                           // Jump out of loop if same as node 1
            for(r=0;HC[i][r]!='\0';r++)    // Find the location of the character in the Huffman tree
                fputc(HC[i][r],fw);    // Save the code corresponding to the character into the file, and write HC[i][r] to the file indicated by fw
            fscanf(fp,"%c",&temp);                     // Read next character from file
        }
        fclose(fw);
        fclose(fp);
        printf("\n\t\t\t\thfmTree.txt    Encoded successfully, saved in CodeFile.txt\n"    );
}

//------------------------------------------------------------------------------
//Perform heffman decoding

void Decoding()
{
    FILE *fp,*fw;
    int    m,i;
    char *code,*text,*p;
    if((fp=fopen("/Users/ahmedmdfoysal/Desktop/Huffman Testcpp01/Huffman Testcpp01/CodeFile.txt","rb"))==NULL)                  // Open file for read-only
        printf("\t\t\t\tCodeFile.txt    Open failure \n");
    if((fw=fopen("Textfile.txt","wb+"))==NULL) // Create new file read / write, if created, the content will be cleared
        printf(    "\t\t\t\tTextfile.txt    Open failure \n");
    code=(char*)malloc(sizeof(char));
    fscanf(fp,"%c",code);                             // Read in a 0 or 1 character from a file
    for(i=1;!feof(fp);i++)
    {
        code=(char *)realloc(code,(i+1)*sizeof(char));
        fscanf(fp,"%c",&code[i]);                 // Read next 0 or 1 character from file
    }
    //    So far, 0 and 1 in CodeFile.txt have been read in and stored in code array
    text=(char *)malloc(100*sizeof(char));
    p=text;
    m=2*n-1;
    if(*code=='0')
        find(HT,code,text,HT[m].Lchild,m);                   // Find from the left subtree of the root node
    else
        find(HT,code,text,HT[m].Rchild,m);                   // Find from the right subtree of the root node
        
    for(i=0;p[i]!='\0';i++)               // Store the decoded characters in Textfile.txt
        fputc(p[i],fw);                               // Write p[i] to the file indicated by fw
    fclose(fp);
    fclose(fw);
    printf(    "\n\t\t\t\tCodeFile.txt    Decoding successful, stored in Textfile.txt\n"    );
}

//------------------------------------------------------------------------------
//A recursive algorithm for finding the corresponding leaf node according to 01 string in decoding

void find(HuffmanTree &HT,char *code,char *text,int i, int m)
{
    if(*code!='\0')                                             // If decoding is not finished
    {
        code++;
        if(HT[i].Lchild==0&&HT[i].Rchild==0)                   // If a leaf node is found
        {
            *text=HT[i].Data;                           // Save leaf node characters in text
            text++;
            if((*code=='0'))
                find(HT,code,text,HT[m].Lchild,m);       // Continue to find from the left subtree of the root node
            else
                find(HT,code,text,HT[m].Rchild,m);       // Continue to search from the right subtree of the root node
        }
        else
             if(*code=='0')
                find(HT,code,text,HT[i].Lchild,m);              // Find from the left subtree of this node
            else
                find(HT,code,text,HT[i].Rchild,m);             // Find from the right subtree of this node
    }
    else
        *text='\0';
}
