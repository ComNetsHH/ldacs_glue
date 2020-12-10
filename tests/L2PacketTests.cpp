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
				unsigned int getBits() const override {
					return 1;
				}
		};
		
		class TestCallback : public L2PacketSentCallback {
			public:
				void notifyPacketBeingSent(TUHH_INTAIRNET_MCSOTDMA::L2Packet* packet) override {
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
			L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101, 102);
			bool exception_occurred = false;
			// Can't add a non-base header as first header.
			try {
				packet->addPayload(unicast_header, nullptr);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
			
			// Can add a base header.
			L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 14);
			exception_occurred = false;
			try {
				packet->addPayload(base_header, nullptr);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
			
			// Now can add any other header.
			exception_occurred = false;
			try {
				packet->addPayload(unicast_header, nullptr);
			} catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
			
			// But not another base header.
			exception_occurred = false;
			try {
				packet->addPayload(base_header, nullptr);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
		}
		
		void testUnicastPayload() {
			L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101, 102);
			L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 14);
			TestPayload* payload = nullptr;
			
			// Add a base and a unicast header.
			bool exception_occurred = false;
			try {
				packet->addPayload(base_header, payload);
				packet->addPayload(unicast_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
			
			// Shouldn't be able to add a unicast header to a different destination.
			MacId id_dest_2 = MacId(44);
			L2HeaderUnicast* second_unicast_header = new L2HeaderUnicast(id_dest_2, true, 100, 101, 102);
			exception_occurred = false;
			try {
				packet->addPayload(second_unicast_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
				delete second_unicast_header;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
			
			// Shouldn't be able to add a broadcast header.
			exception_occurred = false;
			try {
				L2HeaderBroadcast broadcast_header = L2HeaderBroadcast();
				packet->addPayload(&broadcast_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
			
			// Shouldn't be able to add a beacon header.
			exception_occurred = false;
			try {
				L2HeaderBeacon beacon_header = L2HeaderBeacon(CPRPosition(), CPRPosition().odd, 2, CPRPosition::PositionQuality::hi);
				packet->addPayload(&beacon_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
		}
		
		void testBroadcastPayload() {
			L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 14);
			L2HeaderBroadcast* broadcast_header = new L2HeaderBroadcast();
			L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101, 102);
			TestPayload* payload = nullptr;
			
			// Add a base and a broadcast header and then a unicast header.
			bool exception_occurred = false;
			try {
				packet->addPayload(base_header, payload);
				packet->addPayload(broadcast_header, payload);
				packet->addPayload(unicast_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
			
			// Shouldn't be able to add a broadcast header.
			exception_occurred = false;
			try {
				packet->addPayload(broadcast_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
			
			// Shouldn't be able to add a beacon header.
			exception_occurred = false;
			try {
				L2HeaderBeacon beacon_header = L2HeaderBeacon(CPRPosition(), CPRPosition().odd, 2, CPRPosition::PositionQuality::hi);
				packet->addPayload(&beacon_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, exception_occurred);
		}
		
		void testBeaconPayload() {
			L2HeaderBase* base_header = new L2HeaderBase(MacId(42), 12, 13, 14);
			L2HeaderBeacon* beacon_header = new L2HeaderBeacon(CPRPosition(), CPRPosition().odd, 50, CPRPosition::PositionQuality::hi);
			TestPayload* payload = nullptr;
			// Should be able to add a beacon header.
			bool exception_occurred = false;
			try {
				packet->addPayload(base_header, payload);
				packet->addPayload(beacon_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
			
			// Should be able to add a broadcast header.
			exception_occurred = false;
			try {
				L2HeaderBroadcast* broadcast_header = new L2HeaderBroadcast();
				packet->addPayload(broadcast_header, payload);
			} catch (const std::exception& e) {
				exception_occurred = true;
			}
			CPPUNIT_ASSERT_EQUAL(false, exception_occurred);
			
			// Should be able to add a unicast header.
			exception_occurred = false;
			try {
				L2HeaderUnicast* unicast_header = new L2HeaderUnicast(MacId(43), true, 100, 101, 102);
				packet->addPayload(unicast_header, payload);
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
	
	CPPUNIT_TEST_SUITE(L2PacketTests);
		CPPUNIT_TEST(testAddPayload);
		CPPUNIT_TEST(testUnicastPayload);
		CPPUNIT_TEST(testBroadcastPayload);
		CPPUNIT_TEST(testBeaconPayload);
		CPPUNIT_TEST(testCallback);
	CPPUNIT_TEST_SUITE_END();
};