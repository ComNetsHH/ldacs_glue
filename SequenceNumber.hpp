//
// Created by Konrad Fuger on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_SEQUENCENUMBER_HPP
#define INTAIRNET_LINKLAYER_GLUE_SEQUENCENUMBER_HPP

#include <cstdint>

namespace TUHH_INTAIRNET_MCSOTDMA {
	class SequenceNumber {
	public:
		SequenceNumber();

		SequenceNumber(uint8_t sequence_number);

		SequenceNumber(const SequenceNumber& other);

		bool operator==(const SequenceNumber& other) const;

		bool operator>(const SequenceNumber& other) const;

		bool operator<(const SequenceNumber& other) const;

		SequenceNumber operator+(uint8_t increment);

		SequenceNumber operator-(uint8_t increment);

		bool isHigherThan(SequenceNumber other, uint8_t window_size);

		bool isLowerThan(SequenceNumber other, uint8_t window_size);

		bool isLowerThanEqual(SequenceNumber other, uint8_t window_size);

		uint8_t get() const;

		void increment();

		void decrement();

        SequenceNumber next();

        SequenceNumber prev();

	protected:
		uint8_t sequence_number;
	};

	static const uint8_t SEQNO_MAX = 255;
	static const uint8_t SEQNO_UNSET = 0;
	static const uint8_t SEQNO_FIRST = 1;
	static const uint8_t NUM_BYTES = 1;
}

#endif //INTAIRNET_LINKLAYER_GLUE_SEQUENCENUMBER_HPP
