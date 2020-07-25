#ifndef __LINK_LIST_H 
#define __LINK_LIST_H

#include "data_global.h"

typedef struct msg_pack
{
    char msg_type;
    char text[27];
}link_datatype;

typedef struct _node_
{
    link_datatype data;
    struct _node_ *next;
}linknode, *linklist;

linklist CreateEmptyLinklist ();
int EmptyLinklist (linklist h);
linklist GetLinknode (linklist h);
int InsertLinknode(link_datatype x);
#endif 