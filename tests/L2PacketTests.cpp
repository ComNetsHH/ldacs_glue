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

	void testAddPayload() {
		L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101);
		bool exception_occurred = false;
		// Can't add a non-base header as first header.
		try {
			packet->addMessage(unicast_header, nullptr);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(true, exception_occurred);

		// Can add a base header.
		L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 13, 14);
		exception_occurred = false;
		try {
			packet->addMessage(base_header, nullptr);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(false, exception_occurred);

		// Now can add any other header.
		exception_occurred = false;
		try {
			packet->addMessage(unicast_header, nullptr);
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(false, exception_occurred);

		// But not another base header.
		exception_occurred = false;
		try {
			packet->addMessage(base_header, nullptr);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
	}

	void testUnicastPayload() {
		L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101);
		L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 13, 14);
		TestPayload* payload = nullptr;

		// Add a base and a unicast header.
		bool exception_occurred = false;
		try {
			packet->addMessage(base_header, payload);
			packet->addMessage(unicast_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(false, exception_occurred);

		// Shouldn't be able to add a unicast header to a different destination.
		MacId id_dest_2 = MacId(44);
		L2HeaderUnicast* second_unicast_header = new L2HeaderUnicast(id_dest_2, true, 100, 101);
		exception_occurred = false;
		try {
			packet->addMessage(second_unicast_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
			delete second_unicast_header;
		}
		CPPUNIT_ASSERT_EQUAL(true, exception_occurred);

		// Shouldn't be able to add a broadcast header.
		exception_occurred = false;
		try {
			L2HeaderBroadcast broadcast_header = L2HeaderBroadcast();
			packet->addMessage(&broadcast_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(true, exception_occurred);

		// Shouldn't be able to add a beacon header.
		exception_occurred = false;
		try {
			L2HeaderBeacon beacon_header = L2HeaderBeacon(CPRPosition(), CPRPosition().odd, L2HeaderBeacon::CongestionLevel::uncongested, CPRPosition::PositionQuality::hi);
			packet->addMessage(&beacon_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
	}

	void testBroadcastPayload() {
		L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 13, 14);
		L2HeaderBroadcast* broadcast_header = new L2HeaderBroadcast();
		L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101);
		TestPayload* payload = nullptr;

		// Add a base and a broadcast header and then a unicast header.
		L2Packet another_packet = L2Packet();
		another_packet.addMessage(base_header->copy(), payload);
		another_packet.addMessage(broadcast_header->copy(), payload);
		CPPUNIT_ASSERT_NO_THROW(another_packet.addMessage(unicast_header, payload));

		packet->addMessage(base_header, payload);
		packet->addMessage(broadcast_header, payload);
		CPPUNIT_ASSERT_NO_THROW(packet->addMessage(broadcast_header->copy(), payload));

		// Shouldn't be able to add a beacon header.
		auto *beacon_header = new L2HeaderBeacon(CPRPosition(), CPRPosition().odd, L2HeaderBeacon::CongestionLevel::uncongested, CPRPosition::PositionQuality::hi);
		CPPUNIT_ASSERT_THROW(packet->addMessage(beacon_header, payload), std::runtime_error);
		delete beacon_header;
	}

	void testBeaconPayload() {
		L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 13, 14);
		L2HeaderBeacon* beacon_header = new L2HeaderBeacon(CPRPosition(), CPRPosition().odd, L2HeaderBeacon::CongestionLevel::uncongested, CPRPosition::PositionQuality::hi);
		TestPayload* payload = nullptr;
		// Should be able to add a beacon header.
		bool exception_occurred = false;
		try {
			packet->addMessage(base_header, payload);
			packet->addMessage(beacon_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(false, exception_occurred);

		// Should be able to add a broadcast header.
		exception_occurred = false;
		try {
			L2HeaderBroadcast* broadcast_header = new L2HeaderBroadcast();
			packet->addMessage(broadcast_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(false, exception_occurred);

		// Should be able to add a unicast header.
		exception_occurred = false;
		try {
			L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101);
			packet->addMessage(unicast_header, payload);
		} catch (const std::exception& e) {
			exception_occurred = true;
		}
		CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
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
		packet->addMessage(new L2HeaderBase(id, offset, length_next, length_next, timeout), nullptr);
		int payload_value = 42;
		packet->addMessage(new L2HeaderUnicast(id2, use_arq, arq_seqno, ack_seqno), new TestPayload(payload_value));

		L2Packet* copy = packet->copy();
		CPPUNIT_ASSERT_EQUAL(size_t(2), packet->getHeaders().size());
		CPPUNIT_ASSERT_EQUAL(copy->getHeaders().size(), packet->getHeaders().size());
		auto* base_header_1 = (L2HeaderBase*) packet->getHeaders().at(0), * base_header_2 = (L2HeaderBase*) copy->getHeaders().at(0);
		CPPUNIT_ASSERT(*base_header_1 == *base_header_2);
		CPPUNIT_ASSERT_EQUAL(packet->getPayloads().at(0), copy->getPayloads().at(0));
		auto* payload_1 = (TestPayload*) packet->getPayloads().at(1), * payload_2 = (TestPayload*) copy->getPayloads().at(1);
		CPPUNIT_ASSERT_EQUAL(payload_value, payload_1->some_value);
		CPPUNIT_ASSERT_EQUAL(payload_1->some_value, payload_2->some_value);
		delete copy;
	}

	void testPacketSize() {
		CPPUNIT_ASSERT_EQUAL(uint32_t(0), packet->getBits());
		auto *base_header = new L2HeaderBase();
		packet->addMessage(base_header, nullptr);
		CPPUNIT_ASSERT_EQUAL(base_header->getBits(), packet->getBits());
		auto *unicast_header = new L2HeaderUnicast(MacId(42), false, SequenceNumber(1), SequenceNumber(2));
		auto *unicast_payload = new TestPayload();
		packet->addMessage(unicast_header, unicast_payload);
		CPPUNIT_ASSERT_EQUAL(base_header->getBits() + unicast_header->getBits() + unicast_payload->getBits(), packet->getBits());
	}

	void testErase() {
		MacId id = MacId(1), id2 = MacId(2);
		unsigned int offset = 3, length_next = 1, timeout = 12;
		bool use_arq = true;
		SequenceNumber arq_seqno = 12, ack_seqno = 5;
		packet->addMessage(new L2HeaderBase(id, offset, length_next, length_next, timeout), nullptr);
		int payload_value = 42;
		packet->addMessage(new L2HeaderUnicast(id2, use_arq, arq_seqno, ack_seqno), new TestPayload(payload_value));

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
		CPPUNIT_TEST(testAddPayload);
		CPPUNIT_TEST(testUnicastPayload);
		CPPUNIT_TEST(testBroadcastPayload);
		CPPUNIT_TEST(testBeaconPayload);
		CPPUNIT_TEST(testCallback);
		CPPUNIT_TEST(testCopy);
		CPPUNIT_TEST(testPacketSize);
		CPPUNIT_TEST(testErase);
	CPPUNIT_TEST_SUITE_END();
};