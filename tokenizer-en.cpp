#include <stdio.h>
#include <string.h>
#include <queue>

const int N=200005;
const int maxL=100000;
const int wordLen=100;
const int root=1;

int dn,dEnd[N];
FILE *vocab_file,*text_file,*result_file;

int tot;
using namespace std;

struct trieNode{
    char ch;
    int jump,father,isEnd,dep;
    int dead,loc,maxMatch;
    int children[26];
    vector<int> childList;
    vector<int> fpop;
    int getChild(char ch)
    {
        if (ch<'a' || ch>'z') return 0;
        int id=ch-'a';
        return children[id];
    }
    void addChild(char ch)
    {
        int id=ch-'a';
        children[id]=++tot;
        childList.push_back(tot);
    }
}trie[N];

void build(char *buff)
{
    int now=root,q,child,wordType,begin;
    char ch;
    strlwr(buff);
    if (buff[0]=='#') begin=2,wordType=1;
    else begin=0,wordType=2;
    for (int i=begin;i<strlen(buff);i++)
    {
        if (buff[i]<'a' || buff[i]>'z') continue;
        ch=buff[i];
        child=trie[now].getChild(ch);
        if (!child)
        {
            trie[now].addChild(ch);
            trie[tot].dep=trie[now].dep+1;
            trie[tot].father=now;
            trie[tot].ch=ch;
            now=tot;
        }
        else now=trie[now].getChild(ch);
    }
    trie[now].isEnd=wordType;
}
void tokenize(char* begin,char* end,int mode,trieNode &res)
{
    int now=root,fail=0,maxMatch=root,cut,redundent;
    int cutted=0;
    char ch;
    dEnd[dn=0]=-1;
    *(end-1)='*'; //trunc at the end
    for (char* i=begin;i!=end;i++)
    {
        ch=(*i);
        if (mode==1 && begin+dEnd[dn]+2==end) break;
        if (mode==0 && i+1==end && cutted) break;
        cut=0;
        if (trie[now].getChild(ch))
        {
            now=trie[now].getChild(ch);
            if (trie[now].isEnd==1 || (trie[now].isEnd==2 && dn==0 && i+2==end)) maxMatch=now;
        } else cut=1;
        if (cut) {
            if (mode==1)
            {
                while (!trie[now].getChild(ch))
                {
                    if (trie[now].dead) {
                        if (now==root && ch=='*') fail=2;
                        else fail=1;
                        break;
                    }
                    redundent=0;
                    for (int i=0;i<trie[now].fpop.size();i++) {
                        redundent+=trie[now].fpop[i];
                        dn++; dEnd[dn]=dEnd[dn-1]+trie[now].fpop[i];
                    }
                    redundent=trie[now].dep-redundent;
                    while (trie[now].dep!=redundent) now=trie[now].jump;
                }
                if (fail) break;
                now=trie[now].getChild(ch);
            }
            else
            {
                if (maxMatch==root) {fail=1; break; }
                dn++; dEnd[dn]=dEnd[dn-1]+trie[maxMatch].dep;
                redundent=trie[now].dep-trie[maxMatch].dep;
                i=i-redundent-1; // i-redundent is wrong
                now=root; maxMatch=root;
                cutted=1;
            }
        }
    }
    if (fail==2) fail=0;
    if (mode==1)
    {
        if (fail) fputs("unk\n",result_file);
        else
        {
            for (int i=1;i<=dn;i++)
            {
                for (int j=dEnd[i-1]+1;j<=dEnd[i];j++) fputc(*(begin+j),result_file);
                fprintf(result_file,"\n");
            }
        }
    }
    else
    {
        if (fail) res.dead=1;
        else
        {
            res.dead=0;
            for (int i=1;i<=dn;i++)
                res.fpop.push_back(dEnd[i]-dEnd[i-1]);
        }
    }
}
queue<int> que;
void Initialize()
{
    int now,fa,q,l;
    char ch;
    char buff[wordLen],temp[wordLen];
    que.push(root);
    while (!que.empty())
    {
        now=que.front();
        que.pop();
        for (int i=0;i<trie[now].childList.size();i++)
            que.push(trie[now].childList[i]);
        if (now!=root)
        {
            fa=trie[now].father;
            if (fa==root) trie[now].jump=root;
            else
            {
                ch=trie[now].ch;
                q=trie[fa].jump;
                while (q!=root && !trie[q].getChild(ch)) q=trie[q].jump;
                if (trie[q].getChild(ch)) trie[now].jump=trie[q].getChild(ch);
                else trie[now].jump=root;
            }
            q=now; l=0;
            while (q!=root) temp[l++]=trie[q].ch,q=trie[q].father;
            for (int i=l-1;i>=0;i--) buff[l-i-1]=temp[i];
        //    for (int i=0;i<=l;i++) printf("%c",buff[i]);
        //    printf(" fpop=[");
            
            tokenize(buff,buff+l+1,0,trie[now]);
        //    for (size_t i=0;i<trie[now].fpop.size();i++) printf("%d ,",trie[now].fpop[i]);
        //    printf("]\n");
        //    if (trie[fa].dead) trie[now].dead=1;
        //    else {
        //        q=trie[fa].loc;
        //    }

        }
    }
    //for (int i=1;i<=tot;i++) printf("jump[%d]=%d\n",i,trie[i].jump);
}

int main()
{
    char buff[maxL];
    
    tot=1; 
    trie[root].jump=root; trie[root].dead=1;
    trie[root].loc=root; trie[root].maxMatch=root;

    vocab_file=fopen("vocab.in","r");
    text_file=fopen("text.in","r");
    result_file=fopen("result.out","w");

    while (fgets(buff,maxL,vocab_file)!=NULL)
        build(buff);
    Initialize();

    while (fgets(buff,maxL,text_file)!=NULL)
    {
        int i=0,j;
        strlwr(buff);
        while (buff[i]<'a' || buff[i]>'z') i++;
        while (i<strlen(buff))
        {
            for (j=i+1;buff[j]>='a' && buff[j]<='z';j++);
            tokenize(buff+i,buff+j+1,1,trie[root]);
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