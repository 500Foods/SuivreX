#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <dirent.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <pthread.h>

#define MAX_INTERFACES 10
#define MAX_I2C_DEVICES 128

struct can_interface {
    int socket;
    char name[IFNAMSIZ];
};

void print_timestamp() {
    time_t current_time;
    struct tm* time_info;
    char timestamp[20];

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
    printf("[%s] ", timestamp);
}

void* receive_thread(void* arg) {
    struct can_interface* can_if = (struct can_interface*)arg;
    struct can_frame frame;

    while (1) {
        ssize_t nbytes = read(can_if->socket, &frame, sizeof(struct can_frame));
        if (nbytes > 0) {
            print_timestamp();
            printf("Received message on interface %s:\n", can_if->name);
            printf("  ID: 0x%03X\n", frame.can_id);
            printf("  Data: ");
            for (int i = 0; i < frame.can_dlc; i++) {
                printf("%02X ", frame.data[i]);
            }
            printf("\n");
        }
    }

    return NULL;
}

int main() {
    struct can_interface can_interfaces[MAX_INTERFACES];
    int num_can_interfaces = 0;
    int num_i2c_devices = 0;

    // Print app version and current date/time
    print_timestamp();
    printf("CAN Bus and I2C Device Listing App - Version 1.0\n");

    // List available I2C devices
    print_timestamp();
    printf("Scanning for I2C devices...\n");

    struct dirent *i2c_entry;
    DIR *i2c_dir = opendir("/sys/class/i2c-adapter");
    if (i2c_dir == NULL) {
        perror("opendir");
    } else {
        while ((i2c_entry = readdir(i2c_dir)) != NULL) {
            if (i2c_entry->d_type == DT_LNK) {
                char path[256];
                snprintf(path, sizeof(path), "/sys/class/i2c-adapter/%s/name", i2c_entry->d_name);

                FILE *file = fopen(path, "r");
                if (file != NULL) {
                    char name[256];
                    if (fgets(name, sizeof(name), file) != NULL) {
                        name[strcspn(name, "\n")] = '\0';  // Remove trailing newline
                        printf("  - %s: %s\n", i2c_entry->d_name, name);
                        num_i2c_devices++;
                    }
                    fclose(file);
                }
            }
        }
        closedir(i2c_dir);
    }

    if (num_i2c_devices == 0) {
        print_timestamp();
        printf("No I2C devices found.\n");
    }

    // List available network interfaces
    print_timestamp();
    printf("Available network interfaces:\n");

    DIR *net_dir;
    struct dirent *net_entry;
    char path[256];

    net_dir = opendir("/sys/class/net");
    if (net_dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((net_entry = readdir(net_dir)) != NULL) {
        if (net_entry->d_type == DT_LNK) {
            snprintf(path, sizeof(path), "/sys/class/net/%s", net_entry->d_name);
            char resolved_path[256];
            ssize_t len = readlink(path, resolved_path, sizeof(resolved_path) - 1);
            if (len != -1) {
                resolved_path[len] = '\0';
                if (strstr(resolved_path, "/virtual/")) {
                    if (strncmp(net_entry->d_name, "vcan", 4) == 0) {
                        struct can_interface *can_if = &can_interfaces[num_can_interfaces++];
                        strcpy(can_if->name, net_entry->d_name);
                    } else if (strncmp(net_entry->d_name, "veth", 4) == 0) {
                        printf("  - %s: Virtual Ethernet\n", net_entry->d_name);
                    } else {
                        printf("  - %s: Virtual interface\n", net_entry->d_name);
                    }
                } else {
                    if (strncmp(net_entry->d_name, "can", 3) == 0) {
                        struct can_interface *can_if = &can_interfaces[num_can_interfaces++];
                        strcpy(can_if->name, net_entry->d_name);
                    } else if (strncmp(net_entry->d_name, "eth", 3) == 0) {
                        printf("  - %s: Physical Ethernet\n", net_entry->d_name);
                    } else {
                        printf("  - %s: Physical interface\n", net_entry->d_name);
                    }
                }
            }
        }
    }

    closedir(net_dir);

    if (num_can_interfaces == 0) {
        print_timestamp();
        printf("No CAN bus interfaces found.\n");
        exit(EXIT_FAILURE);
    }

    // Open sockets for each CAN bus interface and start receive threads
    print_timestamp();
    printf("Connected CAN bus interfaces:\n");
    pthread_t rx_threads[MAX_INTERFACES];

    for (int i = 0; i < num_can_interfaces; i++) {
        struct can_interface *can_if = &can_interfaces[i];
        can_if->socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (can_if->socket < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        struct ifreq ifr;
        strcpy(ifr.ifr_name, can_if->name);
        ioctl(can_if->socket, SIOCGIFINDEX, &ifr);

        struct sockaddr_can addr;
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(can_if->socket, (struct sockaddr*)&addr, sizeof(addr));

        printf("  - %s\n", can_if->name);

        // Create receive thread for each interface
        pthread_create(&rx_threads[i], NULL, receive_thread, can_if);
    }

    // Send CAN frames using user input
    while (1) {
        char input[100];
        printf("Enter CAN frame (Interface Data): ");
        fgets(input, sizeof(input), stdin);

        char *token = strtok(input, " ");
        if (token == NULL) {
            continue;
        }

        int interface_index = -1;
        for (int i = 0; i < num_can_interfaces; i++) {
            if (strcmp(token, can_interfaces[i].name) == 0) {
                interface_index = i;
                break;
            }
        }

        if (interface_index == -1) {
            fprintf(stderr, "Invalid interface: %s\n", token);
            continue;
        }

        struct can_frame frame;
        frame.can_id = 0;
        frame.can_dlc = 0;
        token = strtok(NULL, " ");
        while (token != NULL && frame.can_dlc < 8) {
            frame.data[frame.can_dlc++] = strtoul(token, NULL, 16);
            token = strtok(NULL, " ");
        }

        ssize_t nbytes = write(can_interfaces[interface_index].socket, &frame, sizeof(struct can_frame));
        if (nbytes > 0) {
            print_timestamp();
            printf("Sent message on interface %s:\n", can_interfaces[interface_index].name);
            printf("  ID: 0x%03X\n", frame.can_id);
            printf("  Data: ");
            for (int i = 0; i < frame.can_dlc; i++) {
                printf("%02X ", frame.data[i]);
            }
            printf("\n");
        }
    }

    // Close the sockets
    for (int i = 0; i < num_can_interfaces; i++) {
        close(can_interfaces[i].socket);
    }

    return 0;
}
