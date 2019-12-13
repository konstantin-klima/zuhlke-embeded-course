#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/mod_devicetable.h>

MODULE_LICENSE("GPL");

static const struct i2c_device_id nunchuk_id[] 
		    = {
			{ "nuchuk", 0 },
			{ }
};

MODULE_DEVICE_TABLE(i2c, nunchuk_id);

static struct of_device_id nunchuk_match_table
	      [] = {
		{ .compatible = "nintendo,wiimote-extension", },
		{ }
};



static int nunchuk_probe(struct i2c_client 
			  *client,
			   const struct i2c_device_id *id)
{
	pr_info("Probing!\n");
	return 0;
}

static int nunchuk_remove(struct i2c_client 
			  *client)
{
	pr_info("Removing!\n");
	return 0;
}


static struct i2c_driver nunchuk_driver = {
	.driver = {
		.name = "nunchuk",
		.owner = THIS_MODULE,
		.of_match_table = nunchuk_match_table,
	},
	.probe = nunchuk_probe,
	.remove = nunchuk_remove,
	.id_table = nunchuk_id,
};

module_i2c_driver(nunchuk_driver);
/*
static int __init hello_init(void){
        pr_alert("Test matf kernel driver!\n");
        return 0;
}



static void __exit hello_exit(void){
    pr_alert("Test driver out\n");
}

module_init(hello_init);
module_exit(hello_exit);
*/

MODULE_DESCRIPTION("NUNCHUK module");
MODULE_AUTHOR("Konstantin Klima");

