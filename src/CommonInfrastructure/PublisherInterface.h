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

using namespace com::rti::atc::generated;

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
template <class T>
class PublisherInterface
{

public:

	// --- Constructor --- 
	// Initializes the interface, including creating a 
	// DomainParticipant, creating all publishers and subscribers, topics 
	// writers and readers.  Takes as input a vector of xml QoS files that
	// should be loaded to find QoS profiles and libraries.
	PublisherInterface(std::vector<std::string> xmlFiles);

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
	DataWriter *_writer;
};

#endif