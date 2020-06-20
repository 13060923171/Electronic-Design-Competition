// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif   /* _BYTE_DEF_ */

//COMMONDS******************************//

#define R_REG				0X00		//should be used with special register
#define W_REG				0X20		//should be used with special register
#define R_RX_PAYLOAD		0X61	//read RX payload
#define W_TX_PAYLOAD		0XA0	//write TX payload
#define FLUSH_TX			0XE1//clear TX FIFO
#define FLUSH_RX			0XE2//clear RX FIFO
#define REUSE_TX_PLD		0XE3	//reuse the packet last tasmitted
#define ACTIVATE			0X50//active or disable the follow three commonds,with 0x73
#define R_RX_PL_WID			0X60	//read the wideth of RX payload ,which is in zhe top of FIFO
#define W_ACK_PAYLOAD		0XA8	//write ack_payload to pipe u by the form of (W_ACK_PAYLOAD|u)
#define W_TX_PAYLOAD_NOACK	0XB0	//disable autoack of the following packet in TX FIFO
#define NOP					0XFF		//might be used to read status reg

//REGISTERS******************************//
#define CONFIG          0x00	// 'Config' register address
#define EN_AA           0x01	// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02	// 'Enabled RX addresses' register address
#define SETUP_AW        0x03	// 'Setup address width' register address
#define SETUP_RETR      0x04	// 'Setup Auto. Retrans' register address
#define RF_CH           0x05	// 'RF channel' register address
#define RF_SETUP        0x06	// 'RF setup' register address
#define STATUS          0x07	// 'Status' register address
#define OBSERVE_TX      0x08	// 'Observe TX' register address
#define CD              0x09  	// 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  	// 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  	// 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  	// 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  	// 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  	// 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  	// 'RX address pipe5' register address
#define TX_ADDR         0x10  	// 'TX address' register address
#define RX_PW_P0        0x11  	// 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  	// 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  	// 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  	// 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  	// 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  	// 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  	// 'FIFO Status Register' register address
#define DYNPD		0x1C  	// 'Enable dynamic payload length' register address
#define FEATURE		0x1D	// 'Feature' register address