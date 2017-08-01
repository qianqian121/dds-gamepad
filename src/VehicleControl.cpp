//
// Created by smartmachines on 8/1/17.
//

#include <CommonInfrastructure/SubscriberInterface.h>
#include <generated/GamePad.h>
#include <generated/GamePadSupport.h>
#include <iostream>

class TwistListener : public Generic_Listener {
    void on_data_available(DDSDataReader *reader) {
        TwistCommands::Seq dataSeq;
        DDS_SampleInfoSeq infoSeq;
        DDS_ReturnCode_t rc;

        /* The following narrow function should never fail in our case, as
         * we have only one reader in the application. It simply performs
         * only a safe cast of the generic data reader into a specific
         * HelloWorldDataReader.
         */
        TwistCommands::DataReader *helloReader = TwistCommands::DataReader::narrow(reader);
        if (helloReader == NULL) {
            std::cerr << "! Unable to narrow data reader" << std::endl;
            return;
        }

        rc = helloReader->take(
                dataSeq,
                infoSeq,
                DDS_LENGTH_UNLIMITED,
                DDS_ANY_SAMPLE_STATE,
                DDS_ANY_VIEW_STATE,
                DDS_ANY_INSTANCE_STATE);
        if (rc == DDS_RETCODE_NO_DATA) {
            return;
        } else if (rc != DDS_RETCODE_OK) {
            std::cerr << "! Unable to take data from data reader, error "
                      << rc << std::endl;
            return;
        }

        for (int i = 0; i < dataSeq.length(); ++i) {
            if (infoSeq[i].valid_data) {
                // Process the data
                processData(dataSeq[i]);
            }
        }

        rc = helloReader->return_loan(dataSeq, infoSeq);
        if (rc != DDS_RETCODE_OK) {
            std::cerr << "! Unable to return loan, error "
                      << rc << std::endl;
        }
    }

    void processData(TwistCommands &commands) {

    }
};

/*****************************************************************************/
/* main                                                                      */
/*                                                                           */
/* Application main entry point                                              */
/*****************************************************************************/
int main(int argc, char *argv[]) {
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
    TwistListener *listener = new TwistListener();
    SubscriberInterface fpInterface;
    fpInterface.subscribe<TwistCommands>("twist_raw", listener);

    DDS_Duration_t send_period = {0, 100000000};

    while (1) {
        NDDSUtility::sleep(send_period);
    }
}
