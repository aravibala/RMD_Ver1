/*
 * Copyright (c) 2021 Nordic Semiconductor
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

 #ifndef BT_PUL_CLIENT_H_
 #define BT_PUL_CLIENT_H_
 
 /**
  * @file
  * @defgroup bt_pul_client Pulse oximeter Service Client
  * @{
  * @brief Pulse oximeter (BP) Service Client API.
  */
 
 #include <zephyr/bluetooth/conn.h>
 #include <bluetooth/gatt_dm.h>
 #include <zephyr/bluetooth/gatt.h>
 
 #include <zephyr/sys/atomic.h>
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /** @brief UUID of the Pulse Oxymeter Service. **/
 #define BT_UUID_PUL_SERVICE_DATA \
     BT_UUID_128_ENCODE(0xCDEACB80, 0x5235, 0x4C07, 0x8846, 0x93A37EE6B86D)   // tracky pulse oximeter UUID
 
 /** @brief UUID of the Pulse Measurement Characteristic. **/
 #define BP_UUID__PUL_CHARACTER_RECEIVE \
     BT_UUID_128_ENCODE(0xCDEACB81, 0x5235, 0x4C07, 0x8846, 0x93A37EE6B86D)
 
 #define BT_UUID_PUL_SERVICE   BT_UUID_DECLARE_128(BT_UUID_PUL_SERVICE_DATA)
 #define BT_UUID_PUL_MEASUREMENT_CHAR        BT_UUID_DECLARE_128(BP_UUID__PUL_CHARACTER_RECEIVE)
 
 /**@brief Pulse oximeter Service error codes
  *
  * This service defines the following Attribute Protocol Application error codes.
  */
 enum bt_pul_client_error {
     /** Control Point value not supported. */
     BT_PUL_CLIENT_ERROR_CP_NOT_SUPPORTED = 0x80
 };
 
 /**@brief Pulse oximeter Measurement flags structure.
  */
 struct bt_pul_flags {
 
     // Units flag
     uint8_t no_signal_flag : 1;
     
     uint8_t probe_unplugged : 1;
 
     uint8_t pulse_beep : 1;
 
     uint8_t no_fingure_detected : 1;
 
     uint8_t pulse_searching : 1;
 };
 
 /**@brief Data structure of the Pulse oximeter Measurement characteristic.
  */
 struct bt_pul_client_measurement {
     /** Flags structure. */
     struct bt_pul_flags flags;
 
     uint16_t pulse_rate;
     uint8_t spO2_val;
     //uint8_t resp_rate;
 
     uint8_t byte_check;
 };
 
 /* Helper forward structure declaration representing Pulse oximeter Service Client instance.
  * Needed for callback declaration that are using instance structure as argument.
  */
 struct bt_pul_client;
 
 /**@brief Pulse oximeter Measurement indication callback.
  *
  * This function is called every time the client receives an indiation
  * with Pulse oximeter Measurement data.
  *
  * @param[in] pul_c Pulse oximeter Service Client instance.
  * @param[in] meas Pulse oximeter Measurement received data.
  * @param[in] err 0 if the indication is valid.
  *                Otherwise, contains a (negative) error code.
  */
 typedef void (*bt_pul_client_notify_cb)(struct bt_pul_client *pul_c,
                     const struct bt_pul_client_measurement *meas,
                     // uint8_t * pData,
                     int err);
 
 /**@brief Pulse oximeter Measurement characteristic structure.
  */
 struct bt_pul_client_pul_meas {
     /** Value handle. */
     uint16_t handle;
 
     /** Handle of the characteristic CCC descriptor. */
     uint16_t ccc_handle;
 
     /** GATT subscribe parameters for indication. */
     struct bt_gatt_subscribe_params notify_params;
 
     /** indication callback. */
     bt_pul_client_notify_cb notify_cb;
     
 };
 
 /**@brief Pulse oximeter Service Client instance structure.
  *        This structure contains status information for the client.
  */
 struct bt_pul_client {
     /** Connection object. */
     struct bt_conn *conn; //code print out and compare
 
     /** Pulse oximeter Measurement characteristic. */
     struct bt_pul_client_pul_meas measurement_char;
 
     /** Internal state. */
     atomic_t state;
 };
 
 /**@brief Function for initializing the Pulse oximeter Service Client.
  *
  * @param[in, out] pul_c Pulse oximeter Service Client instance. This structure must be
  *                       supplied by the application. It is initialized by
  *                       this function and will later be used to identify
  *                       this particular client instance.
  *
  * @retval 0 If the client was initialized successfully.
  *           Otherwise, a (negative) error code is returned.
  */
 int bt_pul_client_init(struct bt_pul_client *pul_c);
 
 /**@brief Subscribe to Pulse oximeter Measurement indication.
  *
  * This function writes CCC descriptor of the Pulse oximeter Measurement characteristic
  * to enable indication.
  *
  * @param[in] pul_c Pulse oximeter Service Client instance.
  * @param[in] notify_cb   indication callback.
  *
  * @retval 0 If the operation was successful.
  *           Otherwise, a (negative) error code is returned.
  */
 int bt_pul_client_measurement_subscribe(struct bt_pul_client *pul_c,
                     bt_pul_client_notify_cb notify_cb);
 
 /**@brief Remove subscription to the Pulse oximeter Measurement indication.
  *
  * This function writes CCC descriptor of the Pulse oximeter Measurement characteristic
  * to disable indication.
  *
  * @param[in] pul_c Pulse oximeter Service Client instance.
  *
  * @retval 0 If the operation was successful.
  *           Otherwise, a (negative) error code is returned.
  */
 int bt_pul_client_measurement_unsubscribe(struct bt_pul_client *pul_c);
 
 
 /**@brief Function for assigning handles to Pulse oximeter Service Client instance.
  *
  * @details Call this function when a link has been established with a peer to
  *          associate the link to this instance of the module. This makes it
  *          possible to handle several links and associate each link to a particular
  *          instance of this module.
  *
  * @param[in]     dm     Discovery object.
  * @param[in,out] pul_c  Pulse oximeter Service Client instance for associating the link.
  *
  * @retval 0 If the operation is successful.
  *           Otherwise, a (negative) error code is returned.
  */
 int bt_pul_client_handles_assign(struct bt_gatt_dm *dm, struct bt_pul_client *pul_c);
 
 #ifdef __cplusplus
 }
 #endif
 
 /**
  *@}
  */
 
 #endif /* BT_PUL_CLIENT_H_ */