#include <windows.h>
#include "resource.h"

typedef struct _HASHMAP
{
   UINT key;
   UCHAR value;
   struct _HASHMAP *next;
}HASHMAP;

HASHMAP  *hashmap=NULL;

void DeleteHashMap()
{
  HASHMAP *p=hashmap;
  HASHMAP *prev;
  if(p==NULL)
   return;
  while(p!=NULL)
  {
      prev=p;
      p=p->next; 
      free(prev);              
  }
  hashmap=NULL;
}

UCHAR GetHashMap(UINT key)
{  
  HASHMAP *p=hashmap;
  while(p!=NULL)
  {
      if(p->key==key){  return p->value; } 
      p=p->next;
  }
 return 0; //this shouldn't happen
}

UCHAR SetHashMap(UINT key,UCHAR value,UCHAR overwrite)
{  
  HASHMAP *p=hashmap;
  HASHMAP *prev;
  if(hashmap==NULL)
  {
     hashmap=(HASHMAP*)malloc(sizeof(HASHMAP));                     
     hashmap->key=key;
     hashmap->value=value;
     hashmap->next=NULL;                      
     return YES;                        
  }
  while(p!=NULL)
  {
      if(p->key==key)
      { 
          if(overwrite==YES)
          {
              p->value=value;
          }
          return NO; 
      } 
      prev=p;
      p=p->next;               
  }
  p=(HASHMAP*)malloc(sizeof(HASHMAP));
  p->key=key;
  p->value=value;
  p->next=NULL;
  prev->next=p;
  return YES;
}

