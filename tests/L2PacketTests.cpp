//
// Created by Sebastian Lindner on 09.11.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include "../L2Packet.hpp"
//#include "../L2PacketCallback.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

class L2PacketTests : public CppUnit::TestFixture {
private:
	L2Packet* packet;

	class TestPayload : public L2Packet::Payload {
	public:
		TestPayload() : some_value(0) {}

		explicit TestPayload(int val) : some_value(val) {}

		unsigned int getBits() const override {
			return 1;
		}

		Payload* copy() const override {
			return new TestPayload(some_value);
		}

		int some_value;
	};

	class TestCallback : public L2PacketSentCallback {
	public:
		void packetBeingSentCallback(TUHH_INTAIRNET_MCSOTDMA::L2Packet* packet) override {
			num_packets++;
		}

		size_t num_packets = 0;
	};

public:
	void setUp() override {
		packet = new L2Packet();
	}

	void tearDown() override {
		delete packet;
	}

	void testCallback() {
		auto callback = TestCallback();
		CPPUNIT_ASSERT_EQUAL(size_t(0), callback.num_packets);
		packet->addCallback(&callback);
		packet->notifyCallbacks();
		CPPUNIT_ASSERT_EQUAL(size_t(1), callback.num_packets);
	}

	void testCopy() {
		MacId id = MacId(1), id2 = MacId(2);
		unsigned int offset = 3, length_next = 1, timeout = 12;
		bool use_arq = true;
		SequenceNumber arq_seqno = 12, ack_seqno = 5;		
		int payload_value = 42;
		packet->addMessage(new L2HeaderPP(id2, use_arq, arq_seqno, ack_seqno), new TestPayload(payload_value));

		L2Packet* copy = packet->copy();
		CPPUNIT_ASSERT_EQUAL(size_t(2), packet->getHeaders().size());
		CPPUNIT_ASSERT_EQUAL(copy->getHeaders().size(), packet->getHeaders().size());				
		CPPUNIT_ASSERT_EQUAL(packet->getPayloads().at(0), copy->getPayloads().at(0));
		auto* payload_1 = (TestPayload*) packet->getPayloads().at(1), * payload_2 = (TestPayload*) copy->getPayloads().at(1);
		CPPUNIT_ASSERT_EQUAL(payload_value, payload_1->some_value);
		CPPUNIT_ASSERT_EQUAL(payload_1->some_value, payload_2->some_value);
		delete copy;
	}

	void testPacketSize() {
		CPPUNIT_ASSERT_EQUAL(uint32_t(0), packet->getBits());				
		auto *unicast_header = new L2HeaderPP(MacId(42), false, SequenceNumber(1), SequenceNumber(2));
		auto *unicast_payload = new TestPayload();
		packet->addMessage(unicast_header, unicast_payload);
		CPPUNIT_ASSERT_EQUAL(unicast_header->getBits() + unicast_payload->getBits(), packet->getBits());
	}

	void testErase() {
		MacId id = MacId(1), id2 = MacId(2);
		unsigned int offset = 3, length_next = 1, timeout = 12;
		bool use_arq = true;
		SequenceNumber arq_seqno = 12, ack_seqno = 5;		
		int payload_value = 42;
		packet->addMessage(new L2HeaderPP(id2, use_arq, arq_seqno, ack_seqno), new TestPayload(payload_value));

		CPPUNIT_ASSERT_EQUAL(size_t(2), packet->getHeaders().size());
		CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::base, packet->getHeaders().at(0)->frame_type);
		CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::unicast, packet->getHeaders().at(1)->frame_type);		

		packet->erase(0);
		CPPUNIT_ASSERT_EQUAL(size_t(1), packet->getHeaders().size());
		CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::unicast, packet->getHeaders().at(0)->frame_type);		

		packet->erase(0);
		CPPUNIT_ASSERT_EQUAL(size_t(0), packet->getHeaders().size());
		CPPUNIT_ASSERT_THROW(packet->erase(0), std::invalid_argument);
	}

CPPUNIT_TEST_SUITE(L2PacketTests);		
		CPPUNIT_TEST(testCallback);
		CPPUNIT_TEST(testCopy);
		CPPUNIT_TEST(testPacketSize);
		CPPUNIT_TEST(testErase);
	CPPUNIT_TEST_SUITE_END();
};