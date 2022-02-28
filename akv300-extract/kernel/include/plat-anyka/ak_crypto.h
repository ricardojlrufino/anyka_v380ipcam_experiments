#ifndef __AK_CRYPTO_H__
#define __AK_CRYPTO_H__

/* Encryption control register */
#define AKENC_CONTROL_REG			(0x000)

#define  AK_ENC_IV_BIT_SEQ		(1<<22)
#define  AK_ENC_KEY_BIT_SEQ		(1<<21)
#define  AK_ENC_CLK_EN			(1<<20)

#define  AK_ENC_OUTPUT_BYTE_SEQ	(1<<19)
#define  AK_ENC_OUTPUT_BIT_SEQ	(1<<18)
#define  AK_ENC_INPUT_BYTE_SEQ	(1<<17)
#define  AK_ENC_INPUT_BIT_SEQ	(1<<16)

#define AK_CRYPT_BIT_SEQ_MASK	(AK_ENC_INPUT_BIT_SEQ | AK_ENC_INPUT_BYTE_SEQ | \
								 AK_ENC_OUTPUT_BYTE_SEQ | AK_ENC_OUTPUT_BIT_SEQ | \
	   							 AK_ENC_KEY_BIT_SEQ	| AK_ENC_IV_BIT_SEQ)

#define AK_CRYPT_AES_BIT_SEQ 	(AK_ENC_INPUT_BIT_SEQ | AK_ENC_OUTPUT_BIT_SEQ | \
	   							 AK_ENC_IV_BIT_SEQ)
#define AK_CRYPT_DES_BIT_SEQ 	(AK_ENC_INPUT_BIT_SEQ | AK_ENC_OUTPUT_BIT_SEQ |\
	   							 AK_ENC_KEY_BIT_SEQ | AK_ENC_IV_BIT_SEQ)


#define  AK_ENC_IV_MODE			(1<<14)
#define  AK_ENC_MULT_GRP_CIPHER	(1<<13)

#define  AK_ENC_ALG_SEL(s)		((s)<<10)
#define  ENC_TYPE_ALG_DES			(0b000)
#define  ENC_TYPE_ALG_3DES_3KEY		(0b001)
#define  ENC_TYPE_ALG_3DES_2KEY		(0b010)
#define  ENC_TYPE_ALG_AES128		(0b011)
#define  ENC_TYPE_ALG_AES192		(0b100)
#define  ENC_TYPE_ALG_AES256		(0b101)


#define  AK_ENC_WIDTH_SEL(s)	((s)<<8)
#define  ENC_WIDTH_DES_64BIT		(0b00)
#define  ENC_WIDTH_DES_8BIT			(0b01)
#define  ENC_WIDTH_DES_1BIT			(0b10)

#define  ENC_WIDTH_AES_128BIT		(0b00)
#define  ENC_WIDTH_AES_8BIT			(0b01)
#define  ENC_WIDTH_AES_1BIT			(0b10)


#define  AK_ENC_OPT_MODE(s)		((s)<<5)
#define  ENC_WORK_MODE_ECB			(0b000)
#define  ENC_WORK_MODE_CBC			(0b001)
#define  ENC_WORK_MODE_CFB			(0b010)
#define  ENC_WORK_MODE_OFB			(0b011)
#define  ENC_WORK_MODE_CTR			(0b100)

#define  AK_ENC_TIMEOUT_INT_EN	(1<<4)
#define  AK_ENC_INT_EN			(1<<3)
#define  AK_ENC_OPT_STATUS		(1<<2)
#define  AK_ENC_STOP			(1<<1)
#define  AK_ENC_START			(1<<0)


#define AKENC_INT_STATUS_REG		(0x004)
#define  AK_ENC_ENCRYPT_TIMEOUT	(1<<1)
#define  AK_ENC_ENCRYPT_DONE	(1<<0)


#define AKENC_TIMEOUT_REG			(0x008)

/*AKENC_GRP_INPUT_REG1: 's' equals 1, s index range:1~4*/
#define AKENC_GRP_INPUT_REG(s)		(0x00c + (((s)-1)<<2))
#define AKENC_VEC_INPUT_REG(s)		(0x01c + (((s)-1)<<2))
#define AKENC_KEY_INPUT_REG(s)		(0x02c + (((s)-1)<<2))
#define AKENC_GRP_OUTPUT_REG(s)		(0x04c + (((s)-1)<<2))

#define AKENC_PLAINT_ADDR_REG		(0x05c)
#define AKENC_DATALEN_REG			(0x060)
#define AKENC_CIPHER_ADDR_REG		(0x064)


#define MAX_TIMEOUT 	(0xffffff)

#define CRYPTO_QUEUE_LEN    (5)
#define AK_CRA_PRIORITY		(50)

#define DES_KEY		(8)
#define DES_KEY_2	(2*DES_KEY)
#define DES_KEY_3	(3*DES_KEY)


#define FLAGS_MODE_MASK		(0x03ff)
#define FLAGS_WORK_MODE_MASK	(0x1f)

#define FLAGS_ECB			BIT(0)
#define FLAGS_CBC			BIT(1)
#define FLAGS_CFB			BIT(2)
#define FLAGS_OFB			BIT(3)
#define FLAGS_CTR			BIT(4)

#define FLAGS_ENCRYPT 		BIT(5)
#define FLAGS_DECRYPT 		BIT(6)

#define FLAGS_ALG_MODE_MASK		(0x7<<8)
#define FLAGS_AES			BIT(8)
#define FLAGS_DES			BIT(9)
#define FLAGS_3DES			BIT(10)


enum crypto_encrypt_mode {
	CRYPTO_SINGLE_GROUP_MODE,
	CRYPTO_MULTI_GROUP_MODE,
};

struct ak_crypto_plat_data {
	enum crypto_encrypt_mode encrypt_mode;	
};

#endif

