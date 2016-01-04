//============================================================================
// Name        : test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <list>

#if 1
typedef struct
{
    char aFildName[256];
    char aValue[256];
} sHashData;

typedef struct
{
    char aData[256];
} sSetData;

typedef struct _UpdateInputObj
{
    _UpdateInputObj(const char *p1, const char **p2, int p3, _UpdateInputObj **p4)
    : Name(p1), propertyKeys(p2), propertyCount(p3), next(p4),
      targetId(""), properties(NULL){};

    /* Code generator */
    const char *Name;
    const char **propertyKeys;
    int propertyCount;
    _UpdateInputObj **next;     //next array count is refer to the propertyCount

    /* Internal used */
    std::string targetId;
    sHashData *properties;      //properties array
} UpdateInputObj;

typedef struct
{
    UpdateInputObj *parent;
    UpdateInputObj *self;
    UpdateInputObj *child;
} ObjListItem;
typedef std::list<ObjListItem*> ObjList;


typedef struct
{
    const char *msgId;
    int ttl;
    UpdateInputObj *inputObj;
    //UpdateFunc;
} UpdateInfo;


const char *MainPropertyList[] =
{
    "Source",
    "Oem1",
    "Oem2"
};
#define MainPropertyCount 3

const char *Oem1PropertyList[] =
{
    "Oem1Source"
};
#define Oem1PropertyCount 1

const char *Oem2PropertyList[] =
{
    "CP4",
    "Oem2Source"
};
#define Oem2PropertyCount 2

const char *CP4PropertyList[] =
{
    "CP4Source"
};
#define CP4PropertyCount 1


#if 1
UpdateInputObj *Oem2InputObj[Oem2PropertyCount] =
{
    new UpdateInputObj      //CP4 object
    (
        "CP4",
        CP4PropertyList,
        CP4PropertyCount,
        NULL
    ),
    NULL                    //Source property
};

UpdateInputObj *MainInputObj[MainPropertyCount] =
{
    NULL,                   //Source property
    new UpdateInputObj      //Oem1 object
    (
        "Oem1",
        Oem1PropertyList,
        Oem1PropertyCount,
        NULL
    ),
    new UpdateInputObj      //Oem2 object
    (
        "Oem2",
        Oem2PropertyList,
        Oem2PropertyCount,
        Oem2InputObj
    )
};
#endif

UpdateInfo updateInfo[] =
{
    {
        "00001",
        300,
        new UpdateInputObj
        (
            "Main",
            MainPropertyList,
            MainPropertyCount,
            MainInputObj
        )
    }
};
#endif


void UpdateInputObjToObjList(UpdateInputObj *parent, UpdateInputObj *self, ObjList &list)
{
    ObjListItem *item = NULL;

    int i = 0;
    for(i=0; i<self->propertyCount; i++)
    {
        if(self->next && self->next[i])
        {
            //printf("self:%s next:%d:%p:%p\n", self->Name, i, self->next+i, *(self->next+i));
            item = new ObjListItem();
            item->parent = parent;
            item->self = self;
            item->child = self->next[i];
            list.push_back(item);

            UpdateInputObjToObjList(self, item->child, list);
        }
    }
}

#if 1
void InputObjFillKey(UpdateInputObj *obj)
{
    int i = 0;

    sHashData *hash = new sHashData[obj->propertyCount];

    for(i=0; i<obj->propertyCount; i++)
    {
        snprintf(hash[i].aFildName, sizeof(hash[i].aFildName), "%s", obj->propertyKeys[i]);
        //printf("%s:key[%d]:%s\n", obj->Name, i, obj->propertyKeys[i]);

        if(obj->next && obj->next[i])
        {
            InputObjFillKey(obj->next[i]);
        }
    }
    obj->properties = hash;

#if 1
    for(i=0; i<obj->propertyCount; i++)
    {
        printf("%s[%d]:%s:%s\n", obj->Name, i, hash[i].aFildName, obj->properties[i].aFildName);
    }
#endif
}
#else

void InputObjFillKey(ObjList &oList)
{
    ObjList::iterator oListIt;
    UpdateInputObj *obj = NULL;
    int i = 0;

    for(oListIt=oList.begin(); oListIt!=oList.end(); oListIt++)
    {
        obj = (*oListIt)->self;

        sHashData *hash = new sHashData[obj->propertyCount];
        for(i=0; i<obj->propertyCount; i++)
        {
            snprintf(hash[i].aFildName, sizeof(hash[i].aFildName), "%s", obj->propertyKeys[i]);
            //printf("%s:key[%d]:%s\n", obj->Name, i, obj->propertyKeys[i]);
        }
        obj->properties = hash;
#if 1
        for(i=0; i<obj->propertyCount; i++)
        {
            printf("%s[%d]:%s:%s\n", obj->Name, i, hash[i].aFildName, obj->properties[i].aFildName);
        }
#endif
    }
}
#endif




int main(void)
{
    printf("Start----\n");

    ObjList objList;

#if 0
    printf("UpdateInputObjToObjList----\n");
    printf("MainAdd:%p\n", MainInputObj);
    printf("MainAdd[0]:%p:%p\n", MainInputObj[0], *MainInputObj);
    printf("MainAdd[1]:%p:%p\n", MainInputObj[1], *(MainInputObj+1));
    printf("MainAdd[2]:%p:%p\n", MainInputObj[2], *(MainInputObj+2));
    printf("Oem2Add:%p\n", Oem2InputObj);
    printf("Oem2Add[0]:%p:%p\n", Oem2InputObj[0], *Oem2InputObj);
    printf("Oem2Add[1]:%p:%p\n", Oem2InputObj[1], *(Oem2InputObj+1));
#endif
    UpdateInputObjToObjList(0, updateInfo[0].inputObj, objList);

    ObjList::iterator oIt;
    printf("ObjList:\n");
    for(oIt=objList.begin(); oIt!=objList.end(); oIt++)
    {
        ObjListItem *oItem = *oIt;
        printf("\t%s | %s | %s\n",
                (oItem->parent == NULL) ? "Null" : oItem->parent->Name,
                oItem->self->Name,
                (oItem->child == NULL) ? "Null" : oItem->child->Name);
    }

    InputObjFillKey(updateInfo[0].inputObj);
	return 0;
}
