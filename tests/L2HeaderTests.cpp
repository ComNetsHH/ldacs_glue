// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

	void testBroadcastHeader() {
		L2HeaderSH header_broadcast = L2HeaderSH();
		CPPUNIT_ASSERT_EQUAL(L2Header::FrameType::broadcast, header_broadcast.frame_type);
	}

	void testUnicastHeader() {
		MacId dest_id = MacId(99);
		bool use_arq = true;
		SequenceNumber arq_seqno = SequenceNumber(50);
		SequenceNumber arq_ack_no = SequenceNumber(51);		
		L2HeaderPP header_unicast = L2HeaderPP(dest_id, use_arq, arq_seqno, arq_ack_no);
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

CPPUNIT_TEST_SUITE(L2HeaderTests);
		CPPUNIT_TEST(testHeader);		
		CPPUNIT_TEST(testBroadcastHeader);
		CPPUNIT_TEST(testUnicastHeader);
	CPPUNIT_TEST_SUITE_END();
};