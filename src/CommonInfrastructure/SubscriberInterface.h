//
// Created by smartmachines on 8/1/17.
//

#ifndef SUBSCRIBERINTERFACE_H
#define SUBSCRIBERINTERFACE_H

#include <stdio.h>
#include "ndds/ndds_cpp.h"
#include "ndds/ndds_namespace_cpp.h"
#include "../CommonInfrastructure/DDSCommunicator.h"
#include "../CommonInfrastructure/DDSTypeWrapper.h"

/* Generic listener class which will print a message if any of the 
   status events like deadline missed occur. The listener will be 
   used for the sensor data as well as platform status
 */
class Generic_Listener : public DDSDataReaderListener {
public:
    virtual void on_requested_deadline_missed(
            DDSDataReader* /*reader*/,
            const DDS_RequestedDeadlineMissedStatus& /*status*/) {
        printf("on_requested_deadline_missed\n");
    }

    virtual void on_requested_incompatible_qos(
            DDSDataReader* /*reader*/,
            const DDS_RequestedIncompatibleQosStatus& /*status*/) {
        printf("on_requested_incompatible_qos\n");
    }

    virtual void on_sample_rejected(
            DDSDataReader* /*reader*/,
            const DDS_SampleRejectedStatus& /*status*/) {
        printf("on_sample_rejected\n");
    }

    virtual void on_liveliness_changed(
            DDSDataReader* /*reader*/,
            const DDS_LivelinessChangedStatus& /*status*/) {
        printf("on_liveliness_changed\n");
    }

    virtual void on_sample_lost(
            DDSDataReader* /*reader*/,
            const DDS_SampleLostStatus& /*status*/) {
        printf("on_sample_lost\n");
    }

    virtual void on_subscription_matched(
            DDSDataReader* /*reader*/,
            const DDS_SubscriptionMatchedStatus& /*status*/) {
        printf("on_subscription_matched\n");
    }

    virtual void on_data_available(DDSDataReader* reader) {}
};


// ------------------------------------------------------------------------- //
//
// SubscriberInterface:
// A class that sets up the DDS interface (the network interface) of this 
//  application, including creating appropriate DDS DataWriters, DDS
// DataReaders, and all other DDS objects.
//
// This example creates a single DataReader, but the same DomainParticipant
// can be used to create DataWriters as well.
//
// ------------------------------------------------------------------------- //

class SubscriberInterface
{

public:

    // --- Constructor --- 
    SubscriberInterface();

    // --- Destructor --- 
    ~SubscriberInterface();

    DDS::Subscriber* GetSubscriber();

    template <typename T>
    void subscribe(std::string topicName, DDSDataReaderListener* listener);

    // --- Getter for Communicator --- 
    // Accessor for the communicator (the class that sets up the basic
    // DDS infrastructure like the DomainParticipant).
    // This allows access to the DDS DomainParticipant/Publisher/Subscriber
    // classes
    DDSCommunicator *GetCommunicator()
    {
        return _communicator;
    }

private:
    // --- Private members ---

    // This contains the calls that allow the interface to create a 
    // "DomainParticipant", the first object that must be created to 
    // communicate over a DDS middleware.
    DDSCommunicator *_communicator;
};

SubscriberInterface::SubscriberInterface() {
    _communicator = new DDSCommunicator();

    // Create a DomainParticipant
    // Start by creating a DomainParticipant.  Generally you will have only
    // one DomainParticipant per application.  A DomainParticipant is
    // responsible for starting the discovery process, allocating resources,
    // and being the factory class used to create Publishers, Subscribers,
    // Topics, etc.  Note:  The string constants with the QoS library name and
    // the QoS profile name are configured as constants in the .idl file.  The
    // profiles themselves are configured in the .xml file.

    if (NULL == _communicator->CreateParticipant())
    {
        std::stringstream errss;
        errss << "Failed to create DomainParticipant object";
        throw errss.str();
    }
}

SubscriberInterface::~SubscriberInterface() {
    delete _communicator;
}

DDS::Subscriber *SubscriberInterface::GetSubscriber() {
    DDS::Subscriber *subscriber = _communicator->GetSubscriber();
    if (subscriber == NULL) {
        // Creating a DDS subscriber.
        // You do _not_ need to create one subscriber per DataReader.
        subscriber = _communicator->CreateSubscriber();
        if (subscriber == NULL)
        {
            std::stringstream errss;
            errss << "Failed to create Subscriber object";
            throw errss.str();
        }
    }
    return subscriber;
}

template <typename T>
void SubscriberInterface::subscribe(std::string topicName, DDSDataReaderListener *listener) {
    DDS::Topic *topic = _communicator->CreateTopic<T>(
            topicName);

    DDS::Subscriber *subscriber = _communicator->CreateSubscriber();

    /* Create the data reader. Since we are using waitsets to receive
       samples the listener will only be enabled for status. On data
       available will handeled by the waitset in the main loop
     */
    DDS::DataReader *_reader = subscriber->create_datareader(
            topic, DDS_DATAREADER_QOS_DEFAULT, listener,
            DDS_STATUS_MASK_ALL);
//            DDS_STATUS_MASK_NONE & ~DDS_DATA_AVAILABLE_STATUS);

    if (NULL == _reader) {
        std::stringstream errss;
        errss << "Failed to create DataReader object";
    }
}

#endif //SUBSCRIBERINTERFACE_H
