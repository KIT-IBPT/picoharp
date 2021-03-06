#ifndef __ASYN_HELPER_H__
#define __ASYN_HELPER_H__

#include <asynDriver.h>
#include <asynDrvUser.h>

#define DECLARE_INTERFACE(self, typ, impl, pvt) \
    self->typ.interfaceType = asyn ## typ ## Type; \
    self->typ.pinterface = &impl; \
    self->typ.drvPvt = pvt;

#define ASYNMUSTSUCCEED(e, m) \
    { \
        asynStatus status = e; \
        if(status != asynSuccess) { \
            errlogPrintf(m); \
            return -1; \
        } \
    }

#define EXPORT_ARRAY(STRUCT, TYPE, MEMBER, ALARMED...) \
    { \
        .offset = offsetof(STRUCT, MEMBER), \
        .name = #MEMBER, \
        ALARMED \
    }

#define EXPORT_ARRAY_END {0}

#define MEMBER_LOOKUP(s, info) (((char *) (s)) + (info)->offset)

struct struct_info
{
    int offset;
    const char * name;
    bool alarmed;       // If set an error is raised on alarm
    bool notify;        // If set the updated flag is set if field written
};

/* common */

void common_report(void *drvPvt, FILE * fp, int details);
asynStatus common_connect(void *drvPvt, asynUser * pasynUser);
asynStatus common_disconnect(void *drvPvt, asynUser * pasynUser);

/* DrvUser */

asynStatus drvuser_create(void *drvPvt, asynUser * pasynUser,
                           const char *drvInfo,
                           const char **pptypeName, size_t * psize);

asynStatus drvuser_get_type(void *drvPvt, asynUser * pasynUser,
                             const char **pptypeName, size_t * psize);

asynStatus drvuser_destroy(void *drvPvt, asynUser * pasynUser);

#endif
