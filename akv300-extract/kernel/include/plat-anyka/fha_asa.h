#ifndef        _FHA_ASA_H_
#define        _FHA_ASA_H_

#define ASA_FILE_FAIL       0
#define ASA_FILE_SUCCESS    1
#define ASA_FILE_EXIST      2
#define ASA_MODE_OPEN       0
#define ASA_MODE_CREATE     1

#define ASA_FORMAT_NORMAL   0
#define ASA_FORMAT_EWR      1
#define ASA_FORMAT_RESTORE  2

/************************************************************************
 * NAME:     FHA_asa_scan
 * FUNCTION  scan nand flash security area
 * PARAM:    [in] bMount -- if buffer is enough, set AK_TURE, scan speedup
 * RETURN:   success return FHA_SUCCESS, fail retuen FHA_ FAIL
**************************************************************************/
T_U32  FHA_asa_scan(T_BOOL  bMount);

/************************************************************************
 * NAME:     FHA_asa_format
 * FUNCTION  nand flash format security area
 * PARAM:    [in] type -- ASA_FORMAT_NORMAL:  only scan initial bad blocks
 *                        ASA_FORMAT_EWR:     Erase-wirte-read test
 *                        ASA_FORMAT_RESTORE: do nothing
 * RETURN:   success return FHA_SUCCESS, fail retuen FHA_ FAIL
**************************************************************************/
T_U32  FHA_asa_format(T_U32 type);

/************************************************************************
 * NAME:     FHA_set_bad_block
 * FUNCTION  set nand flash bad block
 * PARAM:    [in] block -- the bad block item
 * RETURN:   success return FHA_SUCCESS, fail retuen FHA_ FAIL
**************************************************************************/
T_U32  FHA_set_bad_block(T_U32 block);

/************************************************************************
 * NAME:     FHA_check_bad_block
 * FUNCTION  check nand flash bad block
 * PARAM:    [in] block -- need to check block item
 * RETURN:   is bad block return FHA_SUCCESS, or else retuen FHA_ FAIL
**************************************************************************/
T_BOOL  FHA_check_bad_block(T_U32 block);

/************************************************************************
 * NAME:     FHA_get_bad_block
 * FUNCTION  get bad block information of one or more blocks
 * PARAM:    [in] start_block -- start block
 *           [out]pData -------- buffer used to store bad blocks information data
 *           [in] blk_cnt ------ how many blocks you want to know
 * RETURN:   success return FHA_SUCCESS, fail retuen FHA_ FAIL
**************************************************************************/
T_U32  FHA_get_bad_block(T_U32 start_block, T_U8 *pData, T_U32 blk_cnt);

/************************************************************************
 * NAME:     FHA_asa_write_file
 * FUNCTION  write important infomation to security area, data_len can't too large
 * PARAM:    [in] file_name -- asa file name
 *           [in] pData ------ buffer used to store information data
 *           [in] data_len --- need to store data length
 *           [in] mode-------- operation mode 
 *                             ASA_MODE_OPEN------open  
 *                             ASA_MODE_CREATE----create
 * RETURN:   success return ASA_FILE_SUCCESS, fail retuen ASA_FILE_FAIL
**************************************************************************/
T_U32  FHA_asa_write_file(T_U8 file_name[], const T_U8 *pData, T_U32 data_len, T_U8 mode);

/************************************************************************
 * NAME:     FHA_asa_read_file
 * FUNCTION  get infomation from security area by input file name
 * PARAM:    [in] file_name -- asa file name
 *           [out]pData ------ buffer used to store information data
 *           [in] data_len --- need to get data length
 * RETURN:   success return ASA_FILE_SUCCESS, fail retuen ASA_FILE_FAIL
**************************************************************************/
T_U32  FHA_asa_read_file(T_U8 file_name[], T_U8 *pData, T_U32 data_len);
#endif    //

