/*
 * =============================================================================
 *
 *       Filename:  sqlHandle.h
 *
 *    Description:  数据库操作接口
 *
 *        Version:  1.0
 *        Created:  2018-05-21 22:48:57 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _SQL_HANDLE_H
#define _SQL_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>
	extern void sqlInsertUserInfoNoBack(
			char *user_id,
			char *login_token,
			char *nick_name,
			int type,
			int scope);
	extern void sqlInsertUserInfo(
			char *user_id,
			char *login_token,
			char *nick_name,
			int type,
			int scope);
	extern int sqlGetUserInfoUseType(
			int type,
			char *user_id,
			char *login_token,
			char *nick_name,
			int *scope);
	extern int sqlGetUserInfoUseUserId(
			char *user_id,
			char *nick_name,
			int *scope);
	extern int sqlGetUserInfoStart(int type);
	/* ---------------------------------------------------------------------------*/
	/**
	 * @brief sqlGetUserInfoUseScopeStart 根据类型开始获取用户数量
	 *
	 * @param scope
	 *
	 * @returns -1时获取失败，正在有其他线程操作数据库
	 */
	/* ---------------------------------------------------------------------------*/
	extern int sqlGetUserInfoUseScopeStart(int scope);
	extern void sqlGetUserInfosUseScope(
			char *user_id,
			char *nick_name);
	extern void sqlGetUserInfosUseScopeIndex(
			char *user_id,
			int scope,
			int index);
	extern void sqlGetUserInfos(
			char *user_id,
			char *nick_name,
			int *scope);
	extern void sqlGetUserInfoEnd(void);
	extern void sqlInsertFace(char *user_id,
			char *nick_name,
			char *url,
			void *feature,
			int size);

    extern int sqlGetFaceCount(void);
	extern void sqlGetFaceStart(void);
	extern int sqlGetFace(char *user_id,char *nick_name,char *url,void *feature);
	extern void sqlGetFaceEnd(void);
	extern void sqlDeleteFace(char *id);
	extern void sqlCheckBack(void);
	extern void sqlInit(void);

	extern void sqlInsertRecordCapNoBack(
			char *date,
			uint64_t picture_id);
	extern void sqlInsertRecordFaceNoBack(
			char *date_time,
			char *face_id,
			char *nick_name,
			uint64_t picture_id);
	extern void sqlInsertPicUrlNoBack(
			uint64_t picture_id,
			char *url);
    extern void sqlInsertRecordUrlNoBack(
            uint64_t picture_id,
            char *url);
	extern int sqlGetCapInfo(
			uint64_t picture_id,
			char *date);
	extern int sqlGetPicInfoStart(uint64_t picture_id);
	extern void sqlGetPicInfos(char *url);
	extern void sqlGetPicInfoEnd(void);
	extern int sqlGetRecordInfoStart(uint64_t picture_id);
	extern void sqlGetRecordInfos(char *url);
	extern void sqlGetRecordInfoEnd(void);
	extern void sqlInsertRecordAlarm(
			char *date_time,
			int type,
			int has_people,
			int age,
			int sex,
			uint64_t picture_id);
	extern int sqlGetAlarmInfoUseDateType(
			char *date_time,
			int type,
			int *has_people);
	extern void sqlInsertRecordTalkNoBack(
			char *date_time,
			char *people,
			int call_dir,
			int answered,
			int talk_time,
			uint64_t picture_id);
	extern void sqlDeleteDeviceUseTypeNoBack(int type);

	extern void sqlClearFaceNoBack(void);
	extern void sqlClearDeviceNoBack(void);
	extern void sqlClearRecordCaptureNoBack(void);
	extern void sqlClearRecordTalkNoBack(void);
	extern void sqlClearRecordFaceNoBack(void);
	extern void sqlClearRecordAlarmNoBack(void);
	extern void sqlClearPicUrlNoBack(void);
	extern void sqlClearRecordUrlNoBack(void);
	extern void sqlClearAll(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
