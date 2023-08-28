#ifndef YC_GAP_H
#define YC_GAP_H

/* Defined GAP timers */
#define BT_GAP_ADV_STOP_TIMER 150           /* 15s */

#define BT_GAP_SCAN_FAST_INTERVAL               0x0060  /* 60 ms    */
#define BT_GAP_SCAN_FAST_WINDOW                 0x0030  /* 30 ms    */
#define BT_GAP_SCAN_SLOW_INTERVAL_1             0x0800  /* 1.28 s   */
#define BT_GAP_SCAN_SLOW_WINDOW_1               0x0012  /* 11.25 ms */
#define BT_GAP_SCAN_SLOW_INTERVAL_2             0x1000  /* 2.56 s   */
#define BT_GAP_SCAN_SLOW_WINDOW_2               0x0012  /* 11.25 ms */
#define BT_GAP_ADV_FAST_INT_MIN_1               0x0030  /* 30 ms    */
#define BT_GAP_ADV_FAST_INT_MAX_1               0x0060  /* 60 ms    */
#define BT_GAP_ADV_FAST_INT_MIN_2               0x00a0  /* 100 ms   */
#define BT_GAP_ADV_FAST_INT_MAX_2               0x00f0  /* 150 ms   */
#define BT_GAP_ADV_SLOW_INT_MIN                 0x0640  /* 1 s      */
#define BT_GAP_ADV_SLOW_INT_MAX                 0x0780  /* 1.2 s    */
#define BT_GAP_INIT_CONN_INT_MIN                0x0018  /* 30 ms    */
#define BT_GAP_INIT_CONN_INT_MAX                0x0028  /* 50 ms    */

/* EIR/AD data type definitions */
#define BT_DATA_FLAGS                   0x01 /* AD flags */
#define BT_DATA_UUID16_SOME             0x02 /* 16-bit UUID, more available */
#define BT_DATA_UUID16_ALL              0x03 /* 16-bit UUID, all listed */
#define BT_DATA_UUID32_SOME             0x04 /* 32-bit UUID, more available */
#define BT_DATA_UUID32_ALL              0x05 /* 32-bit UUID, all listed */
#define BT_DATA_UUID128_SOME            0x06 /* 128-bit UUID, more available */
#define BT_DATA_UUID128_ALL             0x07 /* 128-bit UUID, all listed */
#define BT_DATA_NAME_SHORTENED          0x08 /* Shortened name */
#define BT_DATA_NAME_COMPLETE           0x09 /* Complete name */
#define BT_DATA_TX_POWER                0x0a /* Tx Power */
#define BT_DATA_SM_TK_VALUE             0x10 /* Security Manager TK Value */
#define BT_DATA_SM_OOB_FLAGS            0x11 /* Security Manager OOB Flags */
#define BT_DATA_SOLICIT16               0x14 /* Solicit UUIDs, 16-bit */
#define BT_DATA_SOLICIT128              0x15 /* Solicit UUIDs, 128-bit */
#define BT_DATA_SVC_DATA16              0x16 /* Service data, 16-bit UUID */
#define BT_DATA_GAP_APPEARANCE          0x19 /* GAP appearance */
#define BT_DATA_LE_BT_DEVICE_ADDRESS    0x1b /* LE Bluetooth Device Address */
#define BT_DATA_LE_ROLE                 0x1c /* LE Role */
#define BT_DATA_SOLICIT32               0x1f /* Solicit UUIDs, 32-bit */
#define BT_DATA_SVC_DATA32              0x20 /* Service data, 32-bit UUID */
#define BT_DATA_SVC_DATA128             0x21 /* Service data, 128-bit UUID */
#define BT_DATA_LE_SC_CONFIRM_VALUE     0x22 /* LE SC Confirmation Value */
#define BT_DATA_LE_SC_RANDOM_VALUE      0x23 /* LE SC Random Value */
#define BT_DATA_URI                     0x24 /* URI */
#define BT_DATA_MESH_PROV               0x29 /* Mesh Provisioning PDU */
#define BT_DATA_MESH_MESSAGE            0x2a /* Mesh Networking PDU */
#define BT_DATA_MESH_BEACON             0x2b /* Mesh Beacon */

#define BT_GAP_APPEARANCE_MOUSE 0x3c2
#define BT_GAP_APPEARANCE_REMOTE 0x180

#endif
