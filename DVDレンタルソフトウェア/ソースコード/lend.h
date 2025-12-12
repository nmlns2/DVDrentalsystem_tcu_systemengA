/*
 * ファイル名	: Lend.h
 * 機能			: 貸出管理を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  
 */

#ifndef LEND_H
#define LEND_H

/* #defineマクロ */
/* バッファサイズ */
/* DisplayLend */
#define DISPLAYLEND_DATE_BUF_SIZE	10			/* DisplayLend 旧作と判断する日付の文字列格納用バッファサイズ */
	
/* UpdateGoodsLendSts */
#define UPDATEGOODSLENDSTS_DATE_BUF_SIZE	10	/* UpdateGoodsLendSts 貸出日の日付の文字列格納用バッファサイズ */

/* 貸出：新作/旧作判断基準（発売日から起算した年月日を設定） */
#define LEND_OLD_YEAR			0				/* 判断年 */
#define LEND_OLD_MONTH			6				/* 判断月 */
#define LEND_OLD_DAY			0				/* 判断日 */

/* 貸出：新作/旧作区分 */
#define LEND_NEW_VAL			"0"				/* 新作の内部値 */
#define LEND_OLD_VAL			"1"				/* 旧作の内部値 */
#define LEND_NEW_NAME			"NEW"			/* 新作の表示文字 */
#define LEND_OLD_NAME			"OLD"			/* 旧作の表示文字 */

/* 貸出：料金体系 */
#define LEND_NEW_PRICE			300				/* 新作料金 */
#define LEND_OLD_PRICE			100				/* 旧作料金 */

/* 貸出：処理 */
#define LEND_MAX				10				/* 最大貸出枚数 */

/* 貸出：レコードサイズ */
#define LEND_OLD_SIZE			1				/* 新作/旧作区分 */

/* structタグ定義 */
/* 貸出情報 */
typedef struct lendRent {
	char id[GOODS_ID_SIZE+1];					/* 商品ID */
	char title[MEMBER_NAME_SIZE+1];				/* タイトル */
	char old[LEND_OLD_SIZE+1];					/* 新作/旧作 */
	char lendSts[GOODS_LEND_STS_SIZE+1];		/* 貸出状況（未使用） */
	struct lendRent *next;						/* 自己参照ポインタ */
}LEND;

/* 関数プロトタイプ宣言 */
extern void CreateLendList(void);
extern void ClearLendBuffer(LEND *lend);
extern int DisplayLend(const GOODS *goods, LEND *lend);
extern int RegistLend(const LEND *lend);
extern void FreeLendList(void);
extern int DisplayLendList(void);
extern int UpdateGoodsLendSts(const char *mID, GOODS *goods);

#endif		/* end LEND_H */

