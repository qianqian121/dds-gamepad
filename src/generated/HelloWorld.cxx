

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from HelloWorld.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#ifndef dds_c_log_impl_h              
#include "dds_c/dds_c_log_impl.h"                                
#endif        

#ifndef cdr_type_h
#include "cdr/cdr_type.h"
#endif    

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h" 
#endif
#else
#include "ndds_standalone_type.h"
#endif

#include "HelloWorld.h"

/* ========================================================================= */
const char *HelloWorldTYPENAME = "HelloWorld";

DDS_TypeCode* HelloWorld_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode HelloWorld_g_tc_prefix_string = DDS_INITIALIZE_STRING_TYPECODE(((HELLODDS_MAX_STRING_SIZE)));
    static DDS_TypeCode HelloWorld_g_tc_payload_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE(((HELLODDS_MAX_PAYLOAD_SIZE)),NULL);
    static DDS_TypeCode_Member HelloWorld_g_tc_members[3]=
    {

        {
            (char *)"prefix",/* Member name */
            {
                0,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"sampleId",/* Member name */
            {
                1,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"payload",/* Member name */
            {
                2,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }
    };

    static DDS_TypeCode HelloWorld_g_tc =
    {{
            DDS_TK_STRUCT,/* Kind */
            DDS_BOOLEAN_FALSE, /* Ignored */
            -1, /*Ignored*/
            (char *)"HelloWorld", /* Name */
            NULL, /* Ignored */      
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            3, /* Number of members */
            HelloWorld_g_tc_members, /* Members */
            DDS_VM_NONE  /* Ignored */         
        }}; /* Type code for HelloWorld*/

    if (is_initialized) {
        return &HelloWorld_g_tc;
    }

    HelloWorld_g_tc_payload_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;

    HelloWorld_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&HelloWorld_g_tc_prefix_string;

    HelloWorld_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    HelloWorld_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)& HelloWorld_g_tc_payload_sequence;

    is_initialized = RTI_TRUE;

    return &HelloWorld_g_tc;
}

RTIBool HelloWorld_initialize(
    HelloWorld* sample) {
    return HelloWorld_initialize_ex(sample,RTI_TRUE,RTI_TRUE);
}

RTIBool HelloWorld_initialize_ex(
    HelloWorld* sample,RTIBool allocatePointers, RTIBool allocateMemory)
{

    struct DDS_TypeAllocationParams_t allocParams =
    DDS_TYPE_ALLOCATION_PARAMS_DEFAULT;

    allocParams.allocate_pointers =  (DDS_Boolean)allocatePointers;
    allocParams.allocate_memory = (DDS_Boolean)allocateMemory;

    return HelloWorld_initialize_w_params(
        sample,&allocParams);

}

RTIBool HelloWorld_initialize_w_params(
    HelloWorld* sample, const struct DDS_TypeAllocationParams_t * allocParams)
{

    void* buffer = NULL;
    if (buffer) {} /* To avoid warnings */

    if (allocParams) {} /* To avoid warnings */

    if (allocParams->allocate_memory){
        sample->prefix= DDS_String_alloc (((HELLODDS_MAX_STRING_SIZE)));
        if (sample->prefix == NULL) {
            return RTI_FALSE;
        }

    } else {
        if (sample->prefix!= NULL) { 
            sample->prefix[0] = '\0';
        }
    }

    if (!RTICdrType_initLong(&sample->sampleId)) {
        return RTI_FALSE;
    }     

    if (allocParams->allocate_memory) {
        DDS_OctetSeq_initialize(&sample->payload  );
        if (!DDS_OctetSeq_set_maximum(&sample->payload , ((HELLODDS_MAX_PAYLOAD_SIZE)))) {
            return RTI_FALSE;
        }
    } else { 
        DDS_OctetSeq_set_length(&sample->payload, 0);
    }
    return RTI_TRUE;
}

void HelloWorld_finalize(
    HelloWorld* sample)
{

    HelloWorld_finalize_ex(sample,RTI_TRUE);
}

void HelloWorld_finalize_ex(
    HelloWorld* sample,RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParams =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;

    if (sample==NULL) {
        return;
    } 

    deallocParams.delete_pointers = (DDS_Boolean)deletePointers;

    HelloWorld_finalize_w_params(
        sample,&deallocParams);
}

void HelloWorld_finalize_w_params(
    HelloWorld* sample,const struct DDS_TypeDeallocationParams_t * deallocParams)
{

    if (sample==NULL) {
        return;
    }
    if (deallocParams) {} /* To avoid warnings */

    if (sample->prefix != NULL) {
        DDS_String_free(sample->prefix);
        sample->prefix=NULL;

    }

    DDS_OctetSeq_finalize(&sample->payload);

}

void HelloWorld_finalize_optional_members(
    HelloWorld* sample, RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParamsTmp =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;
    struct DDS_TypeDeallocationParams_t * deallocParams =
    &deallocParamsTmp;

    if (sample==NULL) {
        return;
    } 
    if (deallocParams) {} /* To avoid warnings */

    deallocParamsTmp.delete_pointers = (DDS_Boolean)deletePointers;
    deallocParamsTmp.delete_optional_members = DDS_BOOLEAN_TRUE;

}

RTIBool HelloWorld_copy(
    HelloWorld* dst,
    const HelloWorld* src)
{

    if (!RTICdrType_copyStringEx (
        &dst->prefix, src->prefix, 
        ((HELLODDS_MAX_STRING_SIZE)) + 1, RTI_FALSE)){
        return RTI_FALSE;
    }
    if (!RTICdrType_copyLong (
        &dst->sampleId, &src->sampleId)) { 
        return RTI_FALSE;
    }
    if (!DDS_OctetSeq_copy(&dst->payload ,
    &src->payload )) {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'HelloWorld' sequence class.
*/
#define T HelloWorld
#define TSeq HelloWorldSeq
#define T_initialize_w_params HelloWorld_initialize_w_params
#define T_finalize_w_params   HelloWorld_finalize_w_params
#define T_copy       HelloWorld_copy

#ifndef NDDS_STANDALONE_TYPE
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#else
#include "dds_c_sequence_TSeq.gen"
#include "dds_cpp_sequence_TSeq.gen"
#endif

#undef T_copy
#undef T_finalize_w_params
#undef T_initialize_w_params
#undef TSeq
#undef T

