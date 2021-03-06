/*
 *  Copyright 2011-2016 Maxim Milakov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "add_layer_tester_plain.h"

#include "../add_layer.h"

#include <cstring>

namespace nnforge
{
	namespace plain
	{
		add_layer_tester_plain::add_layer_tester_plain()
		{
		}

		add_layer_tester_plain::~add_layer_tester_plain()
		{
		}

		std::string add_layer_tester_plain::get_type_name() const
		{
			return add_layer::layer_type_name;
		}

		void add_layer_tester_plain::run_forward_propagation(
			plain_buffer::ptr output_buffer,
			const std::vector<plain_buffer::const_ptr>& input_buffers,
			plain_buffer::ptr temporary_working_fixed_buffer,
			plain_buffer::ptr temporary_working_per_entry_buffer,
			plain_running_configuration::const_ptr plain_config,
			layer::const_ptr layer_schema,
			layer_data::const_ptr data,
			layer_data_custom::const_ptr data_custom,
			const std::vector<layer_configuration_specific>& input_configuration_specific_list,
			const layer_configuration_specific& output_configuration_specific,
			unsigned int entry_count) const
		{
			float * const out = *output_buffer;
			std::vector<const float *> in_list;
			for(std::vector<plain_buffer::const_ptr>::const_iterator it = input_buffers.begin(); it != input_buffers.end(); ++it)
				in_list.push_back(**it);
			const float ** const in_ptr_list = &in_list[0];
			nnforge_shared_ptr<const add_layer> layer_derived = nnforge_dynamic_pointer_cast<const add_layer>(layer_schema);
			const float alpha = layer_derived->alpha;
			const int src_ptr_count = static_cast<int>(in_list.size());
			const int elem_count = static_cast<int>(entry_count * output_configuration_specific.get_neuron_count());
			#pragma omp parallel for default(none) schedule(guided) num_threads(plain_config->openmp_thread_count)
			for(int i = 0; i < elem_count; ++i)
			{
				float sum = 0.0F;
				for(int j = 0; j < src_ptr_count; ++j)
					sum += in_ptr_list[j][i];
				out[i] = sum * alpha;
			}
		}

		int add_layer_tester_plain::get_input_index_layer_can_write(
			plain_running_configuration::const_ptr plain_config,
			layer::const_ptr layer_schema,
			const std::vector<layer_configuration_specific>& input_configuration_specific_list,
			const layer_configuration_specific& output_configuration_specific) const
		{
			return 0;
		}
	}
}
