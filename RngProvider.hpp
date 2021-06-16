//
// Created by seba on 6/16/21.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_RNGPROVIDER_HPP
#define INTAIRNET_LINKLAYER_GLUE_RNGPROVIDER_HPP

#include <map>
#include <stdexcept>
#include <random>

namespace TUHH_INTAIRNET_MCSOTDMA {
	class IntRng {
	public:
		/**
		 * @param min Inclusive
		 * @param max Exclusive
		 * @return
		 */
		virtual int get(int min, int max) = 0;
		virtual ~IntRng() = default;
	};
	/**
	 *
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

	// Forward-declaration of users of RNGs.
	class IRng;

	/**
	 * Provides Random Number Generators (RNGs) to classes.
	 * By default, it provides C++-standard generators, but it can be configured to use simulator-provided generators instead.
	 * Implemented through a singleton instance inspired by https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
	 */
	class RngProvider {
	public:
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
					throw std::runtime_error("not yet implemented");
			}
		}

		int getInt(IRng* caller, int min, int max) {
			auto it = int_rng_callers.find(caller);
			if (it == int_rng_callers.end())
				throw std::invalid_argument("RngProvider::getRandomInt for unregistered caller.");
			return it->second->get(min, max);
		}

	private:
		RngProvider() {} // hide constructor

	protected:
		std::map<IRng*, IntRng*> int_rng_callers;
		bool use_default_rngs = true;

	public:
		RngProvider(RngProvider const&) = delete;
		void operator=(RngProvider const&) = delete;
		~RngProvider() {
			for (auto pair : int_rng_callers)
				delete pair.second;
		}
	};

	/** Interface that classes that wish to obtain a random number generator must implement. */
	class IRng {
	public:
		/**
		 * Signs up to receive an integer RNG.
		 */
		IRng();
		int getRandomInt(int min, int max);
	};
}


#endif //INTAIRNET_LINKLAYER_GLUE_RNGPROVIDER_HPP
