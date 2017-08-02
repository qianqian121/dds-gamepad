//
// Created by smartmachines on 8/1/17.
//

#ifndef DDSNODE_INTERFACE_H
#define DDSNODE_INTERFACE_H

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
// DDSNodeInterface:
// A class that sets up the DDS interface (the network interface) of this 
//  application, including creating appropriate DDS DataWriters, DDS
// DataReaders, and all other DDS objects.
//
// This example creates a single DataReader, but the same DomainParticipant
// can be used to create DataWriters as well.
//
// ------------------------------------------------------------------------- //

class DDSNodeInterface
{

public:

    // --- Constructor --- 
    DDSNodeInterface();

    // --- Destructor --- 
    ~DDSNodeInterface();

    DDS::Subscriber* GetSubscriber();
    DDS::Publisher* GetPublisher();

    template <typename T>
    void subscribe(std::string topicName, DDSDataReaderListener* listener);

    template <typename T>
    DDS::DataWriter *advertise(std::string topicName);

    template <typename T>
    void pub(DDS::DataWriter *writer, DdsAutoType<T> msg_data);
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

DDSNodeInterface::DDSNodeInterface() {
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

DDSNodeInterface::~DDSNodeInterface() {
    delete _communicator;
}

DDS::Subscriber *DDSNodeInterface::GetSubscriber() {
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

DDS::Publisher *DDSNodeInterface::GetPublisher() {
    DDS::Publisher *pub = _communicator->GetPublisher();
    if (pub == NULL) {
        // Create a Publisher
        // This application only writes data, so we only need to create a
        // publisher.  The RadarData application has a more complex pattern
        // so we explicitly separate the writing interface from the overall
        // network interface - meaning that the publisher is created in the
        // network interface, and the DataWriter is created in a separate class
        // Note that one Publisher can be used to create multiple DataWriters
        pub = _communicator->CreatePublisher();

        if (pub == NULL) {
            std::stringstream errss;
            errss << "Failed to create Publisher object";
            throw errss.str();
        }
    }
    return pub;
}

template <typename T>
void DDSNodeInterface::subscribe(std::string topicName, DDSDataReaderListener *listener) {
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

template <typename T>
DDS::DataWriter *DDSNodeInterface::advertise(std::string topicName) {
    // This topic has the name AIRCRAFT_TOPIC - a constant
    // string that is defined in the .idl file.  (It is not required that
    // you define your topic name in IDL, but it is a best practice for
    // ensuring the data interface of an application is all defined in one
    // place. You can register all topics and types up-front, if you nee
    DDS::Topic *topic = _communicator->CreateTopic<T>(
            topicName);


    // Create a DataWriter.
    // This creates a single DataWriter that writes data, with QoS
    // that is used for State Data.	Note: The string constants with the QoS
    // library name and the QoS profile name are configured as constants in the
    // .idl file.  The profiles themselves are configured in the .xml file.
    DDS::DataWriter *_writer = GetPublisher()->create_datawriter(topic,
                                     DDS_DATAWRITER_QOS_DEFAULT,
                                     NULL, DDS_STATUS_MASK_NONE);

    if (_writer == NULL)
    {
        std::stringstream errss;
        errss <<
              "Failure to create  writer. Inconsistent Qos?";
        throw errss.str();
    }

//    return T::DataWriter::narrow(_writer);
    return _writer;
}

template <typename T>
void DDSNodeInterface::pub(DDS::DataWriter *writer, DdsAutoType<T> msg_data) {
    // Write the data to the network.  This is a thin wrapper
    // around the RTI Connext DDS DataWriter that writes data to
    // the network.
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    DDS_InstanceHandle_t handle = DDS_HANDLE_NIL;

    retcode = T::DataWriter::narrow(writer)->write(msg_data, handle);

    if (retcode != DDS_RETCODE_OK)
    {
        std::stringstream errss;
        errss <<
              "Failure to write data";
        throw errss.str();
    }
}

#endif //DDSNODE_INTERFACE_H
