#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/mod_devicetable.h>
#include <linux/delay.h>
 
MODULE_LICENSE("Dual BSD/GPL");
 
 
static const struct i2c_device_id nunchuk_id[] = {
    { "nunchuk", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, nunchuk_id);
 
static struct of_device_id nunchuk_match_table[] = {
    { .compatible = "nintendo,wiimote-extension", },
    { }
};
 
#define EST_CONN_LEN (2U)
static const uint8_t EST_CONN_DATA_1[EST_CONN_LEN] = {0xf0, 0x55};
static const uint8_t EST_CONN_DATA_2[EST_CONN_LEN] = {0xfb, 0x00};
 
#define READ_INIT_LEN (1U)
static const uint8_t READ_INIT_DATA[READ_INIT_LEN] = {0x00};
 
#define READ_LEN (6U)
static uint8_t read_buffer[READ_LEN];
 
 
static int nunchuk_init_per(struct i2c_client *client)
{
    int stat;
 
    stat = i2c_master_send(client, EST_CONN_DATA_1, EST_CONN_LEN);
 
    if (stat != EST_CONN_LEN)
    {
        pr_err("Error when send, sent len %d!\n", stat);
        goto error;
    }
 
    msleep(1);
 
    stat = i2c_master_send(client, EST_CONN_DATA_2, EST_CONN_LEN);
 
    if (stat != EST_CONN_LEN)
    {
        pr_err("Error when send 2, sent len %d!\n", stat);
        goto error;
    }
 
    return 0;
error:
    return -1;
}
 
static int nunchuk_read_registers(struct i2c_client *client)
{
    int stat;
 
    msleep(10);
 
    stat = i2c_master_send(client, READ_INIT_DATA, READ_INIT_LEN);
 
    if (stat != READ_INIT_LEN)
    {
        pr_err("Error when send, sent len %d!\n", stat);
        goto error;
    }
 
    msleep(10);
 
    stat = i2c_master_recv(client, read_buffer, READ_LEN);
 
    if (stat != READ_LEN)
    {
        pr_err("Error when read, read len %d!\n", stat);
        goto error;
    }
 
    pr_info("Read data successfully: %2x %2x %2x %2x %2x %2x!\n",
        read_buffer[0],
        read_buffer[1],
        read_buffer[2],
        read_buffer[3],
        read_buffer[4],
        read_buffer[5]);
 
    return 0;
error:
    return -1;
}
 
static void parse_read_data(void)
{
    bool zpressed = read_buffer[5] & 0x01;
    bool cpressed = read_buffer[5] & 0x02;
 
    pr_info("Z,C button states: %d, %d.\n", zpressed, cpressed);
}
 
static int nunchuk_probe(struct i2c_client *client,
                 const struct i2c_device_id *id)
{
    int stat;
 
    pr_info("Probing!\n");
 
    stat = nunchuk_init_per(client);
 
    if (stat != 0)
    {
        pr_err("Error with init!\n");
        goto error;
    }
 
    stat = nunchuk_read_registers(client);
 
    if (stat != 0)
    {
        pr_err("Error with reading registers!\n");
        goto error;
    }
 
    stat = nunchuk_read_registers(client);
 
    if (stat != 0)
    {
        pr_err("Error with reading registers!\n");
        goto error;
    }
 
 
    parse_read_data();
 
 
    return 0;
error:
    return -1;
}
 
static int nunchuk_remove(struct i2c_client *client)
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
    .probe       = nunchuk_probe,
    .remove      = nunchuk_remove,
    .id_table    = nunchuk_id,
};
 
static int __init nunchuk_init(void)
{
    pr_info("Init!\n");
    return i2c_add_driver(&nunchuk_driver);
}
 
static void __exit nunchuk_exit(void)
{
    pr_info("Exit!\n");
    i2c_del_driver(&nunchuk_driver);
}
 
module_init(nunchuk_init);
module_exit(nunchuk_exit);
