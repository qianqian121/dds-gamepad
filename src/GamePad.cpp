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
#include <CommonInfrastructure/PublisherInterface.h>
#include "ndds/ndds_cpp.h"
#include "GamePad.h"
#include "generated/GamePad.h"
#include "generated/GamePadSupport.h"
#include "generated/GamePadPlugin.h"

using namespace std;

/*****************************************************************************/
/* Local constants & macros                                                  */
/*****************************************************************************/

/* Command line defaults */
#define DEFAULT_DOMAIN_ID           0
#define APPLICATION_NAME            "GamePad"
#define DEFAULT_PAYLOAD             1024

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

    // This is the network interface for this application - this is what
    // actually sends the flight plan information over the transport
    // (shared memory or over the network).  Look into this class to see
    // what you need to do to implement an RTI Connext DDS application that
    // writes data.
    PublisherInterface<DdsAutoType<TwistCommands>> fpInterface(nullptr);

    DDS_Duration_t send_period = {0,100000000};

    cout << "Sending flight plans over RTI Connext DDS" << endl;

    // Allocate a flight plan structure
    DdsAutoType<TwistCommands> twist_commands;

    // Use the current time as a starting point for the expected
    // landing time of the aircraft
    DDS_Time_t time;
    fpInterface.GetCommunicator()->GetParticipant()
            ->get_current_time(time);

    // Write the data to the network.  This is a thin wrapper
    // around the RTI Connext DDS DataWriter that writes data to
    // the network.
    fpInterface.Write(twist_commands);

    NDDSUtility::sleep(send_period);

//    return publisher_main();
}
