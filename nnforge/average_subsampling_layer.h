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

#pragma once

#include "layer.h"

#include <vector>
#include <limits>

namespace nnforge
{
	struct average_subsampling_factor
	{
		average_subsampling_factor(
			unsigned int factor = 0,
			bool is_relative = true)
		{
			raw_val = is_relative ? static_cast<int>(factor) : -static_cast<int>(factor);
		}

		bool is_relative() const
		{
			return (raw_val > 0);
		}

		unsigned int get_factor() const
		{
			return (raw_val > 0 ? static_cast<unsigned int>(raw_val) : static_cast<unsigned int>(-raw_val));
		}

	private:
		int raw_val;
	};

	// subsampling_sizes cannot be empty
	class average_subsampling_layer : public layer
	{
	public:
		average_subsampling_layer(
			const std::vector<average_subsampling_factor>& subsampling_sizes,
			average_subsampling_factor feature_map_subsampling_size = 1,
			unsigned int entry_subsampling_size = 1,
			float alpha = -std::numeric_limits<float>::max());

		virtual layer::ptr clone() const;

		virtual layer_configuration_specific get_output_layer_configuration_specific(const std::vector<layer_configuration_specific>& input_configuration_specific_list) const;

		virtual bool get_input_layer_configuration_specific(
			layer_configuration_specific& input_configuration_specific,
			const layer_configuration_specific& output_configuration_specific,
			unsigned int input_layer_id) const;

		virtual float get_flops_per_entry(
			const std::vector<layer_configuration_specific>& input_configuration_specific_list,
			const layer_action& action) const;

		virtual std::string get_type_name() const;

		virtual void write_proto(void * layer_proto) const;

		virtual void read_proto(const void * layer_proto);

		virtual std::vector<std::string> get_parameter_strings() const;

		virtual tiling_factor get_tiling_factor() const;

		float get_effective_alpha(
			const layer_configuration_specific& input_configuration_specific,
			const layer_configuration_specific& output_configuration_specific) const;

		static const std::string layer_type_name;

		unsigned int get_subsampling_size(
			unsigned int dimension_id,
			unsigned int input,
			unsigned int output) const;

		unsigned int get_fm_subsampling_size(
			unsigned int input,
			unsigned int output) const;

	private:
		void check();

	public:
		std::vector<average_subsampling_factor> subsampling_sizes; 
		average_subsampling_factor feature_map_subsampling_size;
		unsigned int entry_subsampling_size;
		float alpha;
	};
}
