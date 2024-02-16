/*
userspace_tablet_driver_daemon
Copyright (C) 2021 - Aren Villanueva <https://github.com/kurikaesu/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <iomanip>
#include "star_g640s.h"

star_g640s::star_g640s() {
    productIds.push_back(0x0906);

    for (int currentAssignedButton = BTN_0; currentAssignedButton < BTN_6; ++currentAssignedButton) {
        padButtonAliases.push_back(currentAssignedButton);
    }
}

std::string star_g640s::getProductName(int productId) {
    if (productId == 0x0906) {
        return "XP-Pen Star G640S";
    }

    return "Unknown XP-Pen Device";
}

void star_g640s::setConfig(nlohmann::json config) {
    if (!config.contains("mapping") || config["mapping"] == nullptr) {
        config["mapping"] = nlohmann::json({});

        auto addToStylusMap = [&config](int key, int eventType, std::vector<int> codes) {
            std::string evstring = std::to_string(eventType);
            config["mapping"]["stylus_buttons"][std::to_string(key)][evstring] = codes;
        };

        auto addToButtonMap = [&config](int key, int eventType, std::vector<int> codes) {
            std::string evstring = std::to_string(eventType);
            config["mapping"]["buttons"][std::to_string(key)][evstring] = codes;
        };

        addToStylusMap(BTN_STYLUS, EV_KEY, {});
        addToStylusMap(BTN_STYLUS2, EV_KEY, {});

        addToButtonMap(BTN_0, EV_KEY, {KEY_B});
        addToButtonMap(BTN_1, EV_KEY, {KEY_E});
        addToButtonMap(BTN_2, EV_KEY, {KEY_SPACE});
        addToButtonMap(BTN_3, EV_KEY, {KEY_LEFTALT});
        addToButtonMap(BTN_4, EV_KEY, {KEY_V});
        addToButtonMap(BTN_5, EV_KEY, {KEY_LEFTCTRL, KEY_S});
    }
    jsonConfig = config;

    submitMapping(jsonConfig);
}

bool star_g640s::handleTransferData(libusb_device_handle *handle, unsigned char *data, size_t dataLen, int productId) {
    switch (data[0]) {
        case 0x02:
            handleDigitizerEvent(handle, data, dataLen);
            handleFrameEvent(handle, data, dataLen);
            break;

        default:
            std::cout << "Received unknown message" << std::endl;
    }

    return true;
}

void star_g640s::handleFrameEvent(libusb_device_handle *handle, unsigned char *data, size_t dataLen) {
    if (data[1] >= 0xf0) {
        // Extract the button being pressed (If there is one)
        long button = (data[4] << 16) + (data[3] << 8) + data[2];
        // Grab the first bit set in the button long which tells us the button number
        long position = ffsl(button);

        if (button != 0) {
            handlePadButtonPressed(handle, position);
        } else {
            handlePadButtonUnpressed(handle);
        }

        uinput_send(uinputPads[handle], EV_SYN, SYN_REPORT, 1);
    }
}
