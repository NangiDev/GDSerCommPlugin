#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sercomm/sercomm.h>

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;
const godot_gdnative_ext_nativescript_1_1_api_struct *nativescript_api_1_1 = NULL;

GDCALLINGCONV void *SERCOMM_constructor(godot_object *p_instance, void *p_method_data);
GDCALLINGCONV void SERCOMM_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);
godot_variant sercomm_list_ports(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant sercomm_flush(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant sercomm_get_available(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant sercomm_read(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant sercomm_write(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant sercomm_open(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant sercomm_close(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	api = p_options->api_struct;

	// now find our extensions
	for (uint32_t i = 0; i < api->num_extensions; i++) {
		switch (api->extensions[i]->type) {
		case GDNATIVE_EXT_NATIVESCRIPT: {
			nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];
			//nativescript 1.1 compat
			const godot_gdnative_api_struct *extension = nativescript_api->next;
			while (extension) {
				if (extension->version.major == 1 && extension->version.minor == 1) {
					nativescript_api_1_1 = (const godot_gdnative_ext_nativescript_1_1_api_struct *)extension;
				}

				extension = extension->next;
			}
		}; break;
		default: break;
		};
	};
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	api = NULL;
	nativescript_api = NULL;
	nativescript_api_1_1 = NULL;
}

void GDN_EXPORT godot_nativescript_init(void *p_handle) {
	godot_instance_create_func create = { NULL, NULL, NULL };
	create.create_func = &SERCOMM_constructor;

	godot_instance_destroy_func destroy = { NULL, NULL, NULL };
	destroy.destroy_func = &SERCOMM_destructor;

	nativescript_api->godot_nativescript_register_class(p_handle, "SERCOMM", "Reference", create, destroy);

	godot_instance_method sercomm_list = { NULL, NULL, NULL };
	sercomm_list.method = &sercomm_list_ports;
	godot_method_attributes attributes = { GODOT_METHOD_RPC_MODE_DISABLED };
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "list_ports", attributes, sercomm_list);

	godot_instance_method sercomm_flush_queue = { NULL, NULL, NULL };
	sercomm_flush_queue.method = &sercomm_flush;
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "flush", attributes, sercomm_flush_queue);

	godot_instance_method sercomm_available_queue = { NULL, NULL, NULL };
	sercomm_available_queue.method = &sercomm_get_available;
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "get_available", attributes, sercomm_available_queue);

	godot_instance_method sercomm_read_byte = { NULL, NULL, NULL };
	sercomm_read_byte.method = &sercomm_read;
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "read", attributes, sercomm_read_byte);

	godot_instance_method sercomm_write_byte = { NULL, NULL, NULL };
	sercomm_write_byte.method = &sercomm_write;
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "write", attributes, sercomm_write_byte);

	godot_instance_method sercomm_open_port = { NULL, NULL, NULL };
	sercomm_open_port.method = &sercomm_open;
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "open", attributes, sercomm_open_port);

	godot_instance_method sercomm_close_port = { NULL, NULL, NULL };
	sercomm_close_port.method = &sercomm_close;
	nativescript_api->godot_nativescript_register_method(p_handle, "SERCOMM", "close", attributes, sercomm_close_port);

	//check if it has nativescript 1.1, needed for documentation
	if (nativescript_api_1_1 != NULL) { 
		godot_string test;
		api->godot_string_new(&test);

		api->godot_string_parse_utf8(&test, "Sercomm is a serial communication library.");
		nativescript_api_1_1->godot_nativescript_set_class_documentation(p_handle, "SERCOMM", test);

		api->godot_string_parse_utf8(&test, "Lists available serial devices, can see the extra devices codes in debug window.");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "list_ports", test);

		api->godot_string_parse_utf8(&test, "Flushes a buffer. Params: 0 (DEFAULT) for input buffer; 1 for output buffer and 2 for all buffers.");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "flush", test);

		api->godot_string_parse_utf8(&test, "Returns number of available bytes for reading. Returns negative integer at fail.");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "get_available", test);

		api->godot_string_parse_utf8(&test, "Secure read of a char in the input buffer, if fails, it will return a negative integer instead of a string. Params: 0 (DEFAULT); 1 (Read raw value)");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "read", test);

		api->godot_string_parse_utf8(&test, "Write an ASCII string to the output buffer, returns 0 in success, negative integer otherwise.");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "write", test);

		api->godot_string_parse_utf8(&test, "Opens a serial port, given it's (Name,Baudrate,Timeout), returns 0 on success, negative integer otherwise. Can also accept 3 extra params (bytesize=0(8),parity=0,stop_byte=1)");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "open", test);

		api->godot_string_parse_utf8(&test, "Closes a serial port. Returns a \"closed\" string");
		nativescript_api_1_1->godot_nativescript_set_method_documentation(p_handle, "SERCOMM", "close", test);

		api->godot_string_destroy(&test);
	}
}

GDCALLINGCONV void *SERCOMM_constructor(godot_object *p_instance, void *p_method_data) {
	printf("SERCOMM._init()\n");
	ser_t *ser = api->godot_alloc(sizeof(ser_t*));
	ser = ser_create();
	return ser;
}

GDCALLINGCONV void SERCOMM_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data) {
	ser_t *ser = (ser_t *)p_user_data;
	ser_close(ser);
	ser_destroy(ser);
	printf("SERCOMM._byebye()\n");
}

godot_variant sercomm_list_ports(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_string data;
	godot_variant ret;

	int cha = 1;
	ser_dev_list_t *lst;

	godot_string s;

	godot_pool_string_array port_list;

	lst = ser_dev_list_get();

	if (lst != NULL)
	{
		cha = 0;

		ser_dev_list_t *item;

		api->godot_pool_string_array_new(&port_list);

		ser_dev_list_foreach(item, lst)
		{
			printf("\t%s", item->dev.path);

			api->godot_string_new(&s);
			api->godot_string_parse_utf8(&s, item->dev.path);
			api->godot_pool_string_array_append(&port_list, &s);
			api->godot_string_destroy(&s);

			if (item->dev.vid != 0)
			{
				printf(", 0x%04x:", item->dev.vid);
			}

			if (item->dev.pid != 0)
			{
				printf("0x%04x", item->dev.pid);
			}

			printf("\n");
		}
		api->godot_variant_new_pool_string_array(&ret, &port_list);
		api->godot_pool_string_array_destroy(&port_list);
	}

	else {
		api->godot_string_new(&data);
		api->godot_string_parse_utf8(&data, "NO DEVICES DETECTED");
		api->godot_variant_new_string(&ret, &data);
		api->godot_string_destroy(&data);
	}

	ser_dev_list_destroy(lst);

	return ret;
}

godot_variant sercomm_flush(godot_object * p_instance, void * p_method_data, void * p_user_data, int p_num_args, godot_variant ** p_args) {
	godot_variant ret;
	ser_t *ser = (ser_t *)p_user_data;
	ser_queue_t queue = SER_QUEUE_IN;
	int64_t val = 0;

	val = api->godot_variant_as_int(p_args[0]);
	if (val < 3 && val>0) {
		queue = val;
	}

	val = ser_flush(ser, queue);

	api->godot_variant_new_int(&ret, val);
	return ret;
}

godot_variant sercomm_get_available(godot_object * p_instance, void * p_method_data, void * p_user_data, int p_num_args, godot_variant ** p_args) {
	godot_variant ret;
	ser_t *ser = (ser_t *)p_user_data;
	size_t num = 0;
	int32_t val = 0;
		
	val=ser_available(ser,&num);

	if (val < 0) {
		api->godot_variant_new_int(&ret, val);
	}
	else {
		api->godot_variant_new_int(&ret, num);
	}
	return ret;
}

godot_variant sercomm_read(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	godot_string data;
	ser_t *ser = (ser_t *)p_user_data;

	int32_t r = 0;

	unsigned char c;

	int32_t	raw= (int32_t)api->godot_variant_as_int(p_args[0]);

	r = ser_read(ser, &c, sizeof(c), NULL);
	if (r == SER_EEMPTY)
	{
		r = ser_read_wait(ser);
		if (r == SER_ETIMEDOUT)
		{
			printf("Timeout\n");
			goto failed_read;
		}
		else if (r < 0)
		{
			printf("Error while waiting: %s\n", sererr_last());
			goto failed_read;
		}
	}
	else if (r < 0)
	{
		printf("Could not read: %s\n", sererr_last());
		goto failed_read;
	}
	else
	{	
		if (raw==1){
			api->godot_variant_new_int(&ret, (int)c);
			return ret;
		}

		api->godot_string_new(&data);
		#if defined(_MSC_VER)
			uint32_t chango = c;	// godot needs integer instead of char for at least windows (workaround)
			api->godot_string_parse_utf8(&data, &(unsigned char)chango);
		#else
			api->godot_string_parse_utf8(&data, &c);
		#endif
		api->godot_variant_new_string(&ret, &data);
		api->godot_string_destroy(&data);
		return ret;
	}

failed_read:

	api->godot_variant_new_int(&ret, (int64_t)r);

	return ret;
}

godot_variant sercomm_write(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	ser_t *ser = (ser_t *)p_user_data;

	int32_t r = 0;

	const unsigned char* c;

	godot_string cho;
	godot_char_string cha;

	cho = api->godot_variant_as_string(*p_args);
	cha = api->godot_string_ascii(&cho);

	int length = (int)api->godot_char_string_length(&cha);

	c = api->godot_char_string_get_data(&cha);

	//serializing due to the unicode bug
	int i = 0;
	for (i; i < length; i++) {
		r = ser_write(ser, &c[i], sizeof(c[i]), NULL);
		if (r != 0) {
			printf("Failed write\n");
			break;
		}
	}
	api->godot_string_destroy(&cho);
	api->godot_char_string_destroy(&cha);
	api->godot_variant_new_int(&ret, (int64_t)r);

	return ret;
}

godot_variant sercomm_open(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	godot_string data;
	ser_t *ser = (ser_t *)p_user_data;

	godot_string cho;
	godot_char_string cha;

	cho = api->godot_variant_as_string(p_args[0]);
	cha = api->godot_string_ascii(&cho);

	const char *port = api->godot_char_string_get_data(&cha);

	uint32_t baudrate = (uint32_t)api->godot_variant_as_int(p_args[1]);
	int32_t timeout = (int32_t)api->godot_variant_as_int(p_args[2]);

	ser_opts_t opts = SER_OPTS_INIT;

	if (p_args[3] != NULL && p_args[4] != NULL && p_args[5] != NULL) {
		opts.bytesz = (int32_t)api->godot_variant_as_int(p_args[3]);
		opts.parity = (int32_t)api->godot_variant_as_int(p_args[4]);
		opts.stopbits = (int32_t)api->godot_variant_as_int(p_args[5]);
	}

	int32_t r = 0;

	if (ser == NULL)
	{
		printf("Could not create library instance:\n");
	}

	opts.port = port;
	opts.baudrate = baudrate;
	opts.timeouts.rd = timeout;

	r = ser_open(ser, &opts);

	if (r < 0)
	{
		printf("Could not open port: %s\n", sererr_last());
	}

	api->godot_string_new(&data);
	api->godot_string_parse_utf8(&data, "Opened");
	api->godot_variant_new_string(&ret, &data);
	api->godot_string_destroy(&data);
	api->godot_string_destroy(&cho);
	api->godot_char_string_destroy(&cha);

	return ret;
}

godot_variant sercomm_close(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_variant ret;
	godot_string data;
	ser_t *ser = (ser_t *)p_user_data;

	ser_close(ser);

	api->godot_string_new(&data);
	api->godot_string_parse_utf8(&data, "Closed");
	api->godot_variant_new_string(&ret, &data);
	api->godot_string_destroy(&data);

	return ret;
}
