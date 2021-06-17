//
// Created by seba on 6/16/21.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_RNGPROVIDER_HPP
#define INTAIRNET_LINKLAYER_GLUE_RNGPROVIDER_HPP

#include <map>
#include <stdexcept>
#include <random>
#include <functional>

namespace TUHH_INTAIRNET_MCSOTDMA {
	class IntRng {
	public:
		/**
		 * @param min Inclusive
		 * @param max Exclusive
		 * @return Random integer from [min, max).
		 */
		virtual int get(int min, int max) = 0;
		virtual ~IntRng() = default;
	};
	/**
	 * C++-default random number generator for integer values.
	 */
	class IntegerUniformRng : public IntRng {
	public:
		IntegerUniformRng();

		int get(int min, int max) override;

	protected:
		std::uniform_int_distribution<> dist;
		std::random_device random_device;
		std::mt19937 generator;
	};

	/**
	 * OMNeT++-provided random number generator for integer values.
	 */
	class OmnetIntegerUniformRng : public IntRng {
		friend class RngProviderTests;
	public:
		/**
		 * @param k Index of the OMNeT++-RNG that should be associated with this instance.
		 */
		explicit OmnetIntegerUniformRng(int k) : k(k) {}

		int get(int min, int max) override;

	protected:
		/** Association to the OMNeT++-provided RNG at index k. */
		int k;
	};

	/** Interface that classes that wish to obtain a random number generator must implement. */
	class IRng {
	public:
		/**
		 * Signs up to receive an integer RNG.
		 */
		IRng();

		/**
		 * @param min Inclusive.
		 * @param max Exclusive.
		 * @return A random integer from [min, max).
		 */
		int getRandomInt(int min, int max);
	};

	/**
	 * Provides Random Number Generators (RNGs) to classes.
	 * By default, it provides C++-standard generators, but it can be configured to use simulator-provided generators instead.
	 * Implemented through a singleton instance inspired by https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
	 */
	class RngProvider {
		friend class RngProviderTests;
	public:
		/** Function that should be replaced by the OMNeT++ simulator. */
		std::function<int (int min, int max, int k)> omnetGetInt = [] (int min, int max, int k) {throw std::runtime_error("not implemented"); return 0;};

		// Make sure there can't be >1 instances of the RngProvider.
		RngProvider(RngProvider const&) = delete;
		void operator=(RngProvider const&) = delete;

		/** Clears the <user, RNG> mapping. */
		~RngProvider() {
			reset();
		}

		static RngProvider& getInstance() {
			static RngProvider instance; // Instantiated on first use.
			return instance;
		}

		/**
		 * @param value Whether to use C++-default random number generators.
		 */
		void setUseDefaultRngs(bool value) {
			this->use_default_rngs = value;
		}

		/**
		 * Called once by each class that wishes to obtain an integer random number generator.
		 * @param caller
		 */
		void signupInt(IRng* caller) {
			if (int_rng_callers.find(caller) == int_rng_callers.end()) {
				if (use_default_rngs)
					int_rng_callers[caller] = new IntegerUniformRng();
				else
					int_rng_callers[caller] = new OmnetIntegerUniformRng((int) int_rng_callers.size());
			}
		}

		/**
		 * Retrieves the RNG saved for the caller, and utilizes it to obtain a random integer.
		 * @param caller
		 * @param min Inclusive
		 * @param max Exclusive
		 * @return A random integer from [min, max).
		 */
		int getInt(IRng* caller, int min, int max) {
			auto it = int_rng_callers.find(caller);
			if (it == int_rng_callers.end())
				throw std::invalid_argument("RngProvider::getRandomInt for unregistered caller.");
			return it->second->get(min, max);
		}

		/**
		 * Clears the <user, RNG> mappings.
		 */
		void reset() {
			// Delete just the RNGs, not the callers.
			for (auto pair : int_rng_callers)
				delete pair.second;
			int_rng_callers.clear();
		}

		/**
		 * Sets the function that is called whenever an OMNeT++-based RNG should be used.
		 * @param func
		 */
		void setOmnetGetInt(std::function<int (int min, int max, int k)> func) {
			omnetGetInt = func;
		}

	protected:
		/** Maps RNG-user to RNG. */
		std::map<IRng*, IntRng*> int_rng_callers;
		/** Whether to use C++-default RNGs when true, or OMNeT++-provided RNGs when false. */
		bool use_default_rngs = true;

	private:
		/** Private-hidden constructor as per singleton implementation pattern. */
		RngProvider() {}
	};
}


#endif //INTAIRNET_LINKLAYER_GLUE_RNGPROVIDER_HPP
