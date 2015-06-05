#pragma once

#include "stdafx.h"

using namespace VersionOne::TestComplete::V1Connector;

namespace VersionOne {
	namespace TestComplete {
		namespace ManagedWrapper {

			ref class ComponentRepository
			{
			private:
				ComponentRepository();

				static ComponentRepository^ instance;

				Logger^ logger;
                IWorkitemWriter^ workitemWriter;

			public:
				property ComponentRepository^ Instance {
					static ComponentRepository^ get() {
						if(instance == nullptr) {
							instance = gcnew ComponentRepository();
						}

						return instance;
					}
				}

				Logger^ GetLogger();

                IWorkitemWriter^ GetWorkitemWriter();
			};

		}
	}
}