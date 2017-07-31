

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from GamePad.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef GamePad_726381412_h
#define GamePad_726381412_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#else
#include "ndds_standalone_type.h"
#endif

static const DDS_Long HELLODDS_MAX_PAYLOAD_SIZE= 8192;
static const DDS_Long HELLODDS_MAX_STRING_SIZE= 64;
extern "C" {

    extern const char *TwistCommandsTYPENAME;

}

struct TwistCommandsSeq;
#ifndef NDDS_STANDALONE_TYPE
class TwistCommandsTypeSupport;
class TwistCommandsDataWriter;
class TwistCommandsDataReader;
#endif

class TwistCommands 
{
  public:
    typedef struct TwistCommandsSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef TwistCommandsTypeSupport TypeSupport;
    typedef TwistCommandsDataWriter DataWriter;
    typedef TwistCommandsDataReader DataReader;
    #endif

    DDS_Char *   prefix ;
    DDS_Long   sampleId ;
    DDS_OctetSeq  payload ;

};
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

NDDSUSERDllExport DDS_TypeCode* TwistCommands_get_typecode(void); /* Type code */

DDS_SEQUENCE(TwistCommandsSeq, TwistCommands);                                        

NDDSUSERDllExport
RTIBool TwistCommands_initialize(
    TwistCommands* self);

NDDSUSERDllExport
RTIBool TwistCommands_initialize_ex(
    TwistCommands* self,RTIBool allocatePointers,RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool TwistCommands_initialize_w_params(
    TwistCommands* self,
    const struct DDS_TypeAllocationParams_t * allocParams);        

NDDSUSERDllExport
void TwistCommands_finalize(
    TwistCommands* self);

NDDSUSERDllExport
void TwistCommands_finalize_ex(
    TwistCommands* self,RTIBool deletePointers);

NDDSUSERDllExport
void TwistCommands_finalize_w_params(
    TwistCommands* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void TwistCommands_finalize_optional_members(
    TwistCommands* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool TwistCommands_copy(
    TwistCommands* dst,
    const TwistCommands* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* GamePad */

