#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <libudev.h>
#include <iostream>
#include <vector>
#include <string>

// Helper to get removable USB disks
std::vector<std::string> getRemovableDrives() {
    std::vector<std::string> drives;
    struct udev* udev = udev_new();
    struct udev_enumerate* enumerate = udev_enumerate_new(udev);

    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* dev = udev_device_new_from_syspath(udev, path);
        
        // Filter: Must be disk, removable, and USB transport
        const char* devtype = udev_device_get_devtype(dev);
        const char* removable = udev_device_get_sysattr_value(dev, "removable");
        const char* bus = udev_device_get_property_value(dev, "ID_BUS");

        if (devtype && std::string(devtype) == "disk" && 
            removable && std::string(removable) == "1" &&
            bus && std::string(bus) == "usb") {
            drives.push_back(udev_device_get_devnode(dev));
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    return drives;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    std::vector<std::string> drives = getRemovableDrives();
    
    if (drives.empty()) {
        QMessageBox::critical(nullptr, "Error", "No removable USB drives detected!");
        return 1;
    }

    QPushButton button("Flash Axelord to " + QString::fromStdString(drives[0]));
    button.show();

    return app.exec();
}

