#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define DEVICE_NAME "conveyor_motor"
#define BUF_LEN 80

static int major_num;
static int motor_state = 0; 
static char msg[BUF_LEN];

static int dev_open(struct inode *inodep, struct file *filep) { return 0; }
static int dev_release(struct inode *inodep, struct file *filep) { return 0; }

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    unsigned int rand_val;
    int rpm = 0, temp = 0;

    get_random_bytes(&rand_val, sizeof(rand_val));

    if (motor_state == 0) {
        rpm = 1400 + (rand_val % 100);
        temp = 60 + ((rand_val >> 8) % 15);
    } else if (motor_state == 1) {
        rpm = 1450 + (rand_val % 50);
        temp = 95 + ((rand_val >> 8) % 20);
    } else if (motor_state == 2) {
        rpm = 0 + (rand_val % 10);
        temp = 80 + ((rand_val >> 8) % 10);
    }

    snprintf(msg, BUF_LEN, "%d %d %d\n", rpm, temp, motor_state);
    
    if (*offset > 0) return 0;

    error_count = copy_to_user(buffer, msg, strlen(msg));
    if (error_count == 0) {
        *offset = strlen(msg);
        return strlen(msg);
    } else {
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    char input[2] = {0};
    if (copy_from_user(input, buffer, 1)) return -EFAULT;
    
    if (input[0] == '0') motor_state = 0;
    else if (input[0] == '1') motor_state = 1;
    else if (input[0] == '2') motor_state = 2;
    
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init conveyor_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    return (major_num < 0) ? major_num : 0;
}

static void __exit conveyor_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
}

module_init(conveyor_init);
module_exit(conveyor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Debananda Behera");
MODULE_DESCRIPTION("Industrial Conveyor Motor Telemetry Driver");
