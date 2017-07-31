/*********************************************************************************************
(c) 2005-2013 Copyright, Real-Time Innovations, Inc.  All rights reserved.    	                             
RTI grants Licensee a license to use, modify, compile, and create derivative works 
of the Software.  Licensee has the right to distribute object form only for use with RTI 
products.  The Software is provided �as is�, with no warranty of any type, including 
any warranty for fitness for any purpose. RTI is under no obligation to maintain or 
support the Software.  RTI shall not be liable for any incidental or consequential 
damages arising out of the use or inability to use the software.
**********************************************************************************************/

#ifndef PUBLISHER_INTERFACE_H
#define PUBLISHER_INTERFACE_H

#include <sstream>
#include "../CommonInfrastructure/DDSCommunicator.h"
#include "../CommonInfrastructure/DDSTypeWrapper.h"

//using namespace com::rti::atc::generated;

// ----------------------------------------------------------------------------
//
// The interface provides data over the network (or
// shared memory) to other applications that are interested in flight plans.
//
// Writing data:
// -------------------------
// This application sends data, configured to behave as state data
// (or last-value cache).  This will reliably deliver each to both
// existing and late-joining applications that subscribe to data.
//
// For information on the data type, please see the 
// AirTrafficControl.idl file.  
//
// For information on the quality of service for state data, please
// see the flight_plan_profiles.xml file.
//
// ----------------------------------------------------------------------------
template <typename T>
class PublisherInterface : public T
{

public:

	// --- Constructor --- 
	// Initializes the interface, including creating a 
	// DomainParticipant, creating all publishers and subscribers, topics 
	// writers and readers.  Takes as input a vector of xml QoS files that
	// should be loaded to find QoS profiles and libraries.
	PublisherInterface(std::string topicName);

	// --- Destructor --- 
	~PublisherInterface();

	// --- Getter for Communicator --- 
	// Accessor for the communicator (the class that sets up the basic
	// DDS infrastructure like the DomainParticipant).
	// This allows access to the DDS DomainParticipant/Publisher/Subscriber
	// classes
	DDSCommunicator *GetCommunicator() 
	{ 
		return _communicator; 
	}

	// --- Sends the ---
	// Uses DDS interface to send a efficiently over the network
	// or shared memory to interested applications subscribing to flight plan
	// information.
	bool Write(DdsAutoType<T> data);

	// --- Deletes the ---
	// "Deletes" the from the system - removing the DDS instance 
	// from all applications.
	bool Delete(DdsAutoType<T> data);

private:
	// --- Private members ---

	// Used to create basic DDS entities that all applications need
	DDSCommunicator *_communicator;

	// publisher specific to this application
    DDS::DataWriter *_writer;
};



// ----------------------------------------------------------------------------
// The PublisherInterface is the network interface to the whole
// application.  This creates a DataWriter in order to send data
// over the network (or shared memory) to other applications that are
// interested in flight plans.
//
// This interface is built from:
// 1. Network data types and topic names defined in the IDL file
// 2. XML configuration files that describe the QoS profiles that should be
//    used by individual DataWriters and DataReaders.  These describe the
//    movement and persistence characteristics of the data (how reliable should
//    this be?), as well as other QoS such as resource limits.
// 3. The code itself creates DataWriters, and selects which QoS profile to use
//    when creating the DataWriters.
//
// Writing data:
// -------------------------
// This application sends data, configured to act as state data
// (or last-value cache).  This will reliably deliver the to both
// existing and late-joining applications that subscribe to data.
//
// For information on the data type, please see the
// AirTrafficControl.idl file.
//
// For information on the quality of service for state data, please
// see the flight_plan_profiles.xml file.
// ------------------------------------------------------------------------- //

template <typename T>
PublisherInterface<T>::PublisherInterface(std::string topicName)
{

    _communicator = new DDSCommunicator();

    // Create a DomainParticipant
    // Start by creating a DomainParticipant.  Generally you will have only
    // one DomainParticipant per application.  A DomainParticipant is
    // responsible for starting the discovery process, allocating resources,
    // and being the factory class used to create Publishers, Subscribers,
    // Topics, etc.  Note:  The string constants with the QoS library name and
    // the QoS profile name are configured as constants in the .idl file.  The
    // profiles themselves are configured in the .xml file.
//	if (NULL == _communicator->CreateParticipant(0, xmlFiles,
//				QOS_LIBRARY, QOS_PROFILE_FLIGHT_PLAN))
    if (NULL == _communicator->CreateParticipant())
    {
        std::stringstream errss;
        errss << "Failed to create DomainParticipant object";
        throw errss.str();
    }

    // Create a Publisher
    // This application only writes data, so we only need to create a
    // publisher.  The RadarData application has a more complex pattern
    // so we explicitly separate the writing interface from the overall
    // network interface - meaning that the publisher is created in the
    // network interface, and the DataWriter is created in a separate class
    // Note that one Publisher can be used to create multiple DataWriters
    DDS::Publisher *pub = _communicator->CreatePublisher();

    if (pub == NULL)
    {
        std::stringstream errss;
        errss << "Failed to create Publisher object";
        throw errss.str();
    }


    // Creating a Topic
    // The Topic object is the description of the data that you will be
    // sending. It associates a particular data type with a name that
    // describes the meaning of the data.  Along with the data types, and
    // whether your application is reading or writing particular data, this
    // is the data interface of your application.

    // This topic has the name AIRCRAFT_FLIGHT_PLAN_TOPIC - a constant
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
    _writer = pub->create_datawriter(topic,
                                      DDS_DATAWRITER_QOS_DEFAULT,
                                      NULL, DDS_STATUS_MASK_NONE);

    // Downcast the generic datawriter to a T DataWriter

    if (_writer == NULL)
    {
        std::stringstream errss;
        errss <<
              "Failure to create  writer. Inconsistent Qos?";
        throw errss.str();
    }

}

// ----------------------------------------------------------------------------
// Destructor.
// Deletes the DataWriter, and the Communicator object
template <typename T>
PublisherInterface<T>::~PublisherInterface()
{
	DDS::Publisher *pub = _writer->get_publisher();
	pub->delete_datawriter(_writer);
	_writer = NULL;

	delete _communicator;
}


// ----------------------------------------------------------------------------
// Sends the over a transport (such as shared memory or UDPv4)
// This writes the  data using RTI Connext DDS to any DataReader
// that shares the same Topic
template <typename T>
bool PublisherInterface<T>::Write(DdsAutoType<T> data)
{
	DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
	DDS_InstanceHandle_t handle = DDS_HANDLE_NIL;

	// This actually sends the data over the network.

	// The handle that the write() call takes is a handle to the individual
	// instance - a unique described by a
	// unique 8-character flight ID.

	// The data has a very simple ID, and does not need high-
	// throughput, so we are not bothering to pre-register the instance
	// handle.  If we did pre-register the instance handle, this could
	// potentially speed up the writing.
	retcode = T::DataWriter::narrow(_writer)->write(data, handle);

	if (retcode != DDS_RETCODE_OK)
	{
		return false;
	}

	return true;

}

// ----------------------------------------------------------------------------
// Sends a deletion message for the data over a transport (such as
// shared memory or UDPv4) This uses the unregiste_instance call to notify
// other applications that this has gone away and should be deleted
template <typename T>
bool PublisherInterface<T>::Delete(DdsAutoType<T> data)
{
	DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
	DDS_InstanceHandle_t handle = DDS_HANDLE_NIL;

	// Note that the deletion maps to an "unregister" in the RTI Connext
	// DDS world.  This allows the instance to be cleaned up entirely,
	// so the space can be reused for another instance.  If you call
	// "dispose" it will not clean up the space for a new instance -
	// instead it marks the current instance disposed and expects that you
	// might reuse the same instance again later.
	retcode = T::DataWriter::narrow(_writer)->unregister_instance(data, handle);

	if (retcode != DDS_RETCODE_OK)
	{
		return false;
	}

	return true;
}

#endif