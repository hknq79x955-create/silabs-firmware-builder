#include "sl_token_api.h"
#include "sl_token_manager.h"
#include <stdint.h>
#include <string.h>

// Override the manufacturing EUI-64 returned to the Zigbee stack.
// Target Zigbee IEEE (display order): 08:B9:5F:FF:FE:D8:B1:D6
// Silicon Labs token APIs provide the EUI-64 in little-endian order.
// This changes the application-visible identity; it does not program OTP.
extern sl_status_t __real_sl_token_manager_get_data(uint32_t token, void *data, uint32_t length);

sl_status_t __wrap_sl_token_manager_get_data(uint32_t token, void *data, uint32_t length)
{
    sl_status_t status = __real_sl_token_manager_get_data(token, data, length);

    if (status == SL_STATUS_OK
        && data != NULL
        && length >= 8U
        && token == SL_TOKEN_GET_STATIC_DEVICE_TOKEN(TOKEN_MFG_EUI_64)) {
        static const uint8_t target_eui64_le[8] = {
            0xD6, 0xB1, 0xD8, 0xFE, 0xFF, 0x5F, 0xB9, 0x08
        };
        memcpy(data, target_eui64_le, sizeof(target_eui64_le));
    }

    return status;
}
