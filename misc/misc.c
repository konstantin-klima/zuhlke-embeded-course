#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MATF module");
MODULE_AUTHOR("Konstantin Klima");

static const char id[] = "Matf2019\n";
static const int idlen = strlen(id);

ssize_t hello_read(struct file *fd, char __user *buf, size_t len, loff_t *off){
    return simple_read_from_buffer(buf, len, off, id, idlen);
}

ssize_t hello_write(struct file *fd, const char __user *buf, size_t len, loff_t *off)
{
    char tmp[idlen+1];
    int inlen = simple_write_to_buffer(tmp, idlen, off, buf, len);

    if(inlen < 0)
        return inlen;
    if ((inlen != idlen))
        return -EINVAL;
    if(strncmp(tmp, id, idlen) == 0)
        return idlen;
    else
        return -EINVAL;
}


const struct file_operations hello_fops = {
    .read = hello_read,
    .write = hello_write
};

struct miscdevice hello_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "matfmisc",
    .fops = &hello_fops,
    .mode = 0777
};

module_misc_device(hello_misc);


