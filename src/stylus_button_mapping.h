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

#ifndef USERSPACE_TABLET_DRIVER_DAEMON_STYLUS_BUTTON_MAPPING_H
#define USERSPACE_TABLET_DRIVER_DAEMON_STYLUS_BUTTON_MAPPING_H

#include <vector>
#include <map>
#include "aliased_input_event.h"

class stylus_button_mapping {
public:
    stylus_button_mapping();

    std::vector<aliased_input_event> getStylusButtonMap(int eventCode);
    void setStylusButtonMap(int eventCode, const std::vector<aliased_input_event>& events);
private:
    std::map<int, std::vector<aliased_input_event> > eventStylusButtonMap;
};


#endif //USERSPACE_TABLET_DRIVER_DAEMON_STYLUS_BUTTON_MAPPING_H
