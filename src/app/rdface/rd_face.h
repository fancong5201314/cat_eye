/*
 * =============================================================================
 *
 *       Filename:  rd_face.h
 *
 *    Description:  阅面人脸识别算法接口
 *
 *        Version:  1.0
 *        Created:  2019-06-17 17:13:03 
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
#ifndef _RD_FACE_H
#define _RD_FACE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define SIMILAYRITY 0.4

	int rdfaceInit(void);
	void rdfaceUninit(void);
    float rdfaceGetFeatureSimilarity(float *feature1, float *feature2);
	int rdfaceRegist(unsigned char *image_buff,int w,int h,float **out_feature,int *out_feature_size);
    int rdfaceRecognizer(unsigned char *image_buff,int w,int h,
			int (*featureCompare)(float *feature,void *face_data_out,int gender,int age),void *face_data_out);
	int rdfaceRecognizerOnce(unsigned char *image_buff,int w,int h,int *age,int *sex);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
