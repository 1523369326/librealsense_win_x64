// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015 Intel Corporation. All Rights Reserved.

#if defined(RS2_USE_LIBUVC_BACKEND) && !defined(RS2_USE_WMF_BACKEND) && !defined(RS2_USE_V4L2_BACKEND) && !defined(RS2_USE_WINUSB_UVC_BACKEND)  && !defined(RS2_USE_ANDROID_BACKEND)
// UVC support will be provided via libuvc / libusb backend
#elif !defined(RS2_USE_LIBUVC_BACKEND) && defined(RS2_USE_WMF_BACKEND) && !defined(RS2_USE_V4L2_BACKEND) && !defined(RS2_USE_WINUSB_UVC_BACKEND) && !defined(RS2_USE_ANDROID_BACKEND)
// UVC support will be provided via Windows Media Foundation / WinUSB backend
#elif !defined(RS2_USE_LIBUVC_BACKEND) && !defined(RS2_USE_WMF_BACKEND) && !defined(RS2_USE_V4L2_BACKEND) && defined(RS2_USE_WINUSB_UVC_BACKEND)&&!defined(RS2_USE_ANDROID_BACKEND)
// UVC support will be provided via WinUSB / WinUSB backend
#elif !defined(RS2_USE_LIBUVC_BACKEND) && !defined(RS2_USE_WMF_BACKEND) && defined(RS2_USE_V4L2_BACKEND) && !defined(RS2_USE_WINUSB_UVC_BACKEND)&&!defined(RS2_USE_ANDROID_BACKEND)
// UVC support will be provided via Video 4 Linux 2 / libusb backend
#elif !defined(RS2_USE_LIBUVC_BACKEND) && !defined(RS2_USE_WMF_BACKEND) && !defined(RS2_USE_V4L2_BACKEND) && !defined(RS2_USE_WINUSB_UVC_BACKEND) && defined(RS2_USE_ANDROID_BACKEND)
// UVC support will be provided via Android backend
#else
#error No UVC backend selected. Please #define exactly one of RS2_USE_LIBUVC_BACKEND, RS2_USE_WMF_BACKEND, RS2_USE_WINUSB_UVC_BACKEND or RS2_USE_V4L2_BACKEND
#endif

#include "backend.h"
#include "platform/uvc-device.h"

#include <iomanip>
#include <chrono>
#include <algorithm>

namespace librealsense {
namespace platform {


void control_range::populate_raw_data(std::vector<uint8_t>& vec, int32_t value)
{
    vec.resize(sizeof(value));
    auto data = reinterpret_cast<const uint8_t*>(&value);
    std::copy(data, data + sizeof(value), vec.data());
}


std::ostream & operator<<( std::ostream & os, guid const & g )
{
    std::ios_base::fmtflags f( os.flags() );
    os << std::hex;
    auto prev_fill = os.fill();
    os.fill( '0' );
    os << g.data1 << '-' << g.data2 << '-' << g.data3 << '-';
    for( auto b : g.data4 )
        os << std::setw(2) << (int)b;
    os.flags( f );
    os.fill( prev_fill );
    return os;
}


}  // namespace platform


double monotonic_to_realtime(double monotonic)
{
    auto realtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto time_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    return monotonic + (realtime - time_since_epoch);
}


}  // namespace librealsense
