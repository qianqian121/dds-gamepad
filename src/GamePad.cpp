/*****************************************************************************/
/*         (c) Copyright, Real-Time Innovations, All rights reserved.        */
/*                                                                           */
/*         Permission to modify and use for internal purposes granted.       */
/* This software is provided "as is", without warranty, express or implied.  */
/*                                                                           */
/*****************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "ndds/ndds_cpp.h"
#include "GamePad.h"
#include "GamePadSupport.h"
#include "GamePadPublisher.h"
#include "GamePadSubscriber.h"

/*****************************************************************************/
/* Local constants & macros                                                  */
/*****************************************************************************/

/* Command line defaults */
#define DEFAULT_DOMAIN_ID           0
#define APPLICATION_NAME            "GamePad"
#define DEFAULT_PAYLOAD             1024

/* Application running mode */
#define APP_MODE_UNDEFINED          0
#define APP_MODE_PUBLISHER          1
#define APP_MODE_SUBSCRIBER         2

/* This is the name of the topic used for our test */
#define DEFAULT_TOPIC_NAME          "GamePad IDL"

/* The maximum value for Domain ID. The effective value depends on the QoS,
 * in particular to the value of the field:
 *              DDS_RtpsWellKnownPorts_t.domainId_gain
 * The default value is 250, so we just limit to that statically so we can
 * check the validity before attempting to create the domain participant.
 */
#define DOMAIN_ID_MAX               250


/*****************************************************************************/
/* Structure to hold the parsed command line arguments                       */
/*****************************************************************************/
struct CommandLineArguments {
    DDS_Long    domainId;
    DDS_Long    mode;
    DDS_Long    dataSize;
    DDS_Long    sampleCount;
    DDS_Long    verbose;        /* 0=no verbose, 1=app verbose, 2=DDS */
    std::string topicName;
    
    CommandLineArguments() {
        mode      = APP_MODE_UNDEFINED;
        domainId  = DEFAULT_DOMAIN_ID;
        dataSize  = DEFAULT_PAYLOAD;
        sampleCount = 0;
        verbose   = 0;
        topicName = DEFAULT_TOPIC_NAME;
    }
};



/*****************************************************************************/
/* Overload the std::ostream::operator<<() to print a                        */
/* DDS_ProductVersion_t.                                                     */
/*****************************************************************************/
std::ostream &operator<<(std::ostream &os, const DDS_ProductVersion_t &arg) {
    os << (int)arg.major << '.' 
       << (int)arg.minor
       << (char)arg.release << " (build "
       << (int)arg.revision << ')';
    return os;
}

/*****************************************************************************/
/* fileExist                                                                 */
/*                                                                           */
/* Just a convenience function that checks if a file exist and can be read   */
/*****************************************************************************/
static bool fileExist(const char *fileName) {
    std::ifstream stream;
    stream.open(fileName);
    if (!stream) {
        return false;
    }
    stream.close();
    return true;
}


/*****************************************************************************/
/* start_application                                                         */
/*                                                                           */
/* Creates the DDS Domain Participant, then delegates the execution to the   */
/* correct pub/sub function.                                                 */
/* If entity creation is successful, this function does not return.          */
/*                                                                           */
/* Input:                                                                    */
/*   arg: pointer to the CommandLineArguments structure                      */
/*                                                                           */
/* Returns:                                                                  */
/*   false if an error occurred.                                             */
/*                                                                           */
/*****************************************************************************/
static bool startApplication(
                        const CommandLineArguments &arg) {
    DDS_ReturnCode_t         rc;
    DDSDomainParticipant *   participant = NULL;
    DDSTopic *               topic = NULL;
    bool                     returnValue = false;

    std::cout <<
        "# The output below depends on the QoS profile"        << std::endl << 
        "# provided to this application."                      << std::endl <<
        "# -> For more information on the provided example"    << std::endl <<
        "#    profiles, please see the Getting Started Guide." << std::endl <<
        "# -> For detailed product performance metrics, visit" << std::endl <<
        "#    http://www.rti.com/products/data_distribution/index.html" << std::endl <<
        "#    and click on Benchmarks."           << std::endl << std::endl;

    /* This example creates DDS entities using the default QoS.
     * The default QoS can be modified in two ways:
     * 1. By placing a file called 'USER_QOS_PROFILES.xml' in the directory
     *    where the application is launched.
     * 2. By setting the environment variable 'NDDS_QOS_PROFILES' to point
     *    to a valid file containing NDDS QoS profile definitions.
     *
     * This section check if a QoS profile file is accessible, and prints
     * a warning if that's not true.
     */
     
    /* First look in the current directory to see if the USER_QOS_PROFILES.xml
     * file exist.
     */
    if (!fileExist("USER_QOS_PROFILES.xml")) {
        /* Then look for the env variable 'NDDS_QOS_PROFILES'... 
         */
        char *envVal;
#if defined(RTI_WIN32) && (_MSC_VER > 1310) 
        size_t envValSize;
        _dupenv_s(&envVal, &envValSize, "NDDS_QOS_PROFILES"); 
#else 
        envVal = getenv("NDDS_QOS_PROFILES");
#endif
        if (envVal == NULL || !fileExist(envVal)) {
            std::cout << "! Warning:" << std::endl
                      << "! Default QoS profile definition file not found." 
                      << std::endl
                      << "! The application will use the DDS default QoS." 
                      << std::endl
                      << "! If you want to use different QoS, make sure you "
                      << "have the QoS definition file"
                      << std::endl
                      << "! (USER_QOS_PROFILES.xml) in the current working "
                      << "directory"
                      << std::endl
                      << "! or set the environment variable NDDS_QOS_PROFILES "
                      << "to point"
                      << std::endl
                      << "! to a file containing the default QoS profile"
                      << std::endl;
        }
#if defined(RTI_WIN32) && (_MSC_VER > 1310) 
        if (envVal) free(envVal);
#endif
    }

    /* If you need to customize any DDS factory QoS, uncomment the following
     * code: 
     *
    {
        DDS_DomainParticipantFactoryQos factoryQos;
        rc = DDSDomainParticipantFactory::get_instance()->get_qos(factoryQos);
        if (rc != DDS_RETCODE_OK) { 
            std::cerr << "! Unable to get participant factory QoS: "
                      << rc << std::endl;
            goto exitFn;
        }
        
        // Modify the factory QoS here
        
        rc = DDSDomainParticipantFactory::get_instance()->set_qos(factoryQos);
        if (rc != DDS_RETCODE_OK) { 
            std::cerr << "! Unable to set participant factory QoS: "
                      << rc << std::endl;
            goto exitFn;
        }
    }
    */
    
            
    /* Creates the DDS Domain Participant.
     * The following command will create a DDS Domain participant without
     * installing any status callback listener.
     * If you want to create a domain participant with different QoS,
     * use DDSDomainParticipantFactory::get_default_participant_qos
     * to obtain a copy of the default participant QoS, change them,
     * then use them instead of DDS_PARTICIPANT_QOS_DEFAULT:
     *
     
        DDS_DomainParticipantQos myQos;
        DDSDomainParticipantFactory::get_instance()->get_default_participant_qos(
                        myQos);
        // Modify the participant QoS here

        // Then create the domain participant using 'myQos'...
     *
     * Note: for more info on the domain participant API see:
     *     $NDDSHOME/doc/html/api_cpp/group__DDSDomainParticipantModule.html
     */
    if (arg.verbose) {
        std::cout << "Creating domain participant..." << std::endl;
    }
    participant = DDSDomainParticipantFactory::get_instance()->
                        create_participant(
                        arg.domainId,
                        DDS_PARTICIPANT_QOS_DEFAULT,
                        NULL,   /* Listener */
                        DDS_STATUS_MASK_NONE);
    if (participant == NULL) {
        std::cerr << "! Unable to create DDS domain participant" << std::endl;
        goto exitFn;
    }


    /* Registers the type used in this example with the domain participant.
     * For more information on user-defined data types:
     *     $NDDSHOME/doc/html/api_cpp/group__DDSUserDataModule.html
     */
    if (arg.verbose) {
        std::cout << "Registering the type..." << std::endl;
    }
    rc = GamePadTypeSupport::register_type(
                        participant, 
                        GamePadTypeSupport::get_type_name());
    if (rc != DDS_RETCODE_OK) {
        std::cerr << "! Unable to register '"
                  << GamePadTypeSupport::get_type_name() 
                  << "' data type: " 
                  << rc << std::endl;
        goto exitFn;
    }


    /* Creates the topic.
     * The following command will create the topic without
     * installing any status callback listener.
     * If you want to create a topic with different QoS,
     * use DDS_DomainParticipant_get_default_topic_qos
     * to obtain a copy of the default topic QoS, change them,
     * then use them instead of DDS_TOPIC_QOS_DEFAULT:
     *
     {
        DDS_TopicQos myQos;
        participant->get_default_topic_qos(myQos);
        
        // Modify the topic QoS here

        // Then create the topic using 'myQos'...        
     }
     *
     * Note: for more info on the topic API see:
     *     $NDDSHOME/doc/html/api_cpp/group__DDSTopicEntityModule.html
     */
    if (arg.verbose) {
        std::cout << "Creating the topic..." << std::endl;
    }
    topic = participant->create_topic(
                        arg.topicName.c_str(),
                        GamePadTypeSupport::get_type_name(), 
                        DDS_TOPIC_QOS_DEFAULT,
                        NULL,           /* listener */
                        DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        std::cerr << "! Unable to create topic '"
                  << arg.topicName
                  << std::endl;
        goto exitFn;
    }

    /* Creates the publisher or subscriber */
    if (arg.mode == APP_MODE_PUBLISHER) {
        if (!startPublisher(
                        participant, 
                        topic,
                        arg.verbose,
                        arg.dataSize,
                        arg.sampleCount)) {
            goto exitFn;
        }
    } else {
        if (!startSubscriber(
                        participant, 
                        topic,
                        arg.verbose,
                        arg.sampleCount)) {
            goto exitFn;
        }
    }
    returnValue = true;
    
exitFn:
    if (arg.verbose) {
        std::cout << "Cleaning up..." << std::endl;
    }
    if (participant != NULL) {
        /* Perform a clean shutdown of the participant and all the contained
         * entities
         */
        rc = participant->delete_contained_entities();
        if (rc != DDS_RETCODE_OK) {
            std::cerr << "! Unable to delete participant contained entities: "
                      << rc << std::endl;
        }

        rc = DDSDomainParticipantFactory::get_instance()->delete_participant(
                        participant);
        if (rc != DDS_RETCODE_OK) {
            std::cerr << "! Unable to delete participant: " << rc << std::endl;
        }
    }
    return returnValue;
}

/*****************************************************************************/
/* main                                                                      */
/*                                                                           */
/* Application main entry point                                              */
/*****************************************************************************/
int main(int argc, char *argv[])
{
    /* Uncomment this to turn on additional logging
    NDDSConfigLogger::get_instance()->
    set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
    NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */

    return publisher_main();
}
