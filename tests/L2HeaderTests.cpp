//
// Created by Sebastian Lindner on 09.11.20.
//


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../L2Header.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

class L2HeaderTests : public CppUnit::TestFixture {
	private:
		L2Header* header;
		MacId id = MacId(42);
		unsigned int offset = 12;
		unsigned short length_current = 13;
		unsigned short length_next = 10;
		unsigned int timeout = 12;
	
	public:
		void setUp() override {
			header = new L2Header(L2Header::FrameType::unset);
		}
		
		void tearDown() override {
			delete header;
		}
		
		void testHeader() {
			CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::unset, header->frame_type);
		}
		
		void testBaseHeader() {
			L2HeaderBase header_base = L2HeaderBase(id, offset, length_current, length_next, timeout);
			CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::base, header_base.frame_type);
			CPPUNIT_ASSERT(header_base.getId() == id);
			CPPUNIT_ASSERT_EQUAL(offset, header_base.offset);
			CPPUNIT_ASSERT_EQUAL(length_current, header_base.length_current);
			CPPUNIT_ASSERT_EQUAL(length_next, header_base.length_next);
			CPPUNIT_ASSERT_EQUAL(timeout, header_base.timeout);
		}
		
		void testBroadcastHeader() {
			L2HeaderBroadcast header_broadcast = L2HeaderBroadcast();
			CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::broadcast, header_broadcast.frame_type);
		}
		
		void testUnicastHeader() {
			MacId dest_id = MacId(99);
			bool use_arq = true;
			SequenceNumber arq_seqno = SequenceNumber(50);
			SequenceNumber arq_ack_no = SequenceNumber(51);
			unsigned int arq_ack_slot = 52;
			L2HeaderUnicast header_unicast = L2HeaderUnicast(dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot);
			CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::unicast, header_unicast.frame_type);
			CPPUNIT_ASSERT_EQUAL(use_arq, header_unicast.use_arq);
			CPPUNIT_ASSERT(arq_seqno == header_unicast.seqno);
			CPPUNIT_ASSERT(arq_ack_no == header_unicast.seqno_next_expected);
			CPPUNIT_ASSERT_EQUAL(arq_ack_slot, header_unicast.arq_ack_slot);


            SequenceNumber seqNo(1);
            std::vector<SequenceNumber> selRejList;
            selRejList.push_back(SequenceNumber(2));
            selRejList.push_back(SequenceNumber(3));

            header_unicast.setSeqno(seqNo);
            header_unicast.setSeqnoNextExpected(seqNo);
            header_unicast.setSrejList(selRejList);

            CPPUNIT_ASSERT_EQUAL(header_unicast.getSrejListLength(), uint8_t(2));
            CPPUNIT_ASSERT_EQUAL(header_unicast.getSrejList().size(), (unsigned long) 2);
            CPPUNIT_ASSERT_EQUAL(header_unicast.getSeqno().get(), seqNo.get());
            CPPUNIT_ASSERT_EQUAL(header_unicast.getSeqnoNextExpected().get(), seqNo.get());
		}
		
		void testHeaderSizes() {
			L2HeaderBase base_header = L2HeaderBase(id, offset, length_current, length_next, timeout);
			CPPUNIT_ASSERT_EQUAL(uint(70), base_header.getBits());
			
			MacId dest_id = MacId(99);
			bool use_arq = true;
			SequenceNumber arq_seqno = SequenceNumber(50);
			SequenceNumber arq_ack_no = SequenceNumber(51);
			unsigned int arq_ack_slot = 52;
			L2HeaderUnicast unicast_header = L2HeaderUnicast(dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot);
			CPPUNIT_ASSERT_EQUAL(uint(71), unicast_header.getBits());

            std::vector<SequenceNumber> selRejList;
            selRejList.push_back(SequenceNumber(2));
            selRejList.push_back(SequenceNumber(3));
			unicast_header.setSrejList(selRejList);
            CPPUNIT_ASSERT_EQUAL(uint(87), unicast_header.getBits());
			
			L2HeaderBroadcast broadcast_header = L2HeaderBroadcast();
			CPPUNIT_ASSERT_EQUAL(uint(19), broadcast_header.getBits());
			
			L2Header simple_header = L2Header(L2Header::FrameType::unset);
			CPPUNIT_ASSERT(simple_header.getBits() == broadcast_header.getBits());
			
			L2HeaderBeacon beacon_header = L2HeaderBeacon(CPRPosition(1, 2, 3), true, 12, 1);
			CPPUNIT_ASSERT_EQUAL(uint(65), beacon_header.getBits());
		}

	
	CPPUNIT_TEST_SUITE(L2HeaderTests);
		CPPUNIT_TEST(testHeader);
		CPPUNIT_TEST(testBaseHeader);
		CPPUNIT_TEST(testBroadcastHeader);
		CPPUNIT_TEST(testUnicastHeader);
		CPPUNIT_TEST(testHeaderSizes);
	CPPUNIT_TEST_SUITE_END();
};