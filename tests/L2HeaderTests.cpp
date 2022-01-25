//
// Created by Sebastian Lindner on 09.11.20.
//


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include "../L2Header.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

class L2HeaderTests : public CppUnit::TestFixture {
private:
	L2Header* header;
	MacId id = MacId(42);
	unsigned int offset = 12;
	unsigned int length_next = 10;
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
		L2HeaderBase header_base = L2HeaderBase(id, offset, length_next, length_next, timeout);
		CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::base, header_base.frame_type);
		CPPUNIT_ASSERT(header_base.src_id == id);
		CPPUNIT_ASSERT_EQUAL(offset, header_base.burst_offset);
		CPPUNIT_ASSERT_EQUAL(length_next, header_base.burst_length);
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
		L2HeaderUnicast header_unicast = L2HeaderUnicast(dest_id, use_arq, arq_seqno, arq_ack_no);
		CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::unicast, header_unicast.frame_type);
		CPPUNIT_ASSERT_EQUAL(use_arq, header_unicast.use_arq);
		CPPUNIT_ASSERT(arq_seqno == header_unicast.seqno);
		CPPUNIT_ASSERT(arq_ack_no == header_unicast.seqno_next_expected);		


		SequenceNumber seqNo(1);

		header_unicast.setSeqno(seqNo);
		header_unicast.setSeqnoNextExpected(seqNo);
		auto srej_bitmap = header_unicast.getSrejList();



        srej_bitmap[10] = true;
        srej_bitmap[11] = true;

        header_unicast.srej_bitmap = srej_bitmap;

		int sum = 0;
		for(int i =0; i< srej_bitmap.size(); i++) {
		    if(srej_bitmap[i]) {
		        sum++;
		    }
		}

        CPPUNIT_ASSERT_EQUAL(sum, 2);
		CPPUNIT_ASSERT_EQUAL(header_unicast.getSrejList().size(), (unsigned long)16);
		CPPUNIT_ASSERT_EQUAL(header_unicast.getSeqno().get(), seqNo.get());
		CPPUNIT_ASSERT_EQUAL(header_unicast.getSeqnoNextExpected().get(), seqNo.get());

		auto copy = header_unicast.copy();
        auto oldSrej = header_unicast.getSrejList();
        auto newSrej = copy->getSrejList();

		for(int i= 0; i< newSrej.size(); i++) {
            CPPUNIT_ASSERT_EQUAL(oldSrej[i], newSrej[i]);
            std::cout << oldSrej[i] << std::endl;
		}

	}

	void testHeaderSizes_Base() {
		auto header = L2HeaderBase(id, offset, length_next, length_next, timeout);
		CPPUNIT_ASSERT_EQUAL(uint(152), header.getBits());		
	}

	void testHeaderSizes_Broadcast() {
		auto header = L2HeaderBroadcast();
		CPPUNIT_ASSERT_EQUAL(uint(32), header.getBits());		
	}

	void testHeaderSizes_Request() {
		auto header = L2HeaderLinkRequest();
		CPPUNIT_ASSERT_EQUAL(uint(80), header.getBits());		
	}

	void testHeaderSizes_Reply() {
		auto header = L2HeaderLinkReply();
		CPPUNIT_ASSERT_EQUAL(uint(72), header.getBits());		
	}

	void testHeaderSizes_Unicast() {
		auto header = L2HeaderUnicast();
		CPPUNIT_ASSERT_EQUAL(uint(88), header.getBits());		
	}

	void testHeaderSizes_Beacon() {
		auto header = L2HeaderBeacon();
		CPPUNIT_ASSERT_EQUAL(uint(64), header.getBits());		
	}


CPPUNIT_TEST_SUITE(L2HeaderTests);
		CPPUNIT_TEST(testHeader);
		CPPUNIT_TEST(testBaseHeader);
		CPPUNIT_TEST(testBroadcastHeader);
		CPPUNIT_TEST(testUnicastHeader);
		CPPUNIT_TEST(testHeaderSizes_Base);
		CPPUNIT_TEST(testHeaderSizes_Broadcast);
		CPPUNIT_TEST(testHeaderSizes_Request);
		CPPUNIT_TEST(testHeaderSizes_Reply);
		CPPUNIT_TEST(testHeaderSizes_Unicast);
		CPPUNIT_TEST(testHeaderSizes_Beacon);
	CPPUNIT_TEST_SUITE_END();
};