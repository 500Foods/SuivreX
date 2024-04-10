#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <pthread.h>

#define MAX_INTERFACES 10

struct can_interface {
    int socket;
    char name[IFNAMSIZ];
};

void* receive_thread(void* arg) {
    struct can_interface* interface = (struct can_interface*)arg;
    struct can_frame frame;

    while (1) {
        int bytes = read(interface->socket, &frame, sizeof(struct can_frame));
        if (bytes > 0 && frame.can_id != suivrex_id) {
            printf("[%s] Received CAN frame with ID: 0x%03X, Data: ", interface->name, frame.can_id);
            for (int i = 0; i < frame.can_dlc; i++) {
                printf("%02X ", frame.data[i]);
            }
            printf("\n");
        }
    }
}

int suivrex_id;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <suivrex_id> <vcan_interface1> [<vcan_interface2> ...]\n", argv[0]);
        return 1;
    }

    suivrex_id = strtoul(argv[1], NULL, 16);
    int num_interfaces = argc - 2;
    struct can_interface interfaces[MAX_INTERFACES];
    pthread_t rx_threads[MAX_INTERFACES];

    for (int i = 0; i < num_interfaces; i++) {
        // Create a socket
        interfaces[i].socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (interfaces[i].socket < 0) {
            perror("Failed to create socket");
            return 1;
        }

        // Specify the virtual CAN interface
        strcpy(interfaces[i].name, argv[i + 2]);
        struct ifreq ifr;
        strcpy(ifr.ifr_name, interfaces[i].name);
        ioctl(interfaces[i].socket, SIOCGIFINDEX, &ifr);

        // Bind the socket to the CAN interface
        struct sockaddr_can addr;
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(interfaces[i].socket, (struct sockaddr*)&addr, sizeof(addr));

        // Create a separate thread for receiving CAN frames on each interface
        pthread_create(&rx_threads[i], NULL, receive_thread, &interfaces[i]);
    }

    // Send CAN frames using user input
    while (1) {
        char input[100];
        printf("Enter CAN frame (Data): ");
        fgets(input, sizeof(input), stdin);

        struct can_frame frame;
        frame.can_id = suivrex_id;
        frame.can_dlc = 0;
        char* token = strtok(input, " ");
        while (token != NULL && frame.can_dlc < 8) {
            frame.data[frame.can_dlc++] = strtoul(token, NULL, 16);
            token = strtok(NULL, " ");
        }

        // Send the frame on all connected interfaces
        for (int i = 0; i < num_interfaces; i++) {
            write(interfaces[i].socket, &frame, sizeof(struct can_frame));
        }
    }

    // Close the sockets
    for (int i = 0; i < num_interfaces; i++) {
        close(interfaces[i].socket);
    }

    return 0;
}

