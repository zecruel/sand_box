/*
 *  server_example_control.c
 *
 *  How to use the different control handlers (TBD)
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ztto_3m_uac.h"

static int running = 0;
static IedServer iedServer = NULL;

static int disj = 1, sec_1 = 0, sec_2 = 1, sec_5 = 1, sec_6 = 0;

void
sigint_handler(int signalId)
{
    running = 0;
}

static CheckHandlerResult
checkHandler(ControlAction action, void* parameter, MmsValue* ctlVal, bool test, bool interlockCheck)
{
    ClientConnection clientCon = ControlAction_getClientConnection(action);

    if (clientCon) {
        printf("Control from client %s\n", ClientConnection_getPeerAddress(clientCon));
    }
    else {
        printf("clientCon == NULL\n");
    }

    if (ControlAction_isSelect(action))
        printf("check handler called by select command!\n");
    else
        printf("check handler called by operate command!\n");

    if (interlockCheck)
        printf("  with interlock check bit set!\n");

    printf("  ctlNum: %i\n", ControlAction_getCtlNum(action));

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013)
        return CONTROL_ACCEPTED;

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014)
        return CONTROL_ACCEPTED;


    return CONTROL_OBJECT_UNDEFINED;
}

static ControlHandlerResult
controlHandlerForBinaryOutput(ControlAction action, void* parameter, MmsValue* value, bool test)
{
    uint64_t timestamp = Hal_getTimeInMs();

    printf("control handler called\n");
    printf("  ctlNum: %i\n", ControlAction_getCtlNum(action));

    ClientConnection clientCon = ControlAction_getClientConnection(action);

    if (clientCon) {
        printf("Control from client %s\n", ClientConnection_getPeerAddress(clientCon));
    }
    else {
        printf("clientCon == NULL!\n");
    }

    if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001_stVal, value);
        printf("comando == Abrir!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_XCBR1_Pos_stVal, 2);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_XCBR1_Pos_t, timestamp);
      
        disj = 0;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002_stVal, value);
        printf("comando == Fechar!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_XCBR1_Pos_stVal, 1);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_XCBR1_Pos_t, timestamp);
      
        disj = 1;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003_stVal, value);
        printf("comando == Abrir!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_stVal, 2);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_t, timestamp);
      
        sec_1 = 0;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004_stVal, value);
        printf("comando == Fechar!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_stVal, 1);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_t, timestamp);
      
        sec_1 = 1;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005_stVal, value);
        printf("comando == Abrir!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_stVal, 2);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_t, timestamp);
      
        sec_2 = 0;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006_stVal, value);
        printf("comando == Fechar!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_stVal, 1);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_t, timestamp);
      
        sec_2 = 1;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007_stVal, value);
        printf("comando == Abrir!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_stVal, 2);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_t, timestamp);
      
        sec_5 = 0;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008_stVal, value);
        printf("comando == Fechar!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_stVal, 1);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_t, timestamp);
      
        sec_5 = 1;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009_stVal, value);
        printf("comando == Abrir!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_stVal, 2);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_t, timestamp);
      
        sec_6 = 0;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010_stVal, value);
        printf("comando == Fechar!\n");
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_stVal, 0);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_t, timestamp);
      
        Thread_sleep(5000);
      
        IedServer_updateBitStringAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_stVal, 1);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_t, timestamp);
      
        sec_6 = 1;
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013_stVal, value);
        printf("comando == Abrir!\n");
      
        IedServer_updateBooleanAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind042_stVal, true);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind042_t, timestamp);
    }

    else if (parameter == ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014) {
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014_t, timestamp);
        IedServer_updateAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014_stVal, value);
        printf("comando == Fechar!\n");
      
        IedServer_updateBooleanAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind042_stVal, false);
        IedServer_updateUTCTimeAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind042_t, timestamp);
    }

    else
        return CONTROL_RESULT_FAILED;

    return CONTROL_RESULT_OK;
}

static MmsDataAccessError
writeAccessHandler (DataAttribute* dataAttribute, MmsValue* value, ClientConnection connection, void* parameter)
{
    ControlModel ctlModelVal = (ControlModel) MmsValue_toInt32(value);

    /* we only allow status-only and direct-operate */
    if ((ctlModelVal == CONTROL_MODEL_STATUS_ONLY) || (ctlModelVal == CONTROL_MODEL_DIRECT_NORMAL))
    {
        IedServer_updateCtlModel(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001, ctlModelVal);

        printf("Changed GGIO1.SPCSI.ctlModel to %i\n", ctlModelVal);

        return DATA_ACCESS_ERROR_SUCCESS;
    }
    else {
        return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
    }
}

int
main(int argc, char** argv)
{
  
  /* Create new server configuration object */
    IedServerConfig config = IedServerConfig_create();

    /* Set buffer size for buffered report control blocks to 200000 bytes */
    IedServerConfig_setReportBufferSize(config, 200000);

    /* Set stack compliance to a specific edition of the standard (WARNING: data model has also to be checked for compliance) */
    IedServerConfig_setEdition(config, IEC_61850_EDITION_2);

    /* disable MMS file service */
    IedServerConfig_enableFileService(config, false);

    /* enable dynamic data set service */
    IedServerConfig_enableDynamicDataSetService(config, true);

    /* disable log service */
    IedServerConfig_enableLogService(config, false);

    /* set maximum number of clients */
    IedServerConfig_setMaxMmsConnections(config, 5);
    
    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_createWithConfig(&ztto_3m_uac, NULL, config);

    /* configuration object is no longer required */
    IedServerConfig_destroy(config);
    
    //iedServer = IedServer_create(&ztto_3m_uac);
    int tcpPort = 103;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001);

    /*
     * For SPCSO1 we want the user be able to change the control model by online service -
     * so we install a write access handler to change the control model when the client
     * writes to the "ctlModel" attribute.
     */
    IedServer_handleWriteAccess(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO001_ctlModel, writeAccessHandler, NULL);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO002);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO003);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO004);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO005);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO006);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO007);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO008);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO009);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO010);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO013);


    IedServer_setControlHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014,
            (ControlHandler) controlHandlerForBinaryOutput,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014);

    /* this is optional - performs operative checks */
    IedServer_setPerformCheckHandler(iedServer, ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014, checkHandler,
            ZTTO_3M_UAC_LDInst_GGIO4_SPCSO014);
    
    
    
    /* Initialize process values */
    MmsValue* DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_XCBR1_Pos_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 1); /* set DPC to OFF */
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI1_Pos_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 2);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI2_Pos_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 1);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI3_Pos_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 1);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI4_Pos_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 2);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_CWI5_Pos_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 2);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO1_Ind015_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO1_Ind031_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO1_Ind035_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO1_Ind039_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO1_Ind046_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    
    
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind007_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind008_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind016_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind022_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind028_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    DPCSO1_stVal = IedServer_getAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO2_Ind037_stVal);
    MmsValue_setBoolean(DPCSO1_stVal, true);
    
    

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);
    
    float t = 0.f;

    while (running) {
      
      uint64_t timestamp = Hal_getTimeInMs();

        t += 0.13f;

        float an1 = sinf(t);
        float an2 = sinf(t + 1.f);
        float an3 = sinf(t + 2.f);
        float an4 = sinf(t + 3.f);

        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        /* toggle clock-not-synchronized flag in timestamp */
        if (((int) t % 2) == 0)
            Timestamp_setClockNotSynchronized(&iecTimestamp, true);

        IedServer_lockDataModel(iedServer);
        
        
        if (((sec_1 || sec_2) && disj && sec_5) || sec_6){

          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn001_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn001_mag_f, 400.0 + 5.0 * an1);
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn002_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn002_mag_f, 400.0 + 5.0 * an2);
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn003_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn003_mag_f, 400.0 + 5.0 * an3);

          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn007_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn007_mag_f, 230.0 + 4.0 * an2);

          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn008_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn008_mag_f, 140.0 + 4.0 * an4);
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn009_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn009_mag_f, 20.0 + 2.0 * an4);
          
        } else {
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn001_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn001_mag_f, 2.0 * an1);
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn002_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn002_mag_f, 2.0 * an2);
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn003_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn003_mag_f, 2.0 * an3);

          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn007_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn007_mag_f, 4.0 * an2);

          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn008_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn008_mag_f, 2.0 * an4);
          
          IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn009_t, &iecTimestamp);
          IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn009_mag_f, 0.5 * an4);
          
        }
        
        
        IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn010_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn010_mag_f, 0.8 + 0.1 * an4);
        
        IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn004_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn004_mag_f, 230.0 + 4.0 * an1);

        IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn005_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn005_mag_f, 230.0 + 4.0 * an2);
        
        IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn006_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn006_mag_f, 230.0 + 4.0 * an3);
        
        IedServer_updateTimestampAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn011_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, ZTTO_3M_UAC_LDInst_GGIO3_AnIn011_mag_f, 60.0 + 0.1 * an1);

        IedServer_unlockDataModel(iedServer);

        Thread_sleep(5000);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);
    return 0;
} /* main() */
