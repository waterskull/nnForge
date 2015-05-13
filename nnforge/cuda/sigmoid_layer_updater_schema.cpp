/*
 *  Copyright 2011-2015 Maxim Milakov
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

#include "sigmoid_layer_updater_schema.h"

#include "../sigmoid_layer.h"

#include "activation_layer_cudnn_updater_cuda.h"
#include "sigmoid_partial_layer_updater_cuda.h"

namespace nnforge
{
	namespace cuda
	{
		sigmoid_layer_updater_schema::sigmoid_layer_updater_schema()
		{
		}

		sigmoid_layer_updater_schema::~sigmoid_layer_updater_schema()
		{
		}

		layer_updater_schema_smart_ptr sigmoid_layer_updater_schema::create_specific() const
		{
			return layer_updater_schema_smart_ptr(new sigmoid_layer_updater_schema());
		}

		const boost::uuids::uuid& sigmoid_layer_updater_schema::get_uuid() const
		{
			return sigmoid_layer::layer_guid;
		}

		layer_updater_cuda_smart_ptr sigmoid_layer_updater_schema::create_updater_specific(
			const layer_configuration_specific& input_configuration_specific,
			const layer_configuration_specific& output_configuration_specific) const
		{
			nnforge_shared_ptr<const sigmoid_layer> layer_derived = nnforge_dynamic_pointer_cast<const sigmoid_layer>(layer_schema);

			if (layer_derived->affected_feature_map_id_list.empty())
				return layer_updater_cuda_smart_ptr(new activation_layer_cudnn_updater_cuda(CUDNN_ACTIVATION_SIGMOID));
			else
				return layer_updater_cuda_smart_ptr(new sigmoid_partial_layer_updater_cuda());
		}

		std::vector<const_cuda_linear_buffer_device_smart_ptr> sigmoid_layer_updater_schema::get_schema_buffers() const
		{
			std::vector<const_cuda_linear_buffer_device_smart_ptr> res;

			nnforge_shared_ptr<const sigmoid_layer> layer_derived = nnforge_dynamic_pointer_cast<const sigmoid_layer>(layer_schema);
			if (!layer_derived->affected_feature_map_id_list.empty())
			{
				res.push_back(
					cuda_linear_buffer_device_smart_ptr(new cuda_linear_buffer_device(
						&(layer_derived->affected_feature_map_id_list.front()),
						layer_derived->affected_feature_map_id_list.size() * sizeof(unsigned int)))
					);
			}

			return res;
		}
	}
}
