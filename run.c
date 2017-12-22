#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include "run.h"
#include "util.h"

void *base = 0;
p_meta find_meta(p_meta *last, size_t size) {
  p_meta index = base;
  p_meta result = base;
  switch(fit_flag){
    case FIRST_FIT:
    {
      //FIRST FIT CODE
    }
    break;

    case BEST_FIT:
    {
      //BEST_FIT CODE
    }
    break;

    case WORST_FIT:
    {
      //WORST_FIT CODE
    }
    break;

  }
  return result;
}

void *m_malloc(size_t size) {

p_meta new;
new = sbrk(0);
if(base == 0)
{
 int num = size/4;
 if(size%4 != 0)
  num = num+1;
 size = num*4;
 new = sbrk((size+META_SIZE));
 new->size = size;
 new->prev = NULL;
 new->next = NULL;
 new->free = 0;
 base = new;

}
else
{
  int num = size/4;
  if(size - num*4 != 0)
    num = num+1;
  size = num*4;

  p_meta i;
  i = base; 

  while(1){
  if((i->free == 1) && (i->size >= size))
    break;

  if(i->next != NULL)
    i= i->next;
  else 
   break; }
 
  if(i->free == 1)
  {
    if(i->size > size + META_SIZE) 
    {
      p_meta divide = i+META_SIZE+size;
      divide -> prev = i;
      if(i->next != NULL)
       divide->next = i->next;     
      i->next = divide;
      divide->size = i->size-size-META_SIZE;
      i->size = size;
      i->free  =0;
      divide->free = 1;
    }
   return i->data;
  } // there is free block
  else
  {
    new = sbrk(size + META_SIZE);
    new->prev = i;
    new->next = NULL;
    new -> size = size;
    new  -> free = 0;
    i ->next = new;
  }
}
return new->data;
}

void m_free(void *ptr) {
  
  if(ptr == NULL)
    return ;
  p_meta find;

  ptr = ptr-META_SIZE;
  find = ptr;
  find->free = 1;

  if(find->prev != NULL && find->next != NULL){
   if((find->free == 1 ) && (find->prev->free == 1))
   {
     find->prev->size = find->prev->size + find->size + 28;
     find->prev->next = find->next;
     if(find->next != NULL)
     {
       find->next->prev = find->prev;
     }
   }
   else if((find->free == 1) && (find->next->free == 1))
   {
     find->size = find->size + find->next->size + 28;
     if(find->next->next != NULL)
     {
      find->next = find->next->next;
      find->next->prev = find;
     }
   }}
  else if(find->next != NULL)
  {
    if((find->free == 1) && (find->next->free == 1))
    {find->size = find->size + find->next->size + 28;
     if(find->next->next != NULL)
      { find->next = find->next->next;
        find->next->prev = find;
      }
     else
     {
       find->next->prev = NULL;
       find->next = NULL;
     }
    }   
  }
  else if(find->prev != NULL)
  {
    if((find->free == 1) && (find->prev->free == 1))
    {
      find->prev->next = NULL;
      find->prev = NULL;
    }
    else
    {
      find->prev->next = NULL;
      find->prev = NULL;
    }
  }
 
}

void*
m_realloc(void* ptr, size_t size)
{ 
  p_meta i;
  ptr = ptr - META_SIZE;
  i= ptr;
  int num = size-(i->size);
  if((num%4) != 0)
    num = ((num/4)+1)*4;
  size =  num;
  size = size + i->size;
  
  if((i->next != NULL) && (i->next->free == 1))
    {
     i->size = i->size + META_SIZE + i->next->size; 
     if(i->next->next != NULL){
       i->next = i->next->next;
       i->next->prev =i;
     }
     else
     {
      i->next = NULL;
     }
    if(i->size > size + META_SIZE)
    {
      p_meta divide = i+META_SIZE+size;
      divide -> prev = i;
      if(i->next != NULL)
       divide->next = i->next;
      i->next = divide;
      divide->size = i->size-size-META_SIZE;
      i->size = size;
      i->free  =0;
      divide->free = 1;
    }
     return i->data;

   }//paste beefor data isn't already impltmented
  
  i->free = 1;
  p_meta new2;
  new2 = sbrk(size+META_SIZE);
  new2->size = size;
  new2->free = 0;
  if(i->next != NULL)
    {new2->next = i->next;
    new2->next->prev = new2;}
  i->next = new2;
  new2->prev = i;
  memcpy(new2->data,i->data,(i->size));
  
}
