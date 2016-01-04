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


class JObject
{
public:
    JObject(const char *objName, const char **pptKeys, int pptCnt, JObject **subObjs)
    {
        Name = objName;
        propertyKeys = pptKeys;
        propertyCount = pptCnt;
        targetId = "";
        inputProperties = NULL;
        inputPropertiesCnt = 0;

        //printf("Name:%s subObjs:%p\n", objName, subObjs);
        if(subObjs)
        {
            int i = 0;
            for(i=0; i<pptCnt; i++)
            {
                if(*(subObjs + i))
                {
                    //printf("\t++subObjs:%p\n", *(subObjs + i));
                    next.push_back(*(subObjs + i));
                }
            }
        }
    };

    virtual ~JObject() {};

    /* Code generator */
    const char *Name;
    const char **propertyKeys;
    int propertyCount;
    std::list<JObject*> next;   //next array count is refer to the propertyCount

    /* Internal used */
    std::string targetId;
    std::list<sHashData*> properties; //properties array
};

typedef struct
{
    JObject *parent;
    JObject *self;
    JObject *child;
} JObjListItem;

typedef std::list<JObjListItem*> JObjList;


typedef struct
{
    const char *msgId;
    int ttl;
    JObject *inputObj;
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
JObject *Oem2InputObj[Oem2PropertyCount] =
{
    new JObject      //CP4 object
    (
        "CP4",
        CP4PropertyList,
        CP4PropertyCount,
        NULL
    ),
    NULL                    //Source property
};

JObject *MainInputObj[MainPropertyCount] =
{
    NULL,                   //Source property
    new JObject      //Oem1 object
    (
        "Oem1",
        Oem1PropertyList,
        Oem1PropertyCount,
        NULL
    ),
    new JObject      //Oem2 object
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
        new JObject
        (
            "Main",
            MainPropertyList,
            MainPropertyCount,
            MainInputObj
        )
    }
};
#endif


void UpdateInputObjToObjList(JObject *parent, JObject *self, JObjList &list)
{
    JObjListItem *item = NULL;
    JObject *jObj = NULL;
    std::list<JObject*>::iterator jObjIt;

    for(jObjIt=self->next.begin(); jObjIt!=self->next.end(); jObjIt++)
    {
        jObj = *jObjIt;
        //printf("self:%s next:%s\n", self->Name, jObj->Name);

        if(jObj)
        {
            item = new JObjListItem();
            item->parent = parent;
            item->self = self;
            item->child = jObj;
            list.push_back(item);

            UpdateInputObjToObjList(self, jObj, list);
        }
    }
}

void InputObjFillKey(JObjList *joList)
{
    JObjList::iterator joListIt;
    JObjListItem *joListItem = NULL;
    JObject *jo = NULL;
    sHashData *hash = NULL;

    for(joListIt=joList.begin(); joListIt!=joList.end(); joListIt++)
    {
        joListItem = *joListIt;
        jo = joListItem->self;
        snprintf(jo->properties)

        printf("%s | %s | %s\n",
                (oItem->parent == NULL) ? "Null" : oItem->parent->Name,
                oItem->self->Name,
                (oItem->child == NULL) ? "Null" : oItem->child->Name);
    }
}




int main(void)
{
    printf("Start----\n");

    JObjList objList;

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

    JObjList::iterator joListIt;
    for(joListIt=objList.begin(); joListIt!=objList.end(); joListIt++)
    {
        JObjListItem *oItem = *joListIt;
        printf("%s | %s | %s\n",
                (oItem->parent == NULL) ? "Null" : oItem->parent->Name,
                oItem->self->Name,
                (oItem->child == NULL) ? "Null" : oItem->child->Name);
    }

    //DumpInputObjs(updateInfo[0].inputObj);
	return 0;
}
