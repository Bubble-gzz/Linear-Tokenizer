#include <stdio.h>
#include <string.h>

const int N=200005;
const int maxL=100000;
const int root=1;

int trie[N][26];
int jump[N],isEnd[N],dep[N];
int tot;
int dn,dEnd[N];
FILE *vocab_file,*text_file,*result_file;

void build(char *buff)
{
    int now=root,q,child,wordType,begin;
    strlwr(buff);
    if (buff[0]=='#') begin=2,wordType=1;
    else begin=0,wordType=2;
    for (int i=begin;i<strlen(buff);i++)
    {
        if (buff[i]<'a' || buff[i]>'z') continue;
        child=buff[i]-'a';
        if (!trie[now][child])
        {
            trie[now][child]=++tot;
            dep[tot]=dep[now]+1;
            q=jump[now];
            while (!trie[q][child] && q!=root) q=jump[q]; 
            if (trie[q][child]) jump[tot]=trie[q][child];
            else jump[tot]=root;
            now=tot;
        }
        else now=trie[now][child];
    }
    isEnd[now]=wordType;
}
void tokenize(char* begin,char* end)
{
    int now=root,child,fail=0,maxMatch=root,cut,redundent;
    dEnd[dn=0]=-1;
    for (char* i=begin;i!=end;i++)
    {
        child=(*i)-'a';
        cut=0;
        if (trie[now][child])
        {
            now=trie[now][child];
            if (isEnd[now]==1 || (isEnd[now]==2 && dn==0 && i+1==end)) maxMatch=now;
            if (i+1==end) cut=2;
        } else cut=1;
        if (cut) {
            if (maxMatch==root) {fail=1; break; }
            dn++; dEnd[dn]=dEnd[dn-1]+dep[maxMatch];
            redundent=dep[now]-dep[maxMatch];
            if (cut==2) {
                i=i-redundent;
                if (begin+dEnd[dn]+1==end) break;
            }
            else i=i-redundent-1; // i-redundent is wrong
            now=root; maxMatch=root;
        }
    }
    if (fail) fputs("unk\n",result_file);
    else {
        for (int i=1;i<=dn;i++)
        {
            for (int j=dEnd[i-1]+1;j<=dEnd[i];j++) fputc(*(begin+j),result_file);
            fprintf(result_file,"\n");
        }
    }
}
int main()
{
    char buff[maxL];
    jump[1]=1; tot=1;

    vocab_file=fopen("vocab.in","r");
    text_file=fopen("text.in","r");
    result_file=fopen("result.out","w");

    while (fgets(buff,maxL,vocab_file)!=NULL)
        build(buff);
    while (fgets(buff,maxL,text_file)!=NULL)
    {
        int i=0,j;
        strlwr(buff);
        while (buff[i]<'a' || buff[i]>'z') i++;
        while (i<strlen(buff))
        {
            for (j=i+1;buff[j]>='a' && buff[j]<='z';j++);
            tokenize(buff+i,buff+j);
            i=j+1;
            while (i<strlen(buff) && (buff[i]<'a' || buff[i]>'z')) i++;
        }
    }
    fclose(vocab_file);
    fclose(text_file);
    fclose(result_file);
    return 0;
}
// maxMatch brute version