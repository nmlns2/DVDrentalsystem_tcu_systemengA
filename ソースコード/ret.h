/*
 * ファイル名	: ret.h
 * 機能			: 返却情報を管理する
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  
 */

#ifndef RET_H
#define RET_H

/* #defineマクロ */
/* バッファサイズ */
/* DisplayRet */
#define DISPLAYRET_DATE_BUF_SIZE	10			/* DisplayRet 今日の日付の文字列格納用バッファサイズ */
	
/* UpdateGoodsRetSts */
#define UPDATEGOODSRETSTS_DATE_BUF_SIZE	10		/* UpdateGoodsRetSts 本日の日付の文字列格納用バッファサイズ */

/* 返却：延滞あり/なし判断基準（貸出・返却日から起算した年月日を設定） */
#define RET_DLY_YEAR			0				/* 判断年 */
#define RET_DLY_MONTH			0				/* 判断月 */
#define RET_DLY_DAY				7				/* 判断日 */

/* 返却：延滞区分 */
#define RET_DLY_NO_VAL			"0"				/* 延滞なしの内部値 */
#define RET_DLY_YES_VAL			"1"				/* 延滞ありの内部値 */
#define RET_DLY_NO_NAME			"延滞なし"		/* 延滞なし文字 */
#define RET_DLY_YES_NAME		"延滞あり"		/* 延滞あり文字 */

/* 返却：料金体系 */
#define RET_DLY_PRICE			100				/* 延滞料金 */

/* 返却：処理 */
#define RET_MAX					10				/* 最大返却枚数 */

/* 返却：レコードサイズ */
#define RET_DELAY_SIZE			1				/* 延滞あり/なし */

/* structタグ定義 */
/* 返却情報 */
typedef struct retRent {
	char id[GOODS_ID_SIZE+1];					/* 商品ID */
	char title[MEMBER_NAME_SIZE+1];				/* タイトル */
	int delay;									/* 延滞日数 */
	char lendSts[GOODS_LEND_STS_SIZE+1];		/* 貸出状況 */
	struct retRent *next;						/* 自己参照ポインタ */
}RET;

/* 関数プロトタイプ宣言 */
extern void CreateRetList(void);
extern void ClearRetBuffer(RET *ret);
extern int DisplayRet(const GOODS *g, RET *ret);
extern int RegistRet(const RET *ret);
extern void FreeRetList(void);
extern int DisplayRetList(void);
extern int UpdateGoodsRetSts(GOODS *goods);

#endif		/* end RET_H */

