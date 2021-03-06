ImageNet
========

Input data
----------

Input data should be organized in the following folder structure:

	input_data_imagenet/
		ILSVRC2012_img_train/
			n01440764/
				n01440764_10026.JPEG
				n01440764_10027.JPEG
				...
				n01440764_9981.JPEG
			n01443537/
				n01443537_10007.JPEG
				n01443537_10014.JPEG
				...
				n01443537_9977.JPEG
		ILSVRC2012_img_val/
			ILSVRC2012_val_00000001.JPEG
			ILSVRC2012_val_00000002.JPEG
			...
			ILSVRC2012_val_00050000.JPEG

Working folder should contain cls_class_info.txt file. See misc_files/generate_cls_class_info.m, it will generate the file for you.

Train
-----

	./imagenet prepare_training_data
	./imagenet create_normalizer --normalizer_layer_name images
	./imagenet create_resnet_schema
	./imagenet train --schema schema_resnet50.txt
	
Training will take a couple of weeks on modern GPU and will give you about 7.5% Top-5 error on single image inference on validation dataset.

Improved validation
-------------------

You can run training process multiple times (just run "train" again), have multiple networks trained this way, and run samples through all the nets averaging the output. You should get better results this way:

	./imagenet inference --schema schema_resnet50.txt --inference_mode dump_average_across_nets --inference_output_layer_name softmax
	./imagenet inference --schema schema_resnet50.txt --inference_force_data_layer_name softmax

Imagenet app also allows you to run different crops of each sample through the model, it works both with sinlge net and multiple nets:

	./imagenet inference --schema schema_resnet50.txt --inference_mode dump_average_across_nets --inference_output_layer_name softmax --rich_inference 1 --dump_compact_samples 32 --samples_x 4 --samples_y 4
	./imagenet inference --schema schema_resnet50.txt --inference_force_data_layer_name softmax

Here dump_compact_samples = samples_x * samples_y * 2 (mirroring)
