//
// Created by Sebastian Lindner on 07.12.20.
//

#ifndef TUHH_INTAIRNET_MC_SOTDMA_INET_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_INET_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {
	class IRlc; // Forward-declaration so that we can keep a pointer to the RLC sublayer.
	
	/**
	 * Specifies the interfacce the Network Layer must implement.
	 */
	class INet {
		public:
			virtual ~INet() = default;
			
			void setLowerLayer(IRlc* lower_layer) {
				this->lower_layer = lower_layer;
			}
			
			IRlc* getLowerLayer() {
				return this->lower_layer;
			}
			
			/**
			 * @return Number of hops to nearest ground station according to current routing information.
			 */
			virtual unsigned int getNumHopsToGroundStation() const = 0;
			
			/**
			 * When a neighbor's update comes in at a lower layer, this forwards this info to the Network Layer.
			 * @param id
			 * @param num_hops
			 */
			virtual void reportNumHopsToGS(const MacId& id, unsigned int num_hops) = 0;
		
		protected:
			IRlc* lower_layer = nullptr;
	};
}

#endif //TUHH_INTAIRNET_MC_SOTDMA_INET_HPP
