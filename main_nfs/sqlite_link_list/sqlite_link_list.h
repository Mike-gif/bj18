#ifndef __SQLITE_LINK_LIST_H
#define __SQLITE_LINK_LIST_H

#include "data_global.h"

typedef struct _snode_
{
	struct sqlite_operation data;
	struct env_info_clien_addr data_link;
	int storageNum;
	int goodsKinds;
	struct _snode_ *next;
}slinknode, *slinklist;


slinklist sqlite_CreateEmptyLinklist ();
int sqlite_EmptyLinklist (slinklist h);
slinklist sqlite_GetLinknode (slinklist h);


#endif
